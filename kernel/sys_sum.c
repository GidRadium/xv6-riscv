#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_sum(void)
{
  int num1, num2;

  argint(0, &num1);
  argint(1, &num2);

  int res = num1 + num2;

  printk("sys_sum:\n  num1 = %d,\n  num2 = %d,\n  res = %d\n", num1, num2, res);

  return res;
}
