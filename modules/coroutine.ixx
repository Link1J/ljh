
//          Copyright Jared Irwin 2025-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#include "ljh/coroutine.hpp"
#include "ljh/channel.hpp"
#include "ljh/generator.hpp"
#include "ljh/token_bucket.hpp"
export module ljh.coroutine;
export namespace ljh {
	namespace co {
		using ljh::co::task_policy;
		using ljh::co::fire_and_forget;
		using ljh::co::task;
		using ljh::co::cold_task;
#if defined(LJH_TARGET_Windows)
		using ljh::co::com_aware_task;
		using ljh::co::winrt_task;
#endif
		using ljh::co::shared_mutex;
		using ljh::co::no_await;
		using ljh::co::invoke_lambda;
		using ljh::co::get_task_policy;
		using ljh::co::swap;
	}
	using ljh::channel;
	using ljh::generator;
	using ljh::token_bucket;
}