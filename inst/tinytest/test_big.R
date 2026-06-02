library(tinytest)
library(geometria)

# test 1: triangle with many collinearities
geometria:::run(c((0:1e4)/1e4, rep(1, 1e4+1), (0:1e4)/1e4), c(rep(0, 1e4+1), (0:1e4)/1e4, (0:1e4)/1e4), 1)
geometria:::run(c((0:1e5)/1e5, rep(1, 1e5+1), (0:1e5)/1e5), c(rep(0, 1e5+1), (0:1e5)/1e5, (0:1e5)/1e5), 2)
geometria:::run(c((0:1e7)/1e7, rep(1, 1e7+1), (0:1e7)/1e7), c(rep(0, 1e7+1), (0:1e7)/1e7, (0:1e7)/1e7), 3)

# test 2: large number of duplicated points
geometria:::run(rep(0, 1e4), rep(0, 1e4), 1)
geometria:::run(rep(0, 1e6), rep(0, 1e6), 2)
geometria:::run(rep(0, 1e8), rep(0, 1e8), 3)

# test 3: large number of points with a very small hull
geometria:::run(rcauchy(1e4), rcauchy(1e4), 1)
geometria:::run(rcauchy(1e6), rcauchy(1e6), 2)
geometria:::run(rcauchy(1e8), rcauchy(1e8), 3)

# test 4: microscopic hull coordinates
geometria:::run(runif(1e6)/1e6, runif(1e6)/1e6)

# test 5: big hull coordinates
geometria:::run(runif(1e6)*1e12, runif(1e6)*1e12)

# test 6: chull of chull
x <- runif(1e7)
y <- runif(1e7)
c <- chull(x, y)
geometria:::run(x[c], y[c])