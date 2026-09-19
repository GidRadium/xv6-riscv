#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Error! Wrong arguments count.\n");
    exit(1);
  }

  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);
  int res = sum(num1, num2);

  printf("sum:\n  num1 = %d,\n  num2 = %d,\n  res = %d\n", num1, num2, res);

  return res;
}
