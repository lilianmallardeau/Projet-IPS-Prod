#include "../headers/OptimizedRho.h"
#include "../headers/Basis.h"


/**
 * Compute atomic density with optimized algorithm
 *
 * With given basis, density doesn't depend on theta, so r and z are the only arguments.
 * Algorithm is based on naive algorithm, but with several optimization:
 * Indexes are computed and stored before main loop.
 * Rho values is a m-diagonal matrix, removes one loop.
 * Basis function can be decomposed in its R and Z parts, each computed in different places to avoid computing redundancy.
 *
 * @param zVals input values for z. Armadillo vector to optimize compute time with Boost library
 * @param rVals input values for r. Armadillo vector to optimize compute time with Boost library
 * @return matrix with atomic density values in cylindrical coordinate system. Z values are along lines, r values along rows.
 */
arma::mat OptimizedRho::density(arma::vec zVals, arma::vec rVals) {
    arma::mat rho;
    rho.load("rho.arma", arma::arma_ascii);
    arma::mat result = arma::zeros(zVals.size(), rVals.size()); // number of points on r- and z- axes
    Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
/*rPart_b.t()*/
    int i = 0;
    arma::mat zParts(zVals.n_elem, basis.n_zMax(0, 0));
//    arma::cube rParts(rVals.n_elem, basis.mMax, basis.nMax(0));
    arma::icube index(basis.mMax, basis.nMax(0), basis.n_zMax(0, 0), arma::fill::zeros);
    for (int m = 0; m < basis.mMax; ++m) {
        for (int n = 0; n < basis.nMax(m); ++n) {
//            rParts.slice(n).col(m) = basis.rPart(rVals, m, n);
            for (int n_z = 0; n_z < basis.n_zMax(m, n); ++n_z) {
                if (m == 0 && n == 0) {
                    zParts.col(n_z) = basis.zPart(zVals, n_z);
                }
                index(m, n, n_z) = i;
                i++;
            }
        }
    }

    arma::vec rPart_a;
    arma::vec rPart_b;
    for (int m_a = 0; m_a < basis.mMax; m_a++)
    {
        for (int n_a = 0; n_a < basis.nMax(m_a); n_a++)
        {
            rPart_a = basis.rPart(rVals, m_a, n_a);
            for (int n_z_a = 0; n_z_a < basis.n_zMax(m_a, n_a); n_z_a++)
            {
                arma::mat funcA = zParts.col(n_z_a) * rPart_a.t() /*rParts.slice(n_a).col(m_a)*/;
                for (int n_b = 0; n_b < basis.nMax(m_a); n_b++)
                {
                    rPart_b = basis.rPart(rVals, m_a, n_b);
                    for (int n_z_b = 0; n_z_b < basis.n_zMax(m_a, n_b); n_z_b++) {
                        arma::mat funcB = zParts.col(n_z_b) * rPart_b.t() /*rParts.slice(n_b).col(m_a)*/;
                        result += funcA % funcB * rho(index(m_a, n_a, n_z_a), index(m_a, n_b, n_z_b));
                    }
                }
            }
        }
    }
    return result;
}
