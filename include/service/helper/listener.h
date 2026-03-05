#ifndef __HELPER_LISTENER__
#define __HELPER_LISTENER__

#include <coroutine>
#include <iostream>

#include "liburing-cpp/io_uring_callback.h"
class Listener : public UringCallBack {
    std::coroutine_handle<> h_;
    int res{};
    void call(IoUring& ring, IoUringCqe& cqe) override
    {
        res = cqe.Res();
        ring.SeenCqe(cqe);
        h_.resume();
    }

public:
    Listener(int fd, int backlog)
    {
        auto sqe = Threadlocal_ring().GetSqe();
        sqe.pre_listen(fd, backlog);
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
        std::cout << "listen result: " << res << std::endl;
        return res;
    }
};

#endif /* __HELPER_LISTENER__ */
