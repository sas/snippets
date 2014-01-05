/*
 * A small daemon to run a command when something changes in a directory tree.
 */

#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static void usage_die(void)
{
  extern char *__progname; /* from crt0.o */

  fprintf(stderr, "usage: %s directory action [timeout]\n"
      "   directory: the directory to scan\n"
      "   action:    the action to take on update\n"
      "   timeout:   the timout (defaults to 5 seconds)\n"
      , __progname);

  exit(EXIT_FAILURE);
}

static time_t get_last_mtime(const char *path)
{
  time_t        mtime = 0;
  DIR           *dir;
  struct dirent *entry;
  struct stat   buf;

  if (stat(path, &buf) == -1)
    return 0;

  mtime = buf.st_mtime;

  if (S_ISDIR(buf.st_mode)) {
    if ((dir = opendir(path)) == NULL)
      return mtime;

    while ((entry = readdir(dir))) {
      time_t  new_mtime;
      char    *new_path;

      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

      asprintf(&new_path, "%s/%s", path, entry->d_name);
      new_mtime = get_last_mtime(new_path);
      if (new_mtime > mtime)
        mtime = new_mtime;
      free(new_path);
    }

    closedir(dir);
  }

  return mtime;
}

int main(int argc, char **argv)
{
  const char          *directory  = argv[1];
  const char          *action     = argv[2];
  const unsigned int  timeout     = argv[3] ? atoi(argv[3]) : 5;
  time_t              last_up;

  if (argc != 3 && argc != 4)
    usage_die();

  system(action);
  last_up = time(NULL);

  while (42) {
    sleep(timeout);

    if (get_last_mtime(directory) > last_up) {
      last_up = time(NULL);
      system(action);
    }
  }
}
