#include <stdlib.h>
#include <stdio.h>

#define SIZE 10 /* This should be 26 at most ! */
static char devices[SIZE];
static char used[SIZE];
static char to_print[SIZE];

void print(void)
{
  for (int i = 0; i < SIZE; ++i)
    printf("%c%c", to_print[i], (i == SIZE - 1) ? '\n' : ' ');
}

void rec(int level)
{
  if (level == SIZE)
  {
    print();
    return;
  }

  for (int i = 0; i < SIZE; ++i)
  {
    if (used[i])
      continue;

    used[i] = 1;
    to_print[level] = devices[i];
    rec(level + 1);
    used[i] = 0;
  }
}

int main(void)
{
  for (int i = 0; i < SIZE; ++i)
    devices[i] = i + 'a';

  rec(0);

  return 0;
}
