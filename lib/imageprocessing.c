#include <stdlib.h>
#include <stdio.h>
#include <FreeImage.h>
#include <pthread.h>
#include <unistd.h>
#include "imageprocessing.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int protection = PROT_READ | PROT_WRITE;
int visibility = MAP_SHARED | MAP_ANON;

image open_image_multiprocess(char *file_name){
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  image I;


  bitmapIn = FreeImage_Load(FIF_JPEG, file_name, 0);

  if (bitmapIn == 0) {
    printf("Error! File not found - %s\n", file_name);
  } else {
    printf("Opened file: %s\n", file_name);
  }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = mmap(NULL, sizeof(float) * x * y, protection, visibility, 0, 0);
  I.g = mmap(NULL, sizeof(float) * x * y, protection, visibility, 0, 0);
  I.b = mmap(NULL, sizeof(float) * x * y, protection, visibility, 0, 0);

  for (int i=0; i<x; i++) {
    for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
  }
  return I;
}

image open_image(char *file_name) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  image I;

  bitmapIn = FreeImage_Load(FIF_JPEG, file_name, 0);

  if (bitmapIn == 0) {
    printf("Error! File not found - %s\n", file_name);
  } else {
    printf("Opened file: %s\n", file_name);
  }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

  for (int i=0; i<x; i++) {
    for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
  }
  return I;

}

void free_image(image *img) {
  free(img->r);
  free(img->g);
  free(img->b);
}
void munmap_image(image *img) {
  unsigned int limh = img->height;
  unsigned int limw = img->width;
  munmap( img->r, limh*limw*sizeof(float) );
  munmap( img->g, limh*limw*sizeof(float) );
  munmap( img->b, limh*limw*sizeof(float) );

}

void save_image(char *file_name, image *img) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  bitmapOut = FreeImage_Allocate(img->width, img->height, 24, 0, 0, 0);

  for (unsigned int i=0; i<img->width; i++) {
    for (unsigned int j=0; j<img->height; j++) {
      int idx;

      idx = i + (j*img->width);
      color.rgbRed = img->r[idx];
      color.rgbGreen = img->g[idx];
      color.rgbBlue = img->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, file_name, JPEG_DEFAULT);
  printf("Created File: %s\n", file_name);
}

image create_simple_image() {
  image img;
  unsigned int i, j;

  /* Create data matrix for example (10x20) */
  img.width = 10;
  img.height = 20;
  img.r = malloc((10*20)*sizeof(float));
  img.g = malloc((10*20)*sizeof(float));
  img.b = malloc((10*20)*sizeof(float));
    
  for(i=0 ; i<img.height ; i++) {
    for(j=0 ; j<img.width ; j++) {
      /* Black line on top half */
      if( (i<(img.height/2)) ) {
        img.r[i*img.width + j] = 0;
        img.g[i*img.width + j] = 0;
        img.b[i*img.width + j] = 0;
      }
      /* White line on bottom half */
      else {
        img.r[i*img.width + j] = 255;
				img.g[i*img.width + j] = 255;
				img.b[i*img.width + j] = 255;
			}
		}
	}

	return img;
}

void print_image(image *img) {
	unsigned int i, j;

	printf("RED:\n");
	for(i=0 ; i<img->height ; i++) {
		for(j=0 ; j<img->width ; j++) {
			printf(" %3.0f ", img->r[i*img->width + j]);
		}
		putchar('\n');
	}

	printf("GREEN:\n");
	for(i=0 ; i<img->height ; i++) {
		for(j=0 ; j<img->width ; j++) {
			printf(" %3.0f ", img->g[i*img->width + j]);
		}
		putchar('\n');
	}

	printf("BLUE:\n");
	for(i=0 ; i<img->height ; i++) {
		for(j=0 ; j<img->width ; j++) {
			printf(" %3.0f ", img->b[i*img->width + j]);
		}
		putchar('\n');
	}
}

void black_white(image *img) {

	unsigned int i, j;
	float avarage = 0;

	for(i=0 ; i<img->height ; i++) {
		for(j=0 ; j<img->width ; j++) {
			/* Calculate avarage of R+G+B */
			avarage = (
                 img->r[i*img->width + j] +
                 img->g[i*img->width + j] +
                 img->b[i*img->width + j] ) / 3;

			/*  Transform in binary (black or white) */
			if(avarage >= 128) {
				img->r[i*img->width + j] = 255;
				img->g[i*img->width + j] = 255;
				img->b[i*img->width + j] = 255;
			} else {
				img->r[i*img->width + j] = 0;
				img->g[i*img->width + j] = 0;
				img->b[i*img->width + j] = 0;
			}
		}
	}
}

image sobel(image *img) {

	unsigned int i, j;
	unsigned int limh, limw;
	image filtered;

	/* Create blank sample based on *img */
	limh = img->height;
	limw = img->width;

	/* Take black and white version of img */
	black_white(img);
	filtered.width = limw;
	filtered.height = limh;
	filtered.r = malloc(limh*limw*sizeof(float));
	filtered.g = malloc(limh*limw*sizeof(float));
	filtered.b = malloc(limh*limw*sizeof(float));

	/* Filter image */
	for(i=0 ; i<limh ; i++) {
		for(j=0 ; j<limw ; j++) {
			apply_sobel(img, &filtered, i, j);
		}
	}

	return filtered;
}
image sobel_multiprocess(image *img,int num_processes){
  unsigned int i, j;
	unsigned int limh, limw;
	unsigned int block_size;
	image filtered;
	pid_t prc[num_processes];
	thread_args prc_args[num_processes];


	block_size = img->height/num_processes;

	/* Create blank sample based on *img */
	limh = img->height;
	limw = img->width;


	/* Take black and white version of img */
	black_white(img);
	filtered.width = limw;
	filtered.height = limh;

  filtered.r = mmap(NULL, limh*limw*sizeof(float), protection, visibility, 0, 0);
  filtered.g = mmap(NULL, limh*limw*sizeof(float), protection, visibility, 0, 0);
  filtered.b = mmap(NULL, limh*limw*sizeof(float), protection, visibility, 0, 0);

  for(int k = 0; k < num_processes; k++){
		if(k < num_processes - 1) {
			if(k == 0)
				prc_args[k].start = k*block_size;
			else
				prc_args[k].start = k*block_size+1;
			prc_args[k].end = prc_args[k].start + block_size;
			prc_args[k].img = img;
			prc_args[k].filtered = &filtered;
		} else {
			prc_args[k].start = k*block_size+1;
			prc_args[k].end = img->height;
			prc_args[k].img = img;
			prc_args[k].filtered = &filtered;
		}
	}
  for (int k = 0; k < num_processes; k++) {
    prc[k]=fork();
    if(prc[k]==0){
      apply_sobel_threads((void*) &prc_args[k]);
      exit(EXIT_SUCCESS);
    }
	}

  for(int k=0;k < num_processes;k++){
    int status;
    waitpid(prc[k], &status, 0);
  }
  return filtered;

}
image sobel_multithread(image *img, int num_threads) {

	unsigned int i, j;
	unsigned int limh, limw;
	unsigned int block_size;
	image filtered;
	//create threads
	pthread_t threads[num_threads];
	thread_args thread_arguments[num_threads];

	block_size = img->height/num_threads;

	/* Create blank sample based on *img */
	limh = img->height;
	limw = img->width;

	/* Take black and white version of img */
	black_white(img);
	filtered.width = limw;
	filtered.height = limh;
	filtered.r = malloc(limh*limw*sizeof(float));
	filtered.g = malloc(limh*limw*sizeof(float));
	filtered.b = malloc(limh*limw*sizeof(float));

	/* Apply Sobel filter using threads */
	for(int k = 0; k < num_threads; k++){
		if(k < num_threads - 1) {
			if(k == 0)
				thread_arguments[k].start = k*block_size;
			else
				thread_arguments[k].start = k*block_size+1;
			thread_arguments[k].end = thread_arguments[k].start + block_size;
			thread_arguments[k].img = img;
			thread_arguments[k].filtered = &filtered;
		} else {
			thread_arguments[k].start = k*block_size+1;
			thread_arguments[k].end = img->height;
			thread_arguments[k].img = img;
			thread_arguments[k].filtered = &filtered;
		}
	}
	for (int k = 0; k < num_threads; k++) {
		pthread_create(& (threads[k]), NULL, apply_sobel_threads,(void*) &thread_arguments[k]);
	}

	for(int k = 0; k < num_threads; k++){
		pthread_join(threads[k], NULL);
	}

	return filtered;
}

void *apply_sobel_threads(void *args) {
	thread_args *parameters = (thread_args *) args;

	for(unsigned int i=parameters->start; i<parameters->end ; i++) {
		for(unsigned int j=0 ; j<parameters->img->width ; j++) {
		  apply_sobel(parameters->img, parameters->filtered, i, j);
		}
	}
  return NULL;
}

void apply_sobel(image *src, image *dest, unsigned int lin, unsigned int col) {

	unsigned int height = src->height;
	unsigned int width = src->width;
	unsigned int pos = lin*width + col;

	/* Central pixel */
	dest->r[pos] = src->r[pos] * -4;
	dest->g[pos] = src->g[pos] * -4;
	dest->b[pos] = src->b[pos] * -4;

	/* Maximum line test */
	if(lin < (height-1)) {
		dest->r[pos] += src->r[pos + width];
		dest->g[pos] += src->g[pos + width];
		dest->b[pos] += src->b[pos + width];
	} else {
		dest->r[pos] += src->r[pos];
		dest->g[pos] += src->g[pos];
		dest->b[pos] += src->b[pos];
	}

	/* Maximum column test */
	if(col < (width-1)) {
		dest->r[pos] += src->r[pos + 1];
		dest->g[pos] += src->g[pos + 1];
		dest->b[pos] += src->b[pos + 1];
	} else {
		dest->r[pos] += src->r[pos];
		dest->g[pos] += src->g[pos];
		dest->b[pos] += src->b[pos];
	}

	/* Mininum line test */
	if(lin) {
		dest->r[pos] += src->r[pos - width];
		dest->g[pos] += src->g[pos - width];
		dest->b[pos] += src->b[pos - width];
	} else {
		dest->r[pos] += src->r[pos];
		dest->g[pos] += src->g[pos];
		dest->b[pos] += src->b[pos];
	}

	/* Minimum column test */
	if(col) {
		dest->r[pos] += src->r[pos - 1];
		dest->g[pos] += src->g[pos - 1];
		dest->b[pos] += src->b[pos - 1];
	} else {
		dest->r[pos] += src->r[pos];
		dest->g[pos] += src->g[pos];
		dest->b[pos] += src->b[pos];
	}

  /* Bring value to binary 128+ -> 255 and 128- -> 0 */
  if(dest->r[pos] < 128) dest->r[pos] = 0;
  if(dest->g[pos] < 128) dest->g[pos] = 0;
  if(dest->b[pos] < 128) dest->b[pos] = 0;
  if(dest->r[pos] >= 128) dest->r[pos] = 255;
  if(dest->g[pos] >= 128) dest->g[pos] = 255;
  if(dest->b[pos] >= 128) dest->b[pos] = 255;
}
