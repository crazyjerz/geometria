#ifndef POINT
#define POINT
#define eps (1e-12)
#define dup(a, b) (fabs(a.x - b.x) < eps && fabs(a.y - b.y) < eps)
typedef struct{
    double x;
    double y;
    int idx;
} Point;

typedef struct {
    Point* data;
    size_t size;
    size_t capacity;
} Stack;

#endif