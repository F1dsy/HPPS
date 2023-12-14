#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

void skipspaces(FILE *f)
{
  while (1)
  {
    int c = fgetc(f);
    if (!isspace(c))
    {
      if (c != EOF)
      {
        ungetc(c, f);
      }
      return;
    }
  }
}

int read_uint_ascii(FILE *f, uint32_t *out)
{
  int read = 0;
  uint32_t num = 0;

  while (1)
  {
    int c = fgetc(f);

    if (c >= '0' && c <= '9')
    {
      num = num * 10 + (c - '0');
    }
    else
    {
      if (c != EOF)
      {
        ungetc(c, f);
      }
      if (read == 0)
      {
        if (c == EOF)
        {
          return EOF;
        }
        else
        {
          return 1;
        }
      }
      else
      {
        *out = num;
        return 0;
      }
    }
    read++;
  }
}

int read_double_ascii(FILE *f, double *out)
{
  int isNegative = 0;
  double num = 0.0;

  uint32_t first_half = 0;
  uint32_t second_half = 0;
  double decimal_part = 0.0;

  int sign = fgetc(f);
  if (sign == '-')
  {
    isNegative = 1;
  }
  else
  {
    ungetc(sign, f);
  }

  int read_to_dot = read_uint_ascii(f, &first_half);
  if (read_to_dot != 0)
  {
    return read_to_dot;
  }

  int dot = fgetc(f);
  if (dot == '.')
  {

    int read_to_end = read_uint_ascii(f, &second_half);
    if (read_to_end != 0)
    {
      return read_to_end;
    }
    decimal_part = (double)second_half;
    while (decimal_part > 1.0)
    {
      decimal_part /= 10;
    }
  }

  num = (double)first_half + decimal_part;
  if (isNegative == 1)
  {
    num = -num;
  }
  *out = num;
  return 0;
}

int read_uint_le(FILE *f, uint32_t *out)
{
  int b0, b1, b2, b3;
  b0 = fgetc(f);

  if (b0 == EOF)
  {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF)
  {
    return 1;
  }

  *out =
      ((uint32_t)b0) | ((uint32_t)b1 << 8) | ((uint32_t)b2 << 16) | ((uint32_t)b3 << 24);
  return 0;
}

int read_uint_be(FILE *f, uint32_t *out)
{

  int b0, b1, b2, b3;
  b0 = fgetc(f);

  if (b0 == EOF)
  {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF)
  {
    return 1;
  }

  *out =
      ((uint32_t)b0 << 24) | ((uint32_t)b1 << 16) | ((uint32_t)b2 << 8) | ((uint32_t)b3 << 0);
  return 0;
}

int read_double_bin(FILE *f, double *out)
{
  int b0, b1, b2, b3, b4, b5, b6, b7;
  b0 = fgetc(f);

  if (b0 == EOF)
  {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);
  b4 = fgetc(f);
  b5 = fgetc(f);
  b6 = fgetc(f);
  b7 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF || b4 == EOF || b5 == EOF || b6 == EOF || b7 == EOF)
  {
    return 1;
  }
  uint64_t double_bytes = ((uint64_t)b0) | ((uint64_t)b1 << 8) | ((uint64_t)b2 << 16) | ((uint64_t)b3 << 24) | ((uint64_t)b4 << 32) | ((uint64_t)b5 << 40) | ((uint64_t)b6 << 48) | ((uint64_t)b7 << 56);
  *out = *(double *)&double_bytes;

  return 0;
}

int write_uint_ascii(FILE *f, uint32_t x)
{
  if (fprintf(f, "%u", x) < 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int write_double_ascii(FILE *f, double x)
{
  if (fprintf(f, "%lf", x) < 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int write_uint_le(FILE *f, uint32_t x)
{
  fputc(x >> 0, f);
  fputc(x >> 8, f);
  fputc(x >> 16, f);
  fputc(x >> 24, f);
  return 0;
}

int write_uint_be(FILE *f, uint32_t x)
{
  fputc(x >> 24, f);
  fputc(x >> 16, f);
  fputc(x >> 8, f);
  fputc(x >> 0, f);
  return 0;
}

int write_double_bin(FILE *f, double x)
{
  uint64_t px = *(uint64_t *)&x;

  fputc(px >> 0, f);
  fputc(px >> 8, f);
  fputc(px >> 16, f);
  fputc(px >> 24, f);
  fputc(px >> 32, f);
  fputc(px >> 40, f);
  fputc(px >> 48, f);
  fputc(px >> 56, f);
  return 0;
}
