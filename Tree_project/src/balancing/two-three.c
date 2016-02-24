/* This file contains the functions to restore balance to an 2-3-4 the tree
   after the insertion or deletion of a node. 'x' denotes the current node, 'p'
   its parent and 'g' its grandparent, 'rd' is the rank-difference. These cases
   are illustrated in the 'docs/CODE_COMMENNTS.pdf' file. Each function receives
   a pointer to the tree 'T' and the newly inserted node or terminally
   positioned node from the delete function (the inorder successor if the node
   to be deleted was not unary or a leaf). */

#include "two-three.h"

void two_three_insert_balance (Tree *T, Node *x)
{
    Node *sibling, *g; g = NULL;
    bool side, p_side; side = p_side = 0;

    while (x->rd == 0 && x->p != T->head) {                         // VIOLATION
        g = x->p->p;
        sibling = return_sibling (x);
        if ((x->p->rd == 1 || g == T->head) && sibling->rd == 0) {  // Case 1
                promote (T, x->p);
                x = x->p;
        }
        else if (x->p->rd == 0 && g != T->head) {
            side = (x == x->p->child[R]);
            p_side = (x->p == g->child[R]);
            if (side != p_side) {                                   // Case 3
                rotate_balance(x);
                rotate (T, x->p, p_side);
            }
            rotate_balance(g->child[p_side]);                       // Case 2
            rotate (T, g, !p_side);
            promote (T, g->p);
            x = g->p;
        }
        else {                                                      // Balanced
            return; // or x = x->p
        }
    }
}

void two_three_delete_balance (Tree *T, Node *x)
{
    Node *sibling = NULL;
    bool side = 0;

    while (x->rd == 2) {                                        // VIOLATION
        side = (x == x->p->child[R]);
        sibling = return_sibling (x);
        if (sibling->rd == 1) {
            if (sibling->child[!side]->rd == 0) {               // Case 1a
                rotate_balance(sibling);
                rotate (T, x->p, side);
            }
            else if (sibling->child[side]->rd == 0) {           // Case 1b
                rotate_balance(sibling->child[side]);
                rotate (T, sibling, !side);
                rotate_balance(x->p->child[!side]);
                rotate (T, x->p, side);
            }
            else if (sibling->child[side]->rd == 1 &&
                     sibling->child[!side]->rd == 1) {          // Case 2
                demote (T, x->p);
                x = x->p;
            }
        }
        else if (sibling->rd == 0) {                            // Case 3
            rotate_balance(sibling);
            rotate (T, x->p, side);
            T->sentinel->rd++;
        }
        else {                                                  // Balanced
            T->sentinel->rd = 0;
            return;
        }
    }
}
