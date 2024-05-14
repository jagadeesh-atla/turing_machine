#include "parse.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

const char* keywords[] = {
    "STATES",        "FINAL STATES", "SYMBOLS",     "BLANK",
    "INITIAL STATE", "TAPE",         "TRANSITIONS",
};  // list of keywords

char** states = NULL;  // store list of states
int len_states = 0;    // length of states

int* final_states = NULL;      // store list of final states indices
char** final_states_c = NULL;  // store list of final states
int len_final_states = 0;      // length of final states

char* symbols = NULL;  // store list of symbols
int len_symbols = 0;   // length of symbols

char blank = '\0';  // blank
int is_blank = -1;

int initial_state = -1;  // inital state index

int len_tape = 0;
char* tape = NULL;  // store list of tape

transition_parse* transitions = NULL;  // store list of transitions
int len_transitions = 0;

void trim(char* s) {
  int l = strlen(s);
  while (l > 0 && isspace(s[l - 1])) s[--l] = '\0';
  char* p = s;
  while (*p && isspace(*p)) p++, l--;
  strcpy(s, p);
}

void remove_comments(char* s) {
  char* p = s;
  char prev = '\0';
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] == prev && prev == ';') {
      p[i - 1] = '\0';
      break;
    }
    prev = s[i];
  }
  strcpy(s, p);
}

int keyword_index(char* kw) {
  trim(kw);
  int len = sizeof(keywords) / sizeof(char*);
  for (int i = 0; i < len; i++)
    if (strcmp(kw, keywords[i]) == 0) return i;
  return -1;
}

int parse_state_index(char* state) {
  for (int i = 0; i < len_states; i++)
    if (strcmp(states[i], state) == 0) return i;
  return -1;
}

int parse_symbol_index(char symbol) {
  for (int i = 0; i < len_symbols; i++)
    if (symbol == symbols[i]) return i;
  return -1;
}

int parse_states(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  //   printf("States\n");
  long pos = ftell(f);
  states = malloc(sizeof(char*) * MAX_STATES);
  int len = 0;
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    if (keyword_index(line) != -1) {
      // STATES completed
      fseek(f, pos, SEEK_SET);
      states = realloc(states, len * sizeof(char*));
      len_states = len;
      //   for (int i = 0; i < len; i++) {
      //     printf("%s\t", states[i]);
      //   }
      //   printf("\n");
      free(line);
      return 0;
    }
    pos = ftell(f);
    char* pch = strtok(line, " ");
    while (pch != NULL) {
      //   printf("%s\n", pch);
      states[len] = malloc(strlen(pch) + 1);
      strcpy(states[len++], pch);
      trim(states[len - 1]);
      pch = strtok(NULL, " ");
    }
  }
  states = realloc(states, len * sizeof(char*));
  len_states = len;
  //   for (int i = 0; i < len; i++) {
  //     printf("%s\t", states[i]);
  //   }
  //   printf("\n");
  free(line);
  return 0;
}

int parse_final_states(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  //   printf("Final States\n");
  long pos = ftell(f);
  final_states_c = malloc(sizeof(char*) * MAX_STATES);
  int len = 0;
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    if (keyword_index(line) != -1) {
      // FINAL STATES completed
      fseek(f, pos, SEEK_SET);
      final_states = realloc(final_states, sizeof(int) * len);
      len_final_states = len;
      int idx = 0;
      for (int i = 0; i < len; i++) {
        idx = parse_state_index(final_states_c[i]);
        if (idx == -1) {
          fprintf(stderr, "Error: Invalid state - %s\n", final_states_c[i]);

          return 1;
        }
        // printf("%s\t", final_states_c[i]);
        final_states[i] = idx;
        // printf("%s\t", states[final_states[i]]);
      }
      //   printf("\n");
      free(final_states_c);
      free(line);
      return 0;
    }
    pos = ftell(f);
    char* pch = strtok(line, " ");
    while (pch != NULL) {
      //   printf("%s\n", pch);
      final_states_c[len] = malloc(strlen(pch) + 1);
      strcpy(final_states_c[len++], pch);
      trim(final_states_c[len - 1]);
      pch = strtok(NULL, " ");
    }
  }

  final_states = realloc(final_states, sizeof(int) * len);
  len_final_states = len;
  int idx = 0;
  for (int i = 0; i < len; i++) {
    idx = parse_state_index(final_states_c[i]);
    if (idx == -1) {
      fprintf(stderr, "Error: Invalid state - %s\n", final_states_c[i]);

      return 1;
    }
    // printf("%s\t", final_states_c[i]);
    final_states[i] = idx;
    // printf("%s\t", states[final_states[i]]);
  }
  //   printf("\n");

  free(final_states_c);
  free(line);

  return 0;
}

int parse_symbols(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  //   printf("Symbols\n");
  long pos = ftell(f);
  symbols = malloc(sizeof(char) * MAX_SYMBOLS);
  int len = 0;
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    if (keyword_index(line) != -1) {
      // SYMBOLS completed
      fseek(f, pos, SEEK_SET);
      symbols = realloc(symbols, len * sizeof(char));
      len_symbols = len;
      //   for (int i = 0; i < len; i++) {
      //     printf("%c\t", symbols[i]);
      //   }
      //   printf("\n");
      free(line);
      return 0;
    }
    pos = ftell(f);
    char* pch = strtok(line, " ");
    while (pch != NULL) {
      //   printf("%s\n", pch);
      trim(pch);
      symbols[len++] = pch[0];
      pch = strtok(NULL, " ");
    }
  }
  symbols = realloc(symbols, len * sizeof(char));
  len_symbols = len;
  //   for (int i = 0; i < len; i++) {
  //     printf("%c\t", symbols[i]);
  //   }
  //   printf("\n");
  free(line);
  return 0;
}

int parse_blank(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  //   printf("Blank\n");
  long pos = ftell(f);
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    if (keyword_index(line) != -1) {
      // BLANK completed
      fseek(f, pos, SEEK_SET);
      if (is_blank == -1) {
        do {
          printf("Please enter a blank symbol: ");
          scanf("%c", &blank);
        } while (parse_symbol_index(blank) != -1);
      };
      //   printf("%c\n", blank);
      free(line);
      return 0;
    }
    pos = ftell(f);
    char* pch = strtok(line, " ");
    while (pch != NULL) {
      //   printf("%s\n", pch);
      trim(pch);
      is_blank = blank = pch[0];
      if (parse_symbol_index(blank) == -1) {
        fprintf(stderr, "Error: Invalid Blank symbol - %c\n", blank);
        return 1;
      }
      pch = strtok(NULL, " ");
    }
  }
  if (is_blank == -1) {
    do {
      printf("Please enter a blank symbol: ");
      scanf("%c", &blank);
    } while (parse_symbol_index(blank) != -1);
  };
  //   printf("%c\n", blank);
  free(line);
  return 0;
}

int parse_initial_state(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  //   printf("Inital States\n");
  long pos = ftell(f);
  char* state = malloc(MAX_LINE_LENGTH * sizeof(char));
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    if (keyword_index(line) != -1) {
      // INITAL STATE completed
      fseek(f, pos, SEEK_SET);
      if (initial_state == -1) {
        do {
          printf("Please enter a initial state: ");
          scanf("%s", &state);
        } while (parse_state_index(state) != -1);
        initial_state = parse_state_index(state);
      }
      //   printf("%s\n", state);
      free(line);
      return 0;
    }
    pos = ftell(f);
    char* pch = strtok(line, " ");
    while (pch != NULL) {
      //   printf("%s\n", pch);
      trim(pch);
      if (parse_state_index(pch) == -1) {
        fprintf(stderr, "Error: Invalid State - %s\n", pch);
        return 1;
      }
      strcpy(state, pch);
      initial_state = parse_state_index(state);
      pch = strtok(NULL, " ");
    }
  }
  if (initial_state == -1) {
    do {
      printf("Please enter a initial state: ");
      scanf("%s", &state);
    } while (parse_state_index(state) != -1);
    initial_state = parse_state_index(state);
  }
  //   printf("%s\n", state);
  free(line);
  return 0;
}

int parse_tape(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  //   printf("Tape\n");
  long pos = ftell(f);
  tape = malloc(sizeof(char) * MAX_TAPE_LENGTH);
  int len = 0;
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    if (keyword_index(line) != -1) {
      // TAPE completed
      fseek(f, pos, SEEK_SET);
      //   printf("%s:%d\n", tape, len_tape);
      free(line);
      return 0;
    }
    pos = ftell(f);
    for (int i = 0; i < strlen(line); i++) {
      if (parse_symbol_index(line[i]) == -1) {
        fprintf(stderr, "Error: Invalid tape symbol %c\n", line[i]);
        return 1;
      }
    }
    strcpy(tape, line);
    len_tape = strlen(tape);
  }
  //   printf("%s:%d\n", tape, len_tape);
  free(tape);
  return 0;
}

int parse_transitions(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  //   printf("Transitions\n");
  long pos = ftell(f);
  transitions = malloc(sizeof(transition_parse) * MAX_TRANSITIONS);
  int len = 0;
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    if (keyword_index(line) != -1) {
      // TRANSITIONS completed
      fseek(f, pos, SEEK_SET);
      transitions = realloc(transitions, sizeof(transition_parse) * len);
      len_transitions = len;
      //   for (int i = 0; i < len; i++) {
      //     transition_parse x = transitions[i];
      //     char dir = (x.dir == 0 ? '<' : (x.dir == 1 ? '>' : '='));
      //     printf("%s %c %c %c %s\n", states[x.state1], symbols[x.symbol1],
      //            symbols[x.symbol2], dir, states[x.state2]);
      //   }
      free(line);
      return 0;
    }
    pos = ftell(f);
    trim(line);
    char *curr_state, *curr_symbol, *new_symbol, *dir, *new_state;
    // Tokenize the line
    char* pch = strtok(line, " ");
    trim(pch);
    curr_state = strdup(pch);
    pch = strtok(NULL, " ");
    trim(pch);
    curr_symbol = strdup(pch);
    pch = strtok(NULL, " ");
    trim(pch);
    new_symbol = strdup(pch);
    pch = strtok(NULL, " ");
    trim(pch);
    dir = strdup(pch);
    pch = strtok(NULL, " ");
    trim(pch);
    new_state = strdup(pch);
    if (strcmp(curr_state, "*") != 0 && parse_state_index(curr_state) == -1) {
      fprintf(stderr, "Error: Invalid current state - %s\n", curr_state);
      return 1;
    }
    if (strcmp(curr_symbol, "*") != 0 &&
        parse_symbol_index(curr_symbol[0]) == -1) {
      fprintf(stderr, "Error: Invalid current symbol - %s\n", curr_symbol);
      return 1;
    }
    if (strcmp(new_symbol, "*") != 0 &&
        parse_symbol_index(new_symbol[0]) == -1) {
      fprintf(stderr, "Error: Invalid new symbol - %s\n", new_symbol);
      return 1;
    }
    if (strcmp(new_state, "*") != 0 && parse_state_index(new_state) == -1) {
      fprintf(stderr, "Error: Invalid new state - %s\n", new_state);
      return 1;
    }
    if (strcmp(dir, ">") != 0 && strcmp(dir, "<") != 0 &&
        strcmp(dir, "=") != 0) {
      fprintf(stderr, "Error: Invalid direction - %s\n", dir);
      return 1;
    }
    if (strcmp(curr_state, "*") == 0 && strcmp(curr_symbol, "*") == 0) {
      for (int j = 0; j < len_symbols; j++) {
        for (int i = 0; i < len_states; i++) {
          transitions[len++] = (transition_parse){
              .state1 = i,
              .symbol1 = j,
              .symbol2 = (strcmp(new_symbol, "*") == 0
                              ? j
                              : parse_symbol_index(new_symbol[0])),
              .dir = (dir[0] == '=' ? 2 : (dir[0] == '>' ? 1 : 0)),
              .state2 =
                  (strcmp(new_state, "*") == 0 ? i
                                               : parse_state_index(new_state)),
          };
        }
      }
    } else if (strcmp(curr_state, "*") == 0) {
      // Insert transitions for every state
      for (int i = 0; i < len_states; i++) {
        transitions[len++] = (transition_parse){
            .state1 = i,
            .symbol1 = parse_symbol_index(curr_symbol[0]),
            .symbol2 = (strcmp(new_symbol, "*") == 0
                            ? parse_symbol_index(curr_symbol[0])
                            : parse_symbol_index(new_symbol[0])),
            .dir = (dir[0] == '=' ? 2 : (dir[0] == '>' ? 1 : 0)),
            .state2 =
                (strcmp(new_state, "*") == 0 ? i
                                             : parse_state_index(new_state)),
        };
      }
    } else if (strcmp(curr_symbol, "*") == 0) {
      // Insert transitions for every symbol
      for (int i = 0; i < len_symbols; i++) {
        transitions[len++] = (transition_parse){
            .state1 = parse_state_index(curr_state),
            .symbol1 = i,
            .symbol2 = (strcmp(new_symbol, "*") == 0
                            ? i
                            : parse_symbol_index(new_symbol[0])),
            .dir = (dir[0] == '=' ? 2 : (dir[0] == '>' ? 1 : 0)),
            .state2 =
                (strcmp(new_state, "*") == 0 ? parse_state_index(curr_state)
                                             : parse_state_index(new_state)),
        };
      }
    } else {
      transitions[len++] = (transition_parse){
          .state1 = parse_state_index(curr_state),
          .symbol1 = parse_symbol_index(curr_symbol[0]),
          .symbol2 = (strcmp(new_symbol, "*") == 0
                          ? parse_symbol_index(curr_state[0])
                          : parse_symbol_index(new_symbol[0])),
          .dir = (dir[0] == '=' ? 2 : (dir[0] == '>' ? 1 : 0)),
          .state2 =
              (strcmp(new_state, "*") == 0 ? parse_state_index(curr_state)
                                           : parse_state_index(new_state)),
      };
    }
  }
  transitions = realloc(transitions, sizeof(transition_parse) * len);
  len_transitions = len;
  //   for (int i = 0; i < len; i++) {
  //     transition_parse x = transitions[i];
  //     char dir = (x.dir == 0 ? '<' : (x.dir == 1 ? '>' : '='));
  //     printf("%s %c %c %c %s\n", states[x.state1], symbols[x.symbol1],
  //            symbols[x.symbol2], dir, states[x.state2]);
  //   }
  free(line);
  return 0;
}

int check_all() {
  if (len_states == 0) return 1;
  if (len_final_states == 0) return 1;
  if (len_symbols == 0) return 1;
  if (is_blank == -1) return 1;
  if (initial_state == -1) return 1;
  if (len_tape == 0) return 1;
  if (len_transitions == 0) return 1;

  return 0;
}

int helper(FILE* f) {
  char* line = malloc(MAX_LINE_LENGTH);
  char* uncommented_file = "uncommented.tula";
  FILE* fc = fopen(uncommented_file, "w");
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    remove_comments(line);
    trim(line);
    if (strcmp(line, "") == 0) continue;
    fwrite(line, 1, strlen(line), fc);
    fputc('\n', fc);
  }
  fclose(f);
  fclose(fc);
  f = fopen(uncommented_file, "r");
  while (fgets(line, MAX_LINE_LENGTH, f) != NULL) {
    int idx = keyword_index(line);
    // printf("%d: %s\n", idx, line);
    int rt = 0;
    switch (idx) {
      case 0: {
        rt = parse_states(f);
        if (rt != 0) return 1;
        break;
      }
      case 1: {
        rt = parse_final_states(f);
        if (rt != 0) return 1;
        break;
      }
      case 2: {
        rt = parse_symbols(f);
        if (rt != 0) return 1;
        break;
      }
      case 3: {
        rt = parse_blank(f);
        if (rt != 0) return 1;
        break;
      }
      case 4: {
        rt = parse_initial_state(f);
        if (rt != 0) return 1;
        break;
      }
      case 5: {
        rt = parse_tape(f);
        if (rt != 0) return 1;
        break;
      }
      case 6: {
        rt = parse_transitions(f);
        if (rt != 0) return 1;
        break;
      }
      default:
        fprintf(stderr, "Error: Invalid keyword %s", line);
        return 1;
    }
  }
  fclose(f);
  remove(uncommented_file);
}

turing_t* parse_turning_machine(FILE* f) {
  if (helper(f) != 0) {
    fprintf(stderr, "Error: Something is wrong...\n");
    exit(1);
  }

  if (check_all() != 0) {
    fprintf(stderr, "Error: Something is missing in the file.\n");
    exit(1);
  };

  turing_t* t = malloc(sizeof(turing_t));

  t->states_len = len_states;
  t->states = malloc(len_states * sizeof(char*));
  for (int i = 0; i < len_states; i++) {
    t->states[i] = states[i];
  }

  t->final_states_len = len_final_states;
  t->final_states = malloc(len_final_states * sizeof(int));
  for (int i = 0; i < len_final_states; i++) {
    t->final_states[i] = final_states[i];
  }

  t->symbols_len = len_symbols;
  t->symbols = malloc(len_symbols * sizeof(char));
  for (int i = 0; i < len_symbols; i++) {
    t->symbols[i] = symbols[i];
  }

  t->blank = parse_symbol_index(blank);
  t->state = initial_state;

  t->tape_len = len_tape;
  t->tape = NULL;
  t->inital_tape = strdup(tape);
  for (int i = 0; i < len_tape; i++) {
    move(t, RIGHT);
    t->tape->symbol = parse_symbol_index(tape[i]);
  }
  if (!t->tape_len) {
    move(t, RIGHT);
  }

  while (t->tape->left) {
    t->tape = t->tape->left;
  }

  t->transitions_len = len_transitions;
  t->transitions = malloc(t->states_len * sizeof(transition_t**));

  for (int i = 0; i < t->states_len; i++) {
    t->transitions[i] = malloc(t->symbols_len * sizeof(transition_t*));
    for (int j = 0; j < t->symbols_len; j++) {
      t->transitions[i][j] = NULL;
    }
  }

  for (int i = 0; i < len_transitions; i++) {
    transition_t* tran = malloc(sizeof(transition_t));
    tran->state1 = transitions[i].state1;
    tran->symbol1 = transitions[i].symbol1;
    tran->symbol2 = transitions[i].symbol2;
    tran->dir = transitions[i].dir;
    tran->state2 = transitions[i].state2;

    if (t->transitions[tran->state1][tran->symbol1] == NULL) {
      t->transitions[tran->state1][tran->symbol1] = tran;
    } else {
      free(tran);
    }
  }

  return t;
}
