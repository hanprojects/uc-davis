#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 8

/*****************/
/* MAIN FUNCTION  /
******************/
int main(int argc, char** argv)
{
    (void) argc;

// OPEN FILE TO READ UNDER BINARY FORM
    char *file = argv[1];    
    FILE *pgm_file;
    pgm_file = fopen(file, "rb"); // rb for reading binary form
    
    if (!pgm_file)
    {
        printf("Error: Unable to open file %s.\n", file);
        exit(1);
    }

// READ FIRST LINE OF PGM FILE
// Source: https://sun.iwu.edu/~shelley/sie/zoo/journal/pgm.c.html
    unsigned char ch;
    ch = getc(pgm_file);
    if(ch != 'P')
    {
        printf("Error: Not valid pgm/ppm file type\n");
        exit(1);
    }

    ch = getc(pgm_file);
    int type; 
    type = ch - 48;
    if((type != 2) && (type != 3) && (type != 5) && (type != 6))
    {
        printf("Error): Not valid pgm/ppm file type\n");
        exit(1);
    }

// SKIP TO THE END OF THE FIRST LINE AND SKIP THE COMMENT LINE
// Source: https://sun.iwu.edu/~shelley/sie/zoo/journal/pgm.c.html
    while (getc(pgm_file) != '\n') {};
    while (getc(pgm_file) == '#')
    {
        while (getc(pgm_file) != '\n'); 
    }
 
// RETRIEVE DATA FROM PGM FILE
    int width, height, cutoff; 
    unsigned char *pixels;

    fscanf(pgm_file, "%d %d\n", &width, &height); // Get width and height of pgm
    fscanf(pgm_file, "%d\n", &cutoff); // Get cutoff of pgm

    /* SUCCESSFULLY TESTED
    printf("%d\n", width);
    printf("%d\n", height);
    printf("%d\n", cutoff);
    */

    // Source: https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm
    pixels = (unsigned char *) malloc (sizeof(int) * width * height);
    fread(pixels, sizeof(int), width * height, pgm_file); // Get the image pixels in binary form and read into the unsigned char `pixels`

    for (int i = 0; i < width * height; i++)
    {
        printf("%d ", pixels[i]);
    }
    printf("\n");

    fclose(pgm_file);
    return 0;
}