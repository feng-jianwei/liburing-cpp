#ifndef __URING_IO_URING_CALLBACK__
#define __URING_IO_URING_CALLBACK__
#include "io_uring.h"
#include "io_uring_cqe.h"

class UringCallBack {
public:
    virtual void call(IoUring& ring, IoUringCqe& cqe) = 0;
};

#endif /* __URING_IO_URING_CALLBACK__ */
