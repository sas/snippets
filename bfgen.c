/*
 * This piece of code generates the brainfuck code that generates the string
 * read on stdin.
 * There is not a lot of optimisation or anything, just using loops to reduce
 * code size.
 */

#include <stdio.h>

int main(void)
{
  int c;

  while ((c = fgetc(stdin)) != EOF)
  {
    while (c > 10)
    {
      printf("+");
      c -= 10;
    }
    printf("[>++++++++++<-]>");

    while (c > 0)
    {
      printf("+");
      c -= 1;
    }
    printf(".[-]<[-]");

  }

  return 0;
}
