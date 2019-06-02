#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/imageprocessing.h"

int main(int argc, char *argv[]) {
  image img, filtered;
  char out_file[100] = "filtered-";
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

  /* Process image */
  img = open_image(argv[1]);
  filtered = sobel(&img);
  save_image(out_file, &filtered);
  
  /* Remove images from memory */
  free_image(&img);
  free_image(&filtered);
  
  return 0;
}
