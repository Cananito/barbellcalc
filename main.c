#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(void) {
  char* weight = calc_plates_to_weight("45,45");
  printf("%s\n", weight);
  return 0;
}
