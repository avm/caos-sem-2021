#include <stdlib.h>
#include <stdio.h>



int sys_call(int sys_num, int first, int second, int third) {
  
  asm ( 
        "int $0x80\n"
        : "+a"(sys_num)     // outputs
        : "b"(first), "c"(second), "d"(third) 
  );
  return sys_num;
}

int main () {
  sys_call(1, 2, -1, -1);
  return 0;
}
