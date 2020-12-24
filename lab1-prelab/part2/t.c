/************* t.c file ********************/
#include <stdio.h>
#include <stdlib.h>

int *FP;

int main(int argc, char *argv[ ], char *env[ ])
{
    int a,b,c;
    printf("enter main\n");
  
    printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
    printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);

    //(1). Write C code to print values of argc and argv[] entries
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    a=1; b=2; c=3;
    A(a,b);
    printf("exit main\n");
}

int A(int x, int y)
{
    int d,e,f;
    printf("enter A\n");
    // write C code to PRINT ADDRESS OF d, e, f
    printf("address of d: %8x\n", &d);
    printf("address of e: %8x\n", &e);
    printf("address of f: %8x\n", &f);

    d=4; e=5; f=6;
    B(d,e);
    printf("exit A\n");
}

int B(int x, int y)
{
    int g,h,i;
    printf("enter B\n");
    // write C code to PRINT ADDRESS OF g,h,i
    printf("address of g: %8x\n", &g);
    printf("address of h: %8x\n", &h);
    printf("address of i: %8x\n", &i);

    g=7; h=8; i=9;
    C(g,h);
    printf("exit B\n");
}

int C(int x, int y)
{
    int u, v, w, i, *p;

    printf("enter C\n");
    // write C code to PRINT ADDRESS OF u,v,w,i,p;
    printf("address of u: %8x\n", &u);
    printf("address of v: %8x\n", &v);
    printf("address of w: %8x\n", &w);
    printf("address of i: %8x\n", &i);
    printf("address of p: %8x\n\n", &p);

    u=10; v=11; w=12; i=13;

    FP = (int *)getebp();  // FP = stack frame pointer of the C() function


// (2). Write C code to print the stack frame link list.
    printf("FP = ebp = %x\n", FP);
    printf("stack frame link list\n");
    while(FP != NULL) {
        printf("%p -> ", FP);
        FP = *FP;
    }
    printf("NULL\n\n");
    
    p = (int *)&p;

    p = &u; // example output file begins from the address of u -- I will do the same

//(3). Print the stack contents from p to the frame of main()
//     YOU MAY JUST PRINT 128 entries of the stack contents.
    printf("128 stack contents\n");
    for (int i = 0; i < 128; i++) {
        printf("%x\t%x\n", p, *p);
        p++;
    }

//(4). On a hard copy of the print out, identify the stack contents
//     as LOCAL VARIABLES, PARAMETERS, stack frame pointer of each function.

// Annotated stack contents can be found in annotated-outfile.txt.

}