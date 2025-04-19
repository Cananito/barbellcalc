/**
 * Buffer for the web app to write the plates user input.
 *
 * Example of the longest likely string is 30 characters:
 * "45,45,45,45,45,45,25,10,5,2.5\0"
 *
 * So we'll just double it to 60.
 */
char js_text_input_string_buffer[60] = { 0 };

static void r_int_to_string(int v, char* dest) {
  int i = 0;
  int curr = v;

  // Minus sign.
  if (curr < 0) {
    dest[i] = '-';
    curr = -curr;
    i++;
  }

  // Build string in reverse order.
  int j = 0;
  char reverse_num_str[10];
  while (curr > 0) {
    char ascii = (curr % 10) + 48;
    reverse_num_str[j] = ascii;
    j++;
    curr /= 10;
  }
  j--;

  // Transfer over in correct order.
  while (j >= 0) {
    dest[i] = reverse_num_str[j];
    i++;
    j--;
  }

  // Null terminate.
  dest[i] = '\0';
}

static int r_str_len(char* str) {
  int len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

static int r_string_to_int(char* str) {
  int result = 0;
  int len = r_str_len(str);
  int is_negative = 0;
  for (int i = 0; i < len; i++) {
    char ascii = str[i];
    if (ascii == 45) {
      is_negative = 1;
      continue;
    }
    if (ascii < 48 || ascii > 57) {
      // Ignore this character and the remainder of the string.
      break;
    }
    result = (result * 10) + (ascii - 48);
  }
  if (is_negative) {
    result = -result;
  }
  return result;
}

static char calc_plates_to_weight_dest[8] = { 0 };
static void clear_calc_plates_to_weight_dest(void) {
  for (int i = 0; i < 8; i++) {
    calc_plates_to_weight_dest[i] = 0;
  }
}
char* calc_plates_to_weight(char* plates) {
  clear_calc_plates_to_weight_dest();

  // TODO: Convert plates to array of strings first! Temporarily passing as is.
  int i = r_string_to_int(plates);
  const int plates_total_weight = i * 2;
  const int bar_weight = 45;
  const int total_weight = bar_weight + plates_total_weight;

  r_int_to_string(total_weight, calc_plates_to_weight_dest);
  return calc_plates_to_weight_dest;
}

static char calc_weight_to_plates_dest[60] = { 0 };
static void clear_calc_weight_to_plates_dest(void) {
  for (int i = 0; i < 60; i++) {
    calc_plates_to_weight_dest[i] = 0;
  }
}
char* calc_weight_to_plates(__attribute__((unused)) char* weight) {
  clear_calc_weight_to_plates_dest();

  // TODO: Implement for real!
  calc_weight_to_plates_dest[0] = '4';
  calc_weight_to_plates_dest[1] = '5';
  calc_weight_to_plates_dest[2] = '\0';

  return calc_weight_to_plates_dest;
}
