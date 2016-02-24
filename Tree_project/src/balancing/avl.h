#ifndef FILE_AVL_H
#define FILE_AVL_H
#include "../core/tree.h"

void avl_insert_balance (Tree *T, Node *x);
void avl_delete_balance (Tree *T, Node *x);

#endif
