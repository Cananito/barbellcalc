#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(void) {
  // TODO: Hardcoding for now, need to mplement command and input parsing!
  char* weight = calc_plates_to_weight("45,45");
  printf("%s\n", weight);
  return 0;
}
