#ifndef __HELPER_READER__
#define __HELPER_READER__

#include <coroutine>
#include <string>

#include "liburing-cpp/io_uring_callback.h"

class Reader : public UringCallBack {
    std::coroutine_handle<> h_;
    std::unique_ptr<char[]> str = std::make_unique<char[]>(1024);
    int strlen_ = 0;

    void call(IoUring& ring, IoUringCqe& cqe) override
    {
        strlen_ = cqe.Res();
        h_.resume();
        ring.SeenCqe(cqe);
    }

public:
    Reader(int fd)
    {
        auto sqe = Threadlocal_ring().GetSqe();
        sqe.prep_read(fd, str.get(), 1024, 0);
        sqe.set_Data64(std::bit_cast<__u64>(this));
    };

    bool await_ready() noexcept { return false; }
    auto await_suspend(std::coroutine_handle<> h) noexcept
    {
        h_ = h;
        Threadlocal_ring().Submit();
    }
    std::optional<std::string> await_resume()
    {
        if (strlen_ > 0) {
            return std::string(str.get(), strlen_);
        }
        return {};
    }
};

#endif /* __HELPER_READER__ */
