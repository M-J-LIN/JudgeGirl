#ifndef _UTILS_H
#define _UTILS_H
typedef unsigned int uint32_t;
static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}
static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}
#define BLK 100
__kernel void dotProduct(__global uint32_t* C, int N, uint32_t key1, uint32_t key2){
    int id = get_global_id(0);
    int l = id * BLK;
    int r = (id+1) * BLK;
    if (r > N)  r = N;
    uint32_t sum = 0;
    for(int i = l; i < r; i++)
        sum += (encrypt(i, key1) * encrypt(i, key2));
    C[id] = sum;
}
#endif
