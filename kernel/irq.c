/**
 * @file kernel/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

struct full_regs *__irq_regs;
struct irq_descriptor irq_desc[NR_IRQS];


irqreturn_t handle_irq_event(unsigned long irq, struct irqaction *action)
{
	irqreturn_t ret, retval = IRQ_NONE;
	unsigned int status = 0;
	//	printk("handle_irq_event(): action = 0x%x irq = %x\n", action, irq);
	//	printk("handle_irq_event(): action->handler = 0x%x\n", action->handler);

	do {
		printk("handle_irq_event(): irq = 0x%lx call action->handler = 0x%lx\n",
           irq, (unsigned long) action->handler);
		ret = action->handler(irq, action->dev_id);
		printk("handle_irq_event(): irq = 0x%lx return action->handler = 0x%lx\n",
           irq, (unsigned long) action->handler);
		if (ret == IRQ_HANDLED) {
			status |= action->flags;
		}
		retval |= ret;
		action = action->next;
	} while (action);


	disable_local_irq();
	//	printk("handle_irq_event(): end\n");

	return retval;
}


unsigned int __do_irq(unsigned long irq)
{
	//	inf_loop();
	struct irq_descriptor *desc = irq_desc + irq;
  struct irqaction *action;
  unsigned long status;
	printk("__do_irq()\n");
	
	//spin_lock(&desc->lock);

#if 0
	if (desc->chip->ack) {
    desc->chip->ack(irq);
	}
#endif

  /*
   * REPLAY is when Linux resends an IRQ that was dropped earlier
   * WAITING is used by probe to mark irqs that are being tested
   */
  status = desc->status & ~(IRQ_REPLAY | IRQ_WAITING);
  status |= IRQ_PENDING; /* we _want_ to handle it */

	/*
   * If the IRQ is disabled for whatever reason, we cannot
   * use the action we have.
   */
  action = NULL;
  if (!(status & (IRQ_DISABLED | IRQ_INPROGRESS))) {
    action = desc->action;
		//		printk("action = 0x%x\n", action);
		/* we commit to handling */
    status &= ~IRQ_PENDING;
		/* we are handling it */
    status |= IRQ_INPROGRESS;
  } else if (!(status & IRQ_DISABLED) && (desc->action->flags & IRQF_PERCPU)) {
		action = desc->action;
	}

	if (!action) {
		printk("__do_irq(): action == NULL irq handler %lu is busy\n", irq);
		return 1;
	}
  desc->status = status;

  for (;;) {
    irqreturn_t action_ret;
		//spin_unlock(&desc->lock);
		action_ret = handle_irq_event(irq, action);
		if (action_ret == IRQ_NONE) {
			printk("Error: action_ret == IRQ_NONE\n");
			exit(1);
		}
		//spin_lock(&desc->lock);
    if (!(desc->status & IRQ_PENDING)) {
      break;
		}
    desc->status &= ~IRQ_PENDING;
		
  }
	desc->status &= ~IRQ_INPROGRESS;

	//	spin_unlock(&desc->lock);
	//	printk("__do_irq() end\n");
	return 1;
}



/*
 * Internal function to register an irqaction - typically used to
 * allocate special interrupts that are part of the architecture.
 */
int setup_irq(unsigned int irq, struct irqaction *new)
{
	struct irq_descriptor *desc = irq_desc + irq;
	struct irqaction *old, **p;
	//	unsigned long flags;

	//	printk("setup_irq(): irq = %d\n", irq);
	if (irq >= NR_IRQS) {
		return -EINVAL;
	}

	/*
	 * The following block of code has to be executed atomically
	 */
	//	spin_lock_irqsave(&desc->lock, flags);
	//	printk("hoge\n");

	p = &desc->action;
	old = *p;
	if (old) {
		/*
		 * Can't share interrupts unless both agree to and are
		 * the same type (level, edge, polarity). So both flag
		 * fields must have IRQF_SHARED set and the bits which
		 * set the trigger type must match.
		 */
		if (!((old->flags & new->flags) & IRQF_SHARED) ||
				((old->flags ^ new->flags) & IRQF_TRIGGER_MASK)) {
			goto mismatch;
		}

		/* add new interrupt at end of irq queue */
		do {
			p = &old->next;
			old = *p;
		} while (old);
	}

	*p = new;
	//	printk("&desc->action->handler = %x\n", &desc->action->handler);
	//	printk("desc->action->handler = %x\n", desc->action->handler);

#if 0
	struct irqaction *head = desc->action;
	while (head) {
		printk("head->handler = %x\n", head->handler);
		head = head->next;
	}
#endif

#if 0
	unsigned long cpu = get_cpu_id();
	if (cpu == 0) {
		desc->lock.lock = SPIN_UNLOCKED;
	}


	/* Reset broken irq detection when installing new handler */
	desc->irq_count = 0;
	desc->irqs_unhandled = 0;
	//	spin_unlock_irqrestore(&desc->lock, flags);
#endif
	desc->status = 0;

	new->irq = irq;
	new->dir = NULL;
	return 0;

 mismatch:
	//	spin_unlock_irqrestore(&desc->lock, flags);
	return -EBUSY;
}


void request_bottomhalf(unsigned int soft_irq)
{
	//  set_bit(soft_irq, (unsigned long *) &softirq_vec.vector);
}


