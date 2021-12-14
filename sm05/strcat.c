#include <unistd.h>
#include <string.h>

char* strcat_n(char* a, const char* b, size_t n) {
  // ..\0
  char* cur = a;
  while (*cur != '\0') cur++;
  while (*b != '\0') {
    if (cur - a == n - 1) break;
    *cur = *b;
    cur++;
    b++;
  }
  *cur = '\0';
  return a;
}

int main() {
  // argv[]
  char a[10] = "hello";
  char b[] = " world";
  strcat_n(a, b, 10);

  write(STDOUT_FILENO, a, strlen(a));
}
