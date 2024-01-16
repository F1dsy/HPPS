#include "matlib.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Scalar type

struct scalar {
  double value;
};

struct scalar* read_scalar(const char* file) {
  struct scalar* ret = NULL;
  FILE *f = fopen(file, "rb");

  if (f == NULL) {
    return NULL;
  }

  char magic[4];

  if (fread(magic, 4, 1, f) != 1) {
    goto end;
  }

  if (memcmp(magic,"SCLR",4) != 0) {
    goto end;
  }

  double v;
  if (fread(&v, sizeof(double), 1, f) != 1) {
    goto end;
  }

  ret = malloc(sizeof(struct scalar));
  ret->value = v;

 end:
  fclose(f);
  return ret;
}

int write_scalar(const char* file, struct scalar* s) {
  FILE* f = fopen(file, "wb");
  int ret = 0;

  if (f == NULL) {
    return 1;
  }

  fprintf(f, "SCLR");
  if (fwrite(&s->value, sizeof(double), 1, f) != 1) {
    ret = 1;
  }
  fclose(f);
  return ret;
}

void free_scalar(struct scalar* s) {
  free(s);
}

double scalar_value(struct scalar* s) {
  return s->value;
}


// Vector type

struct vector {
  int n;
  double *elements;
};

struct vector* read_vector(const char* file) {
  FILE *f = fopen(file, "rb");
  struct vector* ret = NULL;

  if (f == NULL) {
    return NULL;
  }

  char magic[4];

  if (fread(magic, 4, 1, f) != 1) {
    goto end;
  }

  if (memcmp(magic,"VDNS",4) != 0) {
    goto end;
  }

  int32_t n;
  if (fread(&n, sizeof(int32_t), 1, f) != 1) {
    goto end;
  }

  if (n < 0) {
    goto end;
  }

  ret = malloc(sizeof(struct vector));
  ret->n = n;
  ret->elements = calloc(n, sizeof(double));

  if (fread(ret->elements, sizeof(double), n, f) != (size_t)n) {
    free(ret->elements);
    free(ret);
    ret = NULL;
  }

 end:
  fclose(f);
  return ret;
}

int write_vector(const char* file, struct vector* v) {
  FILE* f = fopen(file, "wb");
  int ret = 0;

  if (f == NULL) {
    return 1;
  }

  fprintf(f, "VDNS");
  if (fwrite(&v->n, sizeof(int), 1, f) != 1) {
    ret = 1;
  }
  if (fwrite(v->elements, sizeof(double), v->n, f) != (size_t)v->n) {
    ret = 1;
  }
  fclose(f);
  return ret;
}

void free_vector(struct vector* v) {
  free(v->elements);
  free(v);
}

int vector_n(struct vector* v) {
  return v->n;
}

double vector_idx(struct vector* v, int i) {
  return v->elements[i];
}

// Dense matrices

struct matrix_dense {
  int n;
  int m;
  double* elements;
};

struct matrix_dense* read_matrix_dense(const char* file) {
  FILE *f = fopen(file, "rb");
  struct matrix_dense* ret = NULL;

  if (f == NULL) {
    return NULL;
  }

  char magic[4];

  if (fread(magic, 4, 1, f) != 1) {
    goto end;
  }

  if (memcmp(magic, "MDNS", 4) != 0) {
    goto end;
  }

  int32_t rows, cols;
  if (fread(&rows, sizeof(int32_t), 1, f) != 1) {
    goto end;
  }
  if (fread(&cols, sizeof(int32_t), 1, f) != 1) {
    goto end;
  }

  if (rows < 0 || cols < 0) {
    goto end;
  }

  ret = malloc(sizeof(struct matrix_dense));
  ret->n = rows;
  ret->m = cols;
  ret->elements = calloc(rows*cols, sizeof(double));

  if (fread(ret->elements, sizeof(double), rows*cols, f) != (size_t)(rows*cols)) {
    free(ret->elements);
    free(ret);
    ret = NULL;
  }

 end:
  fclose(f);
  return ret;
}

int write_matrix_dense(const char* file, struct matrix_dense* X) {
  FILE* f = fopen(file, "wb");
  int ret = 0;

  if (f == NULL) {
    return 1;
  }

  fprintf(f, "MDNS");
  if (fwrite(&X->n, sizeof(int), 1, f) != 1) {
    ret = 1;
  }
  if (fwrite(&X->m, sizeof(int), 1, f) != 1) {
    ret = 1;
  }
  if (fwrite(X->elements, sizeof(double), X->n*X->m, f) != (size_t)(X->n*X->m)) {
    ret = 1;
  }
  fclose(f);
  return ret;
}

void free_matrix_dense(struct matrix_dense* X) {
  free(X->elements);
  free(X);
}

int matrix_dense_n(struct matrix_dense* X) {
  return X->n;
}

int matrix_dense_m(struct matrix_dense* X) {
  return X->m;
}

double matrix_dense_idx(struct matrix_dense* X, int i, int j) {
  return X->elements[i*X->m+j];
}

// Sparse matrices in CSR format

struct matrix_csr {
  int n;
  int m;
  int nnz; // Number of nonzero entries.
  double *v; // Nonzero values, of length nnz.
  int *v_col; // Column of each value, of length nnz.
  int *row_start; // For each row number, offset in v where that row
                  // starts.
};

struct matrix_csr* read_matrix_csr(const char* file) {
  FILE *f = fopen(file, "rb");
  struct matrix_csr* ret = NULL;

  if (f == NULL) {
    return NULL;
  }

  char magic[4];

  if (fread(magic, 4, 1, f) != 1) {
    goto end;
  }

  if (memcmp(magic, "MCSR", 4) != 0) {
    goto end;
  }

  int32_t rows, cols, nnz;
  if (fread(&rows, sizeof(int32_t), 1, f) != 1) {
    goto end;
  }
  if (fread(&cols, sizeof(int32_t), 1, f) != 1) {
    goto end;
  }
  if (fread(&nnz, sizeof(int32_t), 1, f) != 1) {
    goto end;
  }

  if (rows < 0 || cols < 0 || nnz < 0) {
    goto end;
  }

  ret = malloc(sizeof(struct matrix_csr));
  ret->n = rows;
  ret->m = cols;
  ret->nnz = nnz;
  ret->v = calloc(nnz, sizeof(double));
  ret->v_col = calloc(nnz, sizeof(int));
  ret->row_start = calloc(rows, sizeof(int));

  if (fread(ret->v, sizeof(double), nnz, f) != (size_t)nnz ||
      fread(ret->v_col, sizeof(int), nnz, f) != (size_t)nnz ||
      fread(ret->row_start, sizeof(int), rows, f) != (size_t)rows) {
    free(ret->v);
    free(ret->v_col);
    free(ret->row_start);
    free(ret);
    ret = NULL;
  }

 end:
  fclose(f);
  return ret;
}

int write_matrix_csr(const char* file, struct matrix_csr* X) {
  FILE* f = fopen(file, "wb");
  int ret = 0;

  if (f == NULL) {
    return 1;
  }

  fprintf(f, "MCSR");
  if (fwrite(&X->n, sizeof(int), 1, f) != 1) {
    ret = 1;
  }
  if (fwrite(&X->m, sizeof(int), 1, f) != 1) {
    ret = 1;
  }
  if (fwrite(&X->nnz, sizeof(int), 1, f) != 1) {
    ret = 1;
  }
  if (fwrite(X->v, sizeof(double), X->nnz, f) != (size_t)X->nnz) {
    ret = 1;
  }
  if (fwrite(X->v_col, sizeof(int), X->nnz, f) != (size_t)X->nnz) {
    ret = 1;
  }
  if (fwrite(X->row_start, sizeof(int), X->n, f) != (size_t)X->n) {
    ret = 1;
  }
  fclose(f);
  return ret;
}

void free_matrix_csr(struct matrix_csr* A) {
  free(A->v);
  free(A->v_col);
  free(A->row_start);
  free(A);
}

int matrix_csr_n(struct matrix_csr* X) {
  return X->n;
}

int matrix_csr_m(struct matrix_csr* X) {
  return X->m;
}

double matrix_csr_idx(struct matrix_csr* A, int i, int j) {
  int start = A->row_start[i];
  int end = (i+1 < A->n) ? A->row_start[i+1] : A->nnz;
  for (int l = start; l < end; l++) {
    if (A->v_col[l] == j) {
      return A->v[l];
    }
  }
  return 0;
}

// Operations

struct scalar* mul_vv(struct vector* x, struct vector* y) {
  if (x->n != y->n) {
    return NULL;
  }

  struct scalar *z = malloc(sizeof(struct scalar));

  double v = 0;
#pragma omp parallel for reduction(+:v)
  for (int i = 0; i < y->n; i++) {
    v += x->elements[i] * y->elements[i];
  }
  z->value = v;
  return z;
}

struct vector* mul_mv(struct matrix_dense* A, struct vector* x) {
  if (A->m != x->n) {
    return NULL;
  }

  struct vector *y = malloc(sizeof(struct vector));
  y->n = A->n;
  y->elements = malloc(y->n * sizeof(double));
  int n = A->n;
  int m = A->m;
  double* as = A->elements;
  double* xs = x->elements;
  double* ys = y->elements;

#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    double acc = 0;
    for (int j = 0; j < m; j++) {
      acc += as[i*m+j] * xs[j];
    }
    ys[i] = acc;
  }

  return y;
}

struct vector* mul_mTv(struct matrix_dense* A, struct vector* x) {
  if (A->n != x->n) {
    return NULL;
  }

  struct vector *y = malloc(sizeof(struct vector));
  y->n = A->m;
  y->elements = malloc(y->n * sizeof(double));
  int n = A->n;
  int m = A->m;
  double* as = A->elements;
  double* xs = x->elements;
  double* ys = y->elements;

#pragma omp parallel for
  for (int j = 0; j < m; j++) {
    ys[j] = 0;
  }

  for (int i = 0; i < n; i++) {
#pragma omp parallel for
    for (int j = 0; j < m; j++) {
      ys[j] += as[i*m+j] * xs[i];
    }
  }

  return y;
}

struct vector* mul_spmv(struct matrix_csr* A, struct vector* x) {
  if (A->m != x->n) {
      return NULL;
  }

  struct vector *y = malloc(sizeof(struct vector));
  y->n = A->n;
  y->elements = malloc(y->n * sizeof(double));

#pragma omp parallel for
  for (int i = 0; i < A->n; i++) {
    int start = A->row_start[i];
    int end = (i+1 < A->n) ? A->row_start[i+1] : A->nnz;
    double acc = 0;
    for (int l = start; l < end; l++) {
      acc += A->v[l] * x->elements[A->v_col[l]];
    }
    y->elements[i] = acc;
  }

  return y;
}

struct vector* mul_spmTv(struct matrix_csr* A, struct vector* x) {
  if (A->n != x->n) {
      return NULL;
  }

  struct vector *y = malloc(sizeof(struct vector));
  y->n = A->m;
  y->elements = malloc(y->n * sizeof(double));

  for (int j = 0; j < A->m; j++) {
    y->elements[j] = 0;
  }

  for (int i = 0; i < A->n; i++) {
    int start = A->row_start[i];
    int end = (i+1 < A->n) ? A->row_start[i+1] : A->nnz;
#pragma omp parallel for
    for (int l = start; l < end; l++) {
      y->elements[A->v_col[l]] += A->v[l] * x->elements[i];
    }
  }

  return y;
}
