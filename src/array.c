#include "array.h"

#include <string.h>
#include <stdlib.h>

#define MIN_CAPACITY 2

struct Array
{
    char *buf;               /* this is a char * for pointer arithmetic */
    unsigned int capacity;   /* alloc'd size of buf */
    unsigned int length;     /* number of objects */
    size_t object_size;      /* size of each element */
};

Array *array_new_(size_t object_size)
{
    Array *arr;

    arr = malloc(sizeof(Array));
    arr->object_size = object_size;
    arr->capacity = MIN_CAPACITY;
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
    /* too small? double it */
    if (++arr->length > arr->capacity)
        arr->buf = realloc(arr->buf, arr->object_size *
                           (arr->capacity = arr->capacity << 1));
    return arr->buf + arr->object_size * (arr->length - 1);
}
void array_reset(Array *arr, unsigned int num)
{
    free(arr->buf);

    arr->length = num;
    arr->capacity = num < MIN_CAPACITY ? MIN_CAPACITY : num;
    arr->buf = malloc(arr->object_size * arr->capacity);
}
void array_pop(Array *arr)
{
    /* too big (> four times as is needed)? halve it */
    if (--arr->length << 2 < arr->capacity && arr->capacity > MIN_CAPACITY)
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

void array_sort(Array *arr, int (*compar)(const void *, const void *))
{
    qsort(arr->buf, arr->length, arr->object_size, compar);
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

int int_compare(const void *a, const void *b)
{
    const int *ia = a, *ib = b;
    return *ia - *ib;
}

void test_sort()
{
    int *i;
    Array *arr = array_new(int);

    array_add_val(int, arr) = 3;
    array_add_val(int, arr) = 5;
    array_add_val(int, arr) = 1;
    array_add_val(int, arr) = 7;
    array_add_val(int, arr) = 1;
    array_add_val(int, arr) = 0;
    array_add_val(int, arr) = 499;
    array_add_val(int, arr) = 200;

    printf("before sort: ");
    array_foreach(i, arr)
        printf("%d ", *i);
    printf("\n");

    array_sort(arr, int_compare);

    printf("after sort: ");
    array_foreach(i, arr)
        printf("%d ", *i);
    printf("\n");

    array_free(arr);
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

    test_sort();
    
    return 0;
}

#endif

