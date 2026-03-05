#ifndef __HELPER_BINDER__
#define __HELPER_BINDER__

#include <coroutine>
#include <iostream>

#include "liburing-cpp/io_uring_callback.h"
class Binder : public UringCallBack {
    std::coroutine_handle<> h_;
    int res;

    void call(IoUring& ring, IoUringCqe& cqe) override
    {
        res = cqe.Res();
        ring.SeenCqe(cqe);
        h_.resume();
    }

public:
    Binder(int fd, struct sockaddr* addr, socklen_t addrlen)
    {
        auto sqe = Threadlocal_ring().GetSqe();
        sqe.pre_bind(fd, addr, addrlen);
        sqe.set_Data64(std::bit_cast<__u64>(this));
    };

    bool await_ready() noexcept { return false; }
    auto await_suspend(std::coroutine_handle<> h) noexcept
    {
        h_ = h;
        Threadlocal_ring().Submit();
    }
    auto await_resume()
    {
        std::cout << "bind result: " << res << std::endl;
        return res;
    }
};

#endif /* __HELPER_BINDER__ */
