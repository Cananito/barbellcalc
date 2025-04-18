#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(void) {
  char* weight = calc_weight_from_plates("45,45");
  printf("%s\n", weight);
  return 0;
}
