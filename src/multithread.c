#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "../lib/imageprocessing.h"
#include "../lib/chronometer.h"

#define MAX 2
void *sobel_interface(void *args, void *num_thr);

int main(int argc, char *argv[]) {
  FILE *fp;
  image img;
  image *filtered = NULL;
  char out_file[100] = "filtered-";
  unsigned int i, j;


  int *num_thr = malloc(sizeof(int));
  *num_thr = MAX;

  /* Title of running file */
  printf("Started method: MULTITHREAD\n");

  /* Name of file and number of threads */
  if(argc<=2) {
    printf("Missing filename. Expect: ./inline <filename> <n_threads>\n");
    return 1;
  } else {

    /* Append '-filtered' on output picture name */
    strcpy(out_file, argv[1]);
    out_file[strlen(out_file)-strlen(".jpg")] = '\0';
    strcat(out_file, "-filtered.jpg");

    /* Configure number of threads */
    *num_thr = atoi(argv[2]);
  }

  /* Open Image */
  img = open_image(argv[1]);

  /* Write header of time report */
  fp = fopen("./docs/time-report.csv", "a");
  fprintf(fp, "Multithread, %d, %u, %u, ", *num_thr, img.height, img.width);
  fclose(fp);


  /* Process image */
  filtered = measure_time_multithread(sobel_interface, &img, num_thr);
  save_image(out_file, filtered);

  /* Remove images from memory */
  free_image(&img);
  free_image(filtered);
  free(filtered);
  free(num_thr);
  
  return 0;
}

void *sobel_interface(void *args, void *num_thr) {
  image *img = (image *)args;
  int *num_threads = (int *)num_thr;
  image *filtered = malloc(sizeof(image));

  *filtered = sobel_multithread(img, *num_threads);
  return filtered;
}
