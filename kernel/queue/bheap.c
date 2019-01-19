/**
 * @file kernel/queue/bheap.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


struct bheap_node bh_nodes[NR_THREADS];

thread_prio_t thread_prio = thread_is_high_prio;

int empty_bheap(struct rt_runqueue *rq)
{
  return !(rq->head) && !(rq->min);
}

/* make child a subtree of root */
void link_bheap(struct bheap_node *root, struct bheap_node *child)
{
  child->parent = root;
  child->next = root->child;
  root->child = child;
  root->degree++;
}

/* merge root lists */
struct bheap_node *merge_bheap(struct bheap_node *p,
                               struct bheap_node *q)
{
  struct bheap_node *head = NULL;
  struct bheap_node **pos = &head;

  while (p && q) {
    if (p->degree < q->degree) {
      *pos = p;
      p = p->next;
    } else {
      *pos = q;
      q = q->next;
    }
    pos = &(*pos)->next;
  }
  if (p) {
    *pos = p;
  } else {
    *pos = q;
  }
  return head;
}

/* reverse a linked list of nodes. also clears parent pointer */
struct bheap_node *reverse_bheap(struct bheap_node *h)
{
  struct bheap_node* tail = NULL;
  struct bheap_node* next;

  if (!h) {
    return h;
  }
  h->parent = NULL;
  while (h->next) {
    next = h->next;
    h->next = tail;
    tail = h;
    h = next;
    h->parent = NULL;
  }
  h->next = tail;
  return h;
}

void min_bheap(struct rt_runqueue *rq, struct bheap_node **prev, struct bheap_node **node)
{
  struct bheap_node *_prev, *cur;
  *prev = NULL;
  PDEBUG("min_bheap()\n");
  pdebug_bheap(rq, rq->head);
  if (!rq->head) {
    *node = NULL;
    return;
  }

  *node = rq->head;
  _prev = rq->head;
  cur = rq->head->next;
  while (cur) {
    PDEBUG("min_bheap(): cur->value->id = %lu (*node)->value->id = %lu\n",
           cur->value->id, (*node)->value->id);
    if (thread_prio(cur->value, (*node)->value)) {
      *node = cur;
      *prev = _prev;
    }
    _prev = cur;
    cur = cur->next;
  }
}


void union_bheap(struct rt_runqueue *rq, struct bheap_node *h2)
{
  struct bheap_node *h1;
  struct bheap_node *prev, *x, *next;
  if (!h2) {
    return;
  }
  h1 = rq->head;
  if (!h1) {
    rq->head = h2;
    return;
  }
  h1 = merge_bheap(h1, h2);
  prev = NULL;
  x = h1;
  next = x->next;
  while (next) {
    if ((x->degree != next->degree) ||
        (next->next && (next->next->degree == x->degree))) {
      /* nothing to do, advance */
      prev = x;
      x = next;
    } else if (thread_prio(x->value, next->value)) {
      /* x becomes the root of next */
      PDEBUG("union_bheap(): x->value->id = %lu priority = %lu becomes the root of next\n",
             x->value->id, x->value->priority);
      PDEBUG("union_bheap(): next->value->id = %lu priority = %lu\n",
             next->value->id, next->value->priority);
      x->next = next->next;
      link_bheap(x, next);
    } else {
      /* next becomes the root of x */
      if (prev) {
        prev->next = next;
      } else {
        h1 = next;
      }
      link_bheap(next, x);
      x = next;
    }
    next = x->next;
  }
  rq->head = h1;
}

struct bheap_node *min_bheap_extract(struct rt_runqueue *rq)
{
  struct bheap_node *prev, *node;
  min_bheap(rq, &prev, &node);
  if (!node) {
    return NULL;
  }
  if (prev) {
    prev->next = node->next;
  } else {
    rq->head = node->next;
  }
  union_bheap(rq, reverse_bheap(node->child));
  return node;
}

/* insert (and reinitialize) a node into the bheap */
void insert_bheap(struct rt_runqueue *rq, struct bheap_node *node)
{
  struct bheap_node *min;
  PDEBUG("insert_bheap(): node = %lx node->value->id = %lu\n",
         (unsigned long) node, node->value->id);
  node->child = NULL;
  node->parent = NULL;
  node->next = NULL;
  node->degree = 0;
  if (rq->min && thread_prio(node->value, rq->min->value)) {
    /* swap min cache */
    min = rq->min;
    min->child = NULL;
    min->parent = NULL;
    min->next = NULL;
    min->degree = 0;
    union_bheap(rq, min);
    rq->min = node;
  } else {
    union_bheap(rq, node);
  }
}

void min_bheap_uncache(struct rt_runqueue *rq)
{
  struct bheap_node *min;
  if (rq->min) {
    min = rq->min;
    rq->min = NULL;
    insert_bheap(rq, min);
  }
}

/* merge addition into target */
void union_bheap_uncache(struct rt_runqueue *target, struct rt_runqueue *addition)
{
  /* first insert any cached minima, if necessary */
  min_bheap_uncache(target);
  min_bheap_uncache(addition);
  union_bheap(target, addition->head);
  /* this is a destructive merge */
  addition->head = NULL;
}

struct bheap_node *peek_bheap(struct rt_runqueue *rq)
{
  if (!rq->min) {
    rq->min = min_bheap_extract(rq);
  }
  return rq->min;
}

struct bheap_node *take_bheap(struct rt_runqueue *rq)
{
  struct bheap_node *node;
  if (!rq->min) {
    rq->min = min_bheap_extract(rq);
  }
  node = rq->min;
  rq->min = NULL;
  if (node) {
    node->degree = NOT_IN_BHEAP;
  }
  return node;
}

void decrease_bheap(struct rt_runqueue *rq, struct bheap_node *node)
{
  struct bheap_node *parent;
  void *tmp;
  uint64_t node_id;

  if (!node) {
    return;
  }

  if (rq->min != node) {
    /* bubble up */
    parent = node->parent;
    while (parent && thread_prio(node->value, parent->value)) {
      /* swap parent and node */
      tmp = parent->value;
      node_id = parent->node_id;

      parent->value = node->value;
      parent->node_id = node->node_id;

      node->value = tmp;
      node->node_id = node_id;

      /* swap value->node */
      tmp = node->value->node;
      node->value->node = parent->value->node;
      parent->value->node = tmp;

      /* step up */
      node = parent;
      parent = node->parent;
    }
  }
}

void delete_bheap(struct rt_runqueue *rq, struct bheap_node *node)
{
  struct bheap_node *parent, *prev, *pos;
  void *tmp;
  uint64_t node_id;

  if (!node) {
    return;
  }

  PDEBUG("delete_bheap(): node->node_id = %lu\n", node->node_id);

  if (rq->min != node) {
    /* bubble up */
    parent = node->parent;
    while (parent) {
      pdebug_bheap(rq, rq->head);
      /* swap parent and node */
      tmp = parent->value;
      node_id = parent->node_id;

      parent->value = node->value;
      parent->node_id = node->node_id;

      node->value = tmp;
      node->node_id = node_id;

      /* swap value->node */
      tmp = node->value->node;
      node->value->node = parent->value->node;
      parent->value->node = tmp;

      /* step up */
      node = parent;
      parent = node->parent;
    }
    /* now delete:
     * first find prev */
    prev = NULL;
    pos = rq->head;
    while (pos != node) {
      prev = pos;
      pos = pos->next;
    }
    /* we have prev, now remove node */
    if (prev) {
      prev->next = node->next;
    } else {
      rq->head = node->next;
    }
    union_bheap(rq, reverse_bheap(node->child));
  } else {
    rq->min = NULL;
  }
  pdebug_bheap(rq, rq->head);
  node->degree = NOT_IN_BHEAP;
}


void print_bheap(struct rt_runqueue *rq, struct bheap_node *h)
{
  static int nr_tabs = 0;
  int i;
  if (!h) {
    PDEBUG("NULL\n");       
    for (i = 0; i < nr_tabs; i++) {
      PDEBUG("\t");
    }
    return;
  }
  //      PDEBUG("id(%lu)->", h->value->id);
  PDEBUG("node_id(%lu)id(%lu)deg(%lu)->", h->node_id, h->value->id, h->degree);
  nr_tabs++;
  print_bheap(rq, h->child);
  //      print_bheap(rq, h->next);
  nr_tabs--;
  //      print("\nchild\n");
  //      print_bheap(rq, h->child);
  print_bheap(rq, h->next);
}


void enqueue_rq_queue_head(struct rt_runqueue *rq, struct thread_struct *th)
{
#if SCHED_FP
  set_bit(rq->bitmap, th->priority);
#endif /* SCHED_FP */
  insert_bheap(rq, th->node);
}


/* first argument 'rq' is dummy */
void enqueue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th)
{
#if SCHED_FP
  set_bit(rq->bitmap, th->priority);
#endif /* SCHED_FP */
  insert_bheap(rq, th->node);
}


void dequeue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th)
{
  /* dequeue */
  //  struct bheap_node *bn;
  delete_bheap(rq, th->node);
  //  bn = take_bheap(rq);
  //  PDEBUG("dequeue_rq(): bn->node_id = %u, bn_node->value->id = %u, th->id = %u\n", bn->node_id, bn->value->id, th->id);
#if SCHED_FP
  if (rq->array[th->priority].next == &rq->array[th->priority]) {
    clear_bit(rq->bitmap, th->priority);
  }
#endif /* SCHED_FP */
}

struct thread_struct *pick_next_task(void)
{
  struct thread_struct *th = NULL;
  struct bheap_node *bn;
  unsigned long cpu = get_cpu_id();
  if (!empty_bheap(&run_tq[cpu])) {
    struct bheap_node *prev;
    min_bheap(&run_tq[cpu], &prev, &bn);    
    th = bn->value;
  }
  return th;
}

void init_bheap_node(struct bheap_node *h, struct thread_struct *value)
{
  h->parent = NULL;
  h->next   = NULL;
  h->child  = NULL;
  h->degree = NOT_IN_BHEAP;
  h->value  = value;
  h->node_id = value->id;
}

void init_rq(void)
{
  int i, j;
  /* NOTE: other members in run_tq are initialized at compile time.  */
  for (i = 0; i < NR_CPUS; i++) {
    for (j = 0; j < NR_PRIORITIES; j++) {
      run_tq[i].array[j].prev = run_tq[i].array[j].next = &run_tq[i].array[j];
    }
    /* bheap */
    run_tq[i].head = run_tq[i].min = NULL;
  }
  for (i = 0; i < NR_THREADS; i++) {
    ths[i].node = &bh_nodes[i]; 
    init_bheap_node(&bh_nodes[i], &ths[i]);
  }
}
