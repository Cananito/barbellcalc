#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"

// TODO: Create man page.

static void print_usage(void) {
  printf("Usage:\n");
  printf("  barbellcalc [-w weight | --weight weight | -p plates | "
         "--plates plates]\n");
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
        char* plates = "";
        if (optarg && strlen(optarg) <= 58) {
          plates = optarg;
        }
        char* weight = calc_plates_to_weight(plates);
        printf("Weight: %s lbs\n", weight);
        return 0;
      }
      case 'p': {
        char* weight = "";
        if (optarg && strlen(optarg) <= 58) {
          weight = optarg;
        }
        char* plates = calc_weight_to_plates(weight);
        printf("Plates: %s\n", plates);
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
