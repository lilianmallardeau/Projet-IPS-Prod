#include "../headers/Basis.h"
#include "../headers/MathTools.h"
#include "../headers/Poly.h"
#include "math.h"

Basis::Basis(double br, double bz, double N, double Q) : br(br), bz(bz), N(N), Q(Q) {
    int i = -20;
    while (n_zmax_i(i) >= 1) {
        std::cout << n_zmax_i(i) << " | " << i << std::endl;
        i++;
    }
    std::cout << n_zmax_i(i) << std::endl;
    this->mMax = i == 0 ? 0 : i-1;
    this->mMax = 14;

    std::cout << "blabla " << mMax << std::endl;

    arma::vec n_Max = arma::vec(mMax);
    for (int m = 0 ; m < mMax ; m++) {
        n_Max(m) = 0.5 * (mMax - m - 1) + 1;
    }
    this->nMax = n_Max;
    
    int n = nMax[mMax - 1];
    arma::mat n_z_Max = arma::mat(mMax, n);
    for (int j = 0 ; j < mMax ; j++) {
        for (int k = 0 ; k < n ; k++) {
            n_z_Max(j,k) = n_zmax_i(j + 2*k + 1);
        }
    }
    this->n_zMax = n_z_Max;
}

double Basis::n_zmax_i(int i) {
    return (N+2) * pow(Q,2/3) + 1/2 - i * Q;
}

arma::vec Basis::rPart(arma::vec r, int m, int n) {
    Poly poly{};
    double cst = (1/(br * sqrt(M_PIl))) * sqrt(MathTools::factorial(n) / MathTools::factorial(n + abs(m))); // pow(r / br, abs(m))
    arma::vec exp = arma::exp(- r % r / (2 * br * br));
    poly.calcLaguerre(abs(m), n, r % r / (br * br));
    return cst * exp % arma::pow(r / br, abs(m)) % poly.laguerre(abs(m), n);
}

arma::vec Basis::zPart(arma::vec z, int n_z) {
    Poly poly{};
    double cst = 1 / sqrt(bz * pow(2, n_z) * sqrt(M_PIl) * MathTools::factorial(n_z));
    arma::vec exp = arma::exp(- z % z / (2 * bz * bz));
    poly.calcHermite(n_z, z / bz);
    return exp % poly.hermite(n_z) * cst;
}