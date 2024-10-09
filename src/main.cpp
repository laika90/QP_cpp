#include <iostream>
#include <Eigen/Dense>
#include "qp_cpp.hpp"

int main()
{
    Eigen::Matrix<double, 2, 2> G;
    G << 1, -1, -1, 2;
    Eigen::Matrix<double, 2, 1> g(-2, -6);
    Eigen::Matrix<double, 3, 2> A;
    A << 1, 1, -1, 2, 2, 1;
    Eigen::Matrix<double, 3, 1> b(2,2,3);
    Eigen::Matrix<double, 2, 1> ans(1,2);

    QpCpp::solve(G, g, A, b, ans);

    std::cout << ans << std::endl;
}