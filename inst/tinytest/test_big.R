library(tinytest)
library(geometria)

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