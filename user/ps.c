#include "kernel/types.h"
#include "user/user.h"

static char *
state_name(int state)
{
  switch (state) {
  case 2:
    return "SLEEPING";
  case 3:
    return "RUNNABLE";
  case 4:
    return "RUNNING ";
  case 5:
    return "ZOMBIE  ";
  default:
    return "UNKNOWN ";
  }
}

int
main(void)
{
  int bufsize = 2; // сколько хотим
  struct procinfo *buf;

  while (1) {
    buf = malloc(bufsize * sizeof(struct procinfo));
    if (buf == 0) {
      fprintf(2, "ps: malloc failed\n");
      exit(1);
    }

    int numProc = ps_listinfo(buf, bufsize);

    if (numProc < 0) {
      fprintf(2, "ps: ps_listinfo failed\n");
      exit(1);
    }

    if (numProc <= bufsize) {
      printf("PID\tNAME\tSTATE   \tPPID\n");

      for (int i = 0; i < numProc; i++) {
        printf("%d\t%s\t%s\t%d\n",
          buf[i].pid, buf[i].name, state_name(buf[i].state), buf[i].ppid);
      }

      free(buf);
      exit(0);
    }

    bufsize = numProc;
    free(buf);
  }
}
