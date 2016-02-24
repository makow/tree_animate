#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include "../core/tree.h"
#include "../compare/compare.h"
#include "../balancing/avl.h"
#include "../balancing/wavl.h"
#include "../balancing/two-three.h"
#include "../balancing/two-four.h"

#define INSTRUCTION 2
#define LEVEL 0
#define MAX_LEVEL 16
#define MAX_ITEMS 1024
#define MAX_LENGTH 1024
#define strsame(A, B) (strcmp(A, B) == 0)
#define ERROR(PHRASE) {fprintf(stderr, "Error '%s' occurred in %s, code line %d\n of the program. \n", PHRASE, __FILE__, __LINE__); exit(2);}

/* typedef struct input { */
/*     char line[MAX_LENGTH]; */
/*     int parameter; */
/*     char instruction[2]; */
/* }Input; */

// Program reading functions
void get_args (char *argv[]);
Tree *set_tree (char *argv[]);
void read_data (FILE *fp, FILE *ofp, Tree *T);
/* void free_tree (Tree *T, Node *node); */

// Parsing function
void parse_input (Tree *T, char *token);

// Print function
void print_tree_structure (Tree *T, Node *node, int level, FILE *ofp);

#endif
