
run.error <- function(x, y, v = c(1, 2, 3)){
    if(1 %in% v) expect_error(hull_equal(x, y, C.chull(x, y, "graham", "indices")))
    if(2 %in% v) expect_error(hull_equal(x, y, C.chull(x, y, "andrew", "indices")))
    if(3 %in% v) expect_error(hull_equal(x, y, C.chull(x, y, "quick", "indices")))
}

# test 1: unequal lengths
run.error(1, 1:1000)

# test 2: NA detection
run.error(c(1:100, NA, 1:100, NA, 1:100), c(NA, 1:150, NA, 1:150, NA))

# test 3: non-numbers detection
run.error(c("pafy"), c("pafy"))