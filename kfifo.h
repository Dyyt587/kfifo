
#ifndef KFIFO_H
#define KFIFO_H

#ifdef __cplusplus
extern "C"{
#endif

#include "string.h"
#include "stdbool.h"
#define __NSX_PASTE__(A,B) A##B

#define min(A,B) __NSMIN_IMPL__(A,B,__COUNTER__)

#define __NSMIN_IMPL__(A,B,L) ({ __typeof__(A) __NSX_PASTE__(__a,L) = (A); \
                             __typeof__(B) __NSX_PASTE__(__b,L) = (B); \
                             (__NSX_PASTE__(__a,L) < __NSX_PASTE__(__b,L)) ? __NSX_PASTE__(__a,L) : __NSX_PASTE__(__b,L); \
                          })

#define spin_lock_irqsave(lock, flags)
#define spin_unlock_irqrestore(lock, flags)

#define EPERM 1 /* Operation not permitted */
#define ENOENT 2 /* No such file or directory */
#define ESRCH 3 /* No such process */
#define EINTR 4 /* Interrupted system call */

typedef struct{
    int TODO;
}spinlock_t;
typedef struct kfifo {
    unsigned char *buffer;     /* the buffer holding the data */
    unsigned int size;         /* the size of the allocated buffer */
    unsigned int in;           /* data is added at offset (in % size) */
    unsigned int out;          /* data is extracted from off. (out % size) */
    spinlock_t *lock;          /* protects concurrent modifications */
}kfifo_t;


bool is_power_of_2(int n) ;
unsigned int __kfifo_put(struct kfifo *fifo, const unsigned char *buffer, unsigned int len);
unsigned int __kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len);

static inline void *ERR_PTR(long error)
{
    return (void *) error;
}
static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}
static inline long IS_ERR(const void *ptr)
{
    return (unsigned long)ptr > (unsigned long)-1000L;
}



struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size, spinlock_t *lock);
struct kfifo *kfifo_alloc(unsigned int size, spinlock_t *lock);
static inline unsigned int kfifo_put(struct kfifo *fifo, const unsigned char *buffer, unsigned int len)
{
    unsigned long flags;
    unsigned int ret;
    spin_lock_irqsave(fifo->lock, flags);
    ret = __kfifo_put(fifo, buffer, len);
    spin_unlock_irqrestore(fifo->lock, flags);
    return ret;
}

static inline unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len)
{
    unsigned long flags;
    unsigned int ret;
    spin_lock_irqsave(fifo->lock, flags);
    ret = __kfifo_get(fifo, buffer, len);
    //当fifo->in == fifo->out时，buufer为空
    if (fifo->in == fifo->out)
        fifo->in = fifo->out = 0;
    spin_unlock_irqrestore(fifo->lock, flags);
    return ret;
}


#ifdef __cplusplus
};
#endif
#endif