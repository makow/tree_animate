/* This file will contain tests to verify whether valid AVL, WAVL, 2-3 and 2-3-4
 * trees are produced by their respective balancing functions.  */

#include test.h

Node *search (Tree *T, void *k, bool *side)
{
    T->sentinel->key = k;
    Node *x = T->head;
    while (T->compare (x->child[*side]->key, k) != SAME) {
        x = x->child[*side];
        *side = T->compare (k, x->key);
    }
    T->sentinel->key = NULL;
    return x;
}

void binary (Tree *T, bool side)
{
    x = T->head->child[R];
    if (x == T->sentinel) {
        return;
    }
    if (T->compare (x->child[L]->key, x->key) == 0) {
        binary (T, !side);
    }
    if (T->compare (x->child[R]->key, x->key) == 1) {
        binary (T, side);
    }
}
