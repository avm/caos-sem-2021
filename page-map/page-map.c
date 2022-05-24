#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  const size_t block_size = 5000000;
  uint8_t *block = malloc(block_size);
  block[0] = 0x12;
  block[4096] = 0x56;
  int fd = open("/proc/self/pagemap", O_RDONLY);
  if (fd < 0) {
    perror("open /proc/self/pagemap");
    exit(1);
  }
  uintptr_t ptr = (uintptr_t)block & ~0xfff;
  uint64_t *values = malloc(((block_size >> 12) + 1) * sizeof(*values));
  if (pread(fd, values, ((block_size >> 12) + 1) * sizeof(*values), ptr >> 9) < 0) {
    perror("pread");
    exit(1);
  }
  printf("values:\n");
  for (int i = 0; i < (block_size >> 12) + 1; ++i) {
    printf("values[%d] = %016lx\n", i, values[i]);
  }
}
