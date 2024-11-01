#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>

typedef struct _bucket_entry {
    int key;
    int val;
    struct _bucket_entry *next;
} bucket_entry;


void panic(char *msg);

void init(void);
void insert(int key, int val);
bucket_entry * retrieve(int key) ;

#endif /* UTIL_H_ */