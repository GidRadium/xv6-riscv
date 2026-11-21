#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  char *d = "0123456789abcdef";
  unsigned char buf[512];
  int n, fd, first =1;

  if(argc != 3) {
    fprintf(2, "wrong arguments\n");
    exit(1);
  }

  n = atoi(argv[1]);
  fd = open(argv[2], O_RDONLY);
  if (fd < 0) {
    fprintf(2, "can't open %s\n", argv[2]);
    exit(1);
  }
  while(n > 0) {
    int r = read(fd, buf, n < sizeof(buf) ? n : sizeof(buf));
    if (r < 0) {
      printf("read error\n");
      exit(1);
    }
    if (r == 0) {
      break;
    }
    for (int i = 0; i < r; ++ i) {
      printf(first ? "%c%c" : " %c%c", d[buf[i] >> 4], d[buf[i] & 15]);
      first = 0;
    }
    n -= r;
  }
  if (!first) {
    printf("\n");
  }
  close(fd);
  exit(0);
}
