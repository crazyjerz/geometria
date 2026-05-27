library(tinytest)
library(geometria)

# test 1: 1e7 duplicated equal points
run(rep(0, 1e7), rep(0, 1e7))

# test 2: triangle of 1e6 points on each side
run(c((0:1e6)/(1e6), (0:1e6)/(1e6), rep(1, 1e6+1)), c(rep(0, 1e6+1), (0:1e6)/(1e6), (0:1e6)/(1e6)), c(2, 3))

# test 3: extreme collinearity
run(0:1e5, 0:1e5, 1)
run(0:1e6, 0:1e6, 2)
run(0:1e8, 0:1e8, 3)