#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
arma::colvec soft_thresh(arma::colvec x, double tau)
{
    int p = x.n_rows;
    return (arma::sign(x) % arma::max(arma::zeros<arma::colvec>(p), arma::abs(x) -
                                                                        tau));
}
// [[Rcpp::export]]
arma::colvec admm_lasso(arma::mat X, arma::colvec y, double tau, int maxit, double tol)
{
    arma::mat XX = X.t() * X;
    arma::mat Xy = X.t() * y;
    int p = X.n_cols;
    arma::colvec lambda = arma::zeros<arma::colvec>(p);
    // double maxRho = 5.0;
    double rho = 4.0;
    arma::colvec z0 = arma::zeros<arma::colvec>(p);
    arma::colvec z = arma::zeros<arma::colvec>(p);
    arma::colvec beta0 = arma::zeros<arma::colvec>(p);
    arma::colvec beta = arma::zeros<arma::colvec>(p);
    arma::mat Sinv = arma::inv(XX + arma::eye(p, p) * rho);
    for (int i = 1; i <= maxit; i++)
    {
        beta = Sinv * (Xy + rho * z - lambda);
        z = soft_thresh(beta + lambda * (1 / rho), tau * (1 / rho));
        lambda = lambda + rho * (beta - z);
        double change = std::max(arma::norm(beta - beta0, 2), arma::norm(z -
                                                                             z0,
                                                                         2));
        if (change < tol || i > maxit)
        {
            break;
        }
        beta0 = beta;
        z0 = z;
    }
    return (z);
}
