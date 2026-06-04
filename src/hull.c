#include<stdio.h>
#include<stdlib.h>
#include"common.h"
#include"point.h"
#ifdef _OPENMP
#include <omp.h>
#else
static inline int omp_get_max_threads(void) { return 1; }
static inline int omp_get_thread_num(void) { return 0; }
#endif
#include<math.h>
#include<string.h>

#define cmp(X, Y) (fabs(X-Y)<eps)
#define min(X, Y) (X < Y ? X : Y)

Point* chull_graham(Point* polygon, size_t size, size_t* out_size){
    if(size == 2 && (dup(polygon[0], polygon[1]))){
        *out_size = 1;
        Point* output = malloc(sizeof(Point));
        output[0] = polygon[0];
        return output;
    }
    if(size < 3){
        *out_size = size;
        Point* output = malloc(sizeof(Point)*size);
        for(int i = 0; i < size; i++){
            output[i] = polygon[i];
        }
        return output;
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
    int ini = 1;
    while(ini < size && (dup(polygon[0], polygon[ini]))) ini++;
    if(ini != size){
        stack_push(&stack, polygon[ini]); // edge case management
        for(int i = ini+1; i < size; i++){
            if(dup(polygon[i-1], polygon[i])) continue;
            while(stack.size >= 2){
                if(Orient(stack_subpeek(&stack), stack_peek(&stack), polygon[i]) <= 0){
                    stack_pop(&stack);
                }else break;
            }
            stack_push(&stack, polygon[i]);
        }
    }
    *out_size = stack.size;
    return stack.data;
}

Point* chull_andrew(Point* polygon, size_t size, size_t* out_size){
    if(size == 2 && (dup(polygon[0], polygon[1]))){
        *out_size = 1;
        Point* output = malloc(sizeof(Point));
        output[0] = polygon[0];
        return output;
    }
    if(size < 3){
        *out_size = size;
        Point* output = malloc(sizeof(Point)*size);
        for(int i = 0; i < size; i++){
            output[i] = polygon[i];
        }
        return output;
    }
    quicksort_lex(polygon, 0, size-1);
    if(dup(polygon[0], polygon[size-1])){
        Point* output = malloc(sizeof(Point));
        output[0] = polygon[0];
        *out_size = 1;
        return output;
    }
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
    int j = n-1;

    while(1){
        while (i <= j && Orient(P, Q, pts[i]) > 0)   i++;
        while (i <= j && Orient(P, Q, pts[j]) <= 0)  j--;
        if (i >= j) return i;

        Point tmp = pts[i];
        pts[i] = pts[j];
        pts[j] = tmp;

        i++;
        j--;
    }
}

static void _find(Point *pts, int n, Point P, Point Q, Stack* hull, int depth){
    if(n == 0) return;
    if(n < 1e4) depth = 0;
    int ci = 0;
    for(int i = 1; i < n; i++) 
        if(OrientNum(P, pts[i], Q) < OrientNum(P, pts[ci], Q)) ci = i; 
    Point C = pts[ci];
    if(Orient(P, Q, C) < 0) return;   
    stack_push(hull, C);
    pts[ci] = pts[n-1];
    pts[n-1] = C;
    n--;

    int s1 = partition(pts, n, P, C);
    int s2 = partition(pts + s1, n - s1, C, Q);
    if(s1 == 0 && s2 == 0) return;
    //fprintf(stderr, "%.2f (%.2f %.2f) (%.2f %.2f) (%.2f %.2f)", OrientNum(P, C, Q), P.x, P.y, Q.x, Q.y, C.x, C.y);
    //fprintf(stderr, "%d %d %d\n", s1, s2, n);

    if(depth <= 0){
        _find(pts,      s1, P, C, hull, depth-1);
        _find(pts + s1, s2, C, Q, hull, depth-1);
        return;
    }
    
    Stack local1, local2;
    stack_init(&local1, s1 + 1);
    stack_init(&local2, s2 + 1);

    #pragma omp task firstprivate(pts, s1, P, C, depth) shared(local1)
    _find(pts,      s1, P, C, &local1, depth - 1);

    #pragma omp task firstprivate(pts, s1, s2, C, Q, depth) shared(local2)
    _find(pts + s1, s2, C, Q, &local2, depth - 1);

    #pragma omp taskwait

    for(int i = 0; i < local1.size; i++) stack_push(hull, local1.data[i]);
    for(int i = 0; i < local2.size; i++) stack_push(hull, local2.data[i]);
    stack_destroy(&local1);
    stack_destroy(&local2);
}

Point* chull_quick(Point* polygon, size_t size, size_t* out_size){
    if(size == 2 && (dup(polygon[0], polygon[1]))){
        *out_size = 1;
        Point* output = malloc(sizeof(Point));
        output[0] = polygon[0];
        return output;
    }
    if(size < 3){
        *out_size = size;
        Point* output = malloc(sizeof(Point)*size);
        for(int i = 0; i < size; i++){
            output[i] = polygon[i];
        }
        return output;
    }
    Stack buffer;
    stack_init(&buffer, min(size+1, 1024));
    Point A = polygon[0];
    Point B = polygon[0];

    #pragma omp declare reduction(minpt : Point : \
        omp_out = (omp_in.x < omp_out.x || \
                (omp_in.x == omp_out.x && omp_in.y < omp_out.y)) ? omp_in : omp_out) \
        initializer(omp_priv = omp_orig)

    #pragma omp declare reduction(maxpt : Point : \
        omp_out = (omp_in.x > omp_out.x || \
                (omp_in.x == omp_out.x && omp_in.y > omp_out.y)) ? omp_in : omp_out) \
        initializer(omp_priv = omp_orig)

    #pragma omp parallel for reduction(minpt:A) reduction(maxpt:B) if(size > 100000)
    for (int i = 1; i < size; i++) {
        Point p = polygon[i];
        A = (p.x < A.x || (p.x == A.x && p.y < A.y)) ? p : A;
        B = (p.x > B.x || (p.x == B.x && p.y > B.y)) ? p : B;
    }
    int ai = A.idx, bi = B.idx;
    stack_push(&buffer, A);
    stack_push(&buffer, B);
    if(!(bi-1)){
        polygon[ai-1] = polygon[1];
        polygon[1] = A;
    }else{
        polygon[ai-1] = polygon[0];
        polygon[0] = A;
        polygon[bi-1] = polygon[1];
        polygon[1] = B;
    }

    int s1 = partition(polygon + 2, size - 2, A, B);

    int depth1 = (int)log2(omp_get_max_threads());
    int depth2 = (int)(log2(omp_get_max_threads())+log2(4.0/3.0));
    int s2 = size - s1;
    if(size <= 1e5){
        _find(polygon + 2,        s1, A, B, &buffer, -1);
        _find(polygon + 2 + s1,   s2 - 2, B, A, &buffer, -1);
        //fprintf(stderr, "%d\n", buffer.size);
        Point* out = chull_andrew(buffer.data, buffer.size, out_size);
        stack_destroy(&buffer);
        return out;
    }
    Stack local1, local2;
    stack_init(&local1, s1 + 1);
    stack_init(&local2, s2 + 1);
    #pragma omp parallel
    #pragma omp single
    {
        #pragma omp task firstprivate(s1, A, B, depth1) shared(local1)
        _find(polygon + 2,        s1, A, B, &local1, depth1);

        #pragma omp task firstprivate(s1, s2, B, A, depth2) shared(local2)
        _find(polygon + 2 + s1,   s2 - 2, B, A, &local2, depth2);

        #pragma omp taskwait
    }
    for(int i = 0; i < local1.size; i++) stack_push(&buffer, local1.data[i]);
    for(int i = 0; i < local2.size; i++) stack_push(&buffer, local2.data[i]);
    stack_destroy(&local1);
    stack_destroy(&local2);
    //fprintf(stderr, "%d\n", buffer.size);
    Point* out = chull_andrew(buffer.data, buffer.size, out_size);
    stack_destroy(&buffer);
    return out;
}