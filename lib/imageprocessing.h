#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} image;

typedef struct {
	unsigned int start, end;
	image *img;
	image *filtered;
} thread_args;

image open_image(char *file_name);
image open_image_multiprocess(char *file_name);
image create_simple_image();
void print_image(image *img);
void save_image(char *file_name, image *img);
void free_image(image *img);
void munmap_image(image *img);

void black_white(image *img);
image sobel(image *img);
image sobel_multithread(image *img, int num_thr);
void *apply_sobel_threads(void *args);
void apply_sobel(image *src, image *dest, unsigned int lin, unsigned int col);

image sobel_multiprocess(image *img, int num_prc);

#endif
