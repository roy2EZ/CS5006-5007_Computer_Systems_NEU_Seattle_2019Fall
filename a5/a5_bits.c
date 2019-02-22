#include <stdlib.h>
#include <stdio.h>

unsigned int packCharacters(char a, char b);
void unpackCharacters(unsigned int packedInt);
int power2(int num, int pow);

unsigned int packCharacters(char a, char b) {
  return ((a << 8) | b);
}

void unpackCharacters(unsigned int packedInt) {
  // use 65280 which is 0xff00 in hexadecimal as a mask
  unsigned int mask1 = 0xff00;
  // use 0x00ff as another mask
  unsigned int mask2 = 0x00ff;
  unsigned char a = ((packedInt & mask1) >> 8) & mask2;
  unsigned char b = packedInt & mask2;
  printf("Unpack result: \n");
  printf("%c and %c\n", a, b);
}

int power2(int num, int pow) {
  int x = 1;
  return num << pow;
}

void displayBits(unsigned value) {
        // Take 1, shift it all the way to the left.
  unsigned c, displayMask = 1 << 15;
  for (c = 1; c <= 16; c++) {
    // putchar prints a character on the screen
    // 'value & displayMask' gets the relevant bit
    putchar(value & displayMask ? '1' : '0');
    // Shift value over one
    value <<= 1;
    // Put a space in for easy reading
    if (c % 8 == 0) {
      putchar(' ');
    }
  }
  putchar('\n');
}

int main(void) {
  unsigned char a;
  unsigned char b;
  unsigned int c;

  printf("---- Part 1: packing two characters into an integer ----\n");
  printf("Enter two characters with space between them: \n");
  scanf("%c %c", &a, &b);
  // packing
  c = packCharacters(a, b);
  printf("The packed result is: \n");
  displayBits(c);
  // unpacking
  printf("---- Part 2: unpacking the integer into two characters ----\n");
  printf("Input is: ");
  displayBits(c);
  unpackCharacters(c);
  // power2
  printf("---- Part 3: calculate power of 2 using the shift operator ----\n");
  int n, m;
  printf("Enter an int as number: \n");
  scanf("%d", &n);
  printf("Enter an int as power: \n");
  scanf("%d", &m);
  int result = power2(n, m);
  printf("The result of %d * 2^%d is: \n", n, m);
  printf("%d\n", result);
  printf("The result in bits: ");
  displayBits(result);
  return 0;
}


