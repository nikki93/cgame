#ifndef ERROR_H
#define ERROR_H

/*
 * errors cause an exception in script if they occur in C code
 * called from a script, else just abort
 */

/* printf-style error formatting */
__attribute__((format(printf, 1, 2)))
void errorf(const char *fmt, ...);

/* evaluates to a 'filename:linenumber: ' string */
#define line_str__(line)                        \
    __FILE__ ":" #line ": "
#define line_str_(line)                         \
    line_str__(line)
#define line_str()                              \
    line_str_(__LINE__)

/* printf-style error formatting with file name, line number */
#define error(...)                              \
    errorf(line_str() __VA_ARGS__)

/* printf-style error formatting for an assertion, also prints condition */
#define error_assert(cond, ...)                         \
    ((cond) ? 0: error("assertion '" #cond "' failed ... " __VA_ARGS__))

#endif
    
