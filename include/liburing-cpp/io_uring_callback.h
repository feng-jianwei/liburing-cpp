#ifndef __URING_IO_URING_CALLBACK__
#define __URING_IO_URING_CALLBACK__
#include "io_uring.h"
#include "io_uring_cqe.h"

class UringCallBack {
public:
    virtual void call(IoUring& ring, IoUringCqe& cqe) = 0;
};

template <typename T>
class Caller : UringCallBack {
    T &value_;
public:
    Caller(T &value): value_(value) {};
    void call(IoUring& ring, IoUringCqe& cqe) {
        value_.SetCqe(cqe);
        value_.Resume();
        ring.SeenCqe(cqe);
    }
};

#endif /* __URING_IO_URING_CALLBACK__ */
