algorithms <- c("graham", "andrew", "quick")
modes <- c("values", "indices")

C.chull <- function(x, y, algorithm = "andrew", mode = "values") {
    alg <- match(algorithm, algorithms)
    md <- match(mode, modes)
    if(is.na(alg)) stop("Erroneous algorithm")
    .Call("krak", as.numeric(x), as.numeric(y), as.integer(alg-1), as.integer(md))
}