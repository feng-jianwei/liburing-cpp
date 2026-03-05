#ifndef __LIBURING_CPP_IO_URING__
#define __LIBURING_CPP_IO_URING__
#include <optional>
#include <unordered_map>

#include "io_uring_cqe.h"
#include "io_uring_sqe.h"
#include "liburing.h"

class IoBufferRing {
public:
    using BufferRingAddr = struct io_uring_buf_ring*;
    BufferRingAddr addr_{};
    unsigned int bufferSize_{};
    unsigned int count_{};
};

class IoUring {
    io_uring ring_{};
    using gid = int;
    std::unordered_map<gid, IoBufferRing> gidToBuffer;

public:
    explicit IoUring(unsigned int entries);

    ~IoUring();

    IoUring(const IoUring&) = delete;
    IoUring& operator=(const IoUring&) = delete;

    IoUring(IoUring&&) = delete;
    IoUring& operator=(IoUring&&) = delete;

    bool SetupRingBuffer(gid bgid, unsigned int bufSize, unsigned int bufferCount);

    IoUringSqe GetSqe();

    void Submit();

    IoUringCqe PeekCqe();

    std::optional<IoUringCqe> WaitCqe();

    void SeenCqe(const IoUringCqe& cqe) { io_uring_cqe_seen(&ring_, cqe.cqe_); }
};

IoUring& Threadlocal_ring();

void PrintUringProbe();

#endif /* __LIBURING_CPP_IO_URING__ */
