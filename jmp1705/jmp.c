#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

jmp_buf env;
jmp_buf env1, env2;
// int env[6];

typedef void (*func_t)();

void foo() {
    int a = 1, b = 1;
    int val = setjmp(env1);
    int tmp = a;
    a += b;
    b = tmp;
    longjmp(env, a); // jump to main

    // not run

    int x = 17;
    if (x == 0) {
        longjmp(env, 57);
        foo(-1); // breaks
    } else {
        foo(x - 1);
    }
}

void run_on_stack(func_t func, void* stack_bottom) {
    asm("mov %1, %%esp \n\t"
        "jmp *%%eax \n\t" : : "a"(func), "m"(stack_bottom));

}

int main() {
    char* M = malloc(8192);
    int val = setjmp(env);
    if (val == 0) {
        run_on_stack(foo, M + 8192);
    } else if (val < 100) {
        printf("%d\n", val);
        longjmp(env1, val);
    }
    free(M);
}
