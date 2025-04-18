/**
 * Functions delegated out to platform-specific code.
 */
extern int string_to_int(char* string);
extern void int_to_string(int i, char* destination);

/**
 * Buffer for the web app to write the plates user input.
 *
 * Example of the longest likely string is 30 characters:
 * "45,45,45,45,45,45,25,10,5,2.5\0"
 *
 * So we'll just double it to 60.
 */
char js_plates_string_buffer[60] = { 0 };

/**
 * Buffer for the web app to write the conversion from int to string.
 *
 * This will usually be a number no greater than "670\0".
 */
char js_int_to_string_buffer[8] = { 0 };

static char calc_weight_from_plates_dest[8] = { 0 };
static void clear_calc_weight_from_plates_dest(void) {
  calc_weight_from_plates_dest[0] = 0;
  calc_weight_from_plates_dest[1] = 0;
  calc_weight_from_plates_dest[2] = 0;
  calc_weight_from_plates_dest[3] = 0;
  calc_weight_from_plates_dest[4] = 0;
  calc_weight_from_plates_dest[5] = 0;
  calc_weight_from_plates_dest[6] = 0;
  calc_weight_from_plates_dest[7] = 0;
}
char* calc_weight_from_plates(char* plates) {
  clear_calc_weight_from_plates_dest();

  // TODO: Convert plates to array of strings first! Temporarily passing as is.
  int i = string_to_int(plates);
  const int plates_total_weight = i * 2;
  const int bar_weight = 45;
  const int total_weight = bar_weight + plates_total_weight;

  int_to_string(total_weight, calc_weight_from_plates_dest);
  return calc_weight_from_plates_dest;
}
