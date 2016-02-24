/* This file contains the set of core tree search and update functions which
 * will be requested by the user supplied data input, compare/copy functions
 * and the user chosen balancing functions. In effect, this file acts as a
 * library of general functions for the rank-balanced binary tree
 * implementations of the AVL, WAVL, 2-3 and 2-3-4 trees described in the
 * 'src/balancing' directory. */

#include "tree.h"

/******************** Creation functions ******************/

Tree *tree_init (comp_fun *compare, copy_fun *copy, ins_bal_fun *ins_bal,
                 del_bal_fun *del_bal)
{
    Tree *tree;

    assert (compare != NULL);
    assert (ins_bal != NULL);
    assert (del_bal != NULL);

    tree = (Tree*) malloc(sizeof(Tree));
    if (tree == NULL) {
        printf("Malloc failed creating a new tree\n");
        exit(1);
    }
    tree->head = create_node ();
    tree->sentinel = create_node ();

    tree->compare = compare;
    tree->copy = copy;
    tree->ins_bal = ins_bal;
    tree->del_bal = del_bal;

    tree->head->child[L] = NULL;
    tree->head->child[R] = tree->sentinel;

    tree->sentinel->child[L] = tree->sentinel;
    tree->sentinel->child[R] = tree->sentinel;
    tree->sentinel->p = tree->sentinel;

    return tree;
}

Node *create_node ()
{
    Node *new_node = (Node*) malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Malloc failed creating new node\n");
        exit(1);
    }
    new_node->rd = 0;
    new_node->key = NULL;
    new_node->p = NULL;
    new_node->child[L] = NULL;
    new_node->child[R] = NULL;
    return new_node;
}

void free_node (Node *del)
{
    free (del->key);
    free (del);
}

/******************** Helper functions *******************/

bool is_leaf (Tree *T, Node *x)
{
    return (x->child[L] == T->sentinel && x->child[R] == T->sentinel);
}

// Returns true if the children match queried rank differences
bool children (Node *x, char a, char b)
{
    if ((x->child[L]->rd == a && x->child[R]->rd == b) ||
        (x->child[L]->rd == b && x->child[R]->rd == a)) {
        return true;
    }
    else {
        return false;
    }
}

Node *return_sibling (Node *x)
{
    //return (T->compare (x->key, x->p->key)) ? x->p->child[L] : x->p->child[R];
    return ((x == x->p->child[L])) ? x->p->child[R] : x->p->child[L];
}

/******************* Balancing functions ******************/

void promote (Tree *T, Node *x)
{
    if (x->child[L] != T->sentinel) {
        x->child[L]->rd++;
    }
    if (x->child[R] != T->sentinel) {
        x->child[R]->rd++;
    }
    if (x->p != T->head) {
        x->rd--;
    }
}

void demote (Tree *T, Node *x)
{
    if (x->child[L] != T->sentinel) {
        x->child[L]->rd--;
    }
    if (x->child[R] != T->sentinel) {
        x->child[R]->rd--;
    }
    if (x->p != T->head) {
        x->rd++;
    }
}

void rotate_balance (Node *x)
{
    bool side = (x == x->p->child[R]);
    Node *sibling = return_sibling (x);
    x->child[side]->rd += x->rd;
    sibling->rd -= x->rd;
    char tmp = x->rd;
    x->rd = x->p->rd;
    x->p->rd = tmp;
}

void rotate (Tree *T, Node *x, bool side)
{
    Node *y = x->child[!side];
    x->child[!side] = y->child[side];
    if (y->child[side] != T->sentinel) {
        y->child[side]->p = x;
    }
    y->p = x->p;
    if (x->p == T->head) {
        T->head->child[R] = y;
    }
    else if (x == x->p->child[side]) {
        x->p->child[side] = y;
    }
    else {
        x->p->child[!side] = y;
    }
    y->child[side] = x;
    x->p = y;
}

/********************** User functions ********************/

void *find (Tree *T, void *k)
{
    bool side = R;
    Node *probe = search (T, k, &side);

    return probe->child[side]->key;
}

/* Searches for node with key to insert, if not found in sentinel the node is
    already in the tree so return pointer to item in tree. Otherwise, if
    found in sentinel, create new node, insert key into new node and replace
    sentinel node in the relevant location returned by search */
void *insert_control (Tree *T, void *k)
{
    bool side = R;
    Node *probe =  search (T, k, &side);

    if (probe->child[side] == T->sentinel) {
        Node *new_node = create_node ();
        Node *inserted_node = insert (T, probe, new_node, k, side);
        if (inserted_node->p != T->head) {
            T->sentinel->rd = 1;
            T->ins_bal (T, inserted_node);
        }
        return inserted_node->key;
    }
    return NULL;
}

/* Searches for the node with the key to delete. If node was in sentinel then
    node was not in tree and function returns NULL. Otherwise, function calls
    delete function with the tree node found */
void *delete_control (Tree *T, void *k)
{
    bool side = R;
    Node *probe = search (T, k, &side);

    if (probe->child[side] != T->sentinel) {
        Node *discard = delete (T, probe->child[side], probe, side);
        if (discard == NULL) {
            fprintf (stderr, "Panic!");
            exit(1);
        }
        free_node (discard);
        T->sentinel->p = NULL;
        return k;
    }
    return NULL;
}

/*************** Search and update functions **************/

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

Node *minimum (Tree *T, Node *min)
{
    while (min->child[L] != T->sentinel) {
        min = min->child[L];
    }
    return min;
}

Node *insert (Tree *T, Node *probe, Node* new, void *k, bool side)
{
    probe->child[side] = new;
    new->p = probe;
    T->copy (new, k);
    new->child[L] = T->sentinel;
    new->child[R] = T->sentinel;
    return new;
}

/* Instead of simply replacing the internal nodes data and key fields with those
   of the inorder successor, the actual inorder successor node is transferred in
   case there are any secondary structures attached to it. */
Node *replace (Tree *T, Node *parent, Node *old, Node *new, bool p_side)
{
    Node *tmp = create_node();
    tmp->rd = new->rd;
    tmp->child[R] = new->child[R];
    tmp->child[L] = T->sentinel;
    if (new->child[R] != T->sentinel) {
        new->child[R]->p = tmp;
    }
    tmp->p = new->p;

    bool side = (new == new->p->child[R]);
    new->p->child[side] = tmp;
    new->rd = old->rd;
    new->child[R] = old->child[R];
    new->child[L] = old->child[L];
    new->p = parent;

    old->child[L]->p = new;
    old->child[R]->p = new;
    parent->child[p_side] = new;

    free_node (old);

    return tmp;
}

/* If node was internal, replaces it with inorder successor then removes it.
    Otherwise, node already had sentinel as one or both children and is
    deleted according to one of the cases shown below. The various cases deal
    with setting appropriate ranks for the deleted node which simulates what
    the rank difference of a missing node would be*/
Node *delete (Tree *T, Node *old, Node *parent, bool p_side)
{
    if (old->child[L] != T->sentinel &&
        old->child[R] != T->sentinel) {                    // node is internal
        Node *del = minimum (T, old->child[R]);
        old = replace (T, parent, old, del, p_side);
    }
    T->sentinel->rd = 0;
    bool side = (old->child[R] != T->sentinel);
    Node *op = old->p;
    p_side = (old == old->p->child[R]);

    if (is_leaf (T, old)) {                                // node is a leaf
        Node *old_sibling = return_sibling (old);
        if (old_sibling == T->sentinel) {                  // without a sibling
            op->rd += old->rd;
            op->child[p_side] = old->child[side];
            T->del_bal (T, op);
        }
        else {                                             // with a sibling
            old->rd++;
            T->del_bal (T, old);
            op->child[p_side] = old->child[side];
        }
    }
    else {                                                 // node is unary
        op->child[p_side] = old->child[side];
        old->child[side]->p = op;
        op->child[p_side]->rd += old->rd;
        T->del_bal (T, op->child[p_side]);
    }
    return old;
}
