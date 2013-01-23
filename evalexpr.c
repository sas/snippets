#include <stdlib.h>
#include <stdio.h>

void die_token(char token)
{
  fprintf(stderr, "unexpected token: `%c'\n", token);
  exit(1);
}

int parse_int(void)
{
  int c = 0;
  int mult = 1;
  int res = 0;

  while ((c = fgetc(stdin)) == ' ')
    continue;
  if (c == '-')
    mult = -1;
  else if (mult == '+')
    mult = 1;
  else if (c >= '0' && c <= '9')
    res = c - '0';
  else
    die_token(c);

  while ((c = fgetc(stdin)) != EOF)
  {
    if (c < '0' || c > '9')
    {
      if (c != '+' && c != '-' && c != '*' && c != '/'
          && c !=	'%' && c != '(' && c != ')' && c != ' ')
        die_token(c);

      ungetc(c, stdin);
      break;
    }
    res = 10 * res + (c - '0');
  }

  return mult * res;
}

int parse_parent(void)
{
  char c = 0;
  int res = 0;

  while ((c = fgetc(stdin)) == ' ')
    continue;

  if (c == '(')
  {
    res = parse_addmin();
    while ((c = fgetc(stdin)) == ' ')
      continue;
    /* At this point, c == ')' if we have a valid input. */
    return res;
  }
  else
  {
    ungetc(c, stdin);
    return parse_int();
  }
}

int parse_muldiv(void)
{
  char c = 0;
  int res = parse_parent();

  while (1)
  {
    while ((c = fgetc(stdin)) == ' ')
      continue;

    if (c == '*')
      res *= parse_parent();
    else if (c == '/')
      res /= parse_parent();
    else if (c == '%')
      res %= parse_parent();
    else
    {
      ungetc(c, stdin);
      break;
    }
  }

  return res;
}

int parse_addmin(void)
{
  char c = 0;
  int res = parse_muldiv();

  while (1)
  {
    while ((c = fgetc(stdin)) == ' ')
      continue;

    if (c == '+')
      res += parse_muldiv();
    else if (c == '-')
      res -= parse_muldiv();
    else
    {
      ungetc(c, stdin);
      break;
    }
  }

  return res;
}

int main(void)
{
  char c;
  int res;

  res = parse_addmin();
  /* Here we can check for the presence of trailing junk characters. */
  c = fgetc(stdin);
  if (c != EOF && c != '\n')
    die_token(c);

  /* If we are not 'dead', we print the result. */
  printf("%d\n", res);

  return 0;
}
