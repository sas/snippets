#include <fcntl.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define CONFIG_MEM_SIZE 30000
#define CONFIG_STACK_SIZE 200

int main(int argc, char **argv)
{
  int fd;
  struct stat buf;
  unsigned char memory[CONFIG_MEM_SIZE];
  int memory_index = 0;
  unsigned int stack[CONFIG_STACK_SIZE];
  int stack_index = 0;
  char *instr;
  int instr_index = 0;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s FILE\n", basename(argv[0]));
    exit(1);
  }

  if ((fd = open(argv[1], O_RDONLY)) == -1)
  {
    perror(argv[1]);
    exit(2);
  }
  fstat(fd, &buf);
  instr = malloc(buf.st_size);
  read(fd, instr, buf.st_size);
  close(fd);

  for (int i = 0; i < CONFIG_MEM_SIZE; ++i)
    memory[i] = 0;

  while (instr_index < buf.st_size)
  {
    switch (instr[instr_index])
    {
      case '+':
        memory[memory_index] += 1;
        break;
      case '-':
        memory[memory_index] -= 1;
        break;
      case '>':
        memory_index = (memory_index + 1) % CONFIG_MEM_SIZE;
        break;
      case '<':
        memory_index = (memory_index - 1) % CONFIG_MEM_SIZE;
        break;
      case '.':
        write(STDOUT_FILENO, memory + memory_index, 1);
        break;
      case ',':
        read(STDIN_FILENO, memory + memory_index, 1);
        break;
      case '[':
        if (memory[memory_index])
        {
          stack[stack_index] = instr_index;
          ++stack_index;
        }
        else
        {
          int depth_count = 1;
          while (depth_count && instr_index < buf.st_size)
          {
            ++instr_index;
            if (instr[instr_index] == '[')
              ++depth_count;
            else if (instr[instr_index] == ']')
              --depth_count;
          }
        }
        break;
      case ']':
        --stack_index;
        instr_index = stack[stack_index];
        continue;
    }
    ++instr_index;
  }

  free(instr);
}
