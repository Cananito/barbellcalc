#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

static void help(void) {
  printf("TODO: Implement help!\n");
}

static char* short_opts = "vh";
static struct option long_opts[] = {
  { .name = "version", .has_arg = no_argument, .flag = 0, .val = 'v' },
  { .name = "help", .has_arg = no_argument, .flag = 0, .val = 'h' },
  { 0, 0, 0, 0 },
};

int main(int argc, char** argv) {
  if (argc < 2) {
    // If no arguments, print out help.
    help();
    return 0;
  }

  while (1) {
    int option_index = 0;
    int val = getopt_long(argc, argv, short_opts, long_opts, &option_index);

    if (val == -1) {
      // Nothing left.
      break;
    }

    switch (val) {
      case 0: {
        // TODO: Hardcoding and unreachable for now.
        char* weight = calc_plates_to_weight("45,45");
        printf("%s\n", weight);
        break;
      }
      case 'v': {
        printf("Version 1.0\n");
        break;
      }
      case 'h': {
        help();
        break;
      }
      case '?': { // Unrecognized option.
        exit(EXIT_FAILURE);
        break;
      }
    }
  }

  return 0;
}
