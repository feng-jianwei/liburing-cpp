#include "io_uring_cqe.h"

#include <liburing.h>

[[nodiscard]] __s32 IoUringCqe::Res() const { return cqe_->res; }

[[nodiscard]] __u32 IoUringCqe::Flags() const { return cqe_->flags; }

[[nodiscard]] __u64 IoUringCqe::GetData64() const { return io_uring_cqe_get_data64(cqe_); }