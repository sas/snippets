#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  size_t ebp;

  __asm__ volatile (
  "	mov %%ebp, %0;"
  "	mov (%0), %0;"
  "	mov (%0), %0;"
  : "=r"(ebp)
  );

  if (!ebp && argc != 2)
  {
    printf("usage: ./couille chaine\n");
    exit(0);
  }

  if (*argv[1] == 0)
  {
    printf("%d\n", argc - 2);
    exit(0);
  }

  ++argv[1];
  main(argc + 1, argv);

  return 0;
}
