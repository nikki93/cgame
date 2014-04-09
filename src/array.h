#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdbool.h>

/* store objects contiguous in memory
 *
 * NOTE: objects may move in memory, so don't rely on pointers to objects
 *       in an Array
 */

typedef struct Array Array;

Array *array_new_(size_t object_size); /* object_size is size per element */
#define array_new(type) array_new_(sizeof(type))
void array_free(Array *arr);

void *array_get(Array *arr, unsigned int i);
#define array_get_val(type, arr, i) (*((type *) array_get(arr, i)))
void *array_top(Array *arr);
#define array_top_val(type, arr) (*((type *) array_top(arr)))
unsigned int array_length(Array *arr); /* number of objects in array */

/* NOTE: add/remove may change begin/end, so be careful while iterating */
void *array_begin(Array *arr); /* pointer to first element */
void *array_end(Array *arr);   /* pointer to 'one past last' element */

void *array_add(Array *arr); /* add new object, index is length - 1 */
#define array_add_val(type, arr) (*((type *) array_add(arr)))
void array_reset(Array *arr, unsigned int num); /* resize to 'num 'objects
                                                   whose data is undefined */
#define array_clear(arr) array_reset(arr, 0)
void array_pop(Array *arr); /* remove object with highest index */

/* remove fast, may swap some other element into arr[i], returns true if so */
bool array_quick_remove(Array *arr, unsigned int i);

void array_sort(Array *arr, int (*compar)(const void *, const void *));

/*
 * can be used as:
 *
 *     array_foreach(var, arr)
 *         ... use var ...
 *
 * here var must name a variable of type 'pointer to element' declared
 * before -- do not use this if adding/removing from arr while
 * iterating
 *
 * elements are visited in order of increasing index
 */
#define array_foreach(var, arr)                                 \
    for (void *__end = (var = array_begin(arr),                 \
                        array_end(arr)); var != __end; ++var)

#endif

