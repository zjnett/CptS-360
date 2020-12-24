#include "lab3.h"

char gdir[MAX]; // gdir[ ] stores dir strings
char *dir[128];
int ndir;

char gpath[MAX]; // gpath[ ] stores token strings
char *name[64];
int ntoken;

char home[MAX]; // home[ ] stores home (~) directory from env[]

int pd[2];
int lpd[2];

char *head, *tail;

char aliasList[MAX][MAX][MAX];

int main(int argc, char *argv[], char *env[])
{
    int i, r;
    int pid, status;
    char *s, cmd[128], line[MAX];

    populate_alias_list();

    printf(BOLD "************* Welcome to znsh **************\n" RESET);

    i = 0;
    while (env[i])
    {
        #ifdef __DEBUG__
        printf("env[%d] = %s\n", i, env[i]);
        #endif

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

    char cwd[MAX];
    char user[64];

    if (getlogin() != NULL)
            strcpy(user, getlogin());

    printf(BOLD "*********** znsh processing loop **********\n" RESET);

    while (1)
    {
        memset(name, 0, sizeof(name));

        printf(BOLD CYN "%s@znsh" RESET ":", user);

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf(BOLD BLU "%s", cwd);
        }

        printf(GRN " ➜ " RESET);

        fgets(line, 128, stdin);
        line[strlen(line) - 1] = 0; // fgets() has \n at end

        if (line[0] == 0)
            continue;

        #ifdef __DEBUG__
        printf("line = %s\n", line); // print line to see what you got
        #endif

        // base code task 2, tokenize line
        tokenize_cli(line);

        #ifdef __DEBUG__
        printf("tokenized cli\n");
        for (int i = 0; i < ntoken; i++) {
            printf("%s\n", name[i]);
        }
        #endif

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
            #ifdef __DEBUG__
            printf("parent sh %d waits\n", getpid());
            #endif
            pid = wait(&status);
            #ifdef __DEBUG__
            printf("child sh %d died : exit status = %04x\n", pid, status);
            #endif
            continue;
        }
        else
        {
            do_pipe(gpath, 0);
        }
    }
}