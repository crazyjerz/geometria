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
    stack_init(&stack, size+1);
    quicksort(polygon, polygon[0], 1, size-1);
    stack_push(&stack, polygon[0]);
    stack_push(&stack, polygon[1]);
    for(int i = 2; i < size; i++){
        if(dup(polygon[i-1], polygon[i])) continue;
        while(stack.size >= 2){
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
    stack_init(&stack1, size+1);
    stack_init(&stack2, size+1);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for(int i = 0; i < size; i++){
                if(i != 0 && dup(polygon[i], polygon[i-1])) continue;
                while(stack1.size >= 2 && Orient(stack_subpeek(&stack1), stack_peek(&stack1), polygon[i]) <= 0) stack_pop(&stack1);
                stack_push(&stack1, polygon[i]);
            }
        }
        #pragma omp section
        {
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
    for(int i = stack2.size - 2; i > 0; i--) output[stack1.size + i - 1] = stack2.data[i];
    stack_destroy(&stack1);
    stack_destroy(&stack2);
    return output;
}

int partition(Point *pts, int n, Point P, Point Q) {
    int i = 0;
    for(int j = 0; j < n; j++){
        if(OrientNum(P, Q, pts[j]) > 0){
            Point tmp = pts[i];
            pts[i] = pts[j];
            pts[j] = tmp;
            i++;
        }
    }
    return i;
}

static void _find(Point *pts, int n, Point P, Point Q, Stack* hull){
    if (n == 0) return;
    int ci = 0;
    for(int i = 1; i < n; i++) 
        if(OrientNum(P, Q, pts[i]) > OrientNum(P, Q, pts[ci])) ci = i;
    Point C = pts[ci];
    if(OrientNum(P, Q, C) < -eps) return;
    stack_push(hull, C);
    pts[ci] = pts[n-1];
    n--;
    int s1 = partition(pts, n, P, C);
    int s2 = partition(pts + s1, n - s1, C, Q);
    _find(pts,      s1, P, C, hull);
    _find(pts + s1, s2, C, Q, hull);
};

Point* chull_quick(Point* polygon, size_t size, size_t* out_size){
    if(size < 2){
        *out_size = size;
        return polygon;
    }
    Stack buffer;
    stack_init(&buffer, min(size+1, 1024));

    int ai = 0, bi = 0;
    for (int i = 1; i < size; i++) {
        if (polygon[i].x < polygon[ai].x) ai = i;
        if (polygon[i].x > polygon[bi].x) bi = i;
    }
    Point A = polygon[ai], B = polygon[bi];
    stack_push(&buffer, A);
    stack_push(&buffer, B);
    polygon[ai] = polygon[0];
    polygon[0] = A;
    if(!bi) bi = ai;
    polygon[bi] = polygon[1];
    polygon[1] = B;

    int s1 = partition(polygon + 2, size - 2, A, B);
    int s2 = partition(polygon + 2 + s1, size - s1 - 2, B, A);

    _find(polygon + 2,  s1, A, B, &buffer);
    _find(polygon + 2 + s1, s2, B, A, &buffer);
    Point* out = chull_andrew(buffer.data, buffer.size, out_size);
    stack_destroy(&buffer);
    return out;
}

int main(void){
    Point* polygon = malloc(2000000*sizeof(Point));
    for(int i = 0; i < 1000000; i++){
        polygon[i].x = i%8;
        polygon[i].y = i%11;
        if((i%8 == 0 && i%11 == 0) || (i%8 == 7 && i%11 == 10) || (i%8 == 0 && i%11 == 10) || (i%8 == 7 && i%11 == 0)){
            polygon[i].x = 4;
            polygon[i].y = 4;
        }
    }
    for(int i = 0; i < 1000000; i++){
        polygon[1000000+i].x = 1-((double)i)/1000000;
        polygon[1000000+i].y = ((double)i)/1000000;
    }
    size_t out;
    Point* res = chull_graham(polygon, 2000000, &out);
    for(int i = 0; i < out; i++){
        printf("(%.2f, %.2f)", res[i].x, res[i].y);
    }
}