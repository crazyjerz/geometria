#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

extern SEXP chull(SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"chull", (DL_FUNC) &chull, 3},
    {NULL, NULL, 0}
};

void R_init_mypackage(DllInfo *dll){
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}