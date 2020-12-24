#ifndef __LAB_2_H__
#define __LAB_2_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#define TRUE    1
#define FALSE   0
#define SUCCESS 1
#define FAILURE 0

typedef struct node {
    char name[64];      // name string of the node
    char node_type;     // node type, 'D' for directory or 'F' for file
    struct node *childPtr;     // pointer to child of node
    struct node *siblingPtr;   // pointer to sibling of node
    struct node *parentPtr;    // pointer to parent of node
} NODE;

// Function prototypes
void initialize();
int get_user_input();

void setup_dbname(char *pathname);
NODE *new_node(char *name, char type);
int insert_node(char *pathname, char type);
int search(NODE *node, char *dirname);
int tokenize(char *pathname);
NODE *find_node_in_children(NODE *node, char *name);
int check_path_dne(char *pathname);
void print_tree(NODE *root);

int identify(char *command);
int mkdir(char *pathname);
int rmdir(char *pathname);
int cd(char *pathname);
int ls(char *pathname);
int pwd(char *pathname);
int creat(char *pathname);
int rm(char *pathname);
int save(char *pathname);
int reload(char *pathname);
int menu(char *pathname);

#endif /* LAB_2_H */