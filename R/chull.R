algorithms <- c("graham", "andrew", "quick")

C.chull <- function(x, y, algorithm = "andrew") {
    alg <- match(algorithm, algorithms)
    print(alg)
    if(is.na(alg)) stop("Erroneous algorithm")
    .Call("krak", as.numeric(x), as.numeric(y), as.integer(alg-1), 1L)
}