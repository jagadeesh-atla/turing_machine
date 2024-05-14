#include "utm.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int state_index(turing_t *t, char *state) {
  for (int i = 0; i < t->states_len; i++) {
    if (!strcmp(t->states[i], state)) {
      return i;
    }
  }

  return 0;
}

int symbol_index(turing_t *t, char symbol) {
  for (int i = 0; i < t->symbols_len; i++) {
    if (t->symbols[i] == symbol) {
      return i;
    }
  }

  return 0;
}

void move(turing_t *t, int dir) {
  tape_t *orig = t->tape;
  if (dir == RIGHT) {
    if (orig && orig->right) {
      t->tape = orig->right;
    } else {
      t->tape = calloc(1, sizeof(tape_t));
      t->tape->symbol = t->blank;
      if (orig) {
        t->tape->left = orig;
        orig->right = t->tape;
      }
    }
  } else if (dir == LEFT) {
    if (orig && orig->left) {
      t->tape = orig->left;
    } else {
      t->tape = calloc(1, sizeof(tape_t));
      t->tape->symbol = t->blank;
      if (orig) {
        t->tape->right = orig;
        orig->left = t->tape;
      }
    }
  }
}

turing_t *create(int states_len, ...) {
  va_list args;
  va_start(args, states_len);

  turing_t *t = malloc(sizeof(turing_t));

  t->states_len = states_len;
  t->states = malloc(states_len * sizeof(char *));
  for (int i = 0; i < states_len; i++) {
    t->states[i] = va_arg(args, char *);
  }

  t->final_states_len = va_arg(args, int);
  t->final_states = malloc(t->final_states_len * sizeof(int));
  for (int i = 0; i < t->final_states_len; i++) {
    t->final_states[i] = state_index(t, va_arg(args, char *));
  }

  t->symbols_len = va_arg(args, int);
  t->symbols = malloc(t->symbols_len * sizeof(char));
  for (int i = 0; i < t->symbols_len; i++) {
    t->symbols[i] = va_arg(args, int);
  }

  t->blank = symbol_index(t, va_arg(args, int));
  t->state = state_index(t, va_arg(args, char *));

  t->tape_len = va_arg(args, int);
  t->inital_tape = malloc(t->tape_len * sizeof(char));
  t->tape = NULL;
  for (int i = 0; i < t->tape_len; i++) {
    move(t, RIGHT);
    char symbol = va_arg(args, int);
    t->tape->symbol = symbol_index(t, symbol);
    t->inital_tape[i] = symbol;
  }
  if (!t->tape_len) {
    move(t, RIGHT);
  }

  while (t->tape->left) {
    t->tape = t->tape->left;
  }

  t->transitions_len = va_arg(args, int);
  t->transitions = malloc(t->states_len * sizeof(transition_t **));
  for (int i = 0; i < t->states_len; i++) {
    t->transitions[i] = malloc(t->symbols_len * sizeof(transition_t *));
  }
  for (int i = 0; i < t->transitions_len; i++) {
    transition_t *tran = malloc(sizeof(transition_t));
    tran->state1 = state_index(t, va_arg(args, char *));
    tran->symbol1 = symbol_index(t, va_arg(args, int));
    tran->symbol2 = symbol_index(t, va_arg(args, int));
    tran->dir = va_arg(args, int);
    tran->state2 = state_index(t, va_arg(args, char *));
    t->transitions[tran->state1][tran->symbol1] = tran;
  }

  va_end(args);

  return t;
}

void print_state(turing_t *t, long long step) {
  printf("%-10ld: ", step);
  printf("%-10s ", t->states[t->state]);
  tape_t *tape = t->tape;
  while (tape->left) {
    tape = tape->left;
  }

  while (tape) {
    if (tape == t->tape) {
      printf("[%c]", t->symbols[tape->symbol]);
    } else {
      printf(" %c ", t->symbols[tape->symbol]);
    }
    tape = tape->right;
  }
  printf("\n");
}

void run(turing_t *t) {
  long long step = 0;
  while (1) {
    step++;
    print_state(t, step);
    for (int i = 0; i < t->final_states_len; i++) {
      if (t->final_states[i] == t->state) {
        printf("\n");
        printf("=--------------------------------------------=\n");
        printf("[REPORT]:\n");
        printf("\tINITIAL TAPE: %s\n", t->inital_tape);
        printf("\tNo. of steps: %ld\n", step);
        printf("\tFINAL TAPE  : ");
        tape_t *tape = t->tape;
        while (tape->left) {
          tape = tape->left;
        }
        while (tape) {
          printf("%c", t->symbols[tape->symbol]);
          tape = tape->right;
        }
        printf("\n");
        printf("=--------------------------------------------=\n");
        return;
      }
    }

    transition_t *tran = t->transitions[t->state][t->tape->symbol];
    t->tape->symbol = tran->symbol2;
    move(t, tran->dir);
    t->state = tran->state2;
  }
}
