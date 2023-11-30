#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

int run(const char *filename)
{
    int n;
    int length;
    unsigned char jpeg_marker[2];
    unsigned char le_bytes[2];
    int16_t *int_marker = (int16_t *)jpeg_marker;

    printf("Looking inside of %s...\n", filename);
    FILE *input = fopen(filename, "rb");
    if (input == NULL)
    {
        printf("ERROR: cannot open FILE %s\n", filename);
        return 1;
    }

    n = fread(jpeg_marker, sizeof(jpeg_marker), 1, input);
    if (n != 1 || (jpeg_marker[0] != 0xff && jpeg_marker[1] != 0xd8))
    {
        printf("ERROR: %s is not a JPEG. (%x %x)\n", filename, jpeg_marker[0], jpeg_marker[1]);
        goto clean_up;
    }

    while (*int_marker != 0xffd9)
    {
        n = fread(jpeg_marker, sizeof(jpeg_marker), 1, input);
        if (n != 1 || jpeg_marker[0] != 0xff)
        {
            printf("ERROR: %s missed JPEG app marker. (%x %x)\n", filename, jpeg_marker[0], jpeg_marker[1]);
            goto clean_up;
        }
        n = fread(le_bytes, sizeof(le_bytes), 1, input);
        length = le_bytes[1] | le_bytes[0] << 8;
        printf("Marker: (%x, %x) -> %d\n", jpeg_marker[0], jpeg_marker[1], length);
        if (fseek(input, length - 2, SEEK_CUR) != 0)
        {
            printf("ERROR: jump by wrong number of bytes: %d\n", length);
            goto clean_up;
        }
    }

    printf("TODO: it's a JPEG\n");
clean_up:
    fclose(input);
    return n;
}