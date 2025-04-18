#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

int string_to_int(char* string) {
  char* end;
  long result = strtol(string, &end, 10);
  // TODO: Add error checking!
  return (int)result;
}

void int_to_string(int i, char* destination) {
  sprintf(destination, "%d", i);
}

int main(void) {
  char* weight = calc_weight_from_plates("45,45");
  printf("%s\n", weight);
  return 0;
}
