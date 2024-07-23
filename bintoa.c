#include <stdlib.h>
#include <unistd.h>

void ft_putchar(char c) { write(1, &c, 1); }

void ft_putstr(char *str) {
  while (*str)
    ft_putchar(*str++);
}

void ft_putnbr(int n) {
  if (n >= 10)
    ft_putnbr(n / 10);
  ft_putchar(n % 10 + '0');
}

int ft_isspace(int c) { return ((c >= 9 && c <= 13) || c == 32); }

int ft_strlen(char *str) {
  int len;
  len = 0;
  while (str[len])
    len++;
  return (len);
}

void print_error(char *message) {
  ft_putstr("- Error: ");
  ft_putstr(message);
  ft_putchar('\n');
}

int is_valid_binary(char *str) {
  if (!str || !*str)
    return (1);
  while (*str) {
    if (*str != '0' && *str != '1' && !ft_isspace(*str))
      return (0);
    str++;
  }
  return (1);
}

int bin_to_dec(char *str, int len) {
  int result;
  int i;
  result = 0;
  i = 0;
  while (i < len && str[i] && !ft_isspace(str[i])) {
    result = (result * 2) + (str[i] - '0');
    i++;
  }
  return (result);
}

void print_ascii(int decimal, int bits, int index) {
  ft_putnbr(index);
  ft_putstr("- ");
  ft_putnbr(decimal);
  ft_putstr(" (");
  ft_putnbr(bits);
  ft_putstr(" bits) = ");
  if (decimal >= 32 && decimal <= 126) {
    ft_putchar('\'');
    ft_putchar((char)decimal);
    ft_putchar('\'');
  } else
    ft_putstr("non-printable");
  ft_putchar('\n');
}

int realloc_buffer(char **result, int *buffer_size, int needed_size) {
  if (*buffer_size >= needed_size) {
    return 1;
  }
  int new_size = *buffer_size;
  while (new_size < needed_size) {
    new_size += 2;
  }
  char *new_buffer = (char *)realloc(*result, new_size * sizeof(char));
  if (!new_buffer) {
    print_error("Memory allocation failed...");
    return 0;
  }
  *result = new_buffer;
  *buffer_size = new_size;
  return 1;
}

void process_binary_chunk(char *str, int len, char **result, int *result_index,
                          int *buffer_size, int chunk_index) {
  int decimal;
  decimal = bin_to_dec(str, len);
  print_ascii(decimal, len, chunk_index);
  if (decimal >= 32 && decimal <= 126) {
    if (!realloc_buffer(result, buffer_size, *result_index + 1))
      return;
  }
  (*result)[(*result_index)++] = (char)decimal;
}

int process_binary_string(char *str, char **result, int *result_index,
                          int *buffer_size, int *chunk_index) {
  int len;
  int chunk_size;
  while (*str) {
    while (ft_isspace(*str))
      str++;
    if (!*str)
      break;
    len = 0;
    while (str[len] && !ft_isspace(str[len]))
      len++;
    while (len > 0) {
      if (len > 8)
        chunk_size = 8;
      else
        chunk_size = len;
      process_binary_chunk(str, chunk_size, result, result_index, buffer_size,
                           (*chunk_index)++);
      str += chunk_size;
      len -= chunk_size;
    }
  }
  return (1);
}

int main(int argc, char **argv) {
  int i;
  char *result;
  int result_index;
  int buffer_size;
  int chunk_index;
  if (argc < 2) {
    print_error("Please enter any amount of binary strings...");
    return (1);
  }
  buffer_size = 64;
  result = (char *)malloc(buffer_size * sizeof(char));
  if (!result) {
    print_error("Memory allocation failed... closing the process.");
    return (1);
  }
  result_index = 0;
  chunk_index = 1;
  i = 1;
  while (i < argc) {

    if (!is_valid_binary(argv[i])) {
      ft_putnbr(chunk_index++);
      print_error("- Invalid binary input");
    } else {
      if (!argv[i][0]) {
        ft_putnbr(chunk_index++);
        ft_putstr("- Empty string: \"\"\n");
      }
      if (!process_binary_string(argv[i], &result, &result_index, &buffer_size,
                                 &chunk_index)) {
        free(result);

        return (1);
      }
    }
    i++;
  }
  result[result_index] = '\0';
  if (result_index == 0) {
    print_error("Your input does not read as a valid binary string.");
  } else {
    ft_putstr("\n---> Your final string reads as: ");
    ft_putstr(result);
    ft_putchar('\n');
  }
  free(result);
  return (0);
}
