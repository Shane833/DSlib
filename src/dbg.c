#include <dbg.h>

// As defined in the https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing
// If you want to stringize the result of expansion of a macro argument, you have to use two levels of macros.
#define xstr(a) str(a)
#define str(a) #a


static FILE *file = NULL;

void genericDebug(const char *current_file, int current_line, const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);

    #ifdef DEBUG_FILE
        if(!file){
            //file = fopen("file.log", "ab"); // opens a file in the same directory
            file = fopen(xstr(DEBUG_FILE)".log", "ab"); // opens a file in the same directory
        }
                                       // opening it in append mode
        fprintf(file, "[DEBUG] (%s:%d) ", current_file, current_line);
        vfprintf(file, fmt, argp);
        fprintf(file, "\n");
        
        fflush(file);
    #else
        fprintf(stderr, "[DEBUG] (%s:%d) ", current_file, current_line);
        vfprintf(stderr, fmt, argp);
        fprintf(stderr, "\n");
    #endif
    va_end(argp);

}

void genericLog(const char *tag, const char *current_file, int current_line, const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    
    #ifdef DEBUG_FILE
        if(!file){
            //file = fopen("file.log", "ab"); // opens a file in the same directory
            file = fopen(xstr(DEBUG_FILE)".log", "ab"); // opens a file in the same directory
        }
                                       // opening it in append mode
        fprintf(file, "%s (%s:%d: errno: %s) ",tag, current_file, current_line, clean_errno());
        vfprintf(file, fmt, argp);
        fprintf(file, "\n");
        
        fflush(file);
    #else
        fprintf(stderr, "%s (%s:%d: errno: %s) ",tag, current_file, current_line, clean_errno());
        vfprintf(stderr, fmt, argp);
        fprintf(stderr, "\n");
    #endif
    va_end(argp);
}

void closeLog(){
    if(file){ // closing the file
        fclose(file);
        file = NULL;
    }
}
