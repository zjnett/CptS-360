#ifndef LAB_3_H
#define LAB_3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h> 
#include <unistd.h>

#define MAX 256

// color defines
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define BOLD  "\x1B[1m"
#define RESET "\x1B[0m"

// debug macro to toggle debug messages
//#define __DEBUG__

// Function prototypes
int tokenize_PATH(char *PATH);
int tokenize_cli(char *line);
int do_pipe(char *cmdLine, int *pd);
int do_command(char *cmdLine);
int scan(char *cmdLine);
void populate_alias_list(void);
void process_aliases(char *cmdLine);
void append_alias_args(int alias_id);

#endif
