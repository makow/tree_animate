/* This file contains the user-supplied data input functions to
 * insert/search/delete values from the tree. These functions must be user
 * supplied because the type of the keys to be inserted or compared must be
 * known to be dereferenced later by the compare and copy user defined
 * functions. */

#include "input.h"

int main(int argc, char *argv[])
{
    if ((argc < 3) || (argc > 4))
    {
        printf("Usage: executable tree_type input_file.txt\n"
               "Tree types are 'avl', 'wavl', '2-3' and '2-3-4',\n");
        exit(1);
    }
    else {
        get_args (argv);
    }

    return EXIT_SUCCESS;
}

void get_args (char *argv[])
{
    FILE *fp, *ofp = NULL;

    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open input file %s!\n",
                "data.txt");
        exit(1);
    }
    ofp = fopen("bin/proto_tree.txt", "w");
    if (ofp == NULL) {
        fprintf(stderr, "Can't open output file %s!\n",
                "proto_tree.txt");
        exit(1);
    }
    Tree *T = set_tree (argv);
    read_data(fp, ofp, T);
    if(fclose(fp) != 0 || fclose(ofp) != 0)
    {
        printf("\nError closing files.\n");
    }
    /* free_tree(T, T->head->child[R]); */
}

Tree *set_tree (char *argv[])
{
    Tree *tptr;
    int type_length = strlen (argv[1]);
    char *tree_type = (char *) malloc ((type_length + 1) * (sizeof(char)));
    tree_type [type_length - 1] = '\0';

    for (int i = 0; i < type_length; i++) {
        tree_type[i] = toupper ((int)argv[1][i]);
    }
    if (strsame (tree_type, "AVL")) {
        tptr =  tree_init(compare_ints, copy_ints, avl_insert_balance,
                          avl_delete_balance);
        printf("\nAVL tree selected.\n");
    }
    else if (strsame (tree_type, "WAVL")) {
        tptr =  tree_init(compare_ints, copy_ints, wavl_insert_balance,
                          wavl_delete_balance);
        printf("\nWAVL tree selected.\n");
    }
    else if (strsame (tree_type, "2-3")) {
        tptr =  tree_init(compare_ints, copy_ints, two_three_insert_balance,
                          two_three_delete_balance);
        printf("\n2-3 tree selected.\n");
    }
    else if (strsame (tree_type, "2-3-4")) {
        tptr =  tree_init(compare_ints, copy_ints, two_four_insert_balance,
                          two_four_delete_balance);
        printf("\n2-3-4 tree selected.\n");
    }
    else {
        fprintf (stderr, "Expecting a tree type as the second argument.\n"
                 "The options are 'avl', 'wavl', '2-3' and '2-3-4',\n");
        tptr =  NULL;
    }
    free (tree_type);
    return tptr;
}

/* Can't have a datum longer than one line or carries over into another line. */
void read_data (FILE *fp, FILE *ofp, Tree *T)
{
    char line[MAX_LENGTH] = {0};
    char *token, *next_token;
    char *delim = " `~!@#$%^&*(){}[]|\\:;.,<>/?\"\n";
    while (fgets (line, sizeof(line), fp) != NULL) {
        token = strtok_r (line, delim, &next_token);
        while (token) {
            parse_input (T, token);
            print_tree_structure (T, T->head->child[R], LEVEL, ofp);
            token = strtok_r (NULL, delim, &next_token);
        }
    }
    free (token);
}

void parse_input (Tree *T, char *token)
{
    void *check = NULL;
    int parameter = atoi(token + 1);
    char instruction[INSTRUCTION];
    instruction[1] = '\0';
    instruction[0] = toupper((int)token[0]);

    if (parameter == 0) {
        ERROR ("A key of integer value zero is not allowed.");
    }

    printf("\nReceived: instruction=%s, parameter=%d.\n",
           instruction,parameter);

    int *vptr = &parameter;

    if (strsame(instruction, "S"))
    {
        check = find (T, vptr);
        if (check) {
            fprintf (stderr, "Node with key %d was found in the tree.\n",
                     *((int*)check));
        }
        else {
            fprintf (stderr,
                     "Node with key %d was NOT found in the tree.\n",
                     parameter);
            check = vptr;
        }
    }
    else if (strsame(instruction, "I"))
    {
        check = insert_control (T, vptr);
        if (check) {
            fprintf (stderr,
                     "Node with key %d successfully inserted in the tree.\n",
                     *((int*)check));
        }
    }
    else if (strsame(instruction, "D"))
    {
        check = delete_control (T, vptr);
        if (check) {
            fprintf (stderr,
                     "Node with key %d successfully deleted from the tree.\n",
                 *((int*)check));
        }
    }
    if (check == NULL)
    {
        fprintf (stderr,
                 "Node with key %d is already in the tree.\n", parameter);
    }
}

/* This function is modified from:
   http://adtinfo.org/libavl.html/Displaying-BST-Structures.html#119
   Prints the structure of node,
   which is "level" levels from the top of the tree. */
void print_tree_structure (Tree *T, Node *node, int level, FILE *ofp)
{
    /* You can set the maximum level as high as you like.
       Most of the time, you'll want to debug code using small trees,
       so that a large level indicates a “loop”, which is a bug. */
    if (level > MAX_LEVEL) {
        fprintf(ofp, "[...]");
        // printf ("[...]");
        return;
    }

    if (node == T->sentinel) {
        return;
    }
    fprintf(ofp, "%d:%u", *((int*)node->key), node->rd);
    // printf ("%d:%u", *((int*)node->key), node->rd);

    if (node->child[L] != T->sentinel || node->child[R] != T->sentinel) {
        fprintf(ofp, "(");
        // putchar ('(');

        print_tree_structure (T, node->child[L], level + 1, ofp);
        if (node->child[R] != T->sentinel) {
            fprintf(ofp, ",");
            // putchar (',');
            print_tree_structure (T, node->child[R], level + 1, ofp);
        }
        fprintf(ofp, ")");
        // putchar (')');
    }
}

/* void free_tree (Tree *T, Node *node) */
/* { */
/*     if (node = T->sentinel) { */

/*     } */

/*     free_node(T->head); */
/*     free_node(T->sentinel); */
/*     free (T); */
/* } */
