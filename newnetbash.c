#include <syscall.h>
#include <stdio.h>
#include <net/if.h>
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static void print_usage_exit()
{
  printf("Usage: newnetbash [-s|shared|-n|new] [-c|clone|-u|unshare]\n");
  printf("       -s|shared   launch bash in the same network namespace\n");
  printf("       -n|newnet   launch bash in a new network namespace\n");
  printf("       -c|clone    use clone(2) before starting bash\n");
  printf("       -u|unshare  use unshare(2) before starting bash\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  /*
   * Unshare the network namespace.
   */

  int i;
  int create_new_net = 0;
  int use_clone = 0;

  if (argc < 3)
    print_usage_exit();

  for(i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "shared") == 0) || (strcmp(argv[i], "-s") == 0))
      create_new_net = 0;
    else if ((strcmp(argv[i], "newnet") == 0) || (strcmp(argv[i], "-n") == 0))
      create_new_net = 1;
    else if ((strcmp(argv[i], "clone") == 0) || (strcmp(argv[i], "-c") == 0))
      use_clone = 1;
    else if ((strcmp(argv[i], "unshare") == 0) || (strcmp(argv[i], "-u") == 0))
      use_clone = 0;
    else
      print_usage_exit();
  }

  if (create_new_net) {
    if (unshare(CLONE_NEWNET)) {
      perror("unshare");
      return -1;
    }
  }

  execl("/bin/bash", "/bin/bash", NULL);
  return 0;
}
