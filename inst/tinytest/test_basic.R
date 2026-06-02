library(tinytest)
library(geometria)

# test 1: basic point-in-triangle
geometria:::run(c(0, 0, 4, 1), c(0, 4, 0, 1))

# test 2: basic collinearity
geometria:::run(c(0, 1, 2, 3), c(0, 1, 2, 3))

# test 3: point duplication
geometria:::run(c(0, 0, 1, 1, 0, 0), c(0, 0, 0, 0, 1, 1))

# test 4: single point duplicated
geometria:::run(c(0, 0, 0, 0, 0), c(0, 0, 0, 0, 0))

# test 5: duplicates inside of triangle
geometria:::run(c(-1, 1, 1, 0, 0, 0, 0, 0), c(-1, -1, 0, 0, 0, 0, 0, 0))
