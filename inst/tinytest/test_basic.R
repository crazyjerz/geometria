library(tinytest)
library(geometria)

# test 1
x <- c(0, 0, 4, 1)
y <- c(0, 4, 0, 1)
C.chull(c(0, 0, 4, 1), c(0, 4, 0, 1), "graham")
C.chull(c(0, 0, 4, 1), c(0, 4, 0, 1), "andrew")
C.chull(c(0, 0, 4, 1), c(0, 4, 0, 1), "quick")
