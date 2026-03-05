#include <sys/timerfd.h>
#include "service/helper/timer.h"

Timer::Timer(time_t seconds, time_t ns)
{
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    struct itimerspec its = {};
    its.it_value.tv_sec = seconds;
    its.it_value.tv_nsec = ns;
    timerfd_settime(tfd, 0, &its, NULL);

    auto sqe = Threadlocal_ring().GetSqe();
    sqe.pre_poll_add(tfd, POLL_IN);
    sqe.set_Data64(std::bit_cast<__u64>(this));
}

void Timer::await_suspend(std::coroutine_handle<> h) noexcept
{
    h_ = h;
    Threadlocal_ring().Submit();
}