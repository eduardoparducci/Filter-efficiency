#include <time.h>
#include <sys/time.h>
#include <stdio.h>

void *measure_time(void *(*function)(void *args), void *args) {
  FILE *fp;
  clock_t ct0, ct1, dct; /* Measures based on CPU clock */
  struct timeval rt0, rt1, drt; /* Measure real time */
  void *P;


  int i;
  /* Get initial time*/
  gettimeofday(&rt0, NULL);
  ct0 = clock();
  
  /* Call function */
  P = function(args);

  /* Get final time*/
  ct1 = clock();
  gettimeofday(&rt1, NULL);

  timersub(&rt1, &rt0, &drt);

  /* Write to file */
  fp = fopen("./docs/time-report.csv", "a");
  fprintf(fp, "%ld.%06ld, %f\n", drt.tv_sec, drt.tv_usec, (double)(ct1-ct0)/CLOCKS_PER_SEC);
  fclose(fp);
  return P;
}

