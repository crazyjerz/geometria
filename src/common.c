#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include"point.h"
#include<omp.h>
#define INSERTION_THRESHOLD 16
#define PARALLEL_THRESHOLD 10000
#define MAX_DEPTH_MULTIPLIER 2

static inline int Angle(Point o, Point a, Point b){
    double ax = a.x - o.x, ay = a.y - o.y;
    double bx = b.x - o.x, by = b.y - o.y;
    double c = ax * by - ay * bx;
    if (c > eps) return 1;
    if (c < -eps) return -1;
    double da = ax*ax + ay*ay;
    double db = bx*bx + by*by;
    if (da < db) return 1;
    if (da > db) return -1;
    return 0;
}

int Orient(Point o, Point a, Point b){
    double ax = a.x - o.x, ay = a.y - o.y;
    double bx = b.x - o.x, by = b.y - o.y;
    double c = ax * by - ay * bx;
    return (c > eps) - (c < -eps);
} 

double OrientNum(Point o, Point a, Point b){
    double ax = a.x - o.x, ay = a.y - o.y;
    double bx = b.x - o.x, by = b.y - o.y;
    return ax * by - ay * bx;
}

static inline int lex(Point a, Point b){
    if(dup(a, b)) return 0;
    if(a.x < b.x) return -1;
    if(a.x > b.x) return 1;
    if(a.y < b.y) return -1;
    if(a.y > b.y) return 1;
    return 0; // dla uspokojenia gcc
}

static inline Point medianThree(Point zero, Point a, Point b, Point c){
    int g = (Angle(zero, a, b) > 0);
    if ((g ^ (Angle(zero, a, c))) > 0) return a;
    else if ((g ^ (Angle(zero, b, c))) < 0) return b;
    else return c;
}

static inline Point medianThree_lex(Point a, Point b, Point c){
    int g = lex(a, b) > 0;
    if ((g ^ (lex(a, c))) > 0) return a;
    else if ((g ^ (lex(b, c))) < 0) return b;
    else return c;
}

static inline Point ninther(Point* A, Point zero, int l, int h){
    return medianThree(
        zero,
        medianThree(zero, A[l],         A[l+(h-l)/6],   A[l+(h-l)/3]),
        medianThree(zero, A[l+(h-l)/3], A[l+(h-l)/2],   A[l+2*(h-l)/3]),
        medianThree(zero, A[l+2*(h-l)/3], A[l+5*(h-l)/6], A[h])
    );
}

static inline Point ninther_lex(Point* A, int l, int h){
    return medianThree_lex(
        medianThree_lex(A[l],         A[l+(h-l)/6],   A[l+(h-l)/3]),
        medianThree_lex(A[l+(h-l)/3], A[l+(h-l)/2],   A[l+2*(h-l)/3]),
        medianThree_lex(A[l+2*(h-l)/3], A[l+5*(h-l)/6], A[h])
    );
}

inline double ScalarProduct(Point a, Point b){
    return a.x * b.y - a.y * b.x;
}

inline int stack_empty(Stack* s){
    return s->size == 0;
}

inline int stack_size(Stack* s){
    return s->size;
}

int stack_init(Stack *s, size_t capacity){
    s->data = malloc(capacity * sizeof(Point));
    if(!s->data)
        return 0; 
    s->size = 0;
    s->capacity = capacity;
    return 1;
}

void stack_destroy(Stack* s){
    free(s->data);
}

void stack_push(Stack* s, Point value){
    if (s->size == s->capacity){
        size_t new_capacity = s->capacity * 2;
        Point* new_data = realloc(s->data, new_capacity * sizeof(Point));
        if(!new_data) return;
        s->data = new_data;
        s->capacity = new_capacity;
    }

    s->data[s->size++] = value;
}

Point stack_pop(Stack* s){
    return s->data[--s->size];
}

Point stack_peek(Stack* s){
    return s->data[s->size-1];
}

Point stack_subpeek(Stack* s){
    return s->data[s->size-2];
}

static inline void swap(Point* a, Point* b){
    Point t = *a;
    *a = *b;
    *b = t;
}

static void insertionsort(Point* A, Point zero, int lo, int hi){
    for (int i = lo + 1; i <= hi; i++){
        Point k = A[i];
        int j = i - 1;
        while (j >= lo && Angle(zero, A[j], k) < 0){
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = k;
    }
}

static void insertionsort_lex(Point* A, int lo, int hi){
    for (int i = lo + 1; i <= hi; i++){
        Point k = A[i];
        int j = i - 1;
        while (j >= lo && lex(A[j], k) > 0){
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = k;
    }
}

static void quicksort_parallel(Point* A, Point zero, int lo, int hi, int depth){
    if(lo >= hi) return;
    if (hi - lo <= INSERTION_THRESHOLD){
        insertionsort(A, zero, lo, hi);
        return;
    }

    Point pivot = medianThree(zero, A[lo], A[(lo+hi)/2], A[hi]);

    int lt = lo, gt = hi, i = lo;
    while(i <= gt){
        int c = Angle(zero, A[i], pivot);
        if(c > 0)      swap(&A[lt++], &A[i++]);
        else if(c < 0) swap(&A[i],    &A[gt--]);
        else           i++;
    }

    if(dup(A[lo], A[lt]) && dup(A[gt], A[hi])) return;

    if(hi - lo <= PARALLEL_THRESHOLD || depth <= 0){
        quicksort_parallel(A, zero, lo, lt - 1, 0);
        quicksort_parallel(A, zero, gt + 1, hi, 0);
        return;
    }

    #pragma omp task default(none) firstprivate(A, zero, lo, lt, depth)
    quicksort_parallel(A, zero, lo, lt - 1, depth - 1);

    #pragma omp task default(none) firstprivate(A, zero, gt, hi, depth)
    quicksort_parallel(A, zero, gt + 1, hi, depth - 1);

    #pragma omp taskwait
}

static void quicksort_parallel_lex(Point* A, int lo, int hi, int depth){
    if(lo >= hi) return;
    Point pivot = medianThree_lex(A[lo], A[(lo+hi)/2], A[hi]);

    if (hi - lo <= INSERTION_THRESHOLD){
        insertionsort_lex(A, lo, hi);
        return;
    }

    int lt = lo, gt = hi, i = lo;
    while(i <= gt){
        int c = lex(A[i], pivot);
        if(c < 0)      swap(&A[lt++], &A[i++]);
        else if(c > 0) swap(&A[i],    &A[gt--]);
        else           i++;
    }

    if(dup(A[lo], A[lt]) && dup(A[gt], A[hi])) return;

    if(lt - lo <= PARALLEL_THRESHOLD || depth <= 0){
        quicksort_parallel_lex(A, lo, lt - 1, 0);
        quicksort_parallel_lex(A, gt + 1, hi, 0);
        return;
    }

    #pragma omp task firstprivate(A, lo, lt, depth)
    quicksort_parallel_lex(A, lo, lt - 1, depth - 1);
    #pragma omp task firstprivate(A, gt, hi, depth)
    quicksort_parallel_lex(A, gt + 1, hi, depth - 1);
    #pragma omp taskwait
}

void quicksort(Point* A, Point zero, int lo, int hi){
    int depth = (int)log2(omp_get_max_threads());
    #pragma omp parallel
    #pragma omp single
    {
        #pragma omp taskgroup
        {
            quicksort_parallel(A, zero, lo, hi, depth);
        }
    }
}

void quicksort_lex(Point* A, int lo, int hi){
    int depth = (int)log2(omp_get_max_threads());
    #pragma omp parallel
    #pragma omp single
    {
        #pragma omp taskgroup
        {
            quicksort_parallel_lex(A, lo, hi, depth);
        }
    }
}
