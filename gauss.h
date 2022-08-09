/**
 *  gauss.h
 *  Purpose: Solves a system of simultaneous linear equations
 *
 *  @author Manuel Infosec
 *  @version 1.0
 */

#ifndef GAUSS_H

#define GAUSS_H

#include "matrix.h"

/**
 *  Preforms Gaussian elimination to solve a system of linear equations.
 *
 *  @param A the matrix representing the linear equations
 *  @param Y the column vector representing the resultant.
 *  @return an std::vector representing the values
 */
template<typename T>
std::vector<T> gauss(std::vector<std::vector<T>> A, std::vector<T> Y) {
    assert(A.size() == Y.size());
    for(auto &a : A) {
        assert(a.size() <= A.size());
    }

    matrix<T> m = matrix<T>(A.size(), A.size());
    for(size_t i = 0; i < A.size(); ++ i) {
        for(size_t j = 0; j < A[i].size(); ++ j) {
            m(i,j) = A[i][j];
        }
    }

    matrix<T> y = matrix<T>(1, A.size());
    for(size_t i = 0; i < Y.size(); ++ i) {
        m(0,i) = Y[i];
    }

    return m.inverse() * y;
}

#endif
