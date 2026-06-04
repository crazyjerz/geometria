algorithms <- c("graham", "andrew", "quick")
modes <- c("values", "indices")

#' Convex hull
#'
#' Calculates convex hull.
#'
#' @param x Numeric vector of values corresponding to the points’ X coordinate.
#' @param y Numeric vector of values corresponding to the points’ Y coordinate.
#' @param algorithm String determining the algorithm used to compute the convex hull. Available values are: c("graham", "andrew", "quick").
#' @param mode String determining the return values of the function. Available values are: c("values", "indices")
#'
#' @return If mode = "values", returns a 2×length(x) matrix containing the coordinates of points forming the convex hull.
#' If mode = "indices", returns the indices of these points.
#'
#' @examples
#' set.seed(123)
#' 
#' x <- runif(1000)
#' y <- runif(1000)
#' 
#' C.chull(x, y, "graham", "values")
#' 
#' c <- C.chull(x, y)
#' 
#' C.chull(x[c], y[c]) 
#'
#' @export
C.chull <- function(x, y, algorithm = "quick", mode = "indices"){
    alg <- match(algorithm, algorithms)
    md <- match(mode, modes)
    if(is.na(alg)) stop("Erroneous algorithm")
    .Call("krak", as.numeric(x), as.numeric(y), as.integer(alg-1), as.integer(md))
}