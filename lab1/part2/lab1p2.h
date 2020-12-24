#ifndef LAB_1_P_2_H
#define LAB_1_P_2_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define BASE 10
#define BASE16 16
#define BASE8 8

typedef unsigned int u32;
char *ctable = "0123456789ABCDEF";

// Function prototypes
void test_myprintf(void);
int rpu(u32 x, u32 base);
int printu(u32 x);
int prints(char *s);
int printd(int x);
int printx(u32 x);
int printo(u32 x);
int myprintf(char *fmt, ...);

#endif /* LAB_1_P_2_H */