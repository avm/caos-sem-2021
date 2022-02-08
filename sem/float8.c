#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct float8data {
  int sign;
  int exp;
  int frac;
} float8data;

void make_valid(float8data *f) {
  if (f->frac >= (1 << 4)) {
    bool all_zero = true;
    while (f->frac >= (1 << 5)) {
      if (f->frac & 1) {
         all_zero = false;
      }
      f->frac >>= 1;
    }
    if (all_zero && (f->frac & 1)) {
      f->frac >>= 1;
      if (f->frac & 1) ++f->frac;
    } else if (f->frac & 1) {
      f->frac >>= 1;
      ++f->frac;
    } else {
      f->frac >>= 1;
    }
    if (f->frac == (1 << 4)) {
      f->frac >>= 1;
      ++f->exp;
    }
  }
  if (f->exp >= (1 << 3)) {
    f->exp = (1 << 3);
    return;
  }
  
}

float8data to_float8data(unsigned char x) {
  
}

unsigned char tofloat8(int32_t x) {
	unsigned char ans = 0;
  if (x >= 16) {
    ans = 0b01110000;
    return ans;
  }
  if (x <= -16) {
    ans = 0b11110000;
    return ans;
  }
	if (x < 0) {
		ans |= 1 << 7;
		x = -x;
	
	}
	if (x == 0) {
		return 0;	
}
	int32_t numbits = 0;
	int32_t y = x;
	while (y > 0) {
		numbits++;
		y /= 2;	
	}
	int32_t exp = numbits - 1 + 3; //exponent
	// int32_t shift = numbits - 5 > 0 ? numbits - 5 : 0;
	if (numbits < 5) {
		x <<= 5 - numbits;
	} else {
    int shift = numbits - 5;
    if (shift > 0) {
        int need_round = 0;
        uint32_t tail = x;
        tail <<= (32 - shift);
		    x >>= shift;
        if (tail & (1u << 31)) {
            if (tail == (1u << 31)) {
                //Round to even
                if (x & 1) {
                    need_round = 1;
                }
            } else {
                //ROund up
                need_round = 1;
            }
        }
        if (need_round) {
            if (x ==  0x1f) {
                exp++;
                x = 0;
            } else {
                x++;
            }
        }
    }
	}
	// x >>= shift;
	x &= 15; //mantisa	
	// 1 = 1.
	// 2 = 1.
	// 3 = 1.1
	// 4 = 1.0
	ans ^= (exp << 4);
	ans ^= (x);
	return ans;
}

unsigned char mul(unsigned char a, unsigned char b) {
  int sign1 = (a >> 7);
  int sign2 = (b >> 7);
  int sign = (sign1 ^ sign2);
  int exp1 = ((a >> 4) & 7);
  int exp2 = ((b >> 4) & 7);
  int exp = exp1 + exp2 - 3;
  int m1 = (a & 15) + (1 << 4);
  int m2 = (b & 15) + (1 << 4);
  int m = ((m1 * m2) >> 4);
  while (m >= (1 << 6)) {
     m >>= 1;
     exp++;
  }
  m -= (1 << 4);
  unsigned char ans = (sign << 7) + (exp << 4) + m;
  return ans;
}

void print(unsigned char c) {
  int bits[8];
  for (int i = 0; i < 8; i++) {
    bits[7 - i] = c % 2;
    c /= 2;
  }
  for (int i = 0; i < 8; i++) {
    printf("%d", bits[i]);
   }
   printf("\n");
}

int main() {
	int x;
  int y;
	scanf("%d %d", &x, &y);
	unsigned char fl1 = tofloat8(x);
  unsigned char fl2 = tofloat8(y);
	int z = mul(fl1, fl2);
	printf("%x * %x = %x\n", fl1, fl2, z);
  print(z);
}
