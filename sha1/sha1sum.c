#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha1.h"

#define BUFSIZE 4096

void hash_stream(FILE *stream, const char *filename)
{
  int size;
  byte buf[BUFSIZE];
  struct sha1_ctx ctx;
  byte res[SHA1_HASH_SIZE];

  sha1_init(&ctx);

  while ((size = fread(buf, sizeof (char), BUFSIZE, stream)) > 0)
    sha1_update(&ctx, buf, size);

  if (size == -1) // A read error occured
  {
    warn("%s", filename);
    return;
  }

  sha1_final(&ctx, res);

  for (uint i = 0; i < SHA1_HASH_SIZE; ++i)
  {
    printf("%x", res[i] >> 4);
    printf("%x", res[i] & 0xf);
  }

  printf("  %s\n", filename);
}

int main(int argc, char **argv)
{
  FILE *input;

  if (argc == 1)
    hash_stream(stdin, "-");

  for (int i = 1; i < argc; ++i)
  {
    if ((input = fopen(argv[i], "rb")) == NULL)
    {
      warn("%s", argv[i]);
      continue;
    }

    hash_stream(input, argv[i]);

    fclose(input);
  }

  return 0;
}
