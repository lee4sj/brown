#include <stdio.h>
#include <stdlib.h>
#include "state_machine.h"
#include "util/htable.h"
#include "util/dbg.h"


/*
 * Global variables for state function hash table
 * TODO: needs mutex?
 */
htable_t st_funcs;


/*
 * Given a connection's current state and the transition code, returns a pointer to a 
 * state_response struct containing the new state and any actions to take.
 */
struct state_response *next_state(int cur_state, int trans) {
   struct state_response *rtn;

   void *f = htable_get(&st_funcs, cur_state);
   if (f == NULL) {
      dbg(DBG_ERROR, "next_state: unknown current state %d\n", cur_state);
      return NULL;
   }

   rtn = (struct state_response *)malloc(sizeof(struct state_response));
   st_func_t s = (st_func_t)f;
   (*s)(trans, rtn);
   return rtn;
}


void closed_next(int trans, struct state_response *rtn) {
   switch (trans) {
      case PSVOPEN_TR:
         rtn->new_state = LISTEN_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = TCBCREATE_AC;
         rtn->n_actions = 1;
         break;
      case ACTOPEN_TR:
         rtn->new_state = SYNSNT_ST;
         rtn->actions = (int *)malloc(sizeof(int) * 2);
         (rtn->actions)[0] = TCBCREATE_AC;
         (rtn->actions)[1] = SENDSYN_AC;
         rtn->n_actions = 2;
         break;
      default:
         dbg(DBG_ALL, "from CLOSED state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void listen_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case RCVSYN_TR:
         rtn->new_state = SYNRCVD_ST;
         rtn->actions = (int *)malloc(sizeof(int) * 2);
         (rtn->actions)[0] = SENDSYN_AC;
         (rtn->actions)[1] = SENDACK_AC;
         rtn->n_actions = 2;
         break;
      case SEND_TR:
         rtn->new_state = SYNSNT_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDSYN_AC;
         rtn->n_actions = 1;
         break;
      case CLOSE_TR:
         rtn->new_state = CLOSED_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = TCBDELETE_AC;
         rtn->n_actions = 1;
         break;
      default:
         dbg(DBG_ALL, "from LISTEN state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void synrcvd_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case RCVACKSYN_TR: // rcv ACK of SYN
         rtn->new_state = ESTAB_ST;
         rtn->actions = NULL;
         rtn->n_actions = 0;
         break;
      case CLOSE_TR:
         rtn->new_state = FINWT1_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDFIN_AC;
         rtn->n_actions = 1;
         break;
      default:
         dbg(DBG_ALL, "from SYNRCVD state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void synsnt_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case CLOSE_TR: 
         rtn->new_state = CLOSED_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = TCBDELETE_AC;
         rtn->n_actions = 1;
         break;
      case RCVSYN_TR:
         rtn->new_state = SYNRCVD_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDACK_AC;
         rtn->n_actions = 1;
         break;
      case RCVSYNACK_TR: // rcv SYN, ACK
         rtn->new_state = ESTAB_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDACK_AC;
         rtn->n_actions = 1;
         break;
      default:
         dbg(DBG_ALL, "from SYNSNT state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void estab_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case CLOSE_TR:
         rtn->new_state = FINWT1_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDFIN_AC;
         rtn->n_actions = 1;
         break;
      case RCVFIN_TR:
         rtn->new_state = CLOSEWT_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDACK_AC;
         rtn->n_actions = 1;
         break;
      default:
         dbg(DBG_ALL, "from ESTAB state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void finwt1_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case RCVACKFIN_TR:
         rtn->new_state = FINWT2_ST;
         rtn->actions = NULL;
         rtn->n_actions = 0;
         break;
      case RCVFIN_TR:
         rtn->new_state = CLOSING_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDACK_AC;
         rtn->n_actions = 1;
         break;
      default:
         dbg(DBG_ALL, "from FINWT1 state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }  
}


void finwt2_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case RCVFIN_TR:
         rtn->new_state = TIMEWT_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDACK_AC;
         rtn->n_actions = 1;
         break;
      default:
         dbg(DBG_ALL, "from FINWT2 state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void closewt_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case CLOSE_TR:
         rtn->new_state = LASTACK_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDFIN_AC;
         rtn->n_actions = 1;
         break;
      default:
         //dbg(DBG_ALL, "from CLOSEWT state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void closing_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case RCVACKFIN_TR: // rcv ACK of FIN
         rtn->new_state = TIMEWT_ST;
         rtn->actions = NULL;
         rtn->n_actions = 0;
         break;
      default:
         dbg(DBG_ALL, "from CLOSING state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void lastack_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case RCVACKFIN_TR: // rcv ACK of FIN
         rtn->new_state = CLOSED_ST;
         rtn->actions = NULL;
         rtn->n_actions = 0;
         break;
      default:
         dbg(DBG_ALL, "from LASTACK state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


void timewt_next(int trans, struct state_response *rtn) {
   switch(trans) {
      case RCVFIN_TR:
         rtn->new_state = TIMEWT_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = SENDACK_AC;
         rtn->n_actions = 1;
         break;
      case TIMEOUT_TR:
         rtn->new_state = CLOSED_ST;
         rtn->actions = (int *)malloc(sizeof(int));
         (rtn->actions)[0] = TCBDELETE_AC;
         rtn->n_actions = 1;
         break;
      default:
         dbg(DBG_ALL, "from TIMEWT state, bad transition: %d\n", trans);
         rtn->new_state = -1;
   }
}


/*
 * Initialize state machine
 */
void state_machine_init() {
   htable_init(&st_funcs, 11); // 11 possible states
   htable_put(&st_funcs, CLOSED_ST, &closed_next);
   htable_put(&st_funcs, LISTEN_ST, &listen_next);
   htable_put(&st_funcs, SYNRCVD_ST, &synrcvd_next);
   htable_put(&st_funcs, SYNSNT_ST, &synsnt_next);
   htable_put(&st_funcs, ESTAB_ST, &estab_next);
   htable_put(&st_funcs, FINWT1_ST, &finwt1_next);
   htable_put(&st_funcs, FINWT2_ST, &finwt2_next);
   htable_put(&st_funcs, CLOSEWT_ST, &closewt_next);
   htable_put(&st_funcs, CLOSING_ST, &closing_next);
   htable_put(&st_funcs, LASTACK_ST, &lastack_next);
   htable_put(&st_funcs, TIMEWT_ST, &timewt_next);
}


/*
 * When other code calls next_state, it should call this function after processing
 * the response, in order to free the memory associated with the state_response
 * structure.
 */
void free_response(struct state_response *r) {
   free(r->actions);
   free(r);
}
