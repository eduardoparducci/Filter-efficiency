
#ifndef CHRONOMETER_H
#define CHRONOMETER_H

void *measure_time_multithread(void *(*function)(void *args, void *num_thr), void *args, void *num_thr);
void *measure_time(void *(*function)(void *args), void *args);
#endif
