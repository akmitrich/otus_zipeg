#include "slice.h"
#include <stdio.h>

struct Slice slice_from(void *data, size_t len)
{
    return (struct Slice){.data = data, .len = len};
}

void slice_debug(struct Slice *slice)
{
    for (size_t i = 0; i < slice->len; ++i)
    {
        printf("%c", ((char *)(slice->data))[0]);
    }
    printf("\n");
}