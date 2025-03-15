
//          Copyright Jared Irwin 2022-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "coroutine_headers.hpp"

#if !defined(LJH_TARGET_Windows)
#error "This file requires Windows."
#else
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>

namespace ljh::co
{
    template<typename TResult>
    struct winrt_task : winrt::Windows::Foundation::IInspectable,
                        winrt::impl::consume_t<winrt_task<TResult>>,
                        winrt::impl::require<winrt_task<TResult>, winrt::Windows::Foundation::IAsyncInfo>
    {
        winrt_task(std::nullptr_t = nullptr) noexcept
        {}
        winrt_task(void* ptr, winrt::take_ownership_from_abi_t) noexcept
            : winrt::Windows::Foundation::IInspectable(ptr, winrt::take_ownership_from_abi)
        {}
    };

    template<typename TResult>
    inline winrt::impl::await_adapter<winrt_task<TResult>> operator co_await(winrt_task<TResult> const& async)
    {
        return {async};
    }
} // namespace ljh::co

namespace ljh::__::co
{
    template<typename TResult>
    struct winrt_task_completed_handler : winrt::Windows::Foundation::IUnknown
    {
        winrt_task_completed_handler(std::nullptr_t = nullptr) noexcept
        {}
        winrt_task_completed_handler(void* ptr, winrt::take_ownership_from_abi_t) noexcept
            : winrt::Windows::Foundation::IUnknown(ptr, winrt::take_ownership_from_abi)
        {}

        template<typename L>
        winrt_task_completed_handler(L lambda)
            : winrt_task_completed_handler(winrt::impl::make_delegate<winrt_task_completed_handler<TResult>>(std::forward<L>(lambda)))
        {}
        template<typename F>
        winrt_task_completed_handler(F* function)
            : winrt_task_completed_handler([=](auto&&... args) { return function(args...); })
        {}
        template<typename O, typename M>
        winrt_task_completed_handler(O* object, M method)
            : winrt_task_completed_handler([=](auto&&... args) { return ((*object).*(method))(args...); })
        {}
        template<typename O, typename M>
        winrt_task_completed_handler(winrt::com_ptr<O>&& object, M method)
            : winrt_task_completed_handler([o = std::move(object), method](auto&&... args) { return ((*o).*(method))(args...); })
        {}
        template<typename O, typename M>
        winrt_task_completed_handler(winrt::weak_ref<O>&& object, M method)
            : winrt_task_completed_handler([o = std::move(object), method](auto&&... args) {
                if (auto s = o.get())
                {
                    ((*s).*(method))(args...);
                }
            })
        {}

        auto operator()(ljh::co::winrt_task<TResult> const& asyncInfo, winrt::Windows::Foundation::AsyncStatus const& asyncStatus) const
        {
            check_hresult(
                (*(winrt::impl::abi_t<winrt_task_completed_handler<TResult>>**)this)->Invoke(*(void**)(&asyncInfo), static_cast<int32_t>(asyncStatus)));
        }
    };

    template<typename D, typename TResult>
    struct consume_task
    {
        template<typename T>
        decltype(auto) cast_self() const noexcept
        {
            return static_cast<T const&>(static_cast<D const&>(*this));
        }

        template<typename T>
        decltype(auto) shim() const noexcept
        {
            return (*(winrt::impl::abi_t<T>**)&(this->cast_self<T>()));
        }

        auto Completed(winrt_task_completed_handler<TResult> const& handler) const
        {
            winrt::check_hresult(shim<ljh::co::winrt_task<TResult>>()->put_Completed(*(void**)(&handler)));
        }

        [[nodiscard]] auto Completed() const
        {
            void* winrt_impl_result{};
            winrt::check_hresult(shim<ljh::co::winrt_task<TResult>>()->get_Completed(&winrt_impl_result));
            return winrt_task_completed_handler<TResult>{winrt_impl_result, winrt::take_ownership_from_abi};
        }

        auto GetResults() const
        {
            TResult winrt_impl_result{winrt::impl::empty_value<TResult>()};
            winrt::check_hresult(shim<ljh::co::winrt_task<TResult>>()->GetResults(put_abi(winrt_impl_result)));
            return winrt_impl_result;
        }

        auto get() const
        {
            return winrt::impl::wait_get(cast_self<ljh::co::winrt_task<TResult>>());
        }

        auto wait_for(winrt::Windows::Foundation::TimeSpan const& timeout) const
        {
            return winrt::impl::wait_for(cast_self<ljh::co::winrt_task<TResult>>(), timeout);
        }
    };
} // namespace ljh::__::co

template<typename TResult>
struct winrt::impl::category<ljh::co::winrt_task<TResult>>
{
    using type = interface_category;
};
template<typename TResult>
inline constexpr auto& winrt::impl::name_v<ljh::co::winrt_task<TResult>> = L"winrt_task";
template<typename TResult>
inline constexpr winrt::guid winrt::impl::guid_v<ljh::co::winrt_task<TResult>>{"7221256A-FB66-44FB-8526-D07565FF6102"};
template<typename TResult>
struct winrt::impl::abi<ljh::co::winrt_task<TResult>>
{
    struct __declspec(novtable) type : inspectable_abi
    {
        virtual int32_t __stdcall put_Completed(void*) noexcept         = 0;
        virtual int32_t __stdcall get_Completed(void**) noexcept        = 0;
        virtual int32_t __stdcall GetResults(arg_out<TResult>) noexcept = 0;
    };
};
template<typename TResult>
struct winrt::impl::consume<ljh::co::winrt_task<TResult>>
{
    template<typename D>
    using type = ljh::__::co::consume_task<D, TResult>;
};
template<typename TResult>
struct winrt::impl::async_completed_handler<ljh::co::winrt_task<TResult>>
{
    using type = ljh::__::co::winrt_task_completed_handler<TResult>;
};

template<typename TResult>
struct winrt::impl::category<ljh::__::co::winrt_task_completed_handler<TResult>>
{
    using type = interface_category;
};
template<typename TResult>
inline constexpr auto& winrt::impl::name_v<ljh::__::co::winrt_task_completed_handler<TResult>> = L"winrt_task_completed_handler";
template<typename TResult>
inline constexpr winrt::guid winrt::impl::guid_v<ljh::__::co::winrt_task_completed_handler<TResult>>{"3C736D26-5C53-46D7-AF11-14F91C834852"};
template<typename TResult>
struct winrt::impl::abi<ljh::__::co::winrt_task_completed_handler<TResult>>
{
    struct __declspec(novtable) type : unknown_abi
    {
        virtual int32_t __stdcall Invoke(void*, int32_t) noexcept = 0;
    };
};

template<typename D, typename TResult>
struct winrt::impl::produce<D, ljh::co::winrt_task<TResult>> : winrt::impl::produce_base<D, ljh::co::winrt_task<TResult>>
{
    int32_t __stdcall put_Completed(void* handler) noexcept final
    try
    {
        typename D::abi_guard guard(this->shim());
        this->shim().Completed(*reinterpret_cast<ljh::__::co::winrt_task_completed_handler<TResult> const*>(&handler));
        return 0;
    }
    catch (...)
    {
        return to_hresult();
    }

    int32_t __stdcall get_Completed(void** winrt_impl_result) noexcept final
    try
    {
        clear_abi(winrt_impl_result);
        typename D::abi_guard guard(this->shim());
        *winrt_impl_result = detach_from<ljh::__::co::winrt_task_completed_handler<TResult>>(this->shim().Completed());
        return 0;
    }
    catch (...)
    {
        return to_hresult();
    }

    int32_t __stdcall GetResults(arg_out<TResult> winrt_impl_result) noexcept final
    try
    {
        clear_abi(winrt_impl_result);
        typename D::abi_guard guard(this->shim());
        *winrt_impl_result = detach_from<TResult>(this->shim().GetResults());
        return 0;
    }
    catch (...)
    {
        return to_hresult();
    }
};

template<typename H, typename TResult>
struct winrt::impl::delegate<ljh::__::co::winrt_task_completed_handler<TResult>, H> final
    : winrt::impl::implements_delegate<ljh::__::co::winrt_task_completed_handler<TResult>, H>
{
    delegate(H&& handler)
        : implements_delegate<ljh::__::co::winrt_task_completed_handler<TResult>, H>(std::forward<H>(handler))
    {}

    int32_t __stdcall Invoke(void* asyncInfo, int32_t asyncStatus) noexcept final
    try
    {
        (*this)(*reinterpret_cast<ljh::co::winrt_task<TResult> const*>(&asyncInfo),
                *reinterpret_cast<winrt::Windows::Foundation::AsyncStatus const*>(&asyncStatus));
        return 0;
    }
    catch (...)
    {
        return to_hresult();
    }
};

template<typename TResult, typename... Args>
struct std::coroutine_traits<ljh::co::winrt_task<TResult>, Args...>
{
    struct promise_type final : winrt::impl::promise_base<promise_type, ljh::co::winrt_task<TResult>>
    {
        TResult get_return_value() noexcept
        {
            return std::move(m_result);
        }

        TResult copy_return_value() noexcept
        {
            return m_result;
        }

        void return_value(TResult&& value) noexcept
        {
            m_result = std::move(value);
        }

        void return_value(TResult const& value) noexcept
        {
            m_result = value;
        }

        TResult m_result{winrt::impl::empty_value<TResult>()};
    };
};
#endif