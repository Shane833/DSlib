#ifndef hashmap_algos_h
#define hashmap_algos_h

#include <stdint.h>
#include <lcthw/bstrlib.h>

uint32_t Hashmap_fnv1a_hash_gen(const void * key, size_t len, uint32_t seed);
uint32_t Hashmap_fnv1a_hash(void* data, uint32_t  seed);

uint32_t Hashmap_adler32_hash(void * data, uint32_t seed);
uint32_t Hashmap_adler32_hash_gen(const void * key, size_t len, uint32_t seed);

uint32_t Hashmap_djb_hash(void * data, uint32_t seed);
uint32_t Hashmap_djb_hash_gen(const void * key, size_t len, uint32_t seed);


uint32_t Hashmap_murmur3_32_hash(const void * data, size_t len, uint32_t seed);

#endif