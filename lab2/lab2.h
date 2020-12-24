#ifndef __LAB_2_H__
#define __LAB_2_H__

#include <stdio.h>  // for I/O
#include <stdlib.h> // for lib functions
#include <libgen.h> // for dirname()/basename()
#include <string.h>

#define TRUE    1
#define FALSE   0
#define SUCCESS 1
#define FAILURE 0

typedef struct node
{
    char name[64]; // node's name string
    char type;
    struct node *child, *sibling, *parent;
} NODE;

// Function prototypes:

// COMMANDS
int mkdir(char *pathname);
int rmdir(char *pathname);
int cd(char *pathname);
int ls(char *pathname);
int pwd();
int pwd_recursive(NODE *p);
int pwd_no_stdin();
int creat(char *pathname);
int rm(char *pathname);
int save(char *filename);
void print(FILE *fp, NODE *p);
int reload(char *filename);
int menu(char *pathname);
int quit();

// HELPER
int initialize(void);
NODE *search_child(NODE *parent, char *name);
int insert_child(NODE *parent, NODE *q);
int tokenize(char *pathname);
NODE *path2node(char *pathname);
int dir_base_name(char *pathname);
int is_empty(NODE *parent);
int delete_node(NODE *node);
int delete_tree(NODE *p);

// MISC.
int findCmd(char *command);

#endif /* __LAB_2_H__ */