#include "util.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BUCKETS 100000 // Buckets in hash table
pthread_spinlock_t spinlock; // declare lock
bucket_entry *table[NUM_BUCKETS];

int hash(int key) {
  return key % NUM_BUCKETS;
}

void init() {
  // TODO: Initializing the lock for the table goes here
  pthread_spin_init(&spinlock, 0);
}

// Inserts a key-value pair into the table
void insert(int key, int val) {
  int i = hash(key);
  bucket_entry *e = (bucket_entry *)malloc(sizeof(bucket_entry));
  if (!e)
    panic("No memory to allocate bucket!");
  pthread_spin_lock(&spinlock); // acquire lock
  e->next = table[i];
  e->key = key;
  e->val = val;
  table[i] = e;
  pthread_spin_unlock(&spinlock); // release lock
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
