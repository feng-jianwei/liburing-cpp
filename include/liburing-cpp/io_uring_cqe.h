#ifndef __LIBURING_CPP_IO_URING_CQE__
#define __LIBURING_CPP_IO_URING_CQE__
#include <linux/types.h>

class IoUringCqe {
    friend class IoUring;
    struct io_uring_cqe* cqe_;
    explicit IoUringCqe(struct io_uring_cqe* cqe) : cqe_{cqe} {}

public:
    IoUringCqe() = delete;
    ~IoUringCqe() = default;

    IoUringCqe(const IoUringCqe&) = delete;
    IoUringCqe& operator=(const IoUringCqe&) = delete;

    IoUringCqe(IoUringCqe&&) noexcept = default;
    IoUringCqe& operator=(IoUringCqe&&) noexcept = default;

    [[nodiscard]] __s32 Res() const;
    [[nodiscard]] __u32 Flags() const;
    [[nodiscard]] __u64 GetData64() const;
};

#endif /* __LIBURING_CPP_IO_URING_CQE__ */
