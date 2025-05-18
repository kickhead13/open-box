#include "utils.h"

pthread_mutex_t boxf_lock;        /* Mutex lock for the BOXFILE.   */
pthread_mutex_t cryptf_lock;      /* Mutex lock for the CRYPTFILE. */

#include <stdio.h>

ssize_t write_order_to_box(char *restrict const order) {
  pthread_mutex_lock(&boxf_lock);
  ssize_t bytes_wrote = 0;
  ssize_t total_bytes_wrote = 0;
  ssize_t total_len = strlen(order);
  char a[256];
  while(bytes_wrote = write(BOXFILE_FD, order + total_bytes_wrote, total_len)) {
    total_bytes_wrote += bytes_wrote; 
    if(bytes_wrote == -1) {
      sprintf(a, "|  %ld %ld  |", bytes_wrote, total_bytes_wrote);
      write(1, a, strlen(a));
      write(1, "a", 1);
      break;
    } else {
      sprintf(a, "%ld", bytes_wrote);
      write(1, a, strlen(a));
      write(1, "a", 1);
    }
  }
  pthread_mutex_unlock(&boxf_lock);
  return 0;
}
