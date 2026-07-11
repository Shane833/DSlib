#include <String_algos.h>
#include <limits.h>


static inline void String_setup_skip_chars(size_t  				   *skip_chars,
										   const unsigned char     *needle,
										   ssize_t 				    nlen)
{
	size_t i = 0;
	size_t last = nlen - 1; // last is the index of the last character in needle
    
	for(i = 0;i < UCHAR_MAX + 1; i++){
		skip_chars[i] = nlen; // filling all 256 possible characters with the length of the needle
	}
	
	for(i = 0; i < last; i++){
		skip_chars[needle[i]] = last - i; // storing the indexes of the individual characters of the needle
	}
}

static inline const unsigned char* String_base_search(const unsigned char *haystack,
													  ssize_t 			   hlen,
													  const unsigned char *needle,
													  ssize_t 			   nlen,
													  size_t              *skip_chars)
{
	size_t i = 0;
	size_t last = nlen - 1;
	
	assert(haystack != NULL && "Given bad haystack to search");
	assert(needle != NULL && "Given bad needle to search for");
	check(nlen > 0, "nlen can't be <= 0");
	check(hlen > 0, "hlen can't be <= 0");
	
	while(hlen >= nlen){ // matching character by character 
		for(i = last; haystack[i] == needle[i]; i--){
			if(i == 0) return haystack;
		}

        // since haystack is a pointer and in itself is not const but the underlying datatype is const 
        // hence we can modify the value pointed by the pointer but not the underlying value

        // Also haystack here copies the address pointed to by the callee haystack, hence any change
        // to the local haystack pointer does not reflect any change in the callee haystack pointer

        // if the characters were not matched at any point then we will
        // move the pointer ahead by a calculated value and shorten the
        // length of the haystack by that value as well

        // Now depending on the value stored in skip_char array for that specific
        // character, if its not part of the string the we move forward by the length
        // of the string, but if it is part of the string then we move the pointer forward
        // by the index of the character in the needle, such that the haystack pointer will eventually
        // point at the end of the required string
		hlen -= skip_chars[haystack[last]];  
   
        haystack += skip_chars[haystack[last]];
	}
	
error:
	return NULL;
}

int String_find(bstring in, bstring what)
{
	const unsigned char* found = NULL;
	
	const unsigned char* haystack = (const unsigned char* )bdata(in);
	ssize_t hlen = blength(in);
	const unsigned char* needle = (const unsigned char* )bdata(what);
	ssize_t nlen = blength(what);
	size_t skip_chars[UCHAR_MAX + 1] = { 0 };
	
	String_setup_skip_chars(skip_chars, needle, nlen);
	
	found = String_base_search(haystack, hlen, needle, nlen, skip_chars);
	
	return found != NULL ? found - haystack : -1; // This retruns the index of the needle in haystack(first occurence)
}

StringScanner* StringScanner_create(bstring in)
{
    // Creating the Scanner
	StringScanner* scan = calloc(1, sizeof(StringScanner));
	check_mem(scan);
	
	scan->in = in;
	scan->haystack = (const unsigned char*)bdata(in); // This is our search space
	scan->hlen = blength(in);
	
	assert(scan != NULL && "ERROR: Invalid Scanner");
	return scan;

error:
	free(scan);
	return NULL;
}

static inline void StringScanner_set_needle(StringScanner* scan, bstring tofind)
{
	scan->needle = (const unsigned char*)bdata(tofind);
	scan->nlen = blength(tofind);
	
	String_setup_skip_chars(scan->skip_chars, scan->needle, scan->nlen);
}

static inline void StringScanner_reset(StringScanner* scan)
{
	scan->haystack = (const unsigned char*) bdata(scan->in);
	scan->hlen = blength(scan->in);
}

// Using this function we can find multiple occurences of the string tofind if it exists
int StringScanner_scan(StringScanner* scan, bstring tofind)
{
	const unsigned char* found = NULL;
	ssize_t found_at = 0;
	
	if(scan->hlen <= 0){
		StringScanner_reset(scan);
		return -1;
	}
	
	if((const unsigned char*)bdata(tofind) != scan->needle){ // initializing the needle if its not updated or 
															 // if we are searching for different string
		StringScanner_set_needle(scan, tofind);
	}
	
    // uses the same function to find the needle in haystack, subsequent calls 
	found = String_base_search(scan->haystack, scan->hlen, scan->needle, scan->nlen, scan->skip_chars); 
	
	if(found){
		found_at = found - (const unsigned char*)bdata(scan->in);// 'in' stores the haystack used to 
		scan->haystack = found + scan->nlen; // Simply updating the pointer just ahead of the last found index
		scan->hlen -= found_at - scan->nlen; // Accordingly decreasing our search space
	}
	else{
		// done, reset the setup
		StringScanner_reset(scan);
		found_at = -1;
	}
	
	return found_at;
}

void StringScanner_destroy(StringScanner* scan)
{
	if(scan){
		free(scan);
	}
}
