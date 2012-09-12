#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


typedef struct state_response {
   int new_state;
   int *actions;
   int n_actions;
} state_response;


//typedef void (*handler_t)(char *packet);
typedef void (*st_func_t)(int trans, struct state_response *rtn);


/*
 * States
 */
#define CLOSED_ST 0
#define LISTEN_ST 1
#define SYNRCVD_ST 2
#define SYNSNT_ST 3
#define ESTAB_ST 4
#define FINWT1_ST 5
#define FINWT2_ST 6
#define CLOSEWT_ST 7
#define CLOSING_ST 8
#define LASTACK_ST 9
#define TIMEWT_ST 10


/*
 * Transitions
 */
#define CLOSE_TR 0
#define PSVOPEN_TR 1
#define ACTOPEN_TR 2
#define RCVSYN_TR 3
#define SEND_TR 4
#define RCVSYNACK_TR 5  // rcv SYN, ACK
#define RCVACKSYN_TR 6  // rcv ACK of SYN
#define RCVFIN_TR 7
#define RCVACKFIN_TR 8  // rcv ACK of FIN
#define TIMEOUT_TR 9


/*
 * Actions
 */
#define TCBCREATE_AC 1
#define TCBDELETE_AC 2
#define SENDSYN_AC 3
#define SENDACK_AC 4 
#define SENDFIN_AC 5


/*
 * Initialize state machine
 */
void state_machine_init();


/*
 * Given a connection's current state and the transition code, returns a pointer to a 
 * state_response struct containing the new state and any actions to take.
 */
struct state_response *next_state(int cur_state, int trans);


/*
 * When other code calls next_state, it should call this function after processing
 * the response, in order to free the memory associated with the state_response
 * structure.
 */
void free_response(struct state_response *r);


#endif
