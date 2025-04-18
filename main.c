#include <stdio.h>

#include "calc.h"

int main(void) {
  char* weight = calc_weight_from_plates("45,45,25");
  printf("%s\n", weight);
  return 0;
}
