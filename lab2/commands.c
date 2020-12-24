#include "lab2.h"

// External globals
extern NODE *root, *cwd, *start;
extern char command[16], pathname[64];

extern char pwd_string[64];
extern char gpath[128]; // global gpath[] to hold token strings
extern char *name[64];  // token string pointers
extern int n;           // number of token strings

extern char dname[64], bname[64]; // dirname, basename of pathname

/***************************************************************
    mkdir ("make directory")
****************************************************************/

int mkdir(char *pathname) {
    dir_base_name(pathname);
    NODE *p = NULL;
    if (!strcmp(dname, ".")) {
        p = cwd;
    } else {
        p = path2node(dname);
    }

    if (!strcmp(bname, "..") || !strcmp(bname, "/") || !strcmp(bname, ".")) {
        printf("mkdir: %s: File exists\n", bname);
        return FAILURE;
    }

    if (!p || p->type == 'F') {
        printf("mkdir: %s: no such file or directory\n", dname);
        return FAILURE;
    }
    if (!search_child(p, bname)) {
        NODE *q = (NODE *)malloc(sizeof(NODE));
        q->type = 'D';
        strcpy(q->name, bname);
        insert_child(p, q);
        return SUCCESS;
    }
    printf("mkdir: %s: File exists\n", bname);
    return FAILURE;
}

/***************************************************************
    rmdir ("remove directory")
****************************************************************/

int rmdir(char *pathname) {
    NODE *p = path2node(pathname);
    if (p == NULL) {
        printf("rmdir: %s: No such file or directory\n", pathname);
        return FAILURE;
    }
        
    if (p->type == 'D' && is_empty(p)) {
        delete_node(p);
        return SUCCESS;
    }
    printf("rmdir: %s: Is not a directory or is not empty\n", pathname);
    return FAILURE;
}

/***************************************************************
    ls ("list directory")
****************************************************************/

// This ls() list CWD. You MUST improve it to ls(char *pathname)
// note edge case: when making a directory w/ multiple levels, pathname is not reset properly
// so ls without a proper pathname (ls in cwd) will not work correctly in that case
int ls(char *pathname)
{
    dir_base_name(pathname);

    NODE *p = NULL;
    if (!strcmp(bname, ".")) {
        p = cwd->child;
    } else {
        p = path2node(pathname);
        p = p->child;
    }

    printf("cwd contents = ");
    while (p)
    {
        printf("[%c %s] ", p->type, p->name);
        p = p->sibling;
    }
    printf("\n");
}

/***************************************************************
    cd ("change directory")
****************************************************************/

int cd(char *pathname) {
    // change cwd to root
    if (!strcmp(pathname, "/")) {
        cwd = root;
        return SUCCESS;
    }

    // back up pathname
    char backup[128];
    strcpy(backup, pathname);

    // move back levels
    int num = tokenize(pathname);
    while (num > 0 && !strcmp(name[num-1], "..")) {
        if (cwd != root) {
            cwd = cwd->parent;
        }
        num--;
    }

    if (num == 0)
        return SUCCESS;
    
    strcpy(pathname, backup); // restore pathname
    // change cwd to pathname
    NODE *p = path2node(pathname);
    if (p != NULL && p->type == 'D') {
        cwd = p;
        return SUCCESS;
    }
    printf("cd: no such file or directory: %s\n", pathname);
    return FAILURE;
}

/***************************************************************
    pwd ("print working directory")
****************************************************************/

int pwd() {
    // actual recursive definition also found below-- pwd needed to set up cwd
    NODE *p = cwd;
    strcpy(pwd_string, "/");
    pwd_recursive(p);
    printf("cwd = %s\n", pwd_string);
    return SUCCESS;
}

int pwd_recursive(NODE *p) {
    if (p == root) {
        return SUCCESS;
    }
    pwd_recursive(p->parent);
    strcat(pwd_string, p->name);
    strcat(pwd_string, "/");
}

int pwd_no_stdin() {
    // identical to pwd(), but does not print out pwd_string
    NODE *p = cwd;
    strcpy(pwd_string, "/");
    pwd_recursive(p);
    return SUCCESS;
}

/***************************************************************
    creat ("create file")
****************************************************************/

int creat(char *pathname) {
    dir_base_name(pathname);
    NODE *p = NULL;
    if (!strcmp(dname, ".")) {
        p = cwd;
    } else {
        p = path2node(dname);
    }

    if (!strcmp(bname, "..") || !strcmp(bname, "/") || !strcmp(bname, ".")) {
        printf("creat: %s: File exists\n", bname);
        return FAILURE;
    }

    if (!p || p->type == 'F') {
        printf("creat: %s: no such file or directory\n", dname);
        return FAILURE;
    }
    if (!search_child(p, bname)) {
        NODE *q = (NODE *)malloc(sizeof(NODE));
        q->type = 'F';
        strcpy(q->name, bname);
        insert_child(p, q);
        printf("creat %s OK\n", bname);
        return SUCCESS;
    }
    printf("creat: %s: File exists\n", bname);
    return FAILURE;
}

/***************************************************************
    rm ("remove file")
****************************************************************/

int rm(char *pathname) {
    NODE *p = path2node(pathname);
    if (p == NULL) {
        printf("rm: %s: No such file or directory\n", pathname);
        return FAILURE;
    }
    if (p->type == 'F') {
        delete_node(p);
        return SUCCESS;
    }
    printf("rm: %s: Not a file\n", pathname);
    return FAILURE;
}

/***************************************************************
    save ("save file tree")
****************************************************************/

int save(char *filename) {
    FILE *fp = fopen(filename, "w+");
    NODE *cwd_backup = cwd;
    if (fp != NULL) {
        NODE *p = root->child;
        cwd = p;
        print(fp, p);
        fclose(fp);
        cwd = cwd_backup;
        return SUCCESS;
    }
    printf("save: error opening file for saving\n");
    return FAILURE;
}

void print(FILE *fp, NODE *p) {
    if (p == NULL)
        return;
    cwd = p;
    pwd_no_stdin();
    fprintf(fp, "%c %s\n", p->type, pwd_string);
    cwd = p->child;
    print(fp, p->child); // left
    cwd = p->sibling;
    print(fp, p->sibling); // right
}

/***************************************************************
    reload ("reload file tree")
****************************************************************/

int reload(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        delete_tree(root);
        initialize();
        while (!feof(fp)) {
            char node_type = '\0';
            fscanf(fp, "%c %s\n", &node_type, pathname);
            printf("node_type: %c\n", node_type);
            if (node_type == 'D') {
                mkdir(pathname);
            } else if (node_type == 'F') {
                creat(pathname);
            }
        }
        strcpy(pathname, ""); // reset pathname
        return SUCCESS;
    }
    printf("reload: %s: tree file does not exist\n", filename);
    return FAILURE;
}

/***************************************************************
    menu ("show possible commands")
****************************************************************/

int menu(char *pathname) {
    printf("\e[1mCommand:\tUsage:\t\tDescription:\e[0m\n");
    printf("mkdir\t\tmkdir <path>\tMakes a new directory for a given pathname\n");
    printf("rmdir\t\trmdir <path>\tRemoves a directory given that it is empty\n");
    printf("cd\t\tcd <path>\tChanges current working directory to pathname, or to / if path is not provided\n");
    printf("ls\t\tls <path>\tList the directory contents of pathname, or of current working directory\n");
    printf("pwd\t\tpwd\t\tPrints working directory, or absolute pathname of current working directory.\n");
    printf("creat\t\tcreat <path>\tCreates a FILE node at a given path\n");
    printf("rm\t\trm <path>\tRemoves a FILE node at a given path\n");
    printf("save\t\tsave <file>\tSaves the current file system tree as a file\n");
    printf("reload\t\treload <file>\tConstructs a file system tree from a file\n");
    printf("menu\t\tmenu\t\tShows a list of valid commands \e[1m(this menu)\e[0m\n");
    printf("quit\t\tquit\t\tSaves the file system tree and terminates the program.\n");
}

/***************************************************************
    quit ("quit application")
****************************************************************/

int quit()
{
    printf("Saved tree to 'saved_tree' successfully\n");
    save("saved_tree");
    delete_tree(root);
    exit(0);

    // improve quit() to SAVE the current tree as a Linux file
    // for reload the file to reconstruct the original tree
}