#ifndef __LIBURING_CPP_IO_URING_SQE__
#define __LIBURING_CPP_IO_URING_SQE__

#include <linux/types.h>
#include <sys/socket.h>
#include <linux/time_types.h>

class IoUringSqe {
    friend class IoUring;
    struct io_uring_sqe* sqe_;
    explicit IoUringSqe(io_uring_sqe* sqe) : sqe_{sqe} {}

public:
    ~IoUringSqe() = default;

    IoUringSqe(const IoUringSqe&) = delete;
    IoUringSqe& operator=(const IoUringSqe&) = delete;

    IoUringSqe(IoUringSqe&&) noexcept = default;
    IoUringSqe& operator=(IoUringSqe&&) noexcept = default;

    void set_Data64(__u64 data);

    void set_flags(unsigned int flags);

    void prep_multishot_accept(int fd, sockaddr* addr, socklen_t* addrlen, int flags);

    void prep_link_timeout(__kernel_timespec* ts);

    void prep_close(int fd);

    void prep_read(int fd, void* buf, unsigned int nbytes, __u64 offset);

    void prep_write(int fd, void* buf, unsigned int nbytes, int flags);

    void prep_send(int sockfd, void* buf, size_t len, int flags);
    void prep_recv(int sockfd, void* buf, size_t len, int flags);
    void pre_poll_add(int fd, unsigned int poll_mask);
    void pre_bind(int fd, const struct sockaddr* addr, socklen_t addrlen);

    void pre_listen(int fd, int backlog);
};

#endif /* __LIBURING_CPP_IO_URING_SQE__ */
