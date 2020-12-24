
#include "lab2.h"

// External globals
extern NODE *root, *cwd, *start;
extern char command[16], pathname[64];

extern char pwd_string[64];
extern char gpath[128]; // global gpath[] to hold token strings
extern char *name[64];  // token string pointers
extern int n;           // number of token strings

extern char dname[64], bname[64]; // dirname, basename of pathname

int initialize() // create / node, set root and cwd pointers
{
    root = (NODE *)malloc(sizeof(NODE));
    strcpy(root->name, "/");
    root->parent = root;
    root->sibling = 0;
    root->child = 0;
    root->type = 'D';
    cwd = root;
    printf("Root initialized OK\n");
}

NODE *search_child(NODE *parent, char *name)
{
    NODE *p;
    p = parent->child;
    if (p == 0)
        return 0;
    while (p)
    {
        if (strcmp(p->name, name) == 0)
            return p;
        p = p->sibling;
    }
    return 0;
}

int insert_child(NODE *parent, NODE *q)
{
    NODE *p;
    p = parent->child;
    if (p == 0)
        parent->child = q;
    else
    {
        while (p->sibling)
            p = p->sibling;
        p->sibling = q;
    }
    q->parent = parent;
    q->child = 0;
    q->sibling = 0;
}

int tokenize(char *pathname)
{
    n = 0;   // initial # of token strings
    char *s; // temporary char * ptr buffer to hold string
    s = strtok(pathname, "/");

    if (!s)
        return 0;

    while (s)
    {
        name[n] = s;
        n++;
        s = strtok(0, "/");
    }
    return n;
}

NODE *path2node(char *pathname)
{
    // return pointer to the node of pathname, or NULL if invalid
    if (pathname[0] == '/')
        start = root;
    else
        start = cwd;

    tokenize(pathname);

    NODE *node = start;

    for (int i = 0; i < n; i++)
    {
        node = search_child(node, name[i]);
        if (node == NULL) // if search fails, return NULL
            return NULL;
    }
    return node;
}

int dir_base_name(char *pathname)
{
    char temp[128];
    strcpy(temp, pathname);
    strcpy(dname, dirname(temp));
    strcpy(temp, pathname);
    strcpy(bname, basename(temp));
    return SUCCESS;
}

int is_empty(NODE *parent) {
    if (parent->child == NULL) {
        return TRUE;
    }
    return FALSE;
}

int delete_node(NODE *node) {
    NODE *head = node->parent->child;

    if (head == node) { // node is first in linked list

        if(head == root->child)
            root->child = node->sibling;
        else if (head == cwd->child)
            cwd->child = node->sibling;

        head = node->sibling;
        free(node);
        return SUCCESS;

    } else if (node->sibling == NULL) { // node is last in linked list

        // traverse to node before node to delete
        while(head->sibling != node) {
            head = head->sibling;
        }

        head->sibling = NULL;
        free(node);
        return SUCCESS;

    } else { // node is somewhere in the middle

        // traverse to node before node to delete
        while(head->sibling != node) {
            head = head->sibling;
        }

        head->sibling = node->sibling;
        free(node);
        return SUCCESS;

    }

    return FAILURE;
}

int delete_tree(NODE *p) {
    if (p == NULL) {
        return SUCCESS;
    }
    delete_tree(p->child);
    delete_tree(p->sibling);
    free(p);
}