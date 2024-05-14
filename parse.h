#ifndef PARSE_H_
#define PARSE_H_

#include <stdio.h>
#include <stdlib.h>

#include "parse_var.h"
#include "utm.h"

void trim(char *str);             // Remove leading/trailing whitespaces
void remove_comments(char *str);  // Remove comments (;;)

int keyword_index(char *keyword);     // Get index of a keyword or -1
int parse_state_index(char *state);   // Get index of a state or -1
int parse_symbol_index(char symbol);  // Get index of a symbol or -1

int parse_states(FILE *f);         // Parse states from file
int parse_final_states(FILE *f);   // Parse final states from file
int parse_symbols(FILE *f);        // Parse symbols from file
int parse_blank(FILE *f);          // Parse blank symbol from file
int parse_initial_state(FILE *f);  // Parse initial state from file
int parse_tape(FILE *f);           // Parse tape from file
int parse_transitions(FILE *f);    // Parse transitions from file

int check_all();  // Check all are available

int helper(FILE *f);                       // Helper function for parsing
turing_t *parse_turning_machine(FILE *f);  // Parse Turning Machine from file

#endif  // PARSE_H_
