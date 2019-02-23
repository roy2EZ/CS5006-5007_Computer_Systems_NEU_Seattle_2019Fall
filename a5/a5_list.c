#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define MAX         100012
#define INF         (~(0x1<<31))  // define the infinity which is 0X7FFFFFFF
#define isLetter(a) ((((a)>='a')&&((a)<='z')) || (((a)>='A')&&((a)<='Z')))
#define LENGTH(a)   (sizeof(a)/sizeof(a[0]))
