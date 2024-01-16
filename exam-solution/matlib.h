#ifndef MATLIB_H
#define MATLIB_H

// Scalars

struct scalar;

// Returns NULL on failure.  Failures can occur if the file does not
// exist, does not have the right magic number, or if the expected
// number of bytes cannot be read.  The scalar must eventually be
// freed with free_scalar().
struct scalar* read_scalar(const char* file);
// Returns 0 on success.  Failures can occur if the file cannot be
// opened or if any of the writes fail to write the expected number of
// bytes.
int write_scalar(const char* file, struct scalar*);
// Free any dynamically allocated memory related to the scalar.
void free_scalar(struct scalar*);
// Retrieve the value of the scalar.
double scalar_value(struct scalar*);

// Vectors

struct vector;

// Returns NULL on failure.  Failures can occur if the file does not
// exist, does not have the right magic number, if the expected number
// of bytes cannot be read, or if the vector claims to have a negative
// number of elements.  The vector must eventually be freed with
// free_vector().
struct vector* read_vector(const char* file);
// Returns 0 on success.  Failures can occur if the file cannot be
// opened or if any of the writes fail to write the expected number of
// bytes.
int write_vector(const char* file, struct vector*);
// Free any dynamically allocated memory related to the vector.
void free_vector(struct vector*);
// Return number of elements in the vector.
int vector_n(struct vector*);
// Return the element at position i, which must be in-bounds.
double vector_idx(struct vector*, int i);

// Dense matrices

struct matrix_dense;

// Returns NULL on failure.  Failures can occur if the file does not
// exist, does not have the right magic number, if the expected number
// of bytes cannot be read, or if the matrix claims to have a negative
// number of rows or columns.  The vector must eventually be freed
// with free_matrix_dense().
struct matrix_dense* read_matrix_dense(const char* file);
// Returns 0 on success; Failures can occur if the file cannot be
// opened or if any of the writes fail to write the expected number of
// bytes.
int write_matrix_dense(const char* file, struct matrix_dense*);
// Free any dynamically allocated memory related to the matrix.
void free_matrix_dense(struct matrix_dense*);
// Return number of rows in the matrix.
int matrix_dense_n(struct matrix_dense*);
// Return number of columns in the matrix.
int matrix_dense_m(struct matrix_dense*);
// Return the element at position (i,j), which must be in-bounds.
double matrix_dense_idx(struct matrix_dense*, int i, int j);

// Sparse matrices in CSR format

struct matrix_csr;

// Returns NULL on failure.  Failures can occur if the file does not
// exist, does not have the right magic number, if the expected number
// of bytes cannot be read, or if the matrix claims to have a negative
// number of nonzero elements, rows, or columns.  The vector must
// eventually be freed with free_matrix_csr().
struct matrix_csr* read_matrix_csr(const char* file);
// Returns 0 on success.  Failures can occur if the file cannot be
// opened or if any of the writes fail to write the expected number of
// bytes.
int write_matrix_csr(const char* file, struct matrix_csr*);
// Free any dynamically allocated memory related to the CSR matrix.
void free_matrix_csr(struct matrix_csr*);
// Return number of rows in the matrix.
int matrix_csr_n(struct matrix_csr*);
// Return number of columns in the matrix.
int matrix_csr_m(struct matrix_csr*);
// Return the element at position (i,j), which must be in-bounds.
double matrix_csr_idx(struct matrix_csr*, int i, int j);

// Arithmetic Operations.

// Dot product.
//
// Returns NULL if the two vectors are not of the same length.
struct scalar* mul_vv(struct vector* x, struct vector* y);

// Dense matrix-vector multiplication.
//
// Returns NULL if the number of columns in the matrix is not equal to
// the length of the vector.
struct vector* mul_mv(struct matrix_dense*, struct vector*);

// Dense transposed-matrix-vector multiplication.
//
// Returns NULL if the number of rows in the matrix is not equal to
// the length of the vector.
struct vector* mul_mTv(struct matrix_dense*, struct vector*);

// Sparse matrix-vector multiplication.
//
// Returns NULL if the number of columns in the matrix is not equal to
// the length of the vector.
struct vector* mul_spmv(struct matrix_csr*, struct vector*);

// Sparse transposed-matrix-vector multiplication.
//
// Returns NULL if the number of rows in the matrix is not equal to
// the length of the vector.
struct vector* mul_spmTv(struct matrix_csr*, struct vector*);

#endif
