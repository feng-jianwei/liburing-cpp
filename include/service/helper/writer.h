#ifndef __HELPER_WRITER__
#define __HELPER_WRITER__

#include <coroutine>
#include <string>

#include "liburing-cpp/io_uring_callback.h"
using namespace std;

class Writer : public UringCallBack {
    std::coroutine_handle<> h_;
    int strlen_;

public:
    void call(IoUring& ring, IoUringCqe& cqe) override
    {
        strlen_ = cqe.Res();
        h_.resume();
        ring.SeenCqe(cqe);
    }

public:
    Writer(int fd, string&& str)
    {
        auto sqe = Threadlocal_ring().GetSqe();
        sqe.prep_write(fd, (void*) str.data(), str.length(), 0);
        sqe.set_Data64(std::bit_cast<__u64>(this));
    };

    bool await_ready() noexcept { return false; }
    auto await_suspend(std::coroutine_handle<> h) noexcept
    {
        h_ = h;
        Threadlocal_ring().Submit();
    }
    auto await_resume() { return strlen_; }
};

#endif /* __HELPER_WRITER__ */
