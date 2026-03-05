#include <liburing.h>
#include <liburing/io_uring.h>
#include <sys/mman.h>

#include <atomic>
#include <new>
#include <system_error>

#include "liburing-cpp/io_uring.h"

alignas(std::hardware_constructive_interference_size) std::atomic<unsigned int> ringfd(0);

IoUring::IoUring(unsigned int entries)
{
    struct io_uring_params p{};

    if (!ringfd.compare_exchange_strong(p.wq_fd, 0, std::memory_order::relaxed, std::memory_order::relaxed)) {
        p.flags |= IORING_SETUP_ATTACH_WQ;
    }
    int ret = io_uring_queue_init_params(entries, &ring_, &p);
    ringfd.store(ring_.ring_fd, std::memory_order_relaxed);
    if (ret < 0) {
        const char* what = "io_uring_queue_init() failed";
        throw std::system_error(-ret, std::generic_category(), what);
    }
}

bool IoUring::SetupRingBuffer(gid bgid, unsigned int bufSize, unsigned int bufferCount)
{
    if (gidToBuffer.contains(bgid)) {
        return false;
    }
    auto ret = 0;
    auto br = io_uring_setup_buf_ring(&ring_, bufferCount, bgid, 0, &ret);
    if (ret || br == nullptr) {
        return false;
    }
    auto buf = (char*) mmap(NULL, bufSize * bufferCount, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE,  // MAP_POPULATE 预分配物理页
                            -1, 0);
    for (unsigned i = 0; i < bufferCount; i++) {
        io_uring_buf_ring_add(br, buf + i * bufSize, bufSize, bgid, io_uring_buf_ring_mask(bufferCount), i);
    }
    // 4. 告诉内核：我加完了，更新尾指针
    io_uring_buf_ring_advance(br, bufferCount);
    gidToBuffer.emplace(bgid, IoBufferRing{br, bufSize, bufferCount});
    return true;
}

IoUring::~IoUring()
{
    for (auto& [bgid, br] : gidToBuffer) {
        io_uring_free_buf_ring(&ring_, br.addr_, br.count_, bgid);
        munmap(br.addr_, br.bufferSize_ * br.count_);
    }
    io_uring_queue_exit(&ring_);
}

IoUringSqe IoUring::GetSqe()
{
    io_uring_sqe* sqe = io_uring_get_sqe(&ring_);
    if (sqe == nullptr) {
        throw std::runtime_error("io_uring_get_sqe() returned nullptr");
    }

    return IoUringSqe(sqe);
}

void IoUring::Submit()
{
    int ret = io_uring_submit(&ring_);

    if (ret < 0) {
        const char* what = "io_uring_submit() failed";
        throw std::system_error(-ret, std::generic_category(), what);
    }
}

IoUringCqe IoUring::PeekCqe()
{
    io_uring_cqe* cqe;
    int ret = io_uring_peek_cqe(&ring_, &cqe);

    if (ret < 0) {
        const char* what = "io_uring_wait_cqe() failed";
        throw std::system_error(-ret, std::generic_category(), what);
    }

    return IoUringCqe(cqe);
}

std::optional<IoUringCqe> IoUring::WaitCqe()
{
    io_uring_cqe* cqe;
    int ret = io_uring_wait_cqe(&ring_, &cqe);
    if (ret < 0) {
        return {};
    }

    return IoUringCqe(cqe);
}

IoUring& Threadlocal_ring()
{
    thread_local IoUring ring(256);
    return ring;
}