#ifndef FILE_WAVL_H
#define FILE_WAVL_H
#include "../core/tree.h"

void wavl_insert_balance (Tree *T, Node *x);
void wavl_delete_balance (Tree *T, Node *x);

#endif
