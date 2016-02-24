/* This file contains the functions to restore balance to an AVL the tree after
   the insertion or deletion of a node. 'x' denotes the current node and 'p' its
   parent, 'rd' is the rank-difference and 'y' is the inner child of 'x'. These
   cases are illustrated in the 'docs/CODE_COMMENNTS.pdf' file. Each function
   receives a pointer to the tree 'T' and the newly inserted node or terminally
   positioned node from the delete function (the inorder successor if the node
   to be deleted was not unary or a leaf).*/

#include "wavl.h"

void wavl_insert_balance (Tree *T, Node *x)
{
    bool side = 0;
    Node *y = NULL, *sibling = NULL;

    side = (x == x->p->child[L]);
    if (x->p->child[side] != T->sentinel) {                       // Case 0
        x->rd++;
    }

    while (x->p != T->head && x->rd == 0) {                       // VIOLATION
        sibling = return_sibling (x);
        if (sibling->rd == 1) {                                   // Case 1
            promote (T, x->p);
            T->sentinel->rd = 2;
            x = x->p;
        }
        else if (sibling->rd == 2) {
            side = (x == x->p->child[R]);
            y = x->child[!side];
            if (y->rd == 2) {                                     // Case 2
                rotate_balance(x);
                rotate (T, x->p, !side);
                demote (T, x->child[!side]);
            }
            else if (y->rd == 1) {                                // Case 3
                rotate_balance(y);
                rotate (T, x, side);
                rotate_balance(y);
                rotate (T, y->p, !side);
                demote (T, y->child[!side]);

            }
        }
    }
    T->sentinel->rd = 0;
}

void wavl_delete_balance (Tree *T, Node *x)
{
    bool side;
    Node *sibling = NULL;

    while (x->rd == 3) {                                          // VIOLATION
        sibling = return_sibling (x);
        if (sibling->rd == 2) {                                   // Case 1
            demote (T,x->p);
            x = x->p;
        }
        else if (sibling->rd == 1) {                              // Case 1a
            if (children (sibling, 2, 2)) {
                demote (T, sibling);
                demote (T, x->p);
                x = x->p;
            }
            else {
                side = (x == x->p->child[R]);
                if (sibling->child[!side]->rd == 1) {             // Case 2
                    rotate_balance(sibling);
                    rotate (T, x->p, side);
                }
                else if (sibling->child[!side]->rd == 2 ||
                         sibling->child[!side] == T->sentinel) {  // Case 3
                    rotate_balance(sibling->child[side]);
                    rotate (T, sibling, !side);
                    rotate_balance(x->p->child[!side]);
                    rotate (T, x->p, side);
                    demote (T, x->p);
                }
            }
        }
    }
}
