/**
 * rot.h
 * Computing rotations in 3D
 *
 * @author Manuel Infosec
 * @version 1.0
 */

#ifndef ROT_H

#include <cmath>

#include "matrix.h"

/**
 * Euler Angle class, for computing rotations in 3D
 */

class euler_angle {
    matrix<long double>m;

    /**
     * Constructor for an euler_angle
     *
     * @param theta_x the rotation about the x axis, in radians
     * @param theta_y the rotation about the y axis, in radians
     * @param theta_z the rotation about the z axis, in radians
     */

    euler_angle(long double theta_x, long double theta_y, long double theta_z) {
        m = matrix<long double>(3, 3);
        matrix<long double> x = matrix<long double>(3, 3);
        x(0, 0) = 1.0;              x(0, 1) = 0.0;              x(0, 2) = 0.0;
        x(1, 0) = 0.0;              x(1, 1) = cos(theta_x);     x(0, 2) = -sin(theta_x);
        x(2, 0) = 0.0;              x(2, 1) = sin(theta_x);     x(2, 2) = cos(theta_x);
        matrix<long double> y = matrix<long double>(3, 3);
        y(0, 0) = cos(theta_y);     y(0, 1) = 0.0;              y(0, 2) = sin(theta_y);
        y(1, 0) = 0.0;              y(1, 1) = 1.0;              y(1, 2) = 0.0;
        y(2, 0) = -sin(theta_y);    y(2, 1) = 0.0;              y(2, 2) = cos(theta_y);
        matrix<long double> z = matrix<long double>(3, 3);
        z(0, 0) = cos(theta_z);     z(0, 1) = -sin(theta_z);    z(0, 2) = 0.0;
        z(1, 0) = sin(theta_z);     z(1, 1) = cos(theta_z);     z(1, 2) = 0.0;
        z(2, 0) = 0.0;             z(2, 1) = 0.0;              z(2, 2) = 1.0;

        m = z * y * x;
    }

    /**
     * Returns the rotational matrix
     *
     * @return a 3x3 matrix representing the Euler Angle.
     */
    inline matrix<long double> to_matrix() const {
        return m;
    }
}

#endif
