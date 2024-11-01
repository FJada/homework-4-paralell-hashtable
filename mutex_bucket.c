#include "util.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BUCKETS 100000 // Buckets in hash table
pthread_mutex_t lock[NUM_BUCKETS]; // declare a lock for multiple buckets 
bucket_entry *table[NUM_BUCKETS];

int hash(int key) {
  return key % NUM_BUCKETS;
}

void init() {
  // TODO: Initializing the lock for the table goes here
  for(int i = 0; i < NUM_BUCKETS; i++){
    // init for diff buckets 
    pthread_mutex_init(&lock[i], NULL);
  }
}

// Inserts a key-value pair into the table
void insert(int key, int val) {
  int i = hash(key);
  bucket_entry *e = (bucket_entry *)malloc(sizeof(bucket_entry));
  if (!e)
    panic("No memory to allocate bucket!");
  pthread_mutex_lock(&lock[i]); // acquire the lock for different buckets
  e->next = table[i];
  e->key = key;
  e->val = val;
  table[i] = e;
  pthread_mutex_unlock(&lock[i]); // release the lock for different buckets
}

// Retrieves an entry from the hash table by key
// Returns NULL if the key isn't found in the table
bucket_entry *retrieve(int key) {
  bucket_entry *b;
  for (b = table[hash(key)]; b != NULL; b = b->next) {
    if (b->key == key)
      return b;
  }
  return NULL;
}
