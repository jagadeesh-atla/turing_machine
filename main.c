#include <stdio.h>

#include "parse.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);

    return 1;
  }

  char* filename = argv[1];
  FILE* f;
  f = fopen(filename, "r");

  if (f == NULL) {
    fprintf(stderr, "Error opening file %s.\n", filename);
    return 1;
  }

  turing_t* t = parse_turning_machine(f);
  run(t);

  return 0;
}