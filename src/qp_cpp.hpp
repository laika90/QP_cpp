#ifndef QP_CPP_HPP
#define QP_CPP_HPP

#include <Eigen/Dense>
#include <iostream>

namespace QpCpp {

template<typename Scalar, int var_num, int con_num>
void solve(Eigen::Matrix<Scalar, var_num, var_num> &Q, Eigen::Matrix<Scalar, 1, var_num> &c,
           Eigen::Matrix<Scalar, con_num, var_num> &A, Eigen::Matrix<Scalar, con_num, 1> &b,
           Eigen::Matrix<Scalar, var_num, 1> &y)
{
    
}

}

#endif // OP_CPP_HPP