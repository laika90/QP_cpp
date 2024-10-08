#ifndef QP_CPP_HPP
#define QP_CPP_HPP

#include <Eigen/Dense>
#include <iostream>
#include <algorithm>
#include <type_traits> 

namespace QpCpp {

template<typename Scalar>
inline Scalar cube(Scalar x) { return x * x * x; };

template<typename Scalar, int var_num, int con_num>
void solve(const Eigen::Matrix<Scalar, var_num, var_num> &G, const Eigen::Matrix<Scalar, var_num, 1> &g,
           const Eigen::Matrix<Scalar, con_num, var_num> &A, const Eigen::Matrix<Scalar, con_num, 1> &b,
           Eigen::Matrix<Scalar, var_num, 1> &ans)
{
    static_assert(std::is_same<Scalar, double>::value || std::is_same<Scalar, float> ::value, "Please specify either double or float.");

    // alias
    using PrimalVector = Eigen::Matrix<Scalar, var_num, 1>;
    using DualVector   = Eigen::Matrix<Scalar, con_num, 1>;
    
    const Eigen::Matrix<Scalar, var_num, con_num> At = A.adjoint();

    // deceleration factor
    Scalar eta = 0.95;

    // finish criteria
    Scalar epsilon;
    if      (std::is_same<Scalar, double>::value) { epsilon = 1E-9; }
    else if (std::is_same<Scalar, float> ::value) { epsilon = 1E-4f; }
    int max_iteration = 10;
    int iter_count = 0;

    // initial value
    PrimalVector x = PrimalVector::Zero();
    DualVector   z = DualVector  ::Ones();
    DualVector   s = DualVector  ::Ones();

    // initial residuals
    PrimalVector rd  = g - At*z;
    DualVector   rp  = s - b;
    DualVector   rsl = s.array() * z.array();

    // duality measure (complementary measure)
    Scalar mu = (Scalar)var_num / (Scalar)con_num;

    PrimalVector dx;
    DualVector   dz;
    DualVector   ds;
    PrimalVector rbar; 

    while (true)
    {
        // cholesky decomposition
        Eigen::LLT<Eigen::Matrix<Scalar, var_num, var_num>> Gbar = (G + At * ((z.array() / s.array()).matrix().asDiagonal()) * A).llt();
        
        // prediction -- determine affine scaling direction (dx_aff, dz_aff, ds_aff)
        rbar = At * ( ((rsl.array() - z.array()*rp.array()) / s.array()).matrix() );
        dx   = Gbar.solve(-(rd + rbar));
        ds   = A*dx - rp;
        dz.array() = -(rsl.array() + z.array()*ds.array()) / s.array();

        // determine alpha_aff
        Scalar alpha_aff_z = std::min( 1.0, ((dz.array() < 0).select(-z.array()/dz.array(), 1)).minCoeff() );
        Scalar alpha_aff_s = std::min( 1.0, ((ds.array() < 0).select(-s.array()/ds.array(), 1)).minCoeff() );
        Scalar alpha_aff   = std::min( alpha_aff_z, alpha_aff_s );

        // compute complementary measure (aff)
        Scalar mu_aff = (s+alpha_aff*ds).dot(z+alpha_aff*dz) / (Scalar)con_num;

        // compute centering parameter
        Scalar sigma = cube(mu_aff / mu);

        // cerntering 
        rsl.array() += ds.array() * dz.array() - sigma*mu;

        // correction
        rbar = At * ( ((rsl.array() - z.array()*rp.array()) / s.array()).matrix() );
        dx   = Gbar.solve(-(rd + rbar));
        ds   = A*dx - rp;
        dz.array() = -(rsl.array() + z.array()*ds.array()) / s.array();

        // determine alpha
        Scalar alpha_z = std::min( 1.0, ((dz.array() < 0).select(-z.array()/dz.array(), 1)).minCoeff() );
        Scalar alpha_s = std::min( 1.0, ((ds.array() < 0).select(-s.array()/ds.array(), 1)).minCoeff() );
        Scalar alpha   = std::min( alpha_z, alpha_s );

        std::cout <<"ds  "<<   ds << "  " << "s  " << s << std::endl;
        std::cout << "alpha  " << alpha << std::endl;

        // update state
        alpha *= eta; // decelerate
        x += alpha*dx;
        z += alpha*dz;
        s += alpha*ds;

        // update residuals
        rd  = G*x + g - At*z;
        rp  = s + A*x - b;
        rsl = s.array()*z.array();

        // update duality measure
        mu = s.dot(z) / (Scalar)con_num;

        // update iteration count
        iter_count += 1;

        if ( rd.norm() < epsilon && rp.norm() < epsilon && std::abs(mu) < epsilon ) { break; }
        else if ( iter_count > max_iteration ) { break; }

    }

    ans = x;

}

}

#endif // OP_CPP_HPP