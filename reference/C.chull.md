# Convex hull

Calculates convex hull.

## Usage

``` r
C.chull(x, y, algorithm = "quick", mode = "indices")
```

## Arguments

- x:

  Numeric vector of finite values corresponding to the points’ X
  coordinate.

- y:

  Numeric vector of finite values corresponding to the points’ Y
  coordinate.

- algorithm:

  String determining the algorithm used to compute the convex hull.
  Available values are: c("graham", "andrew", "quick").

- mode:

  String determining the return values of the function. Available values
  are: c("values", "indices")

## Value

If mode = "values", returns a 2×length(x) matrix containing the
coordinates of points forming the convex hull. If mode = "indices",
returns the indices of these points.

## Examples

``` r
set.seed(123)

x <- runif(1000)
y <- runif(1000)

C.chull(x, y, "andrew", "values")
#>               [,1]       [,2]
#>  [1,] 0.0004653491 0.99952398
#>  [2,] 0.0006247733 0.80047637
#>  [3,] 0.0038963431 0.05241685
#>  [4,] 0.0704889777 0.01152176
#>  [5,] 0.2675214321 0.00115582
#>  [6,] 0.8397677648 0.00247881
#>  [7,] 0.9674694943 0.00484174
#>  [8,] 0.9714380668 0.01353063
#>  [9,] 0.9842192035 0.05803070
#> [10,] 0.9921706193 0.12730974
#> [11,] 0.9994045279 0.48867354
#> [12,] 0.9966172187 0.80051071
#> [13,] 0.9949364401 0.94777166
#> [14,] 0.9930447803 0.98861006
#> [15,] 0.9770989963 0.99301551
#> [16,] 0.8312219789 0.99637198
#> [17,] 0.2529649285 0.99888265

c <- C.chull(x, y)

C.chull(x[c], y[c]) 
#>  [1]  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
```
