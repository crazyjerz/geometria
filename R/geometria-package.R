#' geometria: Convex Hulls for R
#'
#' Package including methods to compute the convex hull of a set of points in 2D space.
#'
#' @keywords internal
"_PACKAGE"

#' @useDynLib geometria, .registration = TRUE
#' @importFrom stats runif rcauchy
#' @importFrom grDevices chull
#' @importFrom tinytest expect_true
NULL