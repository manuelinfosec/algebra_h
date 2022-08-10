# algebra\_h

Header files for some common (and not so common) linear algebra operations.

## `fft.h`

An FFT implementation in C++, using complex numbers and trigonometric functions.

The function `fft` takes an `std::vector` of `std::complex<long double>` and returns the coefficients of the complex representation of the covolution.

Pass `inv = 1` to preform the covolution and `inv = -1` to preform an inverse FFT.

## `matrix.h`

Contains a matrix class, which defines:

- Addition
- Subtraction
- Multiplication
- Inverse
- Determinant
- Transpose

## `gauss.h`

Solves a system of linear equations. Also known as row reduction, this method can compute:

- rank of a matrix
- determinant of a square matrix
- inverse of an invertible matrix

## `vector.h`

Contains a vector class, which defines:

- Addition
- Subtraction
- Multiplication (by a constant)
- Cross Product
- Dot Product
- Magnitude
- Normalization
