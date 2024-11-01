#include "util.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BUCKETS 100000 // Buckets in hash table
pthread_mutex_t lock; // declare a lock for 1 bucket
bucket_entry *table[NUM_BUCKETS];

int hash(int key) {
  return key % NUM_BUCKETS;
}

void init() {
  // TODO: Initializing the lock for the table goes here
  pthread_mutex_init(&lock, NULL);
}

// Inserts a key-value pair into the table
void insert(int key, int val) {
  //  hash function
  int i = hash(key);
  bucket_entry *e = (bucket_entry *)malloc(sizeof(bucket_entry));
  if (!e)
    panic("No memory to allocate bucket!");
  pthread_mutex_lock(&lock); // acquire the lock 1 bucket
  e->next = table[i];
  e->key = key;
  e->val = val;
  table[i] = e;
  pthread_mutex_unlock(&lock); // release the lock 1 bucket
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
