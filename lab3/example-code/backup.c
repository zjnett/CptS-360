/************** lab3base.c file **************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h> 
#include <unistd.h>

#define MAX 256

// Function prototypes (TODO: move to header)
int tokenize_PATH(char *PATH);
int tokenize_cli(char *line);

char gdir[MAX]; // gdir[ ] stores dir strings
char *dir[128];
int ndir;

char gpath[MAX]; // gpath[ ] stores token strings
char *name[64];
int ntoken;

char home[MAX]; // home[ ] stores home (~) directory from env[]

int pd[2];

int main(int argc, char *argv[], char *env[])
{
    int i, r;
    int pid, status;
    char *s, cmd[128], line[MAX];

    printf("************* Welcome to znsh **************\n");
    i = 0;
    while (env[i])
    {
        printf("env[%d] = %s\n", i, env[i]);

        // Looking for PATH=
        if (strncmp(env[i], "PATH=", 5) == 0)
        {
            printf("show PATH: %s\n", env[i]);

            printf("decompose PATH into dir strings in gdir[ ]\n");
            strcpy(gdir, &env[i][5]);

            // base code task 1, tokenize PATH
            tokenize_PATH(gdir);
            break;
        }

        // Looking for HOME=
        if (strncmp(env[i], "HOME=", 5) == 0)
        {
            printf("show HOME: %s\n", env[i]);
            printf("decompose HOME into dir strings in home[ ]\n");
            strcpy(home, &env[i][5]);
        }
        i++;
    }

    printf("*********** znsh processing loop **********\n");

    while (1)
    {
        memset(name, 0, sizeof(name));

        printf("znsh $ : ");

        fgets(line, 128, stdin);
        line[strlen(line) - 1] = 0; // fgets() has \n at end

        if (line[0] == 0)
            continue;
        printf("line = %s\n", line); // print line to see what you got

        // base code task 2, tokenize line
        tokenize_cli(line);

        printf("tokenized cli\n");
        for (int i = 0; i < ntoken; i++) {
            printf("%s\n", name[i]);
        }

        // 3. Handle name[0] == "cd" or "exit" case
        if (!strcmp(name[0], "cd")) {
            if(name[1] != NULL) { // if we have a specified arg1
                chdir(name[1]);
                continue;
            } else { // no specified arg1, assume HOME
                chdir(home);
                continue;
            }
        } else if (!strcmp(name[0], "exit")) {
            exit(1);
        }

        // 4. name[0] is not cd or exit:
        pid = fork(); // fork a child sh

        if (pid)
        {
            printf("parent sh %d waits\n", getpid());
            pid = wait(&status);
            printf("child sh %d died : exit status = %04x\n", pid, status);
            continue;
        }
        else
        {
            printf("child sh %d begins\n", getpid());

            // I/O redirection
            char **head, **tail;

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
                } else if (!strcmp(name[i], "|")) {
                    // pipe!
                    name[i] = NULL;
                    head = name;
                    tail = (name+i+1);
                    pipe(pd); // create a pipe
                    pid = fork(); // fork a child to share the pipe
                    if (pid) { // parent, writer
                        close(pd[0]);
                        close(1);
                        dup(pd[1]);
                        close(pd[1]);

                        for (int i = 0; i < ndir; i++) {
                            strcpy(cmd, dir[i]);
                            strcat(cmd, "/");
                            strcat(cmd, head[0]);

                            r = execve(cmd, head, env);
                        }
                    } else { // child, reader
                        close(pd[1]);
                        close(0);
                        dup(pd[0]);
                        close(pd[0]);

                        for (int i = 0; i < ndir; i++) {
                            strcpy(cmd, dir[i]);
                            strcat(cmd, "/");
                            strcat(cmd, tail[0]);

                            r = execve(cmd, tail, env);
                        }
                    }
                }
            }

            for (i = 0; i < ndir; i++)
            {
                strcpy(cmd, dir[i]);
                strcat(cmd, "/");
                strcat(cmd, name[0]);
                fprintf(stdin, "i=%d cmd=%s\n", i, cmd);

                r = execve(cmd, name, env);
            }
            printf("cmd %s not found, child sh exit\n", name[0]);
            exit(123); // die with value 123
        }
    }
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

/******************* 6 ***********************
 Handle pipe: check pipe symbol | in input line;
 if so, divide line into head, tail

 create PIPE, fork a child to share the pipe
 parent write to  pipe and exec head;
 child  read from pipe and exec tail
********************************************/
