#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

// Function to initialize and create a radixmap
RadixMap* RadixMap_create(size_t max)
{
	// Allocating memory to the whole map
	RadixMap* map = calloc(1, sizeof(RadixMap));
	check_mem(map);
	
	// Allocating memory to our contents union
	map->contents = calloc(max + 1, sizeof(RMElement));
	check_mem(map->contents);
	
	// Allocating memory to our temp union
	map->temp = calloc(max + 1, sizeof(RMElement));
	check_mem(map->temp);
	
	// initiliazing the max as per the user
	map->max = max;
	map->end = 0; // initializing end to 0 well bcz its the index of the first element
	
	return map; // simply return the map
error:
	return NULL;
}

// Function to free the dynamic memory from the map
void RadixMap_destroy(RadixMap* map)
{
	// Simply check if the memory exists
	if(map){
		// and if it does then simply free it
		free(map->contents);
		free(map->temp);
		free(map);
	}
}

// Handy macro for retreiving the values of the bytes
#define ByteOf(x,y) (((uint8_t*)x)[(y)])

static inline void radix_sort(short offset, uint64_t max, uint64_t* source, uint64_t* dest)
{
	// This array will hold the count/occurences of all the values of bytes i.e. from 0-255
	// i.e. we count which bytes contains a specific value from 0-255
	uint64_t count[256] = { 0 };
	
	uint64_t* cp = NULL; // This is the count pointer, pointing to the count array
	uint64_t* sp = NULL; // This is the source pointer, pointing to the source 
	uint64_t* end = NULL; // End pointer to keep us in bounds
	uint64_t s = 0;
	uint64_t c = 0;
	
	// count occurences of every byte value
	for(sp = source, end = source + max; sp < end; sp++){
		count[ByteOf(sp, offset)]++; // frequency of Byte = sp[offset]
		// Equivalent to:
		// uint64_t j = ByteOf(sp, offset); 
		// count[j] = count[j] + 1;
	}
	
	// transform count into index by summing
	// elements and storing into same array
	for(s = 0, cp = count, end = count + 256; cp < end; cp++){
		c = *cp;
		*cp = s;
		s += c;
	}
	
	// fill dest with right values in the right place
	// We simply obtain the count of the value at address (base_count + sp(basically our element))
	// Which provides us the index at which the value of sp is to be stored in the destination array
	// Then we increment the value at cp by 1 to handle the case if there are multiple values
	for(sp = source, end = source + max; sp < end; sp++){
		cp = count + ByteOf(sp, offset);
		dest[*cp] = *sp;
		++(*cp);
	}
}

// This function will find the position of the element such that it is just smaller than
// the element we wish to add to the map
// NOTE : Also this function won't be called unless you add something
// therefore we don't need to worry about checking if the map is emtpy
// as the first time this function get called is when an element gets added
// so by default the map would not be empty
uint32_t RadixMap_find_min(RadixMap* map, uint32_t key)
{	
	int low = 0;
	int high = map->end - 1; // since end stores the value just past the last element
	RMElement* data = map->contents;
	
	while(low <= high){ // check the base case
		
		// calculate middle and get the middle corresponding to it
		int middle = low + (high - low) / 2;
		uint32_t temp_key = data[middle].data.key;
		
		if(temp_key < key){
			low = middle + 1;
		}
		else if(temp_key > key){
			high = middle - 1;
		}
		else{ 
			// This means that the keys are equal
			// not including this condition seems to start an infinite loop
			// We simply return the index indicating the middle itself
			// This can occur in two cases:
			// 1. There is only a single element in the map
			// 2. You are adding an element with a key that already exists in the map
			// 3. You are trying to delete an element and pass the same key and if the element exists
			// we simply return its index
			return middle <= 0 ? 0 : middle - 1;
		}
	}
	
	return high <= 0 ? 0 : high - 1; // since this will contain the position of the key smaller than 
								// the one we wish to add
	// Also I have to handle a case where if we wish to add an element which is smaller
	// than the element at 0th index then high would become less than 0, which is invalid
	// hence if so is the case, we simply return 0
}

// The heart of the map, i.e. Radix Sort Algorithm function itself
static void RadixMap_sort(RadixMap* map, const uint32_t index)
{	
	// index represents the index from where the algorithm should start sorting
	uint64_t* source = &map->contents[index].raw;
	uint64_t* temp = &map->temp[index].raw;
	
	// Sorting the Keys
	// Since we are sorting one byte at a time and there are 4 bytes (32bits) in total
	// Hence we offset the bytes by 4 indices and sort them individually
	// We go back and forth between changing the src and dest arrays
	radix_sort(0, map->end - index, source, temp); 
	radix_sort(1, map->end - index, temp, source);
	radix_sort(2, map->end - index, source, temp);
	radix_sort(3, map->end - index, temp, source);
	// and at the end we have sorted all of the keys based on the 4 bytes
}

RMElement* RadixMap_find(RadixMap* map, uint32_t to_find)
{
	// Simple binary search algorithm
	int low = 0;
	int high = map->end - 1;
	RMElement* data = map->contents;
	
	while(low <= high){
		int middle = low + (high - low) / 2;
		uint32_t key = data[middle].data.key;
		
		if(to_find < key){
			high = middle - 1;
		}
		else if(to_find > key){
			low = middle + 1;
		}
		else{
			return &data[middle];
		}	
	}
	
	return NULL;
}

int RadixMap_add(RadixMap* map, uint32_t key, uint32_t value)
{
	// We put a condition that the values in the key must always
	// be less than and not equal to UINT32_MAX and if it ain't
	// then that key is not considered valid
	check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");
	
	// Before adding the key into the map we must check if it already exists
	RMElement* exists = RadixMap_find(map, key);
	check(exists == NULL, "ERROR : Key already exists!");
	
	// Even though the element we declare here is static
	// however since we know that we cannot refer to the local 
	// variables outside of the function then how can we add
	// this element into an entity such as map whose scope exists
	// outside of the function ??
	
	RMElement element = {.data = {.key = key, .value = value} };
	check(map->end + 1 < map->max, "RadixMap is full");
	
	// ANSWER: even if the element is static in nature within the function
	// we are not concerned with actually adding the element itself but the value
	// that it holds, as the memory for the element is already allocated within the map
	// we can simply overwrite the struct RMElement at the new position with the statically
	// declared element, this is equivalent to map->contents[map->end++].data = {.key = key, .value = value};
	
	// we add the element to the end of the map then
	// we increment end and resort the order of the elements
	map->contents[map->end++] = element;
	
	
	// First we compute the index of the element with a key just smaller
	// than the element we want to add
	const uint32_t index = RadixMap_find_min(map, key); 
	RadixMap_sort(map, index);

	return 0;
error:
	return -1;
}

int RadixMap_delete(RadixMap* map, RMElement* el)
{
	check(map->end > 0, "There is nothing to delete");
	check(el != NULL, "Can't delete a NULL element");
	
	// We set the key of that element to max value of uint32_t 
	// When sorted this will push the element to the end of the map
	// and since we can only add keys less than UINT32_MAX it will
	// act as if it has been deleted
	
	// Update : Since we are optimizing the radix sort
	// When we delete an element we will find its current index
	// then update the corresponding key and pass the index to radix sort
	
	const uint32_t index = RadixMap_find_min(map, el->data.key); 
	el->data.key = UINT32_MAX;

	if(map->end > 1){
		// don't bother resorting a map of 1 length
		RadixMap_sort(map, index); // since we have set the e
	}
	
	map->end--;
	
	return 0;
error:
	return -1;
}

