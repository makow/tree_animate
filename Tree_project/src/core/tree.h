#ifndef FILE_TREE_H
#define FILE_TREE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define L 0
#define R 1
#define CHILDREN 2
#define SAME -1

/* Balance and compare function pointer to facilitate swapping in
different balancing algorithms that work with this framework */
typedef unsigned char RD;
struct tree;
struct node;
typedef int comp_fun (const void *a, const void *b);
typedef void copy_fun (struct node *n, void *src);
typedef void ins_bal_fun (struct tree *T, struct node *x);
typedef void del_bal_fun (struct tree *T, struct node *x);

/* Contents of each tree, nodes lie between head & sentinel */
typedef struct tree {
    struct node *head;
    struct node *sentinel;
    comp_fun *compare;
    copy_fun *copy;
    ins_bal_fun *ins_bal;
    del_bal_fun *del_bal;
}Tree;

/* Contents of each tree node including head & sentinel */
typedef struct node {
    struct node *child[CHILDREN];
    struct node *p;
    void *key;
    RD rd;
}Node;

/* Helper functions */
bool is_leaf (Tree *T, Node *x);
bool children (Node *x, char a, char b);
Node *return_sibling (Node *x);

/* Balancing functions */
void promote (Tree *T, Node *x);
void demote (Tree *T, Node *x);
void rotate_balance (Node *x);
void rotate (Tree *T, Node *x, bool side);

/* User accessible functions*/
void *find (Tree *T, void *k);
void *insert_control (Tree *T, void *k);
void *delete_control (Tree *T, void *k);

/* Search and update functions */
Tree *tree_init (comp_fun *, copy_fun *, ins_bal_fun *, del_bal_fun *);
Node *search (Tree *T, void *k, bool *side);
Node *create_node ();
void free_node (Node *del);
Node *minimum (Tree *T, Node *min);
Node *insert (Tree *T, Node *probe, Node *new, void *k, bool side);
Node *replace (Tree *t, Node *parent, Node *old, Node *new, bool p_side);
Node *delete(Tree *T, Node *old, Node *parent, bool p_side);

#endif
