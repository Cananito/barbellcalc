#include <stdio.h>

#include "calc.h"

void Java_com_cananito_barbellcalc_MainActivity_lala() {
  char const* r = calc_weight_to_plates("155");
  printf("Result: %s", r);
}
