#include <stdio.h>

#include "calc.h"

void Java_com_cananito_barbellcalc_MainActivity_lala() {
  char result[50] = { 0 };
  calc_weight_to_plates(result, "155");
  printf("Result: %s", result);
}
