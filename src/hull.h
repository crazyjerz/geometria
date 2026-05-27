#include"point.h"
#include<stdlib.h>
extern Point* chull_graham(Point* polygon, size_t size, size_t* out_size);
extern Point* chull_andrew(Point* polygon, size_t size, size_t* out_size);
extern Point* chull_quick(Point* polygon, size_t size, size_t* out_size);
