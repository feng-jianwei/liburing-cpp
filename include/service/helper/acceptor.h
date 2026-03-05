#ifndef __HELPER_ACCEPTOR__
#define __HELPER_ACCEPTOR__

#include <bit>

#include "liburing-cpp/io_uring.h"
#include "liburing-cpp/io_uring_callback.h"
#include "service/task.h"

Task<> accepter(int fd);

class HandleAccept : public UringCallBack {
    std::coroutine_handle<> h_;
    int listenfd_;

    void call(IoUring& ring, IoUringCqe& cqe) override;

public:
    HandleAccept(int listenfd) : listenfd_(listenfd)
    {
        auto sqe = Threadlocal_ring().GetSqe();
        sqe.prep_multishot_accept(listenfd, nullptr, nullptr, 0);
        sqe.set_Data64(std::bit_cast<__u64>(this));
    };

    bool await_ready() noexcept { return false; }
    auto await_suspend(std::coroutine_handle<> h) noexcept
    {
        h_ = h;
        Threadlocal_ring().Submit();
    }
    auto await_resume() {}
};

#endif /* __HELPER_ACCEPTOR__ */
