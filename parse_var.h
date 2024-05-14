#ifndef PARSE_VAR_
#define PARSE_VAR_
#include <stdlib.h>

#define MAX_LINE_LENGTH 256
#define MAX_STATES 100
#define MAX_SYMBOLS 128
#define MAX_TAPE_LENGTH 1024
#define MAX_TRANSITIONS (MAX_LINE_LENGTH * MAX_STATES)

extern const char *keywords[];
extern char **states;   // store list of states
extern int len_states;  // length of states

extern int *final_states;      // store list of final states indices
extern char **final_states_c;  // store list of final states
extern int len_final_states;   // length of final states

extern char *symbols;    // store list of symbols
extern int len_symbols;  // length of symbols

extern char blank;  // blank
extern int is_blank;

extern int initial_state;  // inital state index

extern int len_tape;
extern char *tape;  // store list of tape

typedef struct {
  int state1;
  int symbol1;
  int symbol2;
  int dir;
  int state2;
} transition_parse;  // transition structure

extern transition_parse *transitions;  // store list of transitions
extern int len_transitions;

#endif  // PARSER_VAR_