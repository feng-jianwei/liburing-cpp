
#include <liburing.h>
#include <liburing/io_uring.h>
#include "liburing-cpp/future_probe.h"

#include <iostream>
#include <magic_enum.hpp>
#include <system_error>

void PrintUringProbe()
{
    io_uring_params p = {};
    io_uring ring{};
    int ret = io_uring_queue_init_params(1, &ring, &p);

    if (ret < 0) {
        const char* what = "io_uring_queue_init() failed";
        throw std::system_error(-ret, std::generic_category(), what);
    }
    auto* probe = io_uring_get_probe_ring(&ring);
    std::cout << "Kernel do not support op below:" << std::endl;;
    for (auto e : magic_enum::enum_values<io_uring_op>()) {
        int i = 0;
        for (i = 0; i < probe->ops_len; ++i) {
            if (probe->ops[i].op == magic_enum::enum_integer(e) && probe->ops[i].flags & IO_URING_OP_SUPPORTED) {
                break;
            }
        }
        if (i == probe->ops_len) {
            std::cout << "\t" << magic_enum::enum_name(e) << "\n";
        }
    }
    io_uring_free_probe(probe);
    io_uring_queue_exit(&ring);
}