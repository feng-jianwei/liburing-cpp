#ifndef __SERVICE_TASK__
#define __SERVICE_TASK__

#include <coroutine>
#include <exception>
#include <iostream>

template <typename T = void>
class Task {
public:
    class promise_type;
    auto operator co_await() noexcept;
    explicit Task(std::coroutine_handle<promise_type> coroutine) : coroutine_{coroutine} {}

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&&) noexcept = default;
    Task& operator=(Task&&) noexcept = default;

    ~Task()
    {
        if (coroutine_ && coroutine_.done()) {
            Destroy();
        }
    }
    void Destroy()
    {
        coroutine_.destroy();
        coroutine_ = nullptr;
    }

private:
    std::coroutine_handle<promise_type> coroutine_;
};

template <typename T>
class Task<T>::promise_type {
    friend Task;
    std::coroutine_handle<> parent_coroutine_;
    T return_value_;

public:
    Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(*this)); }

    std::suspend_never initial_suspend() { return {}; }

    auto final_suspend() noexcept
    {
        struct Await {
            std::coroutine_handle<> h;
            bool await_ready() noexcept { return h == nullptr; }

            std::coroutine_handle<> await_suspend(std::coroutine_handle<Task::promise_type> h) noexcept { return h; }
            void await_resume() const noexcept {}
        };
        return Await{parent_coroutine_};
    }

    void return_value(T&& value) { return_value_ = value; }

    void unhandled_exception()
    {
        try {
            std::rethrow_exception(std::current_exception());
        } catch (const std::exception& e) {
            std::cout << "unhandled exception in Task : " << e.what() << std::endl;
        }
    }
};

template <>
class Task<void>::promise_type {
    friend Task;
    std::coroutine_handle<> parent_coroutine_;

public:
    Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(*this)); }

    std::suspend_never initial_suspend() { return {}; }

    auto final_suspend() noexcept
    {
        struct Await {
            std::coroutine_handle<> h;
            bool await_ready() noexcept { return h == nullptr; }

            std::coroutine_handle<> await_suspend(std::coroutine_handle<Task::promise_type> h) noexcept { return h; }
            void await_resume() const noexcept {}
        };
        return Await{parent_coroutine_};
    }

    void return_void() {}

    void unhandled_exception()
    {
        try {
            std::rethrow_exception(std::current_exception());
        } catch (const std::exception& e) {
            std::cout << "unhandled exception in Task : " << e.what() << std::endl;
        }
    }
};

template <typename T>
inline auto Task<T>::operator co_await() noexcept
{
    struct Awaiter {
        std::coroutine_handle<promise_type> h;

        bool await_ready() const noexcept { return h.done(); }

        void await_suspend(std::coroutine_handle<> coroutine) noexcept { h.promise().parent_coroutine_ = coroutine; }

        auto await_resume() -> decltype(auto) { return std::move(h.promise().return_value_); }
    };

    return Awaiter{this->coroutine_};
}

template <>
inline auto Task<void>::operator co_await() noexcept
{
    struct Awaiter {
        std::coroutine_handle<promise_type> h;

        bool await_ready() const noexcept { return h.done(); }

        void await_suspend(std::coroutine_handle<> coroutine) noexcept { h.promise().parent_coroutine_ = coroutine; }

        void await_resume() {}
    };

    return Awaiter{this->coroutine_};
}

#endif /* __SERVICE_TASK__ */
