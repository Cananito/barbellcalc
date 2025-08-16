// TODO: Add proper const modifiers.

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
      c == '!' ||
      c == '"' ||
      c == '#' ||
      c == '$' ||
      c == '%' ||
      c == '&' ||
      c == '*' ||
      c == '+' ||
      c == ',' ||
      c == '-' ||
      c == '/' ||
      c == ':' ||
      c == ';' ||
      c == '=' ||
      c == '?' ||
      c == '@' ||
      c == '\'' ||
      c == '\\' ||
      c == '^' ||
      c == '_' ||
      c == '`' ||
      c == '|' ||
      c == '~';
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
// More accurate version:
// https://github.com/R32/wasm-c/blob/daed77906372f5783b634494da09cfac7d1ea513/src/stdlib.c#L57
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
#pragma clang diagnostic pop

// More accurate version:
// https://github.com/R32/wasm-c/blob/daed77906372f5783b634494da09cfac7d1ea513/src/printf.c#L339
static void r_double_to_str(double v, char* dest) {
  int i = 0;
  int curr_integer = (int)v;
  // 1000 to get float precision of 3.
  int curr_decimal_value = ((double)(v - (int)v)) * 1000;

  // Minus sign.
  if (curr_integer < 0) {
    dest[i] = '-';
    curr_integer = -curr_integer;
    curr_decimal_value = -curr_decimal_value;
    i++;
  }

  int j = 0;
  char reverse_num_str[10];

  // Build the decimals parts of the string in reverse order.
  while (curr_decimal_value > 0) {
    char ascii = (curr_decimal_value % 10) + 48;
    if (ascii != '0') {
      reverse_num_str[j] = ascii;
      j++;
    }
    curr_decimal_value /= 10;
  }
  if (j > 0) {
    reverse_num_str[j] = '.';
    j++;
  }
  // Build the integers parts of the string in reverse order.
  while (curr_integer > 0) {
    char ascii = (curr_integer % 10) + 48;
    reverse_num_str[j] = ascii;
    j++;
    curr_integer /= 10;
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
// More accurate version:
// https://github.com/R32/wasm-c/blob/daed77906372f5783b634494da09cfac7d1ea513/src/printf.c#L281
static int r_str_to_int(char* str, int len) {
  int result = 0;
  int is_negative = 0;
  for (int i = 0; i < len; i++) {
    char ascii = str[i];
    if (ascii == ' ') {
      continue;
    }
    if (!is_negative && ascii == '-') {
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
#pragma clang diagnostic pop

// More accurate version:
// https://github.com/R32/wasm-c/blob/daed77906372f5783b634494da09cfac7d1ea513/src/stdlib.c#L13
static double r_str_to_double(char* str, int len) {
  double result = 0;
  char is_negative = 0;
  char processing_decimals = 0;
  double current_decimal_factor = 1;
  for (int i = 0; i < len; i++) {
    char ascii = str[i];
    if (ascii == ' ') {
      continue;
    }
    if (!is_negative && ascii == '-') {
      is_negative = 1;
      continue;
    }
    if (r_is_digit(ascii) && !processing_decimals) {
      result = (result * 10) + (ascii - 48);
    } else if (ascii == '.') {
      processing_decimals = 1;
    } else if (r_is_digit(ascii) && processing_decimals) {
      current_decimal_factor /= 10;
      result += (ascii - 48) * current_decimal_factor;
    } else {
      // Ignore this character and the remainder of the string.
      break;
    }
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

  double one_side_plates_total_weight = 0;
  char* curr_start = plates;
  char* curr_char = plates;
  int curr_len = 1;
  while (1) {
    if (r_is_digit(curr_char[0]) || curr_char[0] == '.') {
      curr_len++;
      curr_char++;
    } else if (r_is_separator(curr_char[0])) {
      one_side_plates_total_weight += r_str_to_double(curr_start, curr_len);
      curr_start = curr_start + curr_len;
      curr_char = curr_start;
      curr_len = 1;
    } else if (curr_char[0] == '\0') {
      one_side_plates_total_weight += r_str_to_double(curr_start, curr_len - 1);
      break;
    } else {
      // Invalid input, return "0".
      calc_plates_to_weight_dest[0] = '0';
      calc_plates_to_weight_dest[1] = '\0';
      return calc_plates_to_weight_dest;
    }
  }

  const double plates_total_weight = one_side_plates_total_weight * 2;
  const double bar_weight = 45;
  const double total_weight = bar_weight + plates_total_weight;

  r_double_to_str(total_weight, calc_plates_to_weight_dest);
  return calc_plates_to_weight_dest;
}

static char calc_weight_to_plates_dest[60] = { 0 };
static void clear_calc_weight_to_plates_dest(void) {
  for (int i = 0; i < 60; i++) {
    calc_weight_to_plates_dest[i] = 0;
  }
}
char* calc_weight_to_plates(char* weight) {
  clear_calc_weight_to_plates_dest();

  double curr_weight = r_str_to_double(weight, r_str_len(weight));

  if (curr_weight < 45) {
    return calc_weight_to_plates_dest;
  }

  curr_weight -= 45;
  curr_weight /= 2;
  int i = 0;
  while (curr_weight > 0) {
    if (i >= 59) {
      clear_calc_weight_to_plates_dest();
      return calc_weight_to_plates_dest;
    }
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
    } else if (curr_weight >= 2.5) {
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
