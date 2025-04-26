#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

static void print_usage(void) {
  printf("TODO: Implement help!\n");
}

static char* short_opts = "vhw:p:";
static struct option long_opts[] = {
  { .name = "version", .has_arg = no_argument, .flag = 0, .val = 'v' },
  { .name = "help", .has_arg = no_argument, .flag = 0, .val = 'h' },
  { .name = "weight", .has_arg = required_argument, .flag = 0, .val = 'w' },
  { .name = "plates", .has_arg = required_argument, .flag = 0, .val = 'p' },
  { 0, 0, 0, 0 },
};

int main(int argc, char** argv) {
  int val;
  while ((val = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
    switch (val) {
      case 'v': {
        printf("Version 1.0\n");
        return 0;
      }
      case 'h': {
        print_usage();
        return 0;
      }
      case 'w': {
        // TODO: Override value to empty string if input is more than 58 chars.
        // TODO: Hardcode for now.
        char* weight = calc_plates_to_weight("45,45");
        printf("%s lbs\n", weight);
        return 0;
      }
      case 'p': {
        // TODO: Override value to empty string if input is more than 58 chars.
        // TODO: Hardcode for now.
        char* plates = calc_weight_to_plates("235");
        printf("%s\n", plates);
        return 0;
      }
      case '?': { // Unrecognized option.
        print_usage();
        return 0;
      }
      default: {
        print_usage();
        return 0;
      }
    }
  }
  // If no arguments, print usage.
  print_usage();
  return 0;
}
