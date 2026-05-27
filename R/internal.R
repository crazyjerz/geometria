run <- function(x, y, v = c(1, 2, 3)){
    x <- x + runif(1)
    y <- y + runif(1)
    if(1 %in% v) expect_true(hull_equal(x, y, C.chull(x, y, "graham", "indices"), chull(x, y)))
    if(2 %in% v) expect_true(hull_equal(x, y, C.chull(x, y, "andrew", "indices"), chull(x, y)))
    if(3 %in% v) expect_true(hull_equal(x, y, C.chull(x, y, "quick", "indices"), chull(x, y)))
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