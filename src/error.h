#ifndef ERROR_H
#define ERROR_H

/*
 * errors cause an exception in script if they occur in C code
 * called from a script, else just abort
 */

/* printf-style error formatting */
void errorf(const char *fmt, ...);

/* printf-style error formatting with file name, line number */
#define error(fmt, ...)                                 \
    errorf(__FILE__ ":%d: " fmt, __LINE__, __VA_ARGS__)

/* printf-style error formatting for an assertion, also prints condition */
#define error_assert(cond, fmt, ...)                            \
    error("assertion '" #cond "' failed: " fmt, __VA_ARGS__)

#endif
    
