#include "array.h"

#include <string.h>
#include <stdlib.h>

struct Array
{
    char *buf;               /* this is a char * for pointer arithmetic */
    unsigned int capacity;   /* alloc'd size of buf */
    unsigned int length;     /* number of objects */
    size_t object_size;      /* size of each object */
};

Array *array_new_(size_t object_size)
{
    Array *arr;

    arr = malloc(sizeof(Array));
    arr->object_size = object_size;
    arr->capacity = 2;
    arr->buf = malloc(arr->object_size * arr->capacity);
    arr->length = 0;

    return arr;
}
void array_free(Array *arr)
{
    free(arr->buf);
    free(arr);
}

void *array_get(Array *arr, unsigned int i)
{
    return arr->buf + arr->object_size * i;
}
void *array_top(Array *arr)
{
    return arr->buf + arr->object_size * (arr->length - 1);
}
unsigned int array_length(Array *arr)
{
    return arr->length;
}

void *array_begin(Array *arr)
{
    return arr->buf;
}
void *array_end(Array *arr)
{
    return arr->buf + arr->object_size * arr->length;
}

void *array_add(Array *arr)
{
    if (++arr->length > arr->capacity)
        arr->buf = realloc(arr->buf, arr->object_size *
                           (arr->capacity = arr->capacity << 1));
    return arr->buf + arr->object_size * (arr->length - 1);
}
void array_reset(Array *arr, unsigned int num)
{
    free(arr->buf);

    arr->length = num;
    arr->capacity = num < 2 ? 2 : num;
    arr->buf = malloc(arr->object_size * arr->capacity);
}
void array_pop(Array *arr)
{
    if (--arr->length << 2 < arr->capacity && arr->capacity > 2)
        arr->buf = realloc(arr->buf, arr->object_size *
                           (arr->capacity = arr->capacity >> 1));
}

bool array_quick_remove(Array *arr, unsigned int i)
{
    bool ret = false;

    if (i + 1 < arr->length)
    {
        memcpy(arr->buf + arr->object_size * i,
               arr->buf + arr->object_size * (arr->length - 1),
               arr->object_size);
        ret = true;
    }

    array_pop(arr);
    return ret;
}

/* ------------------------------------------------------------------------- */

#ifdef ARRAY_TEST

#include <stdio.h>

typedef struct { int a, b; } IntPair;

void dump(Array *arr)
{
    printf("{ (%d, %d) -- ", arr->capacity, arr->length);
    for (unsigned int i = 0; i < arr->length; ++i)
    {
        IntPair *p = array_get(arr, i);
        printf("(%d, %d) ", p->a, p->b);
    }
    printf("}\n");
}

int main()
{
    Array *arr = array_new(IntPair);

    /* add some */
    for (unsigned int i = 0; i < 7; ++i)
    {
        array_add_val(IntPair, arr) = (IntPair) { i, i * i };
        dump(arr);
    }

    /* remove some */
    array_quick_remove(arr, 2);
    dump(arr);
    array_quick_remove(arr, 4);
    dump(arr);
    while (array_length(arr) > 0)
    {
        array_quick_remove(arr, 0);
        dump(arr);
    }

    array_free(arr);
    return 0;
}

#endif

