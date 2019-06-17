#include <stdio.h>
#include "../lib/imageprocessing.h"
#include "../lib/chronometer.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>


#define MAX 3
void *sobel_interface(void *args, void *num_prc);

int main(int argc, char *argv[]) {
  FILE *fp;
  image img;
  image *filtered = NULL;
  char out_file[100] = "filtered-";
  unsigned int i, j;


  int *num_prc = malloc(sizeof(int));
  *num_prc = MAX;

  /* Title of running file */
  printf("Started method: MULTIPROCESS\n");

  /* Name of files to be opened and saved */
  if(argc<=1) {
    printf("Missing filename. Expect: ./inline <filename>\n");
    return 1;
  } else {
    /* Append '-filtered' on output picture name */
    strcpy(out_file, argv[1]);
    out_file[strlen(out_file)-strlen(".jpg")] = '\0';
    strcat(out_file, "-filtered.jpg");
  }

  /* Open Image */
  img = open_image_multiprocess(argv[1]);

  /* Write header of time report */
  fp = fopen("./docs/time-report.csv", "a");
  fprintf(fp, "Multiprocess, 1, %u, %u, ", img.height, img.width);
  fclose(fp);


  /* Process image */
  filtered = measure_time_multithread(sobel_interface, &img, num_prc);
  save_image(out_file, filtered);

  /* Remove images from memory */
  munmap_image(filtered);
	munmap( filtered, sizeof(image) ) ;


  return 0;
}

void *sobel_interface(void *args, void *num_prc) {
  image *img = (image *)args;
  int *num_processes = (int *)num_prc;

	int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  image *filtered =mmap(NULL, sizeof(image), protection, visibility, 0, 0);;
	 // image *filtered =mmap(NULL, sizeof(image), protection, visibility, 0, 0);
  *filtered = sobel_multiprocess(img, *num_processes);
  return filtered;
}
