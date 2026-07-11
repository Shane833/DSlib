#ifndef string_algos_h
#define string_algos_h

#include <bstrlib.h>
#include <DArray.h>
#include <sys/types.h> // for ssize_t

typedef struct StringScanner{
	bstring in;
	const unsigned char* haystack;
	ssize_t hlen;
	const unsigned char* needle;
	ssize_t nlen;
	size_t skip_chars[UCHAR_MAX + 1]; // UCHAR_MAX = 255 is defined in limit.h
}StringScanner;

int String_find(bstring in, bstring what); // Finds the first occurence of string 'what' in string 'in'

StringScanner* StringScanner_create(bstring in); // Create the scanner to scan the string 'in'

int StringScanner_scan(StringScanner* scan, bstring tofind); // Find the string 'tofind' in string 'in'

void StringScanner_destroy(StringScanner* scan); // destroy the scanner

#endif
