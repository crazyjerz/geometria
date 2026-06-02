algorithms <- c("graham", "andrew", "quick")
modes <- c("values", "indices")

#' Convex hull
#'
#' Calculates convex hull.
#'
#' @param x Numeric.
#' @param y Numeric.
#' @param algorithm String.
#' @param mode String.
#'
#' @return Numeric.
#'
#' @examples
#' C.chull(c(0, 0, 1), c(0, 1, 0))
#'
#' @export
C.chull <- function(x, y, algorithm = "andrew", mode = "values") {
    alg <- match(algorithm, algorithms)
    md <- match(mode, modes)
    if(is.na(alg)) stop("Erroneous algorithm")
    .Call("krak", as.numeric(x), as.numeric(y), as.integer(alg-1), as.integer(md))
}