#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>

typedef int(*puts_type)(const char *);

int puts(const char *s) {
    puts_type puts_ptr = dlsym(RTLD_NEXT, "puts");
    assert(puts_ptr);
    puts_ptr("This works");
    puts_ptr(s);
    return 0;
}

