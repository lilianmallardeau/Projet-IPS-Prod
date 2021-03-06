#ifndef HERMIT_H
#define HERMIT_H

/**
 * @file Poly.h
 */


#include <armadillo> // using tuple for fast Poly polynomial computing

/*
 * Polynomials class
 *
 * Class used to compute and evaluate calcHermite and laguerre polynomials.
 * Objects of this class must be initiated with a mesh_hermite, an `arma::colvec` on which to evaluate the polynomials. This mesh_hermite CANNOT BE CHANGED after object instantiation.
 */
class Poly {
    public:
        Poly(){};
        void calcHermite(int, arma::vec);
        arma::vec hermite(int); /*!< Getter for Hermite polynomials values. */
        void calcLaguerre(int, int, arma::vec);
        arma::vec laguerre(int, int); /*!< Getter for Laguerre polynomials values. */

    private:
        arma::colvec mesh; /*!< Mesh on which polynomials of this instance are evaluated. */
        arma::mat hermite_values; /*!< Matrix in which calcHermite evaluation are stored. */
        arma::cube laguerre_values; /*!< Matrix in which calcLaguerre evaluation are stored. */
};


#endif // HERMIT_H