//g++
int SIZE=1<<20;
char *p=(char *)malloc(SIZE)+SIZE;
__asm__(¡°movl %0, %%esp\n¡± :: ¡°r¡±(p));
//C++
#pragma comment(linker, ¡°/STACK:16777216¡±)
