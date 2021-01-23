/*
 * Copyright 2021, Joël Porquet-Lupine
 *
 * Copying and distribution of this file, with or without modification, are
 * permitted in any medium without royalty provided the copyright notice and
 * this notice are preserved. This file is offered as-is, without any warranty.
 * --GNU All-Permissive License
 */

#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/*
 * Useful macros
 */
#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define MIN(x, y)			((x) < (y)) ? (x) : (y)
#define MAX(x, y)			((x) > (y)) ? (x) : (y)
#define CLAMP(x, lo, hi)	min(max(x, lo), hi)

/*
 * Program helpers
 */
#define die(...)                    \
do {                                \
	fprintf(stderr, ##__VA_ARGS__); \
	fprintf(stderr, "\n");          \
	exit(1);                        \
} while (0)

#define die_perror(s) \
do {                  \
	perror(s);        \
	exit(1);          \
} while (0)

static inline void *xcalloc(size_t nmemb, size_t size)
{
	void *p = calloc(nmemb, size);

	if (!p)
		die_perror("calloc");
	return p;
}

static inline void *xmalloc(size_t size)
{
	void *p = malloc(size);

	if (!p)
		die_perror("malloc");
	return p;
}

/*
 * Image representation and loading
 */
struct image {
	int width, height;
	unsigned char *gpix;	/* One byte per grayscale pixel */
};

void destroy_image(struct image *img)
{
	free(img->gpix);
}

void load_image(char *filename, struct image *img)
{
	FILE *fp;
	int n;

	/* Open file */
	fp = fopen(filename, "rb");
	if (!fp)
		die("Error: cannot open file %s", filename);

	/* Get structural information */
	n = fscanf(fp, "P5\n%d %d\n255\n", &img->width, &img->height);
	if (n != 2)
		die("Error: invalid PGM information");

	/* Read pixel content */
	img->gpix = xmalloc(img->width * img->height);
	n = fread(img->gpix, sizeof(char), img->width * img->height, fp);
	if (n != img->width * img->height)
		die("Error: invalid PGM pixels");

	fclose(fp);
}

/*
 * Image reduction
 */
#define BLOCK_W 4 	/* 4 pixels per output character in width */
#define BLOCK_H 8 	/* 8 pixels per output character in height */

unsigned char block_avg(struct image *img, int xblock, int yblock)
{
	int x, y;
	unsigned int sum = 0;
	int npixels = 0;

	/* Derive image coordinates from block coordinates */
	int x0 = xblock * BLOCK_W;
	int y0 = yblock * BLOCK_H;

	/* Go through block and sum all available pixels */
	for (y = y0; y < y0 + BLOCK_H && y < img->height; y++) {
		for (x = x0; x < x0 + BLOCK_W && x < img->width; x++) {
			sum += img->gpix[y * img->width + x];
			npixels++;
		}
	}

	/* Return average block value */
	return sum / npixels;
}

void reduce_image(struct image *in, struct image *out)
{
	/* Output image characteristics */
	out->width = DIV_ROUND_UP(in->width, BLOCK_W);
	out->height = DIV_ROUND_UP(in->height, BLOCK_H);
	out->gpix = xmalloc(out->width * out->height);

	/* Compute each pixel of output image, from input image */
	int x, y;
	for (y = 0; y < out->height; y++)
		for (x = 0; x < out->width; x++)
			out->gpix[y * out->width + x] = block_avg(in, x, y);
}

/*
 * Image display
 */
#define N_THRESHOLD	5	/* 5 block colors */

/* Define ranges of unicode colors */
static struct {
	unsigned char threshold;
	wchar_t color;
} blocks[N_THRESHOLD] = {
	(UCHAR_MAX / N_THRESHOLD) * 4,	L' ',		/* 204 - 255: Empty block */
	(UCHAR_MAX / N_THRESHOLD) * 3, 	L'\u2591',	/* 153 - 203: Light shade */
	(UCHAR_MAX / N_THRESHOLD) * 2,	L'\u2592',	/* 102 - 152: Medium shade */
	(UCHAR_MAX / N_THRESHOLD) * 1,	L'\u2593',	/*  51 - 101: Dark shade */
	(UCHAR_MAX / N_THRESHOLD) * 0,	L'\u2588',	/*   0 -  50: Full block */
};

void display_image(struct image *img)
{
	int x, y;

	/* Configure output for unicode */
    setlocale(LC_ALL, "en_US.UTF-8");

	/* Display output image pixel by pixel */
	for (y = 0; y < img->height; y++) {
		for (x = 0; x < img->width; x++) {
			int t;
			unsigned char block = img->gpix[y * img->width + x];

			/* Find correct block color for pixel */
			for (t = 0; t < N_THRESHOLD; t++)
				if (block >= blocks[t].threshold)
					break;

			printf("%lc", blocks[t].color);
		}
		printf("\n");
	}
}

/*
 * Main function
 */
int main(int argc, char *argv[])
{
	/* Usage */
	if (argc < 2)
		die("Usage: %s <file.pgm>", argv[0]);

	/* Open input image */
	struct image img_in;
	load_image(argv[1], &img_in);

	/* Compute output image */
	struct image img_out;
	reduce_image(&img_in, &img_out);

	/* Display output image */
	display_image(&img_out);

	/* Clean up */
	destroy_image(&img_in);
	destroy_image(&img_out);

	return 0;
}
