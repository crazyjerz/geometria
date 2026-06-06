# algorithms

## Algorithms used

### Graham’s algorithm

Graham’s algorithm (also known as Graham scan; available with the
[`C.chull()`](https://crazyjerz.github.io/geometria/reference/C.chull.md)
function’s `"graham"` option) is the basic algorithm for computing the
convex hull. It has little practical importance and is mostly included
in this package as a curiosity.

Originally published in [Graham
(1972)](https://doi.org/10.1016%2F0020-0190%2872%2990045-2), more
information on [Wikipedia](https://en.wikipedia.org/wiki/Graham_scan).

### Andrew monotone chain

Andrew monotone chain (available with the
[`C.chull()`](https://crazyjerz.github.io/geometria/reference/C.chull.md)
function’s `"andrew"` option) is an algorithm similar to Graham scan,
working by computing the upper and lower hulls of a set of points. Its
recommended use comes in the case of sets of points where the resulting
hull is of size comparable to the initial size of the input.

Originally published in Andrew (1979).

### Quickhull

Quickhull (also spelt Quick**H**ull; available with the
[`C.chull()`](https://crazyjerz.github.io/geometria/reference/C.chull.md)
function’s `"quick"` option) is a divide-and-conquer algorithm utilising
an approach similar to quicksort. This is the algorithm with highest
practical speed (convex hull algorithms with lower algorithmic
complexity exist, however they are usually not faster in practice)

Originally published in Eddy (1977), more information on
[Wikipedia](https://en.wikipedia.org/wiki/Quickhull).
