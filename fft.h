/**
 * fft.h
 * Purpose: FFT library
 *
 * @author Manuel Infosec
 * @version 1.0
 */

#ifndef FFT_H

#define FFT_H

#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>

/**
 * An iterative implementation of the Fast Fourier Transform.
 *
 * @param P an std::vector of std::complex<long double> representing the values to compute the FFT of. Note that this vector will be overwritten with the values returned by the FFT.
 * @param inv pass 1 to compute the Fast Fourier Transform, and -1 to compute the inverse Fast Fourier Transform.
 */

void FFT(std::vector<std::complex<long double>> &P, int inv = 1) {
    static std::vector<std::complex<long double>> roots;

    size_t length = 1;

    while(length < P.size()) length <<= 1;

    P.resize(length);

    for(size_t i = 1, j = 0; i < P.size(); ++ i) {
        int b = P.size() >> 1;
        while(j >= b) {
            j -= b;
            b >>= 1;
        }
        if(i < j) std::swap(P[i], P[j]);
    }

    long double theta = inv * 2 * M_PI / P.size();

    if(roots.size() != P.size() / 2) {
        roots.clear();
        roots.resize(P.size());
        for(size_t i=0; i < P.size() / 2; ++ i)
            roots[i] = std::complex<long double> (cos(theta * i), sin(theta * i));
    }

    for(size_t i = 2; i <= P.size(); i <<= 1) {
        int layer = P.size() / i;
        for(size_t j = 0; j < P.size(); j += i) {
            for(size_t k = 0; k < i / 2; ++ k) {
                auto u = P[j + k];
                auto v = P[j + k + i / 2] * roots[layer * k];
                P[j + k] = u + v;
                P[j + k + i / 2] = u - v;
            }
        }
    }

    if(inv == -1) {
        for(auto & a: P) {
            a /= P.size();
        }
    }
}

#endif
