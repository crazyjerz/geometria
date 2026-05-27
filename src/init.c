#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

extern SEXP chull(SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"krak", (DL_FUNC) &chull, 4},
    {NULL, NULL, 0}
};

void R_init_geometria(DllInfo *dll){
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
} 