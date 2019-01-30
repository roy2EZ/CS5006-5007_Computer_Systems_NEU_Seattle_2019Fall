#include <stdio.h>
#include <stdlib.h>


//Section 1

void NumPtrExample() {
  int num;
  int* numPtr;
  num = 42;
  numPtr = &num;
}

void PointerTest() {
  // allocate three integers and two pointers
  int a = 1;
  int b = 2;
  int c = 3;
  int* p;
  int* q;
  p = &a; // set p to refer to a
  q = &b; // set q to refer to b
  // Now we mix things up a bit...
  c = *p; // retrieve p's pointee value (1) and put it in c
  p = q; // change p to share with q (p's pointee is now b)
  *p = 13; // dereference p to set its pointee (b) to 13 (*q is now 13)

}

void BadPointer() {
  int* p; // allocate the pointer, but not the pointee
  *p = 42; // this dereference is a serious runtime error
}
// What happens at runtime when the bad pointer is dereferenced...

public void JavaShallow() {
  Foo a = new Foo(); // Create a Foo object (no * in the declaration)
  Foo b = new Foo(); // Create another Foo object
  b=a; // This is automatically a shallow assignment --
  // a and b now refer to the same object.
  a.Bar(); // This could just as well be written b.Bar();
  // There is no memory leak here -- the garbage collector
  // will automatically recycle the memory for the two objects.
}

//-----------------------------------------------------------------------
//Section 2
// Local storage example
int Square(int num) {
  int result;
  result = num * num;
  return result;
}

void Foo(int a) { // (1) Locals (a, b, i, scores) allocated when Foo runs
  int i;
  float scores[100]; // This array of 100 floats is allocated locally.
  a = a + 1; // (2) Local storage is used by the computation
  for (i=0; i<a; i++) {
    Bar(i + a); // (3) Locals continue to exist undisturbed,
  } // even during calls to other functions.
} // (4) The locals are all deallocated when the function exits.

void X() {
  int a = 1;
  int b = 2;
  // T1
  Y(a);
  // T3
  Y(b);
  // T5
}
void Y(int p) {
  int q;
  q = p + 2;
  // T2 (first time through), T4 (second time through)
}

// TAB -- The Ampersand Bug function
// Returns a pointer to an int
int* TAB() {
  int temp;
  return(&temp); // return a pointer to the local int
}
void Victim() {
  int* ptr;
  ptr = TAB();
  *ptr = 42; // Runtime error! The pointee was local to TAB
}
//---------------------------------------------------------
//Section 3
void B(int worth) {
  worth = worth + 1;
// T2
}
void A() {
  int netWorth;
  netWorth = 55; // T1
  B(netWorth);
  // T3 -- B() did not change netWorth
}

//Bill Gates By Reference
//Here is the Bill Gates example written to use reference parameters.

// B() now uses a reference parameter -- a pointer to
// the value of interest. B() uses a dereference (*) on the
// reference parameter to get at the value of interest.
void B(int* worthRef) { // reference parameter
  *worthRef = *worthRef + 1; // use * to get at value of interest
  // T2
}
void A() {
  int netWorth;
  netWorth = 55; // T1 -- the value of interest is local to A()

  B(&netWorth); // Pass a pointer to the value of interest.
  // In this case using &.

  // T3 -- B() has used its pointer to change the value of interest
}

//Swap function
void Swap(int* a, int* b) {
  int temp;

  temp = *a;
  *a = *b;
  *b = temp;
}
//Swap caller
void SwapCaller() {
  int x = 1;
  int y = 2;

  Swap(&x, &y); // Use & to pass pointers to the int values of interest
  // (x and y).
}


void SwapCaller2() {
  int scores[10];
  scores[0] = 1;
  scores[9[ = 2;
  Swap(&(scores[0]), &(scores[9]));// the ints of interest do not need to be
  // simple variables -- they can be any int. The caller is responsible
  // for computing a pointer to the int.
}
