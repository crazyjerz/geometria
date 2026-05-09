#ifndef POINT
#define POINT

typedef struct{
    double x;
    double y;
} Point;

typedef struct {
    Point* data;
    size_t size;
    size_t capacity;
} Stack;

#endif