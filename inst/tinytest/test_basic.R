library(tinytest)
library(geometria)

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
