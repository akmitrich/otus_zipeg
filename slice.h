#ifndef H_SLICE
#define H_SLICE

#include <stddef.h>

struct Slice
{
    void *data;
    size_t len;
};

struct Slice slice_from(void *data, size_t len);

void slice_debug(struct Slice *slice);
#endif