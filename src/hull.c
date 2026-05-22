#include<stdio.h>
#include<stdlib.h>
#include"common.h"
#include"point.h"
#include<omp.h>
#include<math.h>

#define cmp(X, Y) (fabs(X-Y)<eps)
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
        if(dup(polygon[i-1], polygon[i])) continue;
        while(1){
            if(Orient(stack_subpeek(&stack), stack_peek(&stack), polygon[i]) <= 0){
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
    quicksort_lex(polygon, 0, size-1);
    Stack stack1, stack2;
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            stack_init(&stack1, min(size+1, 1024));
            for(int i = 0; i < size; i++){
                if(i != 0 && dup(polygon[i], polygon[i-1])) continue;
                while(stack1.size >= 2 && Orient(stack_subpeek(&stack1), stack_peek(&stack1), polygon[i]) <= 0) stack_pop(&stack1);
                stack_push(&stack1, polygon[i]);
            }
        }
        #pragma omp section
        {
            stack_init(&stack2, min(size+1, 1024));
            for(int i = size-1; i >= 0; i--){
                if(i != size-1 && dup(polygon[i+1], polygon[i])) continue;
                while(stack2.size >= 2 && Orient(stack_subpeek(&stack2), stack_peek(&stack2), polygon[i]) <= 0) stack_pop(&stack2);
                stack_push(&stack2, polygon[i]);
            }
        }
    }
    *out_size = (stack1.size+stack2.size-2);
    Point* output = malloc(((stack1.size+stack2.size-2))*sizeof(Point));
    for(int i = 0; i < stack1.size; i++) output[i] = stack1.data[i];
    for(int i = stack2.size - 2; i > 0; i--) output[(stack1.size+stack2.size-2)-i] = stack2.data[i];
    return output;
}

int main(void){
    Point* polygon = malloc(100*sizeof(Point));
    for(int i = 0; i < 1000; i++){
        polygon[i].x = i%3;
        polygon[i].y = i%4;
    }
    size_t out;
    Point* res = chull_andrew(polygon, 1000, &out);
    for(int i = 0; i < out; i++){
        printf("(%.2f, %.2f)", res[i].x, res[i].y);
    }
}