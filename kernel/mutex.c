#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"

struct file *
mutexalloc(void)
{
  struct file *f;
  struct sleeplock *sl;

  f = filealloc();
  if (f == 0) {
    return 0;
  }

  sl = (struct sleeplock *)kalloc();
  if (sl == 0) {
    fileclose(f);
    return 0;
  }
  initsleeplock(sl, "mutex");

  f->type = FD_MUTEX;
  f->readable = 0;
  f->writable = 0;
  f->mutex = sl;

  printk("mutexalloc: mutex=%p pid=%d\n", sl, myproc()->pid);

  return f;
}

void
mutexclose(struct sleeplock *sl)
{
  printk("mutexclose: mutex=%p pid=%d\n", sl, myproc()->pid);
  kfree(sl);
}
