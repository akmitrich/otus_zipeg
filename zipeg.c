#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "slice.h"

static int run(const char *);

int main(int argc, char *argv[])
{
    const char *program = argv[0];
    printf("%s -- determine whether the FILE is a Rarjpeg, i.e. the image and some zip-archive after it.\n", program);
    if (argc < 2 || argc > 2)
    {
        printf("ERROR! input file must be the only argument.\nUsage:\n%s [FILE]\n", program);
        return 1;
    }

    return run(argv[1]);
}

#define MAX_BUF_SIZE 4096
#define JPEG_MARKER_LEN 2

int find_jpeg_end(FILE *input)
{
    int n;
    uint16_t *int_marker;
    unsigned char buffer[MAX_BUF_SIZE];
    struct Slice chunk;
    struct Slice jpeg_marker;
    size_t end_jpeg_pos;
    int len = 0;

    n = fread(buffer, 1, JPEG_MARKER_LEN, input);
    if (n != JPEG_MARKER_LEN)
    {
        printf("ERROR: unable to read first bytes from FILE\n");
        return -1;
    }
    int_marker = (uint16_t *)buffer;
    if (*int_marker != 0xd8ff)
    {
        printf("ERROR: FILE is not a JPEG. %x\n", *int_marker);
        return -1;
    }

    buffer[0] = 0;
    end_jpeg_pos = n;
    jpeg_marker = (struct Slice){.data = NULL, .len = 2};
    while (*int_marker != 0xd9ff)
    {
        ++end_jpeg_pos;
        if (slice_is_none(jpeg_marker))
        {
            len = fread(buffer + 1, 1, MAX_BUF_SIZE - 1, input);
            if (len == 0)
            {
                printf("ERROR: unexpected end of JPEG.\n");
                return -2;
            }
            chunk = slice_from(buffer, len + 1);
            jpeg_marker = slice_window_new(chunk, JPEG_MARKER_LEN);
        }
        int_marker = (uint16_t *)jpeg_marker.data;
        struct Slice new_marker = slice_window_shift(jpeg_marker, chunk, 1);
        if (slice_is_none(new_marker))
        {
            unsigned char *last_marker = jpeg_marker.data;
            buffer[0] = last_marker[JPEG_MARKER_LEN - 1];
        }
        jpeg_marker = new_marker;
    }
    printf("JPEG ends at %ld\n", end_jpeg_pos);

    return end_jpeg_pos;
}

int check_zip(FILE *input)
{
    printf("%d", input->_mode);
    return 0;
}

int run(const char *filename)
{

    printf("Looking inside of %s...\n", filename);
    FILE *input = fopen(filename, "rb");
    if (input == NULL)
    {
        printf("ERROR: cannot open FILE %s\n", filename);
        return 1;
    }

    int n = find_jpeg_end(input);
    if (n > 0)
    {
        fseek(input, n, SEEK_SET);
        n = check_zip(input);
        printf("Finish to process %s\n", filename);
    }
    else
    {
        printf("Unsuccessful.\n");
    }

    fclose(input);
    return n;
}