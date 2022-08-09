/**
 *  matrix.h
 *  Purpose: general matrix operations
 *
 *  @author Manuel Infosec
 *  @version 1.0
 */

#ifndef MATRIX_H

#define MATRIX_H

// I feel bad for including a whole header just for std::swap
// But it's far more efficient in most use cases than what I'd write myself...

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

/**
 *  degenerate_matrix_error class, thrown whenever one tries to invert a degenerate matrix
 */

class degenerate_matrix_error : public std::exception {

    public:

    virtual const char* what() const throw() {
      return "Attempted to take the inverse of degenerate matrix!";
    }
};

/**
 *  matrix class, for representation and manipulation of matrices
 *
 *  @param T the data type being stored in the matrix.
 */

template <typename T = int>
class matrix {

    private:

    std::vector<std::vector<T>> data;

    public:

    /**
     *  Default matrix constructor. All entries are set to their default.
     *
     *  @param Rows the number of rows in the matrix.
     *  @param Columns the number of columns in the matrix.
     */

    inline matrix (size_t Rows, size_t Columns) {
        for(size_t i = 0; i < Rows; ++ i) {
            data.push_back(std::vector<T>(Columns, T()));
        }
    }

    /**
     *  Alternate matrix constructor. All entries are set to t.
     *
     *  @param Rows the number of rows in the matrix.
     *  @param Columns the number of columns in the matrix.
     *  @param t the value to set all entries equal to.
     */

    inline matrix (size_t Rows, size_t Columns, T t) {
        for(size_t i = 0; i < Rows; ++ i) {
            data.emplace_back(Columns, t);
        }
    }

    /**
     *  Copy constructor.
     *
     *  @param m the matrix to copy.
     */
        
    inline matrix (const matrix<T> &m) {
        for(size_t i = 0; i < m.rows(); ++ i) {
            data.push_back(std::vector<T>());
            for(size_t j = 0; j < m.columns(); ++ j) {
                data[i].push_back(m(i,j));
            }
        }
    }

    /**
     *  Returns the identity matrix of size N x N.
     *
     *  @return a N x N identity matrix, if such a matrix is valid.
     */

    static inline matrix<T> identity(size_t N) {
        matrix<T>ret = matrix(N, N, T(0));

        for(size_t i = 0; i < N; ++ i)
            ret(i,i) = 1;

        return ret;
    }

    /**
     *  Retrieves the number of rows in the matrix.
     *
     *  @return the number of rows in the matrix.
     */

    inline size_t rows() const {
        if(data.empty()) {
            return 0;
        }
        return data.size();
    }


    /**
     *  Retrieves the number of columns in the matrix.
     *
     *  @return the number of columns in the matrix.
     */

    inline size_t columns() const {
        if(data.empty()) {
            return 0;
        }
        return data[0].size();
    }


    /**
     *  Adds two matrices together and returns their result
     *
     *  @param m the matrix to add.
     *  @return the sum of the two matrices.
     */

    inline matrix<T> operator + (const matrix<T> &m) const {
        assert(rows() == m.rows() && columns() == m.columns());

        matrix<T> ret;

        for(size_t i = 0; i < rows(); ++ i) {
            for(size_t j = 0; j < columns(); ++ j) {
                ret[i][j] = data[i][j] + m[i][j];
            }
        }

        return ret;
    }

    /**
     *  Negates all entries in the matrix.
     *
     *  @return the matrix scaled by -1.
     */

    inline matrix<T> operator - () const {
        matrix<T> ret;

        for(size_t i = 0; i < rows(); ++ i) {
            for(size_t j = 0; j < columns(); ++ j) {
                ret(i,j) = -data[i][j];
            }
        }

        return ret;
    }

    /**
     *  Subtracts two matrices and returns their result.
     *
     *  @param m the matrix to subtract.
     *  @return the difference of the two matrices.
     */

    inline matrix<T> operator - (const matrix<T> &m) const {
        return (*this) + -m;
    }

    /**
     *  Scales a matrix by a constant factor.
     *
     *  @param t the constant to scale the matrix by.
     *  @return the matrix scaled by t.
     */

    inline matrix<T> operator * (const T t) const {
        matrix<T> ret(*this);

        for(size_t i = 0; i < rows(); ++ i)
            for(size_t j = 0; j < columns(); ++ j)
                ret(i,j) = ret(i,j) * t;

        return ret;
    }

    /**
     *  Multiplies two matrices together and returns their result.
     *
     *  @param m the matrix to multiply by.
     *  @return the result of multiplying the two matrices.
     */

    inline matrix<T> operator * (const matrix<T> & m) const {
        assert(columns() == m.rows());

        matrix<T> ret = matrix(rows(), m.columns(), T(0));

        // The unusual order of loops is an optimization

        for(size_t i = 0; i < rows(); ++ i) {
            for(size_t k = 0; k < columns(); ++ k) {
                for(size_t j = 0; j < m.columns(); ++ j) {
                    ret(i,j) = ret(i, j) + data[i][k] * m(k,j);
                }
            }
        }

        return ret;
    }

    /**
     *  Computes the inverse of the matrix.
     *
     *  @param T1 the data type of the inverted matrix.
     *  @return the inverse matrix.
     *  @throws degenernate_matrix_error if the matrix is degenerate
     */

    template<typename T1 = long double>
    inline matrix<T1> inverse() const {
        assert(rows() == columns());

        matrix<T1> tmp = matrix(*this);
        matrix<T1> ret = matrix<T1>::identity(rows());

        // This is where the fun starts...

        for(size_t i = 0, j; i < rows(); ++ i) {
            for(j = i; j < rows() && tmp(j,i) == T1(0); ++ j);
            if(j == rows()) {
                throw degenerate_matrix_error();
            }
            if(i != j) {
                for(size_t k = i; k < columns(); ++ k) {
                    std::swap(tmp(i,k), tmp(j,k));
                    std::swap(ret(i,k), ret(j,k));
                }
            }
            for(int j = 0; j < columns(); ++ j) {
                if(j == i) continue;
                tmp(i,j) = tmp(i,j) / tmp(i,i);
                ret(i,j) = ret(i,j) / tmp(i,i);
            }
            ret(i,i) = ret(i,i) / tmp(i,i);
            tmp(i,i) = 1.0;
            for(j = 0; j < rows(); ++ j) {
                if(j == i) continue;
                T1 entry = tmp(j,i);
                for(size_t k = 0; k < columns(); ++k) {
                    tmp(j,k) = tmp(j,k) - (tmp(i,k) / tmp(i,i)) * entry;
                    ret(j,k) = ret(j,k) - (ret(i,k) / tmp(i,i)) * entry;
                }
            }
        }

        return ret;
    }

    /**
     *  Computes the matrix determinant.
     *
     *  @param T1 the data type of the determinant.
     *  @return the determinant.
     */

    template<typename T1 = long double>
    inline T1 determinant() const {
        assert(rows() == columns());

        matrix<T1> tmp = matrix(*this);

        T1 res = T1(1);

        bool b = 0;

        for(size_t i = 0, j; i < rows() - 1; ++ i) {
            for(j = i; j < rows() && tmp(j,i) == T1(0); ++ j);
            if(j == rows()) {
                return T1(0);
            }
            if(i != j) {
                for(size_t k = i; k < columns(); ++ k) {
                    std::swap(tmp(i,k), tmp(j,k));
                }
                b ^= 1;
            }
            for(j = i + 1; j < rows(); ++ j) {
                T entry = tmp(j,i);
                for(size_t k = i; k < columns(); ++ k) {
                    tmp(j,k) = tmp(j,k) - (tmp(i,k) / tmp(i,i)) * entry;
                }
            }
        }

        for(size_t i = 0; i < rows(); ++ i)
            res = res * tmp(i,i);

        return b ? -res : res;
    }

    /**
     *  Allows access to the matrix entries.
     *
     *  @param row the row the entry is in.
     *  @param column the column the entry is in.
     *  @return a reference corresponding the the [row][column]-th element of the matrix.
     */

    inline T &operator () (size_t row, size_t column) {
        return data[row][column];
    }

    /**
     *  Allows access to the matrix entries.
     *  
     *  @param row the row the entry is in.
     *  @param column the column the entry is in.
     *  @return a const_reference corresponding to the [row][column]-th element of the matrix.
     */

    inline const T &operator () (size_t row, size_t column) const {
        return data[row][column];
    }

    /**
     *  Checks if two matrices are not equal to each other.
     *
     *  @param m the matrix to compare to.
     *  @return true if the two are not equal, and false otherwise.
     */

    inline bool operator != (const matrix<T> &m) const {
        if(rows() != m.rows() || columns() != m.columns()) return true;
        for(size_t i = 0; i < rows(); ++ i)
            for(size_t j = 0; j < columns(); ++ j)
                if(data[i][j] != m[i][j])
                    return true;
        return false;
    }

    /**
     *  Checks if two matrices are equal to each other.
     *
     *  @param m the matrix to compare to.
     *  @return true if the two are equal, and false otherwise.
    */

    inline bool operator == (const matrix<T> &m) const {
        return !((*this) != m);
    }

    /**
     *  Computes the transpose of the matrix.
     *
     *  @return the transposed matrix.
     */

    inline matrix transpose() const {
        matrix ret = matrix(columns(), rows());

        for(size_t i = 0; i < rows(); ++ i)
            for(size_t j = 0; j < columns(); ++ j)
                ret(j,i) = data[i][j];

        return ret;
    }
};

/**
 *  Override to print a matrix with an std::ostream.
 *
 *  @param out the ostream to print on.
 *  @param m the matrix to print.
 *  @return out.
 */

template <typename T>
std::ostream& operator <<(std::ostream &out, const matrix<T> &m){
    for(size_t i = 0; i < m.rows(); ++ i){
        for(size_t j = 0; j < m.columns(); ++ j) {
            out << m(i,j);
            if(j < m.columns() - 1){
                out << " ";
            }
        }
        out << "\n";
    }
    return out;
}

#endif
