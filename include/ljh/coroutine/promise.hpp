
//          Copyright Jared Irwin 2021-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "task_policies.hpp"
#include "coroutine_headers.hpp"

#include <atomic>
#include <exception>
#include <memory>
#include <bit>
#include <cassert>

namespace ljh::__::co
{
    template<typename T>
    struct promise;

    template<typename T>
    struct promise_value
    {
        T value;

        T&& get_value()
        {
            return static_cast<T&&>(value);
        }
    };

    template<>
    struct promise_value<void>
    {
        void get_value()
        {}
    };

    template<typename T>
    struct promise_result_holder
    {
        enum class result_status
        {
            empty,
            value,
            error
        };

        result_status status{result_status::empty};

        union result_holder
        {
            result_holder()
            {}
            ~result_holder()
            {}
            promise_value<T>   wrapper;
            std::exception_ptr error;
        } result;

        template<typename... Args>
        void set_value(Args&&... args)
        {
            new (std::addressof(result.wrapper)) promise_value<T>{std::forward<Args>(args)...};
            status = result_status::value;
        }

        void unhandled_exception() noexcept
        {
            new (std::addressof(result.error)) std::exception_ptr(std::current_exception());
            status = result_status::error;
        }

        T get_value()
        {
            switch (status)
            {
            case result_status::value: 
                return result.wrapper.get_value();
            case result_status::error: 
                std::rethrow_exception(std::exchange(result.error, {}));
                break;
            default:
                assert(false);
                std::terminate();
                break;
            }
        }

        ~promise_result_holder()
        {
            switch (status)
            {
            case result_status::value: 
                result.wrapper.~promise_value(); 
                break;
            case result_status::error:
                if (result.error)
                    std::rethrow_exception(result.error);
                result.error.~exception_ptr();
                break;
            default:
                // Do nothing
                break;
            }
        }
    };

    template<typename T>
    struct promise_base
    {
        static constexpr void*          running_ptr   = nullptr;
        static constexpr std::uintptr_t completed_ptr = 1;
        static constexpr std::uintptr_t abandoned_ptr = 2;
        static constexpr std::uintptr_t cold_ptr      = 3;

        void (*m_resumer)(void*);
        std::atomic<void*>       m_waiting{reinterpret_cast<void*>(cold_ptr)};
        promise_result_holder<T> m_holder;
        promise_policies         m_policies;

        promise_base()                      = default;
        promise_base(promise_base const&)   = delete;
        void operator=(promise_base const&) = delete;

        void resume_waiting_coroutine(void* waiting) const
        {
            if (m_resumer)
            {
                m_resumer(waiting);
            }
            else
            {
                std::coroutine_handle<>::from_address(waiting).resume();
            }
        }

        using Promise = promise<T>;
        auto as_promise() noexcept
        {
            return static_cast<Promise*>(this);
        }

        auto as_handle() noexcept
        {
            return std::coroutine_handle<Promise>::from_promise(*as_promise());
        }

        void destroy()
        {
            as_handle().destroy();
        }

        void abandon()
        {
            auto waiting = m_waiting.exchange(reinterpret_cast<void*>(abandoned_ptr), std::memory_order_acquire);
            if (waiting != running_ptr)
                destroy();
        }

        auto get_return_object() noexcept
        {
            return as_promise();
        }

        void start()
        {
            m_waiting.store(running_ptr, std::memory_order_relaxed);
            as_handle().resume();
        }

        std::suspend_always initial_suspend() noexcept
        {
            return {};
        }

        template<typename... Args>
        void set_value(Args&&... args)
        {
            m_holder.set_value(std::forward<Args>(args)...);
        }

        void unhandled_exception() noexcept
        {
            if (m_policies.m_terminate_on_unhandled_exception)
            {
                std::terminate();
            }
            m_holder.unhandled_exception();
        }

        auto await_transform(ljh::co::get_task_policy) noexcept
        {
            struct awaiter : std::suspend_never
            {
                promise_policies& policies;
                auto              await_resume() const noexcept
                {
                    return ljh::co::task_policy(policies);
                }
            };
            return awaiter{{}, m_policies};
        }

        template<typename U>
        U&& await_transform(U&& awaitable) noexcept
        {
            return static_cast<U&&>(awaitable);
        }

        auto final_suspend() noexcept
        {
            struct awaiter : std::suspend_always
            {
                promise_base& self;
                void          await_suspend(std::coroutine_handle<> /* handle */) const noexcept
                {
                    auto waiting = self.m_waiting.exchange(reinterpret_cast<void*>(completed_ptr), std::memory_order_acq_rel);
                    if (waiting == reinterpret_cast<void*>(abandoned_ptr))
                        self.destroy();
                    if (waiting != running_ptr)
                        self.resume_waiting_coroutine(waiting);
                }
            };
            return awaiter{{}, *this};
        }

        template<bool COLD = false>
        bool client_await_ready()
        {
            if constexpr (!COLD)
            {
                auto waiting = m_waiting.load(std::memory_order_acquire);
                assert(waiting == running_ptr || waiting == reinterpret_cast<void*>(completed_ptr));
                return waiting != running_ptr;
            }
            else
            {
                return false;
            }
        }

        template<bool COLD = false>
        auto client_await_suspend(void* waiting, void (*resumer)(void*) = nullptr)
        {
            if constexpr (!COLD)
            {
                m_resumer = resumer;
                assert(reinterpret_cast<uintptr_t>(waiting) > reinterpret_cast<uintptr_t>(cold_ptr));
                return m_waiting.exchange(waiting, std::memory_order_acq_rel) == running_ptr;
            }
            else
            {
                start();
                return client_await_suspend(waiting, resumer);
            }
        }

        T client_await_resume()
        {
            return m_holder.get_value();
        }
    };

    template<typename T>
    struct promise : promise_base<T>
    {
        void return_value(T&& value)
        {
            this->set_value(std::forward<T>(value));
        }

        template<typename Dummy = void>
        std::enable_if_t<!std::is_reference_v<T>, Dummy> return_value(T const& value)
        {
            this->set_value(value);
        }
    };

    template<>
    struct promise<void> : promise_base<void>
    {
        void return_void()
        {
            this->set_value();
        }
    };

    template<typename T>
    struct promise_deleter
    {
        void operator()(promise_base<T>* promise) const noexcept
        {
            promise->abandon();
        }
    };

    template<typename T>
    using promise_ptr = std::unique_ptr<promise_base<T>, promise_deleter<T>>;
} // namespace ljh::__::co
