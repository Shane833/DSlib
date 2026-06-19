#include <lcthw/hashmap_algos.h>
#include <lcthw/bstrlib.h>

constexpr uint32_t FNV_PRIME = 16777619;
constexpr uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t Hashmap_fnv1a_hash(void* data, uint32_t  seed)
{
	bstring s = (bstring) data;
	uint32_t hash = FNV_OFFSET_BASIS;
	int i = 0;
	
	for(i = 0; i < blength(s); i++){
		hash ^= bchare(s, i, 0);
		hash *= FNV_PRIME;
	}
	
	return hash;
}

uint32_t Hashmap_fnv1a_hash_gen(const void * key, size_t len, uint32_t seed)
{
	const uint8_t * data = (const uint8_t *)key; 

	uint32_t hash = FNV_OFFSET_BASIS;
	int i = 0;
	
	for(i = 0; i < len; i++){
		hash ^= data[i];
		hash *= FNV_PRIME;
	}
	
	return hash;
}

constexpr int MOD_ADLER = 65521;

uint32_t Hashmap_adler32_hash(void* data, uint32_t seed)
{
	bstring s = (bstring) data;
	uint32_t a = 1, b = 0;
	int i = 0;
	
	for(i = 0; i < blength(s); i++){
		a = (a + bchare(s, i, 0)) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}
	
	return (b << 16) | a;
}

uint32_t Hashmap_adler32_hash_gen(const void * key, size_t len, uint32_t seed)
{
	const uint8_t * data = (const uint8_t * )key;
	uint32_t a = 1, b = 0;
	int i = 0;
	
	for(i = 0; i < len; i++){
		a = (a + data[i]) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}
	
	return (b << 16) | a;
}

uint32_t Hashmap_djb_hash(void* data, uint32_t seed)
{
	bstring s = (bstring) data;
	uint32_t hash = 5381;
	int i = 0;
	
	for(i = 0;i < blength(s); i++){
		hash = ((hash << 5) + hash) + bchare(s, i, 0); /* hash * 33 + c */

	}
	
	return hash;
}

uint32_t Hashmap_djb_hash_gen(const void * key, size_t len, uint32_t seed)
{
	const uint8_t * data = (const uint8_t *)key;
	uint32_t hash = 5381;
	int i = 0;
	
	for(i = 0;i < len; i++){
		hash = ((hash << 5) + hash) + data[i]; /* hash * 33 + c */
	}
	
	return hash;
}

// Helper functions for Hashmap_murmur3_hash

// This is a simple rotate left shift function
// which left shifts the specified bits and then
// appends the bits that get cut-off onto the right
static inline uint32_t ROTL32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}

// This will help us get a 4 byte / 32-bit chunk at time
// from the buffer that is being pointed by p
static inline uint32_t getblock32 ( const uint32_t * p, int i )
{
  return p[i];
}

// Finilization mix : forces all the bits of the hash to avalanche
// Avalanche behaviour is crucial for all hash functions which says
// that single bit change in the input should be reflected by atleast
// half the bit changes in the output 
// for 32-bit hash, if 1 bit is different input then atleast 16-bits should be 
// different in the generated output hash
static inline uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}


uint32_t Hashmap_murmur3_32_hash(const void * key, size_t len, uint32_t seed)
{
	// Point to the data in byte fashion
	const uint8_t * data = (const uint8_t *)key;
	// compute the no. of 4 byte blocks
	const int nblocks = len / 4; // We will handle the remaining bits at the end

	// Fetches the random seed once for the lifetime of the program
	//static uint32_t random_seed = rand();

	// This will contain our computed hash at the end
	// Update : If seed is provided use it else use 0 as the seed
	uint32_t h1 = seed == 0 ? 0 : seed; // Starts with a seed (I'm using 0 by default)

	// Constants to be used later
	const uint32_t c1 = 0xcc9e2d51;
 	const uint32_t c2 = 0x1b873593;

 	// Now we will point to the end of the data (basically the MSB)
 	// As the x86 architecture is little-endian 
 	const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);
 	// Since pointers are relative we can point backwards / -ve index
 	// which would return the 4byte chunk pointing to LSB and making
 	// our way back from there, however some MSB will be left untouched in 
 	// this fashion
	for(int i = -nblocks; i; i++)
  	{
	    uint32_t k1 = getblock32(blocks,i);

	    k1 *= c1;
	    k1 = ROTL32(k1,15);
	    k1 *= c2;
	    
	    h1 ^= k1;
	    h1 = ROTL32(h1,13); 
	    h1 = h1*5+0xe6546b64;
  	}

  	// Now we point to the remaining bytes through tail pointer
  	// which will address the individual bytes
  	const uint8_t * tail = (const uint8_t *)(data + nblocks * 4);

  	// temporary variables
  	uint32_t k1 = 0;

  	// We are basically checking how many bytes are left
  	// which could not be grouped in chunks of 4bytes
  	// The expression in switch is equivalent to len % 4
	switch(len & 3){ 
		// We are using the falling property of switch cases
		// to handle the remaining number of bytes
		case 3: k1 ^= tail[2] << 16;
		case 2: k1 ^= tail[1] << 8;
		case 1: k1 ^= tail[0];
		  k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    }

    // Finalization
 	h1 ^= len;
  	h1 = fmix32(h1); // For avalance behaviour
	
	return h1;
}

/*
uint32_t Hashmap_murmur3_hash(void * data, uint32_t seed)
{
	char * key = bdata((bstring)data);
	size_t len = blength((bstring)data);

	return murmur3_32(key, len, seed);
}
*/
