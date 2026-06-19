#ifndef _radixmap_h
#define _radixmap_h

#include <stdint.h> // Since we will using exact size integers across all platforms 
#include <stdbool.h>

// This represents a single element in the radixmap
// We have a union of 2 things : 1. Raw Data (64 bits) 2. A struct (32 bits key, 32 bits value)
typedef union RMElement{
	uint64_t raw;
	struct{
		uint32_t key;
		uint32_t value;
	}data;
}RMElement;

// We define a RadixMap Struct with some attributes : 1. max (maximum possible elements) 2. end (index of the last element)
// 3. counter (no idea right now) 4. Contents (RMElement Union) 5. temp (RMElement Union)
typedef struct RadixMap{
	size_t max;
	size_t end;
	uint32_t counter;
	RMElement* contents;
	RMElement* temp; // This is used for 
}RadixMap;

// Function to create and initialize a RadixMap based on the initial max size
RadixMap* RadixMap_create(size_t max);
// Function to destroy and free data from the map
void RadixMap_destroy(RadixMap* map);
// Function to find an element within the map based on a unique key
RMElement* RadixMap_find(RadixMap* map, uint32_t key);
// Function to add the elements in the map with unique keys and integer values
int RadixMap_add(RadixMap* map, uint32_t key, uint32_t value);
// Function to delete the elements from the map
int RadixMap_delete(RadixMap* map, RMElement* el);

#endif