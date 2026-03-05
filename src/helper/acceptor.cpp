#include "service/helper/acceptor.h"
#include <thread>

#include "service/helper/closer.h"
#include "service/helper/reader.h"
#include "service/helper/writer.h"
#include "liburing.h"

Task<> accepter(int fd)
{
    bool keep_alive = true;

    while (keep_alive) {
        auto read_str = co_await Reader(fd);
        if (!read_str.has_value()) {
            break;
        }
        co_await Writer(fd, std::move(read_str.value()));
    }
    co_await Closer(fd);
}

void HandleAccept::call(IoUring& ring, IoUringCqe& cqe)
{
    auto clientfd = cqe.Res();
    auto flags = cqe.Flags();
    std::cout << std::format("accept new conn : {} current thread: {}\n", clientfd,
                             std::hash<std::thread::id>()(std::this_thread::get_id()));
    ring.SeenCqe(cqe);
    if ((flags & IORING_CQE_F_MORE) == 0) {
        auto sqe = ring.GetSqe();
        sqe.prep_multishot_accept(listenfd_, nullptr, 0, 0);
        sqe.set_flags(IORING_CQE_F_MORE);
        sqe.set_Data64(std::bit_cast<__u64>(this));
        ring.Submit();
    }

    if (clientfd < 0) {
        std::cout << std::format("accept CQE failed: {}\n", std::strerror(-clientfd));
        return;
    }
    accepter(clientfd);
}