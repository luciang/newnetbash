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
  printf("Usage: newnetbash [-s|shared|-n|new]\n");
  printf("       -s|shared   launch bash in the same network namespace\n");
  printf("       -n|newnet   launch bash in a new network namespace\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  /*
   * Unshare the network namespace.
   */

  int create_new_net = 0;

  if (argc != 2)
    print_usage_exit();

  
  if ((strcmp(argv[1], "shared") == 0) || (strcmp(argv[1], "-s") == 0))
    create_new_net = 0;
  else if ((strcmp(argv[1], "newnet") == 0) || (strcmp(argv[1], "-n") == 0))
    create_new_net = 1;
  else
    print_usage_exit();

  if (create_new_net) {
    if (unshare(CLONE_NEWNET)) {
      perror("unshare");
      return -1;
    }
  }

  execl("/bin/bash", "/bin/bash", NULL);
  return 0;
}
