#include "io_uring_sqe.h"

#include "liburing.h"

void IoUringSqe::set_Data64(__u64 data) { io_uring_sqe_set_data64(sqe_, data); }

void IoUringSqe::set_flags(unsigned int flags) { io_uring_sqe_set_flags(sqe_, flags); }

void IoUringSqe::prep_multishot_accept(int fd, sockaddr* addr, socklen_t* addrlen, int flags)
{
    io_uring_prep_multishot_accept(sqe_, fd, addr, addrlen, flags);
}

void IoUringSqe::prep_link_timeout(__kernel_timespec* ts) { io_uring_prep_link_timeout(sqe_, ts, 0); }

void IoUringSqe::prep_close(int fd) { io_uring_prep_close(sqe_, fd); }

void IoUringSqe::prep_read(int fd, void* buf, unsigned int nbytes, __u64 offset)
{
    io_uring_prep_read(sqe_, fd, buf, nbytes, offset);
}
void IoUringSqe::prep_write(int fd, void* buf, unsigned int nbytes, int flags)
{
    io_uring_prep_write(sqe_, fd, buf, nbytes, flags);
}

void IoUringSqe::prep_send(int sockfd, void* buf, size_t len, int flags)
{
    io_uring_prep_send(sqe_, sockfd, buf, len, flags);
}

void IoUringSqe::prep_recv(int sockfd, void* buf, size_t len, int flags)
{
    io_uring_prep_recv(sqe_, sockfd, buf, len, flags);
}

void IoUringSqe::pre_poll_add(int fd, unsigned int poll_mask) { io_uring_prep_poll_add(sqe_, fd, poll_mask); }

void IoUringSqe::pre_bind(int fd, const struct sockaddr* addr, socklen_t addrlen)
{
    io_uring_prep_bind(sqe_, fd, addr, addrlen);
}

void IoUringSqe::pre_listen(int fd, int backlog) { io_uring_prep_listen(sqe_, fd, backlog); }