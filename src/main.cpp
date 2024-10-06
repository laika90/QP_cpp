#include <iostream>
#include <Eigen/Dense>

int main()
{
    Eigen::Vector3d a(1,2,3);
    Eigen::Vector3d b(0,1,2);

    std::cout << a.array() * b.array() << std::endl;

}