#include <f3d/engine/core/debug.h>
#include <f3d/engine/core/log.h>

#include <stdlib.h>
#include <stdio.h>

#define BACKTRACE_NAMES

#ifdef __linux__
#define __USE_GNU
#include <execinfo.h>
#include <dlfcn.h>
#ifndef _DLFCN_H
#define BACKTRACE_NO_FUNCTION_NAMES
#endif
#endif

#define BACKTRACE_SIZE 32

char *get_location(char *str) {
    char ch;
    while ((ch = *(str++)) != '(');
    return str-1;
}

void debug_print_backtrace(void) {
    #ifdef __linux__
    void *trace[BACKTRACE_SIZE];
    int trace_size;
    trace_size = backtrace(trace, BACKTRACE_SIZE);
    char **symbols;
    symbols = backtrace_symbols(trace, trace_size);
    #ifndef BACKTRACE_NO_FUNCTION_NAMES
    char *end;
    int size;
    log_msg(LOG_DEBUG, "Retrieving function names...\n", 0);
    Dl_info bt_info;
    #endif
    log_msg(LOG_DEBUG, "Printing Backtrace\n", 0);
    int i;
    for (i = 0; i < trace_size; i++) {
        #ifndef BACKTRACE_NO_FUNCTION_NAMES
        end = get_location(symbols[i]);
        size = end-symbols[i];
        
        dladdr(trace[i], &bt_info);
        // print function pointer, function name, and location
        printf("|%-14p || %-24s || %-16.*s|\n", trace[i], bt_info.dli_sname, size, symbols[i]);
        #else
        // we dont have access to the dl library, so just print location+address
        printf("%s\n", symbols[i]);
        #endif
    }
    log_msg(LOG_DEBUG, "Backtrace end\n", 0);
    free(symbols);
    #endif //__linux__
}
