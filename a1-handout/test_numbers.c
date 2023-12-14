#include <stdlib.h>
#include "numbers.h"

int main()
{

  // struct bits8 bit = bits8_from_int(11);
  // bits8_print(bit);
  // printf("\n");
  // int r = bits8_to_int(bit);
  // printf("\n");
  // printf("%d\n", r);

  // struct bits8 bit2 = bits8_from_int(74);
  // bits8_print(bit2);
  // printf("\n");
  // int r2 = bits8_to_int(bit2);
  // printf("\n");
  // printf("%d\n", r2);

  // struct bits8 bit3 = bits8_add(bits8_from_int(0), bits8_from_int(1));
  // int bit3r = bits8_to_int(bit3);
  // printf("\n%d\n", bit3r);

  // struct bits8 bitneg = bits8_from_int((int8_t)-1);
  // bits8_print(bitneg);
  // struct bits8 bitpos = bits8_negate(bitneg);
  // printf("\n");
  // bits8_print(bitpos);
  // struct bits8 bitneg2 = bits8_negate(bitpos);
  // printf("\n");
  // bits8_print(bitneg2);
  struct bits8 bit1 = bits8_from_int(3);
  struct bits8 bit2 = bits8_from_int(3);
  struct bits8 bit3 = bits8_mul(bit1, bit2);
  bits8_print(bit3);
  printf("\n");
}
