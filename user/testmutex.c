#include "kernel/types.h"
#include "user/user.h"


void
lock(int mutex_d) {
  int rc = mutex_lock(mutex_d);
  if (rc) {
    printf("Error: can't lock mutex.\n");
    exit(1);
  }
}

void
unlock(int mutex_d) {
  int rc = mutex_unlock(mutex_d);
  if (rc) {
    printf("Error: can't unlock mutex.\n");
    exit(1);
  }
}

// sync - 1 / 0 (true/false)
void
print_args(int argc, char **argv, int pid, int sync, int mutex_d) {
  for (int i = 0; i < argc; i++) {
    for (int ci = 0; argv[i][ci] != '\0'; ci++) {
      if (sync) lock(mutex_d);
      printf("%d: arg %d, char '%c'\n", pid, i, argv[i][ci]);
      if (sync) unlock(mutex_d);
    }
  }
}

// sync - 1 / 0 (true/false)
void
test(int argc, char **argv, int sync) {
  if (sync) printf("WITH SYNC:\n");
  else printf("WITHOUT SYNC:\n");

  int mutex_d = 0;
  if (sync) {
    mutex_d = mutex();
    if (mutex < 0) {
      printf("Error: mutex create failed.\n");
      exit(1);
    }
  }

  int pid = fork();
  if (pid < 0) {
    printf("Error: fork failed.\n");
    exit(1);
  }

  print_args(argc, argv, pid, sync, mutex_d);

  if (pid == 0) exit(0);

  if (wait(0) < 0) {
    printf("Error: wait failed.\n");
    exit(1);
  }
}

int
main(int argc, char **argv)
{
  test(argc, argv, 0);
  // test(argc, argv, 1);
}
