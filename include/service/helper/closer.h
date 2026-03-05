#ifndef __HELPER_CLOSER__
#define __HELPER_CLOSER__

#include <coroutine>
#include <iostream>

#include "liburing-cpp/io_uring_callback.h"

class Closer : public UringCallBack {
    int fd_{};
    int res{};
    std::coroutine_handle<> h_;

    void call(IoUring& ring, IoUringCqe& cqe) override
    {
        res = cqe.Res();
        ring.SeenCqe(cqe);
        h_.resume();
    }

public:
    Closer(int fd) : fd_{fd}
    {
        auto sqe = Threadlocal_ring().GetSqe();
        sqe.prep_close(fd_);
        sqe.set_Data64(std::bit_cast<__u64>(this));
    };

    bool await_ready() noexcept { return false; }
    auto await_suspend(std::coroutine_handle<> h) noexcept
    {
        h_ = h;
        Threadlocal_ring().Submit();
    }
    auto await_resume() { std::cout << "close : " << fd_ << " result: " << res << std::endl; }
};

#endif /* __HELPER_CLOSER__ */
