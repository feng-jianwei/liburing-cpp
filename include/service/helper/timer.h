#ifndef __SERVICE_TIMER__
#define __SERVICE_TIMER__

#include <coroutine>

#include "liburing-cpp/io_uring.h"
#include "liburing-cpp/io_uring_callback.h"

class Timer : public UringCallBack {
    std::coroutine_handle<> h_;

    void call(IoUring& ring, IoUringCqe& cqe) override
    {
        ring.SeenCqe(cqe);
        h_.resume();
    }

public:
    Timer(time_t seconds, time_t ns = 0);
    bool await_ready() noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) noexcept;
    virtual void await_resume() {}
};

#endif /* __SERVICE_TIMER__ */
