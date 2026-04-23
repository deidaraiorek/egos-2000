void terminal_write(const char *str, int len) {
  for (int i = 0; i < len; i++) {
    *(char *)(0x10000000) = str[i];
  }
}

/* Uncomment line11 - line39
 * when implementing formatted output
 */

#include <stdarg.h> // for va_start(), va_end(), va_arg() and va_copy()
#include <stdlib.h> // for itoa() and utoa()
#include <string.h> // for strlen() and strcat()

void ulltoa(unsigned long long n, char *buf);
unsigned int format_to_str_len(const char *fmt, va_list args);

void format_to_str(char *out, const char *fmt, va_list args) {
  for (out[0] = 0; *fmt != '\0'; fmt++) {
    if (*fmt != '%') {
      strncat(out, fmt, 1);
    } else {
      fmt++;
      if (*fmt == 'l' && *(fmt + 1) == 'l' && *(fmt + 2) == 'u') {
        ulltoa(va_arg(args, unsigned long long), out + strlen(out));
        fmt += 2;
      } else
        switch (*fmt) {
        case 's':
          strcat(out, va_arg(args, char *));
          break;
        case 'd':
          itoa(va_arg(args, int), out + strlen(out), 10);
          break;
        case 'c': {
          char tmp[2] = {va_arg(args, int), '\0'};
          strcat(out, tmp);
          break;
        }
        case 'x':
          itoa(va_arg(args, int), out + strlen(out), 16);
          break;
        case 'u':
          utoa(va_arg(args, unsigned int), out + strlen(out), 10);
          break;
        case 'p':
          strcat(out, "0x");
          itoa((int)va_arg(args, void *), out + strlen(out), 16);
          break;
        }
    }
  }
}

unsigned int format_to_str_len(const char *fmt, va_list args) {
  unsigned int len = 0;
  char tmp[32];
  for (; *fmt != '\0'; fmt++) {
    if (*fmt != '%') {
      len++;
    } else {
      fmt++;
      if (*fmt == 'l' && *(fmt + 1) == 'l' && *(fmt + 2) == 'u') {
        ulltoa(va_arg(args, unsigned long long), tmp);
        len += strlen(tmp);
        fmt += 2;
      } else
        switch (*fmt) {
        case 's':
          len += strlen(va_arg(args, char *));
          break;
        case 'd':
          itoa(va_arg(args, int), tmp, 10);
          len += strlen(tmp);
          break;
        case 'c':
          va_arg(args, int);
          len += 1;
          break;
        case 'x':
          itoa(va_arg(args, int), tmp, 16);
          len += strlen(tmp);
          break;
        case 'u':
          utoa(va_arg(args, unsigned int), tmp, 10);
          len += strlen(tmp);
          break;
        case 'p':
          itoa((int)va_arg(args, void *), tmp, 16);
          len += 2 + strlen(tmp);
          break;
        }
    }
  }
  return len + 1;
}

void ulltoa(unsigned long long n, char *buf) {
  int i = 0;

  if (n == 0) {
    buf[i++] = '0';
    buf[i] = '\0';
    return;
  }

  while (n != 0) {
    int d = n % 10;
    buf[i++] = '0' + d;
    n /= 10;
  }

  buf[i] = '\0';

  for (int i = 0, j = strlen(buf) - 1; i < j; i++, j--) {
    char t = buf[i];
    buf[i] = buf[j];
    buf[j] = t;
  }
}

int printf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  va_list args_copy;
  va_copy(args_copy, args);
  unsigned int len = format_to_str_len(format, args_copy);
  char *buf = malloc(len);

  format_to_str(buf, format, args);
  va_end(args);
  terminal_write(buf, strlen(buf));

  va_end(args_copy);
  free(buf);

  return 0;
}

/* Uncomment line46 - line57
 * when implementing dynamic memory allocation
 */

extern char __heap_start, __heap_end;
static char *brk = &__heap_start;
char *_sbrk(int size) {
  if (brk + size > (char *)&__heap_end) {
    terminal_write("_sbrk: heap grows too large\r\n", 29);
    return NULL;
  }

  char *old_brk = brk;
  brk += size;
  return old_brk;
}

int main() {
  char *msg = "Hello, World!\n\r";
  terminal_write(msg, 15);

  /* Uncomment this line of code
   * when implementing formatted output
   */
  printf("%s-%d is awesome!\n\r", "egos", 2000);
  printf("%c is character $\n\r", '$');
  printf("%c is character 0\n\r", (char)48);
  printf("%x is integer 1234 in hexadecimal\n\r", 1234);
  printf("%u is the maximum of unsigned int\n\r", (unsigned int)0xFFFFFFFF);
  printf("%p is the hexadecimal address of the hello-world string\n\r", msg);
  printf("%llu is the maximum of unsigned long long\n\r",
         0xFFFFFFFFFFFFFFFFULL);

  return 0;
}
