# Benchmarks

## Comparison with builtin `grDevices::chull`

Each sample is of the size of `100 000 000` points.

`rcauchy`, `runif`, `rnorm` – numbers drawn from the Cauchy, uniform and
normal distribution.

“Segment”: all points are collinear and located on the `(0, 0) – (1, 1)`
segment.

Tested on: Windows 10 IoT Enterprise LTSC, Intel Core i7-14700HX @ 2.10
GHz (20c/28t).

![](benchmarks_files/figure-html/unnamed-chunk-2-1.png)
