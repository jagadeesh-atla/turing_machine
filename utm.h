#ifndef UTM_H_
#define UTM_H_

enum direction_e {
  LEFT,
  RIGHT,
  STAY,
};

typedef struct {
  int state1;
  int symbol1;
  int symbol2;
  int dir;
  int state2;
} transition_t;

typedef struct tape_t tape_t;
struct tape_t {
  int symbol;
  tape_t *left;
  tape_t *right;
};

typedef struct {
  int states_len;
  char **states;
  int final_states_len;
  int *final_states;
  int symbols_len;
  char *symbols;
  int blank;
  int state;
  int tape_len;
  char *inital_tape;
  tape_t *tape;
  int transitions_len;
  transition_t ***transitions;
} turing_t;

int state_index(turing_t *t, char *state);
int symbol_index(turing_t *t, char symbol);
void move(turing_t *t, int dir);
turing_t *create(int states_len, ...);
void print_state(turing_t *t, long long step);
void run(turing_t *t);

#endif  // UTM_H_