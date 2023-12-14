#include <stdint.h>
#include "bits.h"

struct bits8
{
  struct bit b0; // Least significant bit
  struct bit b1;
  struct bit b2;
  struct bit b3;
  struct bit b4;
  struct bit b5;
  struct bit b6;
  struct bit b7;
};

struct bits8 bits8_from_int(unsigned int x)
{
  struct bits8 bit8;

  bit8.b0.v = (x >> 0) & 1;
  bit8.b1.v = (x >> 1) & 1;
  bit8.b2.v = (x >> 2) & 1;
  bit8.b3.v = (x >> 3) & 1;
  bit8.b4.v = (x >> 4) & 1;
  bit8.b5.v = (x >> 5) & 1;
  bit8.b6.v = (x >> 6) & 1;
  bit8.b7.v = (x >> 7) & 1;

  return bit8;
}

unsigned int bits8_to_int(struct bits8 x)
{
  int sum = 0;
  sum = sum + bit_to_int(x.b0);
  sum = sum + (bit_to_int(x.b1) << 1);
  sum = sum + (bit_to_int(x.b2) << 2);
  sum = sum + (bit_to_int(x.b3) << 3);
  sum = sum + (bit_to_int(x.b4) << 4);
  sum = sum + (bit_to_int(x.b5) << 5);
  sum = sum + (bit_to_int(x.b6) << 6);
  sum = sum + (bit_to_int(x.b7) << 7);
  return sum;
}

void bits8_print(struct bits8 v)
{
  printf("%d", v.b7.v);
  printf("%d", v.b6.v);
  printf("%d", v.b5.v);
  printf("%d", v.b4.v);
  printf("%d", v.b3.v);
  printf("%d", v.b2.v);
  printf("%d", v.b1.v);
  printf("%d", v.b0.v);
}
struct bit bit_add(struct bit a, struct bit b, struct bit c)
{
  return bit_xor(bit_xor(a, b), c);
}
struct bit bit_carry(struct bit a, struct bit b, struct bit c)
{
  return bit_or(bit_and(a, b), bit_and(bit_or(a, b), c));
}
struct bits8 bits8_add(struct bits8 x, struct bits8 y)
{
  struct bits8 sum;
  struct bits8 carry;

  sum.b0 = bit_xor(x.b0, y.b0);
  carry.b0 = bit_and(x.b0, y.b0);

  sum.b1 = bit_add(x.b1, y.b1, carry.b0);
  carry.b1 = bit_carry(x.b1, y.b1, carry.b0);
  sum.b2 = bit_add(x.b2, y.b2, carry.b1);
  carry.b2 = bit_carry(x.b2, y.b2, carry.b1);
  sum.b3 = bit_add(x.b3, y.b3, carry.b2);
  carry.b3 = bit_carry(x.b3, y.b3, carry.b2);
  sum.b4 = bit_add(x.b4, y.b4, carry.b3);
  carry.b4 = bit_carry(x.b4, y.b4, carry.b3);
  sum.b5 = bit_add(x.b5, y.b5, carry.b4);
  carry.b5 = bit_carry(x.b5, y.b5, carry.b4);
  sum.b6 = bit_add(x.b6, y.b6, carry.b5);
  carry.b6 = bit_carry(x.b6, y.b6, carry.b5);
  sum.b7 = bit_add(x.b7, y.b7, carry.b6);
  carry.b7 = bit_carry(x.b7, y.b7, carry.b6);

  return sum;
}
struct bits8 bits8_negate(struct bits8 x)
{
  x.b0 = bit_not(x.b0);
  x.b1 = bit_not(x.b1);
  x.b2 = bit_not(x.b2);
  x.b3 = bit_not(x.b3);
  x.b4 = bit_not(x.b4);
  x.b5 = bit_not(x.b5);
  x.b6 = bit_not(x.b6);
  x.b7 = bit_not(x.b7);

  struct bits8 one;
  one.b0.v = true;
  x = bits8_add(x, one);

  return x;
}

struct bits8 bits8_mul_bit(struct bits8 x, struct bit y)
{
  struct bits8 bit;
  bit.b0 = bit_and(x.b0, y);
  bit.b1 = bit_and(x.b1, y);
  bit.b2 = bit_and(x.b2, y);
  bit.b3 = bit_and(x.b3, y);
  bit.b4 = bit_and(x.b4, y);
  bit.b5 = bit_and(x.b5, y);
  bit.b6 = bit_and(x.b6, y);
  bit.b7 = bit_and(x.b7, y);

  return bit;
}

struct bits8 bits8_ls(struct bits8 x)
{
  struct bits8 y;
  y.b7 = x.b6;
  y.b6 = x.b5;
  y.b5 = x.b4;
  y.b4 = x.b3;
  y.b3 = x.b2;
  y.b2 = x.b1;
  y.b1 = x.b0;
  y.b0.v = false;
  return y;
}

struct bits8 bits8_mul(struct bits8 x, struct bits8 y)
{
  struct bits8 bit;

  bit = bits8_mul_bit(x, y.b0);
  bit = bits8_add(bit, bits8_ls(bits8_mul_bit(x, y.b1)));
  bit = bits8_add(bit, bits8_ls(bits8_ls(bits8_mul_bit(x, y.b2))));
  bit = bits8_add(bit, bits8_ls(bits8_ls(bits8_ls(bits8_mul_bit(x, y.b3)))));
  bit = bits8_add(bit, bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_mul_bit(x, y.b4))))));
  bit = bits8_add(bit, bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_mul_bit(x, y.b5)))))));
  bit = bits8_add(bit, bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_mul_bit(x, y.b6))))))));
  bit = bits8_add(bit, bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_ls(bits8_mul_bit(x, y.b7)))))))));

  return bit;
}
