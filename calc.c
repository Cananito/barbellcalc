/**
 * Buffer for the web app to write the plates user input.
 *
 * Example of the longest likely string is 30 characters:
 * "45,45,45,45,45,45,25,10,5,2.5\0"
 *
 * So we'll just double it to 60.
 */
char js_text_input_string_buffer[60] = { 0 };

static int r_str_len(char* str) {
  int len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

static int r_is_digit(char c) {
  return c >= 48 && c <= 57;
}

static int r_is_separator(char c) {
  return
      c == ' ' ||
      c == ',' ||
      c == '|' ||
      c == '/' ||
      c == '\'' ||
      c == '-';
}

// TODO: Update to double!
static void r_int_to_str(int v, char* dest) {
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

// TODO: Update to double!
static int r_str_to_int(char* str, int len) {
  int result = 0;
  int is_negative = 0;
  for (int i = 0; i < len; i++) {
    char ascii = str[i];
    if (ascii == '-') {
      is_negative = 1;
      continue;
    }
    if (!r_is_digit(ascii)) {
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

  int one_side_plates_total_weight = 0;
  char* curr_start = plates;
  char* curr_char = plates;
  int curr_len = 1;
  while (1) {
    if (r_is_digit(curr_char[0])) {
      curr_len++;
      curr_char++;
    } else if (r_is_separator(curr_char[0])) {
      one_side_plates_total_weight += r_str_to_int(curr_start, curr_len);
      curr_start = curr_start + curr_len;
      curr_char = curr_start;
      curr_len = 1;
    } else if (curr_char[0] == '\0') {
      one_side_plates_total_weight += r_str_to_int(curr_start, curr_len - 1);
      break;
    } else {
      // Invalid input, return "0".
      calc_plates_to_weight_dest[0] = '0';
      calc_plates_to_weight_dest[1] = '\0';
      return calc_plates_to_weight_dest;
    }
  }

  const int plates_total_weight = one_side_plates_total_weight * 2;
  const int bar_weight = 45;
  const int total_weight = bar_weight + plates_total_weight;

  r_int_to_str(total_weight, calc_plates_to_weight_dest);
  return calc_plates_to_weight_dest;
}

static char calc_weight_to_plates_dest[60] = { 0 };
static void clear_calc_weight_to_plates_dest(void) {
  for (int i = 0; i < 60; i++) {
    calc_plates_to_weight_dest[i] = 0;
  }
}
char* calc_weight_to_plates(char* weight) {
  clear_calc_weight_to_plates_dest();

  int curr_weight = r_str_to_int(weight, r_str_len(weight));

  if (curr_weight < 45) {
    return calc_weight_to_plates_dest;
  }

  curr_weight -= 45;
  curr_weight /= 2;
  int i = 0;
  while (curr_weight > 0) {
    if (curr_weight >= 45) {
      calc_weight_to_plates_dest[i] = '4';
      i++;
      calc_weight_to_plates_dest[i] = '5';
      i++;
      calc_weight_to_plates_dest[i] = ',';
      i++;
      curr_weight -= 45;
    } else if (curr_weight >= 25) {
      calc_weight_to_plates_dest[i] = '2';
      i++;
      calc_weight_to_plates_dest[i] = '5';
      i++;
      calc_weight_to_plates_dest[i] = ',';
      i++;
      curr_weight -= 25;
    } else if (curr_weight >= 10) {
      calc_weight_to_plates_dest[i] = '1';
      i++;
      calc_weight_to_plates_dest[i] = '0';
      i++;
      calc_weight_to_plates_dest[i] = ',';
      i++;
      curr_weight -= 10;
    } else if (curr_weight >= 5) {
      calc_weight_to_plates_dest[i] = '5';
      i++;
      calc_weight_to_plates_dest[i] = ',';
      i++;
      curr_weight -= 5;
    } else if (curr_weight >= 2) { // TODO: Change to 2.5 after float support.
      calc_weight_to_plates_dest[i] = '2';
      i++;
      calc_weight_to_plates_dest[i] = '.';
      i++;
      calc_weight_to_plates_dest[i] = '5';
      i++;
      calc_weight_to_plates_dest[i] = ',';
      i++;
      curr_weight -= 2.5;
    } else {
      calc_weight_to_plates_dest[i] = '1';
      i++;
      calc_weight_to_plates_dest[i] = ',';
      i++;
      curr_weight -= 1;
    }
  }

  // If last char is a comma, replace it with null terminator. Otherwsie append.
  if (i > 0 && calc_weight_to_plates_dest[i-1] == ',') {
    calc_weight_to_plates_dest[i-1] = '\0';
  } else {
    calc_weight_to_plates_dest[i] = '\0';
  }

  return calc_weight_to_plates_dest;
}
