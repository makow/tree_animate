/* This file contains the user-supplied functions to compare and insert the
 * values of keys into a tree. These functions must be user supplied because the
 * type of the keys to be inserted or compared must be known to be
 * dereferenced. */

#include "compare.h"

int compare_ints (const void *a, const void *b)
{
    const int *x = a;
    const int *y = b;

    if (*x > *y) {
        return 1;
    }
    else if (*x < *y) {
        return 0;
    }
    else {
        return SAME;
    }
}

void copy_ints (Node *new, void *data)
{
    int *key = data;
    new->key = malloc(sizeof(int*));
    if (new->key == NULL) {
        printf("Malloc failed copying key\n");
        exit(1);
    }
    *((int*)new->key) = *key;
}
