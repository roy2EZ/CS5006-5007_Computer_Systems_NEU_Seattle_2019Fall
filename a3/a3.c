#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "a3.h"

// Name:Rongyi CHEN
// Date: 1/31/2019

void big(Name *name) {
  char strBig[100];
  strcpy(strBig, ("%s", name->first));
  strcat(strBig, " ");
  if (name->middle != NULL) {
    strcat(strBig, ("%s", name->middle));
    strcat(strBig, " ");
    }
  strcat(strBig, ("%s", name->last));
  puts(strBig);
}

void last(Name *name) {
  char strLast[100];
  strcpy(strLast, ("%s", name->last));
  strcat(strLast, ", ");
  strcat(strLast, ("%s", name->first));
  puts(strLast);
}

void reg(Name *name) {
  char strReg[100];
  strcpy(strReg, ("%s", name->first));
  strcat(strReg, " ");
  strcat(strReg, ("%s", name->last));
  puts(strReg);
}

void mid(Name *name) {
  char strMid[100];
  strcpy(strMid, ("%s", name->first));
  strcat(strMid, " ");
  if (name->middle != NULL) {
    strncat(strMid, ("%s", name->middle), 1);
    strcat(strMid, ". ");
    }
  strcat(strMid, ("%s", name->last));
  puts(strMid);
}

void small(Name *name) {
  char strSmall[100];
  strcpy(strSmall, ("%s", name->first));
  puts(strSmall);
}


void formatName(Name *name, char format) {
  switch ((char)toupper(format)) {
  case 'B':
    big(name);
    break;
  case 'L':
    last(name);
    break;
  case 'R':
    reg(name);
    break;
  case 'M':
    mid(name);
    break;
  case 'S':
    small(name);
    break;
  default:
    break;
  }
}

void fillName(Name *name, char format, char *dest) {
  switch ((char)toupper(format)) {
  case 'B': {
    strcpy(dest, ("%s", name->first));
    strcat(dest, " ");
    if (name->middle != NULL) {
      strcat(dest, ("%s", name->middle));
      strcat(dest, " ");
      }
    strcat(dest, ("%s", name->last));
    }
    break;
  case 'L': {
    strcpy(dest, ("%s", name->last));
    strcat(dest, ", ");
    strcat(dest, ("%s", name->first));
    }
    break;
  case 'R': {
    strcpy(dest, ("%s", name->first));
    strcat(dest, " ");
    strcat(dest, ("%s", name->last));
    }
    break;
  case 'M': {
    strcpy(dest, ("%s", name->first));
    strcat(dest, " ");
    if (name->middle != NULL) {
      strncat(dest, ("%s", name->middle), 1);
      strcat(dest, ". ");
      }
    strcat(dest, ("%s", name->last));
    }
    break;
  case 'S':
    strcpy(dest, ("%s", name->first));
    break;
  default:
    break;
  }
}

void listByName(Name *n1, Name *n2, Name *n3, char format) {
  char dest1[100] = {0};
  fillName(n1, format, dest1);
  char dest2[100] = {0};
  fillName(n2, format, dest2);
  char dest3[100] = {0};
  fillName(n3, format, dest3);
  // Bubble sort
  char *a[] = {dest1, dest2, dest3};
  int i, j;
  char *temp;
  for (i = 0;i < 2;i++) {
    for (j = 0;j < 2;j++) {
      if (strcmp(a[j], a[j+1]) > 0) {
        temp = a[j];
        a[j] = a[j+1];
        a[j+1] = temp;
      }
    }
  }
  // print the sorted array
  int k;
  for (k = 0;k < 3;k++) {
    printf("%s\n", a[k]);
  }
}

void listByAge(Name* names[], int numNames, char format) {
  int i, j;
  Name *temp;
  for (i = 0;i < (numNames-1);i++) {
    for (j = 0;j < (numNames-1);j++) {
      if ((names[j]->age) <= (names[j+1]->age)) {
        temp = names[j];
        names[j] = names[j+1];
        names[j+1] = temp;
      }
    }
  }

  // print the sorted array by age from older to younger
  int k;
  for (k = 0;k < numNames;k++) {
    char dest[numNames][100];
    fillName(names[k], format, dest[k]);
      printf("%s: %d\n", dest[k], names[k]->age);
  }
}
