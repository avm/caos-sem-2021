#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _SCHED_H 1
#define __USE_GNU 1
#include <bits/sched.h>
#include <stdbool.h>

#include <linux/futex.h>
#include <sys/time.h>
#include <sys/syscall.h>

struct tinfo {
  int return_code;
  _Atomic int returned;
  char stack[];
};

int f(void *useful_arg) {
  struct tinfo *ptr = (struct tinfo *)useful_arg;
  printf("hello from f()\n");
  sleep(4);
  ptr->return_code = 57;
  ptr->returned = 1;
  syscall(SYS_futex, &ptr->returned, FUTEX_WAKE, 1, NULL, NULL, 0);
  return 0;
}

/*

Thread 0         Thread 1
result = 57;
done = 1; -----> while (!done);
                 printf(result);

int main() { 
  size_t stack_size = 8000000;
  struct tinfo *ptr = calloc(1, sizeof(*ptr) + stack_size);
  int code = clone(f, ptr->stack + stack_size, CLONE_THREAD | CLONE_SIGHAND | CLONE_VM, ptr);
  if (code == -1) {
    perror("clone");
    exit(1);
  }
  syscall(SYS_futex, &ptr->returned, FUTEX_WAIT, 0, NULL, NULL, 0);
  //while (!ptr->returned) {
    //asm volatile("pause\n": : :"memory");
  //}

  printf("success (return code %d)!\n", ptr->return_code);
}
