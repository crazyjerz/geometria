#include <R.h>
#include <Rinternals.h>
#include "hull.h"
#include "common.h"
#include "point.h"
typedef Point* (*hull)(Point* polygon, size_t size, size_t* out_size);

SEXP chull(SEXP X, SEXP Y, SEXP alg){
    if(!Rf_isNumeric(X) || !Rf_isNumeric(Y)) Rf_error("The provided vectors are not numeric.");
    if(XLENGTH(X) != XLENGTH(Y)) Rf_error("The provided vectors are not of equal length.");
    if(XLENGTH(X) > 0x7fffffff) Rf_error("The provided vectors are too large.");
    if (!Rf_isInteger(alg) || XLENGTH(alg) != 1 || *INTEGER(alg) < 0 || *INTEGER(alg) > 2) Rf_error("Erroneous algorithm.");
    Point* in = malloc(sizeof(Point)*Rf_length(X));
    size_t n = XLENGTH(X);
    double* px = REAL(X);
    double* py = REAL(Y);
    for(int i = 0; i < XLENGTH(Y); i++){
        in[i].x = px[i];
        in[i].y = py[i];
    }
    size_t out_size;
    hull hulls[] = {chull_graham, chull_andrew, chull_quick};
    Point *result = hulls[*INTEGER(alg)](in, n, &out_size);
    free(in);
    SEXP rx = PROTECT(allocVector(REALSXP, out_size));
    SEXP ry = PROTECT(allocVector(REALSXP, out_size));
    for(int i = 0; i < out_size; i++){
        REAL(rx)[i] = result[i].x;
        REAL(ry)[i] = result[i].y;
    }
    SEXP out = PROTECT(allocVector(VECSXP, 2));
    SET_VECTOR_ELT(out, 0, rx);
    SET_VECTOR_ELT(out, 1, ry);
    free(result);
    UNPROTECT(3);
    return out;
}