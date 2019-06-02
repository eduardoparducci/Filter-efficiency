#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} image;

image open_image(char *file_name);
image create_simple_image();
void print_image(image *img);
void save_image(char *file_name, image *img);
void free_image(image *img);

image sobel(image *img);
void apply_sobel(image *src, image *dest, unsigned int lin, unsigned int col);
#endif
