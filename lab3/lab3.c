#include "lab3.h"

extern char gdir[MAX]; // gdir[ ] stores dir strings
extern char *dir[128];
extern int ndir;

extern char gpath[MAX]; // gpath[ ] stores token strings
extern char *name[64];
extern int ntoken;

extern char home[MAX]; // home[ ] stores home (~) directory from env[]

extern int pd[2];
extern int lpd[2];

extern char *head, *tail;

extern char aliasList[MAX][MAX][MAX];

int do_pipe(char *cmdLine, int *pd) {
    if (pd) {
        close(pd[0]);
        dup2(pd[1], 1);
        close(pd[1]);
    }
    int hasPipe = scan(cmdLine);
    if (hasPipe) {
        pipe(lpd); // create pipe lpd
        int pid = fork();
        if (pid) { // parent
            // as READER on lpd
            close(lpd[1]);
            dup2(lpd[0], 0);
            close(lpd[0]);
            do_command(tail);
        }
        else
        {
            do_pipe(head, lpd);
        }
        
    }
    else
    {
        do_command(cmdLine);
    }
}

int do_command(char *cmdLine) {

    memset(name, 0, sizeof(name)); // memset name, it's about to be reset

    char line[MAX];
    strcpy(line, cmdLine);
    tokenize_cli(line);

    process_aliases(gdir);

    // scan commandLine for redirection
    for (int i = 0; i < ntoken; i++) {
        if (!strcmp(name[i], "<")) {
            // take inputs from infile (exploit stdin)
            close(0);
            int fd = open(name[i+1], O_RDONLY);
            name[i] = NULL;
        } else if (!strcmp(name[i], ">")) {
            // send outputs to outfile (exploit stdout)
            close(1);
            int fd = open(name[i+1], O_WRONLY|O_CREAT, 0644);
            name[i] = NULL;
        } else if (!strcmp(name[i], ">>")) {
            // append outputs to outfile (exploit stdout, but append)
            close(1);
            int fd = open(name[i+1], O_WRONLY|O_CREAT|O_APPEND, 0644);
            name[i] = NULL;
        }
    }

    char cmd[128];
    // if cmd starts with ./
    if (!strncmp(name[0], "./", 2)) {
        getcwd(cmd, sizeof(cmd));
        strcat(cmd, "/");
        strcat(cmd, name[0]);
        execve(cmd, name, __environ);
    }

    for (int i = 0; i < ndir; i++)
    {
        strcpy(cmd, dir[i]);
        strcat(cmd, "/");
        strcat(cmd, name[0]);

        #ifdef __DEBUG__
        printf("TRY PATH i=%d cmd=%s\n", i, cmd);
        #endif

        execve(cmd, name, __environ);
    }
    #ifdef __DEBUG__
    printf("cmd %s not found, child sh exit\n", name[0]);
    #else
    printf("znsh: command not found: %s\n", name[0]);
    #endif

    exit(123); // die with value 123
}

int scan(char *cmdLine) {
    //printf("len : %d, line: %s \n", strlen(cmdLine), cmdLine);
    for (int i = strlen(cmdLine); i >= 0; i--) {
        if (cmdLine[i] == '|') { // found rightmost pipe
            cmdLine[i] = '\0';
            head = cmdLine;
            tail = (cmdLine + i + 1);
            return 1;
        }
    }
    head = cmdLine;
    tail = NULL;
    return 0;
}

int tokenize_PATH(char *PATH)
{
    strcpy(gdir, PATH); // ensure backup PATH to gdir
    ndir = 0;   // initial # of token strings
    char *s; // temporary char * ptr buffer to hold string
    s = strtok(PATH, ":");

    if (!s)
        return 0;

    while (s)
    {
        dir[ndir] = s;
        ndir++;
        s = strtok(0, ":");
    }
    return ndir;
}

int tokenize_cli(char *line)
{
    strcpy(gpath, line); // ensure backup line to gpath
    ntoken = 0;   // initial # of token strings
    char *s; // temporary char * ptr buffer to hold string
    s = strtok(line, " ");

    if (!s)
        return 0;

    while (s)
    {
        name[ntoken] = s;
        ntoken++;
        s = strtok(0, " ");
    }
    return ntoken;
}

void populate_alias_list(void) {
    // TODO: constant macros or hash function
    // alias IDs could be read from .znshrc or something similar (emulate sh shells better)
    // but, hardcoding works for now

    // initialize aliasList w/ 0's
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            strcpy(aliasList[i][j], "");
        }
    }

    // alias=ls, ID 0
    strcpy(aliasList[0][0], "--color=auto");
    // alias=grep, ID 1
    strcpy(aliasList[1][0], "--color=auto");

    // alias=ll, ID 2
    strcpy(aliasList[2][0], "-a");
    strcpy(aliasList[2][1], "-l");
    strcpy(aliasList[2][2], "-F");
    strcpy(aliasList[2][3], "--color=auto");

    // alias=la, ID 3
    strcpy(aliasList[3][0], "-A");
    strcpy(aliasList[3][1], "--color=auto");

    // alias=l, ID 4
    strcpy(aliasList[4][0], "-C");
    strcpy(aliasList[4][1], "-F");
    strcpy(aliasList[4][2], "--color=auto");

    // alias=dir, ID 5
    strcpy(aliasList[5][0], "--color=auto");
}

void process_aliases(char *cmdLine) {
    // assumes cmdLine has been tokenized into name[]
    // else-if ladder to convert string literal command to alias ID
    if(!strcmp(name[0], "ls")) {
        append_alias_args(0);
    } else if (!strcmp(name[0], "grep")) {
        append_alias_args(1);
    } else if (!strcmp(name[0], "ll")) {
        strcpy(name[0], "ls"); // ls aliases' name[0] need to be replaced w/ ls
        append_alias_args(2);
    }  else if (!strcmp(name[0], "la")) {
        strcpy(name[0], "ls"); // ls aliases' name[0] need to be replaced w/ ls
        append_alias_args(3);
    }  else if (!strcmp(name[0], "l")) {
        strcpy(name[0], "ls"); // ls aliases' name[0] need to be replaced w/ ls
        append_alias_args(4);
    } else if (!strcmp(name[0], "dir")) {
        append_alias_args(5);
    }
    // no alias args otherwise
}

void append_alias_args(int alias_id) {
    // get to end of argument list
    // when at end, name[i] is NULL, insert here
    int i = 0, j = 0;
    for(; name[i] != NULL; i++) {}
    // add aliasList strings to name[i+j]
    for (; strcmp(aliasList[alias_id][j], ""); j++) {
        name[i+j] = aliasList[alias_id][j];
        ntoken++;
    }
}