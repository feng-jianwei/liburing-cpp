#ifndef __URING_IO_URING_CQE__
#define __URING_IO_URING_CQE__
#include "liburing.h"

class IoUringCqe {
    friend class IoUring;
    io_uring_cqe* cqe_;
    explicit IoUringCqe(io_uring_cqe* cqe) : cqe_{cqe} {}

public:
    IoUringCqe() = delete;
    ~IoUringCqe() = default;

    IoUringCqe(const IoUringCqe&) = delete;
    IoUringCqe& operator=(const IoUringCqe&) = delete;

    IoUringCqe(IoUringCqe&&) noexcept = default;
    IoUringCqe& operator=(IoUringCqe&&) noexcept = default;

    [[nodiscard]] __s32 Res() const { return cqe_->res; }

    [[nodiscard]] __u32 Flags() const { return cqe_->flags; }

    [[nodiscard]] __u64 GetData64() const { return io_uring_cqe_get_data64(cqe_); }
};

#endif /* __URING_IO_URING_CQE__ */
