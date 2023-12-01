#ifndef H_SLICE
#define H_SLICE

#include <stddef.h>

struct Slice
{
    void *data;
    size_t len;
};

struct Slice slice_from(void *data, size_t len);

int slice_is_empty(struct Slice slice);
int slice_is_none(struct Slice slice);

struct Slice slice_window_new(struct Slice from, size_t len);
struct Slice slice_window_shift(struct Slice window, struct Slice on, int step);

void slice_debug(struct Slice slice);

#endif