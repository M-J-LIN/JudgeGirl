#ifndef _UTILS_H
#define _UTILS_H
typedef unsigned int uint32_t;
static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}
static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}
#define BLK 256
#define div 8
#define chunk 131072
__kernel void dotProduct(__global uint32_t* C, int N, uint32_t key1, uint32_t key2){
 
    int id = get_global_id(0);
    int gid = get_group_id(0);
    int localId = get_local_id(0);
 
    __local uint32_t localVec[BLK];
    int index = gid*chunk;
    uint32_t sum = 0;
    int r = chunk/BLK;
    for(int i = 0; i < r; i++){
        int pos = index+i*BLK+localId;
        sum += (pos < N) ? (encrypt(pos, key1) * encrypt(pos, key2)) : 0;
    }
    localVec[localId] = sum;
    barrier(CLK_LOCAL_MEM_FENCE);
 
    for(int i = 0; i < div; i++)
    {
        if(localId < BLK/(2<<i))
            localVec[localId] += localVec[localId+(1<<(div-i-1))];
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if(localId == 0)
        C[gid] = localVec[localId];
}
#endif
