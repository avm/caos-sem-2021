#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

void pow_n(float array[], size_t size, size_t pow) {
    assert((uintptr_t)array % 16 == 0);
    float ones[4] = {1.0, 1.0, 1.0, 1.0};
    for (size_t i = 0; i + 4 <= size; i += 4) {
        asm volatile (
            "movups (%0), %%xmm0\n\t"
            "movups (%2), %%xmm1\n\t"
            "1:\n\t"
            "test   %1, %1\n\t"
            "jz     2f\n\t"
            "test   $1, %1        \n\t"
            "jz     3f\n\t"
            "mulps  %%xmm0, %%xmm1\n\t"
            "3:                   \n\t"
            "mulps  %%xmm0, %%xmm0\n\t"
            "shr    $1, %1\n\t"
            "jmp    1b\n\t"
            "2:       \n\t"
            "movups %%xmm1, (%0)\n\t"
            :
            : "r"(array + i), "r"(pow), "r"(ones)  //%0 %1 %2
            : "memory", "xmm0", "xmm1"
        );
    }
}


int main() {
    float array[4] __attribute__((aligned(16))) = {1, 2, 3, 4};
    pow_n(array, 4, 3);
    for (size_t i = 0; i < 4; i++) {
        printf("%f ", array[i]);
    }
    printf("\n");
}
