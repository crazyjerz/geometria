library(tinytest)
library(geometria)
set.seed(123) # consistency
hull_equal <- function(x, y, h1, h2, tol = 1e-12){
    if(length(h1) != length(h2)) return(FALSE)

    pts <- cbind(x, y)
    
    p1 <- unique(pts[h1, , drop = FALSE])
    p2 <- unique(pts[h2, , drop = FALSE])
    
    p1 <- p1[order(p1[,1], p1[,2]), , drop = FALSE]
    p2 <- p2[order(p2[,1], p2[,2]), , drop = FALSE]
    
    if(nrow(p1) != nrow(p2)) return(FALSE)
    
    all(abs(p1 - p2) < tol)
}
run <- function(x, y, v = c(1, 2, 3)){
    if(1 %in% v) expect_true(hull_equal(x, y, C.chull(x, y, "graham", "indices"), chull(x, y)))
    if(2 %in% v) expect_true(hull_equal(x, y, C.chull(x, y, "andrew", "indices"), chull(x, y)))
    if(3 %in% v) expect_true(hull_equal(x, y, C.chull(x, y, "quick", "indices"), chull(x, y)))
}
# test 1: triangle with many collinearities
run(c((0:1e4)/1e4, rep(1, 1e4+1), (0:1e4)/1e4), c(rep(0, 1e4+1), (0:1e4)/1e4, (0:1e4)/1e4), 1)
run(c((0:1e5)/1e5, rep(1, 1e5+1), (0:1e5)/1e5), c(rep(0, 1e5+1), (0:1e5)/1e5, (0:1e5)/1e5), 2)
run(c((0:1e7)/1e7, rep(1, 1e7+1), (0:1e7)/1e7), c(rep(0, 1e7+1), (0:1e7)/1e7, (0:1e7)/1e7), 3)

# test 2: large number of duplicated points
run(rep(0, 1e4), rep(0, 1e4), 1)
run(rep(0, 1e6), rep(0, 1e6), 2)
run(rep(0, 1e8), rep(0, 1e8), 3)

# test 3: large number of points with a very small hull
run(rcauchy(1e4), rcauchy(1e4), 1)
run(rcauchy(1e6), rcauchy(1e6), 2)
run(rcauchy(1e8), rcauchy(1e8), 3)

# test 4: microscopic hull coordinates
run(runif(1e6)/1e6, runif(1e6)/1e6)

# test 5: big hull coordinates
run(runif(1e6)*1e12, runif(1e6)*1e12)

# test 6: chull of chull
x <- runif(1e7)
y <- runif(1e7)
c <- chull(x, y)
run(x[c], y[c])