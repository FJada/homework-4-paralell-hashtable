
#include "util.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NUM_KEYS 2000000 // Number of keys
int num_threads = 1;     // Number of threads (configurable using command)
int keys[NUM_KEYS];      // This is not the hashtable

double now() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void *put_phase(void *arg) {
  long tid = (long)arg;
  int key = 0;

  for (key = tid; key < NUM_KEYS; key += num_threads) {
    insert(keys[key], tid);
  }

  pthread_exit(NULL);
}

void *get_phase(void *arg) {
  long tid = (long)arg;
  int key = 0;
  long lost = 0;

  for (key = tid; key < NUM_KEYS; key += num_threads) {
    if (retrieve(keys[key]) == NULL)
      lost++;
  }
  printf("[thread %ld] %ld keys lost!\n", tid, lost);

  pthread_exit((void *)lost);
}

int main(int argc, char **argv) {
  long i;
  pthread_t *threads;
  init();
  double insert_start, insert_end, retrieve_start, retrieve_end;

  if (argc != 2) {
    panic("usage: ./parallel_hashtable <num_threads>");
  }
  if ((num_threads = atoi(argv[1])) <= 0) {
    ("must enter a valid number of threads to run");
  }
  
  // =================  Keys Preparation Phase Start =================
  // set the seed of random number generator to be the current time
  srandom(time(NULL));
  for (i = 0; i < NUM_KEYS; i++)
    keys[i] = random();
  // ================= Keys Preparation Phase End =================
 
  threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
  if (!threads) {
    panic("out of memory allocating thread handles");
  }

  // ================= Put Phase Start =================
  // Insert keys in parallel
  insert_start = now();
  for (i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, put_phase, (void *)i);
  }

  // Barrier
  for (i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  insert_end = now();
  // ================= Put Phase End =================

  // Reset the thread array
  memset(threads, 0, sizeof(pthread_t) * num_threads);

  // ================= Get Phase Start =================
  // Retrieve keys in parallel
  retrieve_start = now();
  for (i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, get_phase, (void *)i);
  }

  // Collect count of lost keys
  long total_lost = 0;
  long *lost_keys = (long *)malloc(sizeof(long) * num_threads);
  for (i = 0; i < num_threads; i++) {
    pthread_join(threads[i], (void **)&lost_keys[i]);
    total_lost += lost_keys[i];
  }
  retrieve_end = now();
  // ================= Get Phase End =================

  printf("[main] Inserted %d keys in %f seconds\n", NUM_KEYS,
         insert_end - insert_start);
  printf("[main] Retrieved %ld/%d keys in %f seconds\n", NUM_KEYS - total_lost,
         NUM_KEYS, retrieve_end - retrieve_start);

  return 0;
}
