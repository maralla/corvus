#ifndef __MBUF_H
#define __MBUF_H

#include <sys/queue.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MBUF_SIZE 16384

#ifndef STAILQ_LAST
#define STAILQ_LAST(head, type, field)                       \
    (STAILQ_EMPTY((head))                                    \
     ?  NULL                                                 \
     : ((struct type *)(void *) ((char *)((head)->stqh_last) \
             - (size_t)(&((struct type *)0)->field))))
#endif

struct context;

struct mbuf {
    STAILQ_ENTRY(mbuf) next;
    uint8_t *pos;
    uint8_t *last;
    uint8_t *start;
    uint8_t *end;
    int refcount;
};

STAILQ_HEAD(mhdr, mbuf);

static inline bool mbuf_empty(struct mbuf *mbuf)
{
    return mbuf->pos == mbuf->last ? true : false;
}

static inline bool mbuf_full(struct mbuf *mbuf)
{
    return mbuf->last == mbuf->end ? true : false;
}

void mbuf_init(struct context *);
struct mbuf *mbuf_get(struct context *);
void mbuf_recycle(struct context *, struct mbuf *);
uint32_t mbuf_read_size(struct mbuf *);
uint32_t mbuf_write_size(struct mbuf *);
void mbuf_destroy(struct context *ctx);
void mbuf_inc_ref(struct mbuf *buf);
void mbuf_dec_ref(struct mbuf *buf);
void mbuf_dec_ref_by(struct mbuf *buf, int count);
struct mbuf *mbuf_queue_get(struct context *ctx, struct mhdr *q);
void mbuf_queue_copy(struct context *ctx, struct mhdr *q, uint8_t *data, int n);

#endif
