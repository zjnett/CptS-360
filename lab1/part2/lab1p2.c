#include "lab1p2.h"

int main(int argc, char *argv[], char *env[])
{
    myprintf("argc: %d\n", argc);

    for (int i = 0; i < argc; i++) {
        myprintf("argv[%d]: %s\n", i, argv[i]);
    }

    int count = 0;
    while (env[count] != NULL) {
        myprintf("env[%d]: %s\n", count, env[count++]);
    }

    myprintf("cha=%c string=%s      dec=%d hex=%x oct=%o neg=%d\n",
             'A', "this is a test", 100, 100, 100, -100);
    test_myprintf();
    return 0;
}

void test_myprintf(void) {
    int a = 31294;
    myprintf("the %s jumped over\nthe %s, %d times\n", "cow", "moon", 2);
    myprintf("the address of the local variable in this function is %x\n", &a);
}

int rpu(u32 x, u32 base)
{
    char c;
    if (x)
    {
        c = ctable[x % base];
        rpu(x / base, base);
        putchar(c);
    }
}

int printu(u32 x)
{
    (x == 0) ? putchar('0') : rpu(x, 10);
}

int prints(char *s)
{
    while (*s != '\0')
    {
        putchar(*s);
        s++;
    }
}

int printd(int x)
{
    if (x < 0)
    {
        putchar('-');
    }
    printu(x);
}

int printx(u32 x)
{
    putchar('0');
    putchar('x');
    (x == 0) ? putchar('0') : rpu(x, 16);
    putchar(' ');
}

int printo(u32 x)
{
    putchar('0');
    (x == 0) ? putchar('0') : rpu(x, 8);
    putchar(' ');
}

int myprintf(char *fmt, ...)
{
    int *ip = (int *)&fmt + 1; // set ip to thing on stack directly above fmt
    while (*fmt != '\0')
    {
        switch (*fmt)
        {
        case '%':
            fmt++; // get next character
            switch (*fmt)
            { // switch for format specifier
            case 'c':
                putchar(*ip);
                ip++;
                break;
            case 's':
                prints((char *)*ip);
                ip++;
                break;
            case 'u':
                printu((u32)*ip);
                ip++;
                break;
            case 'd':
                printd(*ip);
                ip++;
                break;
            case 'o':
                printo((u32)*ip);
                ip++;
                break;
            case 'x':
                printx((u32)*ip);
                ip++;
                break;
            case '%':
                putchar('%');
                ip++;
                break;
            default:
                putchar(' ');
                break;
            }
            break;
        case '\n':
            putchar('\r');
        default:
            putchar(*fmt);
        }
        fmt++;
    }
}