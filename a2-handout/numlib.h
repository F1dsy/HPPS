#pragma once

#include <stdint.h>
#include <stdio.h>

// Skip spaces in file.
void skipspaces(FILE *f);

// All functions return 1 on format failure, and EOF if the first read
// is EOF.

int read_uint_ascii(FILE *f, uint32_t *out); // given
int read_double_ascii(FILE *f, double *out); // todo
int read_uint_le(FILE *f, uint32_t *out);    // given
int read_uint_be(FILE *f, uint32_t *out);    // solved
int read_double_bin(FILE *f, double *out);   // todo

int write_uint_ascii(FILE *f, uint32_t x); // given
int write_double_ascii(FILE *f, double x); // todo
int write_uint_le(FILE *f, uint32_t x);    // given
int write_uint_be(FILE *f, uint32_t x);    // solved
int write_double_bin(FILE *f, double x);   // todo
