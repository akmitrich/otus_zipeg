#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

int process_file(FILE *input)
{
    int n;
    unsigned char *jpeg_marker;
    uint16_t *int_marker;
    unsigned char buffer[MAX_BUF_SIZE];
    struct Slice chunk;
    int len = 0;
    jpeg_marker = buffer;

    n = fread(buffer, 1, 2, input);
    if (n != 2)
    {
        printf("ERROR: unable to read first two bytes from FILE\n");
        return 77;
    }
    int_marker = (uint16_t *)jpeg_marker;
    if (*int_marker != 0xd8ff)
    {
        printf("ERROR: FILE is not a JPEG. %x\n", *int_marker);
        return 1;
    }
    while (*int_marker != 0xd9ff)
    {
        if (jpeg_marker - buffer > len - 2)
        {
            buffer[0] = jpeg_marker[0];
            len = fread(buffer + 1, 1, MAX_BUF_SIZE, input);
            if (len == 0)
            {
                printf("ERROR: end of JPEG not found\n");
                return 2;
            }
            chunk = slice_from(buffer, len);
            slice_debug(&chunk);
            printf("Had read %d bytes", len);
            return 88;
        }
    }
    printf("FILE is a JPEG");

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

    int n = process_file(input);
    printf("Finish to process %s\n", filename);

    fclose(input);
    return n;
}