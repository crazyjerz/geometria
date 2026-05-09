#ifndef COMMON_H
#define COMMON_H
#include<stdlib.h>
#include"point.h"

extern int Angle(Point o, Point a, Point b);
extern double ScalarProduct(Point a, Point b);
extern int stack_empty(Stack* s);
extern int stack_init(Stack* s, size_t capacity);
extern void stack_destroy(Stack* s);
extern void stack_push(Stack* s, Point value);
extern Point stack_pop(Stack* s);
extern int stack_size(Stack* s);
extern Point stack_peek(Stack* s);
extern Point stack_subpeek(Stack* s);
extern void quicksort(Point* A, Point zero, int lo, int hi);
#endif