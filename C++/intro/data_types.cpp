#include <iostream>
#include <cstdio>

/*Some C++ data types, their format specifiers, and their most common bit widths are as follows:
 
  Int ("%d"): 32 Bit integer
  Long ("%ld"): 32 bit integer (same as Int for modern systems)
  Long Long ("%lld"): 64 bit integer
  Char ("%c"): Character type
  Float ("%f"): 32 bit real value
  Double ("%lf"): 64 bit real value
  */

int main()
{ 
  int a;
  long b; 
  long long c;
  char d; 
  float e; 
  double f;

  scanf("%d, %ld, %lld, %c, %f, %lf", &a, &b, &c, &d, &e, &f);

  
  printf("%d\n%ld\n%lld\n%c\n%f\n%lf\n", a, b, c, d, e, f);

  return 0;

}
