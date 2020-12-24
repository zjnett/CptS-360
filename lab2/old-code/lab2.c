#include "lab2.h"

// Global variables
NODE *root, *cwd;
char line[128];
char command[16], pathname[64];
char dname[64], bname[64];

// Command table
char *cmd[] = { "mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload", "save", "menu", "quit", "exit", NULL };

// Function pointers table
int (*fptr[])(char *) = { (int (*)())mkdir, rmdir, ls, cd, pwd, creat, rm, reload, save, menu };

int main(int argc, char *argv[]) {
    int index, running = 1;
    initialize();
    while (running) {
        // get user input line = [command pathname]
        printf("input a command line: ");
        get_user_input();
        // identify the command
        index = identify(command);
        // execute the command
        if (index >= 0 && index <= 9) {
            fptr[index](pathname);  // use function pointers table to execute command
        } else if (index == 10 || index == 11) {
            running = 0;
        } else {
            printf("invalid command %s\n", command);
        }
    }
}

int get_user_input() {
    fgets(line, 128, stdin);
    line[strlen(line)-1] = 0;
    sscanf(line, "%s %s", command, pathname);
}

int identify(char *command) {
    int i = 0;
    while (cmd[i]) {
        if (!strcmp(command, cmd[i]))
            return i;
        i++;
    }
    return -1;
}

int tokenize(char *pathname) {
    int count = 0;
    char *s;
    s = strtok(pathname, "/");

    if (!s)
        return 0;

    while (s) {
        //printf("%s", s);
        count++;
        s = strtok(0, "/");
    }
    return count;
}

int mkdir(char *pathname) {
    return insert_node(pathname, 'D');
}

int rmdir(char *pathname) {
    printf("rmdir\n");
}

int cd(char *pathname) {
    printf("cd\n");
}

int ls(char *pathname) {
    setup_dbname(pathname);
    print_tree(root);
    printf("\n");
}

int pwd(char *pathname) {
    printf("pwd\n");
}

int creat(char *pathname) {
    return insert_node(pathname, 'F');
}

int rm(char *pathname) {
    printf("rm\n");
}

int save(char *pathname) {
    printf("save\n");
}

int reload(char *pathname) {
    printf("reload\n");
}

int menu(char *pathname) {
    printf("menu\n");
}