/**
 * @file ipc/bakery.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/*
 * Lamport's bakery algorithm.
 * Leslie Lamport.
 * A New Solution of Dijkstra's Concurrent Programming Problem.
 * Communications of the ACM, Volume 17, Issue 8, pp. 453--455, August 1974.
 */

/**
 * @var tickets[NR_THREADS]
 * @brief Tickets.
 */
static volatile int tickets[NR_THREADS] = {0};

/**
 * @var entering[NR_THREADS]
 * @brief Entering.
 */
static volatile bool entering[NR_THREADS] = {false};

void bakery_lock(int thid)
{
  /* TODO: support distributed shared memory. */
  int max_ticket = 0;
  int ticket;
  entering[thid] = true;
  sync();

  for (int i = 0; i < NR_THREADS; i++) {
    ticket = tickets[i];
    max_ticket = MAX(ticket, max_ticket);
  }

  tickets[thid] = max_ticket + 1;
  sync();
  entering[thid] = 0;
  sync();

  for (int other = 0; other < NR_THREADS; other++) {
    while (entering[other]) {
      sync();

      while (tickets[other] != 0
             && (tickets[other] < tickets[thid]
                 || (tickets[other] == tickets[thid] && other < thid))) {
      }
    }
  }
}

void bakery_unlock(int thid)
{
  sync();
  tickets[thid] = 0;
}
