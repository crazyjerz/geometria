#ifndef POINT
#define POINT
#define eps (1e-12)

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