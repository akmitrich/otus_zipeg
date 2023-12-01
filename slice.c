#include "slice.h"
#include <stdio.h>

struct Slice slice_from(void *data, size_t len)
{
    return (struct Slice){.data = data, .len = len};
}

int slice_is_empty(struct Slice slice)
{
    return slice.len == 0;
}

int slice_is_none(struct Slice slice)
{
    return slice.data == NULL;
}

struct Slice slice_window_new(struct Slice from, size_t len)
{
    if (len == 0 || len > from.len)
        return (struct Slice){0};
    return (struct Slice){.data = from.data, .len = len};
}

struct Slice slice_window_shift(struct Slice window, struct Slice on, int step)
{
    char *main = on.data;
    char *ptr = window.data;
    if ((on.len < window.len) || (ptr - main + step < 0) || (ptr - main > (int)(on.len - window.len) - step))
        return (struct Slice){0};
    return (struct Slice){.data = ptr + step, .len = window.len};
}

void slice_debug(struct Slice slice)
{
    for (size_t i = 0; i < slice.len; ++i)
    {
        printf("%c", ((char *)slice.data)[0]);
    }
    printf("\n");
}