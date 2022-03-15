#include <stdio.h>

void puts(int arg);

int foo() {
  puts("Hello");
  return 3;
}

int bar() {
  printf("FooBar\n");
  return 3;
}
