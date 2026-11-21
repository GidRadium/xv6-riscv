#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static void
check(int ok, char *test)
{
  if (!ok) {
    printf("FAIL: %s\n", test);
    exit(1);
  }
  printf("OK: %s\n", test);
}

static int
device(char *name, int minor)
{
  int fd = open(name, O_RDWR);
  if (fd < 0) {
    check(mknod(name, 2, minor) == 0, name);
    fd = open(name, O_RDWR);
  }
  check(fd >= 0, name);
  return fd;
}

int
main(void)
{
  char buf[4] = {1, 1, 1, 1};
  uint64 before, after;
  int fd;

  fd = device("zero", 1);
  check(read(fd, buf, 4) == 4, "zero: read returns 4");
  check(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0,
        "zero: all four bytes are zero");
  check(write(fd, "abc", 3) == -1, "zero: write returns -1");
  close(fd);

  fd = device("null", 0);
  check(write(fd, "abc", 3) == 3, "null: write returns 3");
  check(read(fd, buf, 4) == 0, "null: read returns EOF");
  close(fd);

  fd = device("nullstat", 2);
  check(read(fd, buf, 4) == -1, "nullstat: 4-byte read returns -1");
  check(read(fd, &before, 8) == 8, "nullstat: read counter");
  check(write(fd, "abc", 3) == 3, "nullstat: write returns 3");
  check(read(fd, &after, 8) == 8, "nullstat: read counter again");
  check(after == before + 3, "nullstat: counter increased by 3");
  close(fd);

  printf("PASS\n");
  exit(0);
}
