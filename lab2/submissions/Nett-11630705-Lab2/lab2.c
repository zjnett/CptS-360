#include "lab2.h"

// Global variables
NODE *root, *cwd, *start;
char command[16], pathname[64];

char pwd_string[64];
char gpath[128]; // global gpath[] to hold token strings
char *name[64];  // token string pointers
int n;           // number of token strings

char dname[64], bname[64]; // dirname, basename of pathname

// Command table
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload", "save", "menu", "help", "?", "quit", "exit"};

// Function pointers table
int (*fptr[])(char *) = {(int (*)())mkdir, rmdir, ls, cd, pwd, creat, rm, reload, save, menu, menu, menu, quit, quit};

int main()
{
    int index;
    char line[128];

    initialize();

    printf("Type '?', 'help', or 'menu' for commands.\n");

    while (1)
    {
        pwd_no_stdin();
        printf("➜  %s: ", pwd_string);
        fgets(line, 128, stdin);
        line[strlen(line) - 1] = 0;

        sscanf(line, "%s %s", command, pathname);
        printf("command=%s pathname=%s\n", command, pathname);

        if (command[0] == 0)
            continue;

        index = findCmd(command);

        if (index != -1)
        {
            fptr[index](pathname); // use function pointers table to execute command
        }
        else
        {
            printf("invalid command %s\n", command);
        }
        strcpy(pathname, ""); // clear out pathname after every command
    }
}

int findCmd(char *command)
{
    int i = 0;
    while (cmd[i])
    {
        if (strcmp(command, cmd[i]) == 0)
            return i;
        i++;
    }
    return -1;
}