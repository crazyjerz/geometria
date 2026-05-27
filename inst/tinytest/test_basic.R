library(tinytest)
library(geometria)

run <- function(x, y){
    x <- x + runif(1)
    y <- y + runif(1)
    expect_true(hull_equal(x, y, C.chull(x, y, "graham", "indices"), chull(x, y)))
    expect_true(hull_equal(x, y, C.chull(x, y, "andrew", "indices"), chull(x, y)))
    expect_true(hull_equal(x, y, C.chull(x, y, "quick", "indices"), chull(x, y)))
}

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


# test 1: basic point-in-triangle
run(c(0, 0, 4, 1), c(0, 4, 0, 1))

# test 2: basic collinearity
run(c(0, 1, 2, 3), c(0, 1, 2, 3))

# test 3: point duplication
run(c(0, 0, 1, 1, 0, 0), c(0, 0, 0, 0, 1, 1))

# test 4: single point duplicated
run(c(0, 0, 0, 0, 0), c(0, 0, 0, 0, 0))

# test 5: duplicates inside of triangle
run(c(-1, 1, 1, 0, 0, 0, 0, 0), c(-1, -1, 0, 0, 0, 0, 0, 0))
