#include <stdio.h>

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
    printf("Looking inside of %s...\n", filename);
    return 0;
}