#include "lab2.h"

extern NODE *root, *cwd;
extern char line[128];
extern char command[16], pathname[64];
extern char dname[64], bname[64];

void initialize() {
    root = (NODE *)malloc(sizeof(NODE));
    root->parentPtr = root->siblingPtr = root;
    root->childPtr = NULL;
    strcpy(root->name, "/");
    root->node_type = 'D';
    cwd = root;
}

void setup_dbname(char *pathname) {
    char temp[128];
    strcpy(temp, pathname);
    strcpy(dname, dirname(temp));
    strcpy(temp, pathname);
    strcpy(bname, basename(temp));
}

NODE *new_node(char *name, char type) {
    NODE *node = (NODE *)malloc(sizeof(NODE));
    strcpy(node->name, name);
    node->node_type = type;
    node->childPtr = node->siblingPtr = NULL;
    return node;
}

// prerequisites: bname and dname are populated, type is known
int insert_node(char *pathname, char type) {
    setup_dbname(pathname); // populates dname and bname with expected strings
    NODE *current_path;
    if (dname != NULL && dname[0] == '/') { // absolute path
        strcpy(dname, dname+1);
        current_path = root;
    } else { // relative path
        current_path = cwd;
    }

    // if current path already exists w/ same dname and dname exists
    if (!(check_path_dne(pathname)) && dname != NULL) {
        printf("nah fam\n");
        return FAILURE;
    }

    NODE *node = new_node(bname, type);
    node->parentPtr = current_path; // parent ptr set prior to traversal

    if(!current_path->childPtr) { // if current path does not yet have a child
        current_path->childPtr = node;
    } else {
        current_path = current_path->childPtr;
        while(current_path->siblingPtr != NULL) { // traverse to end of current directory
            current_path = current_path->siblingPtr;
        }
        current_path->siblingPtr = node; // set new node to end of current directory
    }
    return SUCCESS;
}

NODE *find_node_in_children(NODE *node, char *name) {
    NODE *ptr = node->childPtr;
    while (ptr) {
        if (!strcmp(ptr->name, name)) {
            return ptr;
        }
        if(!ptr)    
            break;
        ptr = ptr->siblingPtr;
    }
    return NULL;
}

// precondition: bname has been filled correctly
int check_path_dne(char *pathname) {
    NODE *ptr = root;
    int num_levels = tokenize(pathname);
    for (int i = 0; i < num_levels; i++) {
        ptr = find_node_in_children(ptr, bname);
        if (!ptr)
            return 1;
    }
    return 0;
}

int search(NODE *node, char *dirname) {
    char *search = strtok(dirname, "/");
    while (search) {
        //printf("%s ", search);
        if (!strcmp(bname, search)) {
            printf("found bname in tree\n");
            return SUCCESS;
        }
        if (node->childPtr == NULL) {

        } else if (node->siblingPtr != NULL) {
            node = node->siblingPtr;
        }
        search = strtok(NULL, "/");
    }
    return FAILURE;
}

NODE *go_down_path(char *dirname) {
    NODE *start;
    char *search = strtok(dirname, "/");
    int n_tokens = tokenize(dirname);
    if (dirname != NULL && dirname[0] == '/') {
        start = root;
    } else {
        start = cwd;
    }
    while (search) {
        // if tokenized directory name is same as sibling ptr name and node is a directory
        if (!strcmp(start->siblingPtr->name, search) && start->siblingPtr->node_type == 'D') {
            return start->siblingPtr;
        }
    }
    return NULL;
}

void print_tree(NODE *root) {
    if (root == NULL) {
        printf("should be null\n");
        return;
    }
    print_tree(root->childPtr); // right
    printf("%s\t", root->name);
    getchar();
    print_tree(root->parentPtr); // left
}