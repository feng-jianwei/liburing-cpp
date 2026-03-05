#include <arpa/inet.h>  // 提供 inet_pton(), inet_addr(), inet_ntoa() 等（可选，但常用）
#include <fcntl.h>
#include <netinet/in.h>  // 提供 struct sockaddr_in, sockaddr_in6, AF_INET, htons 等
#include <sys/socket.h>  // 提供 struct sockaddr, socket(), bind() 等
#include <unistd.h>      // 提供 close

#include <atomic>
#include <bit>
#include <csignal>
#include <cstddef>
#include <iostream>
#include <string>

#include "liburing-cpp/io_uring.h"
#include "service/helper/acceptor.h"
#include "service/helper/closer.h"
#include "service/service.h"

using namespace std;

Task<> EchoServer(std::string address, int port)
{
    auto isIpV4 = address.find('.') != string::npos;
    auto domain = isIpV4 ? AF_INET : AF_INET6;
    int listen_fd = socket(domain, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        cout << "socket() failed" << endl;
        co_return;
    }
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    sockaddr_storage ss{};
    socklen_t len = sizeof(ss);
    if (!isIpV4) {
        auto sin6 = (sockaddr_in6*) &ss;
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(port);
        inet_pton(AF_INET6, address.c_str(), &sin6->sin6_addr);
        len = sizeof(sockaddr_in6);
    } else {
        auto sin = (sockaddr_in*) &ss;
        sin->sin_family = AF_INET;
        sin->sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &sin->sin_addr);
        len = sizeof(sockaddr_in);
    }
    // 
    if (0 > ::bind(listen_fd, (struct sockaddr*) &ss, len)) {
        cout << std::format("bind {} failed\n", address);
        co_await Closer(listen_fd);
        co_return;
    }
    if (0 > listen(listen_fd, SOMAXCONN)) {
        cout << format("listen {} failed\n", address);
        co_await Closer(listen_fd);
        co_return;
    }
    co_await HandleAccept(listen_fd);
    co_return;
}

std::atomic<bool> g_running{true};

void signal_handler(int sig)
{
    static_assert(decltype(g_running)::is_always_lock_free, "g_running must be lock-free");
    if (sig == SIGINT || sig == SIGTERM) {
        g_running.store(false, std::memory_order_relaxed);
    }
}

void EchoService(std::string address, int port)
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    auto task = EchoServer(address, port);
    auto& ring = Threadlocal_ring();
    while (g_running.load(memory_order_relaxed)) {
        auto cqe = ring.WaitCqe();
        if (!cqe.has_value()) {
            continue;
        }
        if (auto handler = std::bit_cast<UringCallBack*>(cqe.value().GetData64()); handler != nullptr) {
            handler->call(ring, cqe.value());
        }
    }
    task.Destroy();
}