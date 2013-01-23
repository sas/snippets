#ifdef __amd64__

# include <asm/unistd_64.h>

void my_exit(unsigned long int i)
{
  __asm__ volatile (
  "	syscall\n\t"
  : /* no output */
  : "a"(__NR_exit), "D"(i)
  );
}

#else

# include <asm/unistd_32.h>

void my_exit(unsigned long int i)
{
  __asm__ volatile (
  "	int $0x80\n\t"
  : /* no output */
  : "a"(__NR_exit), "b"(i)
  );
}

#endif

int main(void)
{
  my_exit(42);
  return 69;
}
