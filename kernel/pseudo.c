#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "defs.h"


static struct spinlock countlock;
static uint64 bytes_written;

static int
pseudoread(int user_dst, uint64 dst, int n, int minor) // dst - адрес начала буфера
{
    static char zeroes[128];
    uint64 count;
    int done = 0;
    
    if (n < 0) 
        return -1;
    switch (minor) {
    case PSEUDO_NULL: // null — при чтении всегда возвращает «конец файла», все записываемые данные игнорируются (успех);
        return 0;
    case PSEUDO_ZERO: // zero — при чтении возвращает нулевые байты, запись не допускается (возвращается ошибка);
        while (done < n) {
            int chunk = n - done;
            if (chunk > (int)sizeof(zeroes))
                chunk = sizeof(zeroes);
            if (either_copyout(user_dst, dst + done, zeroes, chunk) < 0) // флаг куда пишем (какой буфер), куда, откуда, чанк сам
                return done > 0 ? done : -1;
            done += chunk;
        }
        return done;
    
     case PSEUDO_NULLSTAT: // nullstat — при записи отбрасывает записываемые байты, но считает их количество (uint64), при чтении ровно sizeof(uint64) байтов возвращает общее количество записанных байт за все время работы (при несоответствии запрошенного объема — ошибка чтения).
        if (n != sizeof(count)) //проверка на запрос 8 байтов размер unit64=8
            return -1;

        acquire(&countlock); // берём блокировку
        count = bytes_written;  // копируем текущее значение записанных юайтов
        release(&countlock); // отпускаем 

        if (either_copyout(user_dst, dst, &count, sizeof(count)) < 0) // флаг куда копируем, адрес начала буфера, откуда копуировать, 8 байт
            return -1;
        return sizeof(count);

    default:
        return -1;
    }
}

static int
pseudowrite(int user_src, uint64 src, int n, int minor)
{
  if (n < 0)
    return -1;

  switch (minor) {
  case PSEUDO_NULL:
    return n;

  case PSEUDO_ZERO:
    return -1; // запись нельзя 

  case PSEUDO_NULLSTAT:
    acquire(&countlock);
    bytes_written += n;
    release(&countlock);
    return n;

  default:
    return -1;
  }
}

void pseudoinit(void)
{
    initlock(&countlock, "nullstat");
    devsw[PSEUDO].read = pseudoread;
    devsw[PSEUDO].write = pseudowrite;
}
