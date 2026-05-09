#include<stdio.h>
#include<stdlib.h>
#include"common.h"
#include"point.h"
#include<omp.h>
#include<math.h>

#define cmp(X, Y) (fabs(X-Y)<1e-12)
#define min(X, Y) (X < Y ? X : Y)

Point* chull_graham(Point* polygon, size_t size, size_t* out_size){
    if(size < 3){
        *out_size = size;
        return polygon;
    }
    int bindex = 0;
    for(int i = 1; i < size; i++){
        if(polygon[i].y < polygon[bindex].y || (cmp(polygon[i].y, polygon[bindex].y) && polygon[i].x < polygon[bindex].x)){
            bindex = i;
        }
    }
    Point tmp = polygon[0];
    polygon[0] = polygon[bindex];
    polygon[bindex] = tmp;
    Stack stack;
    stack_init(&stack, min(size+1, 1024));
    quicksort(polygon, polygon[0], 1, size-1);
    stack_push(&stack, polygon[0]);
    stack_push(&stack, polygon[1]);
    for(int i = 2; i < size; i++){
        while(1){
            if(Angle(stack_subpeek(&stack), stack_peek(&stack), polygon[i]) <= 0){
                stack_pop(&stack);
            }else break;
        }
        stack_push(&stack, polygon[i]);
    }
    *out_size = stack.size;
    return stack.data;
}

Point* chull_andrew(Point* polygon, size_t size, size_t* out_size){
    if(size < 3){
        *out_size = size;
        return polygon;
    }
    
}

int main(void){
    Point* polygon = malloc(4*sizeof(Point));
    polygon[0].x = 0;
    polygon[1].x = 0;
    polygon[2].x = 1;
    polygon[3].x = 0;
    polygon[0].y = 0;
    polygon[1].y = 1;
    polygon[2].y = 1;
    polygon[3].y = 2;
    size_t out;
    Point* res = chull_graham(polygon, 4, &out);
    for(int i = 0; i < out; i++){
        printf("(%.2f, %.2f)", res[i].x, res[i].y);
    }
}