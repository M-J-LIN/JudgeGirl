#include "labeling.h"
#include <cstdio>
#include <cassert>
#include <stdlib.h>
#include <thrust/scan.h>
#include <thrust/transform.h>
#include <thrust/functional.h>
#include <thrust/device_ptr.h>
#include <thrust/execution_policy.h>
struct is_space
{
  __host__ __device__
  bool operator()(char x)
  {
    return x == ' ' || x == '\n';
  }
};
struct zero
{
  __host__ __device__
  bool operator()(int x)
  {
    return 0;
  }
};
 
void labeling(const char *text, int *pos, int text_size){
struct is_space is_space;
    struct zero zero;
    thrust::fill(thrust::device, pos, pos + text_size, 1);
    thrust::transform_if(thrust::device, text, text + text_size, pos, zero, is_space);
    thrust::inclusive_scan_by_key(thrust::device, pos, pos + text_size, pos, pos);
 
}
