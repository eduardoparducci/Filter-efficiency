#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/imageprocessing.h"
#include "../lib/chronometer.h"

void *sobel_interface(void *args);

int main(int argc, char *argv[]) {
  FILE *fp;
  image img;
  image *filtered = NULL;
  char out_file[100];
  unsigned int i, j;

  /* Title of running file */
  printf("Started method: INLINE\n");

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
  img = open_image(argv[1]);

  /* Write header of time report */
  fp = fopen("./docs/time-report.csv", "a");
  fprintf(fp, "Inline, 1, %u, %u, ", img.height, img.width);
  fclose(fp);
  
  /* Process image */
  filtered = measure_time(sobel_interface, &img);
  save_image(out_file, filtered);

  /* Remove images from memory */
  free_image(&img);
  free_image(filtered);  
  free(filtered);

  return 0;
}

void *sobel_interface(void *args) {
  image *img = (image *)args;
  image *filtered = malloc(sizeof(image));

  *filtered = sobel(img);
  return filtered;
}
