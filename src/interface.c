#include <R.h>
#include <Rinternals.h>
#include "hull.h"
#include "common.h"
#include "point.h"

typedef Point* (*hull)(Point* polygon, size_t size, size_t* out_size);

SEXP chull(SEXP X, SEXP Y, SEXP alg, SEXP restype){
    if(!Rf_isNumeric(X) || !Rf_isNumeric(Y)) Rf_error("The provided vectors are not numeric.");
    if(XLENGTH(X) != XLENGTH(Y)) Rf_error("The provided vectors are not of equal length.");
    if(XLENGTH(X) > 0x7fffffff) Rf_error("The provided vectors are too large.");
    if (!Rf_isInteger(alg) || XLENGTH(alg) != 1 || *INTEGER(alg) < 0 || *INTEGER(alg) > 2) Rf_error("Erroneous algorithm.");
    if (!Rf_isInteger(restype) || XLENGTH(restype) != 1 || *INTEGER(restype) < 1 || *INTEGER(restype) > 2) Rf_error("Erroneous algorithm.");

    Point* in = malloc(sizeof(Point)*Rf_length(X));
    size_t n = XLENGTH(X);
    double* px = REAL(X);
    double* py = REAL(Y);

    #pragma omp parallel for if(n > 100000)
    for(int i = 0; i < n; i++){
        in[i].x = px[i];
        in[i].y = py[i];
        in[i].idx = i+1;
    }

    size_t out_size;
    hull hulls[] = {chull_graham, chull_andrew, chull_quick};
    Point *result = hulls[*INTEGER(alg)](in, n, &out_size);
    if(*INTEGER(restype) == 1){
        SEXP out = PROTECT(allocMatrix(REALSXP, out_size, 2));
        double* outp = REAL(out);
        for(int i = 0; i < out_size; i++){
            outp[i] = result[i].x;
            outp[i + out_size] = result[i].y;
        }
        free(result);
        free(in);
        UNPROTECT(1);
        return out;
    }
    if(*INTEGER(restype) == 2){
        SEXP out = PROTECT(allocVector(INTSXP, out_size));
        int* outp = INTEGER(out);
        for(int i = 0; i < out_size; i++){
            outp[i] = result[i].idx;
        }
        free(result);
        free(in);
        UNPROTECT(1);
        return out;
    }
    Rf_error("unreachable");
    return R_NilValue;
}