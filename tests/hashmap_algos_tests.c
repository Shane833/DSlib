#include "minunit.h"
#include <Hashmap.h>
#include <Hashmap_algos.h>
#include <DArray.h>
#include <bstrlib.h>

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");

char* test_fnv1a()
{
	uint32_t hash = Hashmap_fnv1a_hash(&test1, 0);
	mu_assert(hash != 0, "Bad hash");
	
	hash = Hashmap_fnv1a_hash(&test2, 0);
	mu_assert(hash != 0, "Bad hash");
	
	hash = Hashmap_fnv1a_hash(&test3, 0);
	mu_assert(hash != 0, "Bad hash");
	
	return NULL;
}


char* test_adler32()
{
	uint32_t hash = Hashmap_adler32_hash(&test1, 0);
	mu_assert(hash != 0, "Bad hash");
	
	hash = Hashmap_adler32_hash(&test2, 0);
	mu_assert(hash != 0, "Bad hash");
	
	hash = Hashmap_adler32_hash(&test3, 0);
	mu_assert(hash != 0, "Bad hash");
	
	return NULL;
}


char* test_djb()
{
	uint32_t hash = Hashmap_djb_hash(&test1, 0);
	mu_assert(hash != 0, "Bad hash");
	
	hash = Hashmap_djb_hash(&test2, 0);
	mu_assert(hash != 0, "Bad hash");
	
	hash = Hashmap_djb_hash(&test3, 0);
	mu_assert(hash != 0, "Bad hash");
	
	return NULL;
}



#define BUCKETS 100
#define BUFFER_LEN 20
#define NUM_KEYS BUCKETS * 1000

enum {ALGO_FNV1A, ALGO_ADLER32, ALGO_DJB};

int gen_keys(DArray* keys, int num_keys)
{
	#ifdef __linux__
		int i = 0;

		FILE * urand = fopen("/dev/urandom", "r");
		check(urand != NULL, "Failed to open /dev/urandom");
		
		struct bStream* stream = bsopen((bNread) fread, urand);
		check(stream != NULL, "Failed to open /dev/urandom");
		
		bstring key = bfromcstr("");
		int rc = 0;
		

		// FNV1A Histogram
		for(i = 0;i < num_keys; i++){
			rc = bsread(key, stream, BUFFER_LEN);
			check(rc >= 0, "Failed to read from /dev/urandom");
			// We create a new bstring and add it to the DArray using the value of the bstring key
			DArray_push(keys, bstrcpy(key));
		}
		
		bsclose(stream);
		fclose(urand);

	#elif _WIN32

		for(int i = 0;i < num_keys;i++){
			// Generating a 64-bit no.
			uint64_t raw_key = (uint64_t)( (uint64_t)rand() | ((uint64_t)rand()) << 48);
			char buff[20];
			snprintf(buff, sizeof(buff), "%llu", raw_key);
			// Now to convert this into a bstring
			bstring key = bfromcstr(buff);
			check(key != NULL, "ERROR : Failed to create the bstring!");
			// Now push it onto the DArray
			DArray_push(keys, key);
		}

	#endif

	return 0;
error:
	return -1;
}

void destroy_keys(DArray* keys)
{
	int i = 0;
	for(i = 0;i < NUM_KEYS; i++){
		bdestroy(DArray_get(keys, i));
	}
	
	DArray_destroy(keys);
}

void fill_distribution(int* stats, DArray* keys,  Hashmap_hash hash_func)
{
	int i = 0;
	uint32_t hash = 0;
	
	for(i = 0; i < DArray_count(keys); i++){
		hash = hash_func(DArray_get(keys, i), 0);
		stats[hash % BUCKETS] += 1;
	}
}

char* test_distribution()
{
	int i = 0;
	int stats[3][BUCKETS] = { {0} };
	DArray* keys = DArray_create(0, NUM_KEYS);
	
	mu_assert(gen_keys(keys, NUM_KEYS) == 0, "Failed to generate random keys");
	
	fill_distribution(stats[ALGO_FNV1A], keys, Hashmap_fnv1a_hash);
	fill_distribution(stats[ALGO_ADLER32], keys, Hashmap_adler32_hash);
	fill_distribution(stats[ALGO_DJB], keys, Hashmap_djb_hash);
	
	// Improvement: We will write the data line by line into the file along side printing it
	FILE * test_csv = fopen("tests/test.csv","w");
	mu_assert(test_csv != NULL, "ERROR : Failed to create/open the csv file");

	fprintf(stderr, "FNV\tA32\tDJB\n");
	fprintf(test_csv, "FNV,A32,DJB\n");

	for(i = 0;i < BUCKETS; i++){
		fprintf(stderr, "%d\t%d\t%d\n", stats[ALGO_FNV1A][i], stats[ALGO_ADLER32][i], stats[ALGO_DJB][i]);
		fprintf(test_csv, "%d,%d,%d\n", stats[ALGO_FNV1A][i], stats[ALGO_ADLER32][i], stats[ALGO_DJB][i]);
	}

	// close the file
	fclose(test_csv);
	
	destroy_keys(keys);
	
	return NULL;
}

char* all_tests()
{
	mu_suite_start();
	mu_run_test(test_fnv1a);
	mu_run_test(test_adler32);
	mu_run_test(test_djb);
	mu_run_test(test_distribution);

	return NULL;
}

RUN_TESTS(all_tests);