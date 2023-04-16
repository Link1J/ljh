
//          Copyright Jared Irwin 2020-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// dbus.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++20
// 
// ABOUT
//     A wrapper for dbus
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include <dbus/dbus.h>

#include <string>
#include <vector>
#include <variant>
#include <tuple>
#include <map>
#include <stdexcept>
#include <thread>
#include <span>
#include <any>
#include <typeindex>
#include <utility>

#include <cstring>

#include <ljh/type_traits.hpp>
#include <ljh/casting.hpp>
#include <ljh/compile_time_string.hpp>
#include <ljh/function_traits.hpp>

#undef unix

namespace ljh::unix::dbus
{
	class connection;
	class error;
	class interface;
	class message;
	class object;

	template<typename... T>
	struct multiout
	{
		std::tuple<T...> data;

		multiout(T... args)
			: data(args...)
		{}
	};

	struct expand_type {
		template<typename... T>
		expand_type(T&&...) {}
	};

	template<template<int> class W, std::size_t... I, typename...A>
	void caller_impl(std::index_sequence<I...>, A&&... args) {
		int t[] = { 0, ((void)W<I>()(args...), 1)... };
		(void)t;
	}
	
	template<template<int> class W, std::size_t N, typename Indices = std::make_index_sequence<N>, typename...A>
	void call_times(A&&... args) {
		caller_impl<W>(Indices(), std::forward<A>(args)...);
	}
	
	template<typename T>
	struct _i_object_path_allocator : public std::allocator<T>
	{
	public:
		using size_type     = size_t;
		using pointer       = T*;
		using const_pointer = const T*;

		template<typename T1>
		struct rebind
		{
			using other = _i_object_path_allocator<T1>;
		};

		pointer allocate(size_type n, const void* hint = nullptr)
		{
			return std::allocator<T>::allocate(n, hint);
		}

		void deallocate(pointer p, size_type n)
		{
			return std::allocator<T>::deallocate(p, n);
		}

		_i_object_path_allocator()
			: std::allocator<T>()
		{}

		_i_object_path_allocator(const _i_object_path_allocator& a)
			: std::allocator<T>(a)
		{}

		template <class U>
		_i_object_path_allocator(const _i_object_path_allocator<U>& a) 
			: std::allocator<T>(a)
		{}

		~_i_object_path_allocator() = default;
	};
	using object_path = std::basic_string<char, std::char_traits<char>, _i_object_path_allocator<char>>;
	static_assert(!std::is_same_v<object_path, std::string>, "std::string and object_path must be different types");

	template<typename T>
	constexpr auto _i_type_value_f()
	{
		if constexpr (ljh::is_instance_v<T, std::vector> || ljh::is_instance_v<T, std::map>)
			return DBUS_TYPE_ARRAY;
		else if constexpr (ljh::is_instance_v<T, std::variant>)
			return DBUS_TYPE_VARIANT;
		else if constexpr (std::is_same_v<std::string, T> || std::is_same_v<const char*, T>)
			return DBUS_TYPE_STRING;
		else if constexpr (std::is_same_v<int32_t, T>)
			return DBUS_TYPE_INT32;
		else if constexpr (std::is_same_v<uint32_t, T>)
			return DBUS_TYPE_UINT32;
		else if constexpr (std::is_same_v<int16_t, T>)
			return DBUS_TYPE_INT16;
		else if constexpr (std::is_same_v<uint16_t, T>)
			return DBUS_TYPE_UINT16;
		else if constexpr (std::is_same_v<int64_t, T>)
			return DBUS_TYPE_INT64;
		else if constexpr (std::is_same_v<uint64_t, T>)
			return DBUS_TYPE_UINT64;
		else if constexpr (std::is_same_v<uint8_t, T>)
			return DBUS_TYPE_BYTE;
		else if constexpr (std::is_same_v<object_path, T>)
			return DBUS_TYPE_OBJECT_PATH;
		else if constexpr (std::is_same_v<double, T>)
			return DBUS_TYPE_DOUBLE;
		else if constexpr (ljh::is_instance_v<T, std::tuple>)
			return DBUS_TYPE_STRUCT;
		else if constexpr (std::is_same_v<bool, T>)
			return DBUS_TYPE_BOOLEAN;
		else if constexpr (ljh::is_instance_v<T, multiout>)
			return (int)0;
		else
			return 0.1;
	}

	template<typename T>
	constexpr bool _i_type_fixed_array_f()
	{
		switch (_i_type_value_f<T>())
		{
		case DBUS_TYPE_BYTE:
		case DBUS_TYPE_BOOLEAN:
		case DBUS_TYPE_INT16:
		case DBUS_TYPE_UINT16:
		case DBUS_TYPE_INT32:
		case DBUS_TYPE_UINT32:
		case DBUS_TYPE_INT64:
		case DBUS_TYPE_UINT64:
		case DBUS_TYPE_DOUBLE:
			return true;
		
		default:
			return false;
		}
	}
	
	template <typename, template <typename...> typename>
	struct _i_gen_sig_s {};

	template<typename T>
	constexpr auto _i_gen_sig_f()
	{
		if constexpr (ljh::is_instance_v<T, std::vector>)
			return compile_time_string{DBUS_TYPE_ARRAY_AS_STRING} + _i_gen_sig_f<typename T::value_type>();
		else if constexpr (ljh::is_instance_v<T, std::variant>)
			return compile_time_string{DBUS_TYPE_VARIANT_AS_STRING};
		else if constexpr (std::is_same_v<std::string, T> || std::is_same_v<const char*, T>)
			return compile_time_string{DBUS_TYPE_STRING_AS_STRING};
		else if constexpr (std::is_same_v<int32_t, T>)
			return compile_time_string{DBUS_TYPE_INT32_AS_STRING};
		else if constexpr (std::is_same_v<uint32_t, T>)
			return compile_time_string{DBUS_TYPE_UINT32_AS_STRING};
		else if constexpr (std::is_same_v<int16_t, T>)
			return compile_time_string{DBUS_TYPE_INT16_AS_STRING};
		else if constexpr (std::is_same_v<uint16_t, T>)
			return compile_time_string{DBUS_TYPE_UINT16_AS_STRING};
		else if constexpr (std::is_same_v<int64_t, T>)
			return compile_time_string{DBUS_TYPE_INT64_AS_STRING};
		else if constexpr (std::is_same_v<uint64_t, T>)
			return compile_time_string{DBUS_TYPE_UINT64_AS_STRING};
		else if constexpr (std::is_same_v<uint8_t, T>)
			return compile_time_string{DBUS_TYPE_BYTE_AS_STRING};
		else if constexpr (std::is_same_v<object_path, T>)
			return compile_time_string{DBUS_TYPE_OBJECT_PATH_AS_STRING};
		else if constexpr (std::is_same_v<double, T>)
			return compile_time_string{DBUS_TYPE_DOUBLE_AS_STRING};
		else if constexpr (ljh::is_instance_v<T, std::tuple>)
			return compile_time_string{DBUS_STRUCT_BEGIN_CHAR_AS_STRING} + _i_gen_sig_s<T, std::tuple>()() + compile_time_string{DBUS_STRUCT_END_CHAR_AS_STRING};
		else if constexpr (ljh::is_instance_v<T, std::map>)
			return compile_time_string{DBUS_TYPE_ARRAY_AS_STRING} + compile_time_string{DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING} + _i_gen_sig_f<typename T::key_type>() + _i_gen_sig_f<typename T::mapped_type>() + compile_time_string{DBUS_DICT_ENTRY_END_CHAR_AS_STRING};
		else if constexpr (std::is_same_v<bool, T>)
			return compile_time_string{DBUS_TYPE_BOOLEAN_AS_STRING};
	}

	template <template <typename...> typename U, typename...Ts>
	struct _i_gen_sig_s<U<Ts...>, U> {
		constexpr auto operator()() { return (_i_gen_sig_f<std::decay_t<Ts>>() + ...); }
	};

	template<typename T> constexpr auto _i_type_value       = _i_type_value_f      <std::decay_t<T>>();
	template<typename T> constexpr auto _i_type_fixed_array = _i_type_fixed_array_f<std::decay_t<T>>();
	template<typename T> constexpr auto gen_sig             = _i_gen_sig_f         <std::decay_t<T>>();

	enum class bus
	{
		SESSION = DBUS_BUS_SESSION,
		SYSTEM  = DBUS_BUS_SYSTEM
	};

	class error
	{
		DBusError data;

	public:
		error();
		~error();

		error(error&& other);
		error& operator=(error&& other);

		std::string name() const;
		std::string message() const;

		operator bool() const;
		explicit operator DBusError*();
	};

	class connection
	{
		friend class object;

		DBusConnection* data = nullptr;
		bool weak = false;

		connection(DBusConnection* data);

	public:
		connection() = default;

		connection(bus type);
		~connection();

		connection(const connection& other);
		connection& operator=(const connection& other);

		connection(connection&& other);
		connection& operator=(connection&& other);

		operator bool() const;
		operator DBusConnection*() const;

		std::string unqiue_name() const;

		interface get(std::string server, std::string path, std::string interface);

		object create(std::string path);
	};

	class message
	{
		friend interface;
		friend object;
		connection _connection;
		DBusMessage* _message = nullptr;
		bool weak = false;

		message(const interface& base, std::string interface, std::string method);
		message(const connection& _connection, DBusMessage* _message, bool weak);
		message(DBusMessage* _message);

		error do_call()
		{
			error pending;
			auto temp = dbus_connection_send_with_reply_and_block(_connection, _message, DBUS_TIMEOUT_INFINITE, (DBusError*)pending);
			dbus_message_unref(_message);
			_message = temp;
			return pending;
		}

		message create_reply()
		{
			return {_connection, dbus_message_new_method_return(_message), false};
		}

		class iter;
		iter create_iter();
		iter create_iter_append();

	public:
		~message()
		{
			if (_message != nullptr && !weak)
				dbus_message_unref(std::exchange(_message, nullptr));
		}

		std::string interface() const
		{
			return dbus_message_get_interface(_message);
		}

		std::string member() const
		{
			return dbus_message_get_member(_message);
		}

		std::string signature() const
		{
			return dbus_message_get_signature(_message);
		}
		
		template<typename... A>
		message& args(A&&... args);

		void send()
		{
			dbus_connection_send(_connection, _message, nullptr);
		}

		template<typename... A>
		void run(A&... args);

		template<typename A = void>
		A run()
		{
			if constexpr (std::is_void_v<A>)
			{
				do_call();
			}
			else
			{
				A data;
				run(data);
				return data;
			}
		}
	};

	class object
	{
		friend class connection;
		friend class message;

		template<typename... T>
		static std::span<std::string_view const> gen_info()
		{
			if constexpr (sizeof...(T) == 1)
			{
				if constexpr (ljh::is_instance_v<T..., multiout>)
				{
					return std::apply([]<typename... A>(A...){ return gen_info<A...>(); }, decltype(std::declval<T>().data){}...);
				}
				else if constexpr (std::is_void_v<T...>)
				{
					static constexpr std::array<std::string_view, 0> items{};
					return items;
				}
				else
				{
					static constexpr std::array<std::string_view, sizeof...(T)> items{ std::string_view{ gen_sig<T> }... };
					return items;
				}
			}
			else 
			{
				static constexpr std::array<std::string_view, sizeof...(T)> items{ std::string_view{ gen_sig<T> }... };
				return items;
			}
		}

		struct func;
		struct sign;
		struct prop;

		struct interface
		{
			std::unordered_map<std::string, func> methods;
			std::unordered_map<std::string, sign> signals;
			std::unordered_map<std::string, prop> properties;
		};

		connection  _connection;
		std::string path;
		std::unordered_map<std::string, interface> interfaces;

		object(const connection& _connection, std::string path);

	public:
		object() = default;

		static void              _dbus_unregister(DBusConnection* connection, object* user_data);
		static DBusHandlerResult _dbus_message(DBusConnection* connection, DBusMessage* message, object* user_data);

		template<typename F>
		void add_method(std::string interface, std::string name, F&& function);

		template<typename G, typename S>
		void add_prop(std::string interface, std::string name, G&& getter, S&& setter)
			requires std::same_as<std::decay_t<typename ljh::function_traits<decltype(std::function{getter})>::return_type>, std::decay_t<typename ljh::function_traits<decltype(std::function{setter})>::template argument_type<0>>>;
		template<typename G>
		void add_prop(std::string interface, std::string name, G&& getter);
	};

	class message::iter
	{
		friend message;
		friend object;
		friend object::prop;

		DBusMessageIter item;

		template<int i>
		struct wrapper_v {
			template<typename T>
			void operator()(iter& _this, T& data) const 
			{
				char cur = dbus_message_iter_get_arg_type(&_this.item);
				char req = _i_type_value<std::variant_alternative_t<i, T>>;
				if (cur == req)
				{
					auto& temp = std::get<i>(data);
					_this.get(temp);
				}
			}
		};

		template<int i>
		struct wrapper_t {
			template<typename T>
			void operator()(iter& _this, T& data) const 
			{
				char cur = dbus_message_iter_get_arg_type(&_this.item);
				char req = _i_type_value<std::tuple_element_t<i, T>>;
				if (cur == req)
				{
					auto& temp = std::get<i>(data);
					_this.get(temp);
				}
			}
		};

		struct map_builder
		{
			iter& par;
			DBusMessageIter sub{};

			template<typename K, typename V>
			void add_item(K&& k, V&& v);

			map_builder(iter& par, std::string_view sig);
			~map_builder();
		};

		map_builder create_map_builder(std::string_view sig)
		{
			return {*this, sig};
		}

	public:
		template<typename T>
		void add(T&& data);

		template<typename T>
		bool get(T& data) requires (!std::is_const_v<T>);
	};


	struct object::func
	{
		std::function<void(message mes)> callback;
		std::span<std::string_view const> (*arg_info)();
		std::span<std::string_view const> (*return_info)();

		template<typename F>
		static func create(connection con, F&& cb);
	};

	struct object::sign
	{
		std::function<void(message mes)> callback;
		std::span<std::string_view const> (*arg_info)();
		std::span<std::string_view const> (*return_info)();

		template<typename F>
		static sign create(connection con, F&& cb);
	};

	struct object::prop
	{
		std::function<void(message::iter&)> getter;
		std::function<void(message::iter&)> setter;
		std::function<void(std::string const&, message::iter::map_builder&)> getall;
		std::span<std::string_view const> (*arg_info)();

		template<typename G, typename S>
		static prop create(connection con, G&& g, S&& s);
	};


	class interface
	{
		friend class connection;
		friend class message;

		connection _connection;
		std::string path;
		std::string server;
		std::string _interface;

		interface(const connection& _connection, std::string server, std::string path, std::string interface);
		message get(std::string property);

	public:
		interface() = default;

		message call(std::string method);

		template<typename T>
		T get(std::string property);
	};

	template<typename T>
	T interface::get(std::string property)
	{
		return std::get<0>(get(property).run<std::variant<T>>());
	}

	inline message::iter message::create_iter()
	{
		iter output{};
		dbus_message_iter_init(_message, &output.item);
		return output;
	}

	inline message::iter message::create_iter_append()
	{
		iter output{};
		dbus_message_iter_init_append(_message, &output.item);
		return output;
	}
	

	template<typename... A>
	message& message::args(A&&... args)
	{
		auto iter = create_iter_append();
		expand_type{(iter.add(std::forward<A>(args)), 0)...};
		return *this;
	}

	template<typename... A>
	void message::run(A&... args)
	{
		auto pending = do_call();
		if (pending) throw pending;

		error err;
		if (dbus_set_error_from_message((DBusError*)err, _message))
			throw err;

		auto iter = create_iter();
		expand_type{(iter.get(args), 0)...};
	}

	template<typename T>
	void message::iter::add(T&& data)
	{
		using type = std::decay_t<T>;
		static_assert(std::is_same_v<decltype(_i_type_value<T>), decltype(_i_type_value<int>)>, "Unknown type");
		if constexpr (ljh::is_instance_v<type, multiout>)
		{
			std::apply([&](auto&&... args){ expand_type{(add(std::forward<decltype(args)>(args)), 0)...}; }, data.data);
		}
		else if constexpr (std::is_same_v<bool, type>)
		{
			int value = data ? 1 : 0;
			dbus_message_iter_append_basic(&item, _i_type_value<T>, &value);
		}
		else if constexpr (std::is_fundamental_v<type>)
		{
			dbus_message_iter_append_basic(&item, _i_type_value<T>, &data);
		}
		else if constexpr (std::is_same_v<type, std::string> || std::is_same_v<type, object_path>)
		{
			auto temp = data.data();
			dbus_message_iter_append_basic(&item, _i_type_value<T>, &temp);
		}
		else if constexpr (std::is_same_v<type, const char*>)
		{
			type temp = data;
			dbus_message_iter_append_basic(&item, _i_type_value<T>, &temp);
		}
		else if constexpr (ljh::is_bounded_array_v<std::remove_reference_t<T>>)
		{
			type temp = data;
			dbus_message_iter_append_fixed_array(&item, _i_type_value<T>, &temp, std::size(data));
		}
		else if constexpr (ljh::is_instance_v<type, std::vector>)
		{
			iter sub{};
			dbus_message_iter_open_container(&item, _i_type_value<T>, gen_sig<typename type::value_type>.data(), &sub.item);
			for (int a = 0; a < data.size(); sub.add(data[a++]));
			dbus_message_iter_close_container(&item, &sub.item);
		}
		else if constexpr (ljh::is_instance_v<type, std::variant>)
		{
			iter sub{};
			dbus_message_iter_open_container(&item, _i_type_value<T>, std::visit([](auto&& a){ return gen_sig<decltype(a)>.data(); }, data), &sub.item);
			std::visit([&sub](auto&& a){ sub.add(std::forward<decltype(a)>(a)); }, data);
			dbus_message_iter_close_container(&item, &sub.item);
		}
		else if constexpr (ljh::is_instance_v<type, std::tuple>)
		{
			iter sub{};
			dbus_message_iter_open_container(&item, _i_type_value<T>, nullptr, &sub.item);
			std::apply([&sub](auto&&... args){ expand_type{(sub.add(std::forward<decltype(args)>(args)), 0)...}; }, data);
			dbus_message_iter_close_container(&item, &sub.item);
		}
		else if constexpr (ljh::is_instance_v<type, std::map>)
		{
			auto map = create_map_builder(gen_sig<T>);
			for (auto& [key, value] : data)
				map.add_item(key, value);
		}
		else
		{
			static_assert(sizeof(T*) == 0, "Can't handle type");
		}
	}

	inline message::iter::map_builder::map_builder(iter& par, std::string_view sig)
		: par(par)
	{
		dbus_message_iter_open_container(&par.item, DBUS_TYPE_ARRAY, sig.data() + 1, &sub);
	}

	inline message::iter::map_builder::~map_builder()
	{
		dbus_message_iter_close_container(&par.item, &sub);
	}

	template<typename K, typename V>
	void message::iter::map_builder::add_item(K&& k, V&& v)
	{
		iter member{};
		dbus_message_iter_open_container(&sub, DBUS_TYPE_DICT_ENTRY, nullptr, &member.item);
		member.add(k); member.add(v);
		dbus_message_iter_close_container(&sub, &member.item);
	}

	template<typename T>
	bool message::iter::get(T& data) requires (!std::is_const_v<T>)
	{
		static_assert(std::is_same_v<std::decay_t<decltype(_i_type_value<T>)>, int>, "Unknown type");

		char cur = (char)dbus_message_iter_get_arg_type(&item);
		char req = (char)_i_type_value<T>;
		if (cur != req)
		{
			if (cur == DBUS_TYPE_INVALID)
				throw std::runtime_error(std::string{"Got an invalid type over the requested type ("} + req + ")");
			throw std::runtime_error(std::string{"Type ("} + cur + ") does not equal requested type (" + req + ")");
		}
		if constexpr (std::is_same_v<bool, T>)
		{
			int value;
			dbus_message_iter_get_basic(&item, &value);
			data = (value == 1);
		}
		else if constexpr (std::is_fundamental_v<T>)
		{
			dbus_message_iter_get_basic(&item, &data);
		}
		else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, object_path>)
		{
			const char* temp;
			dbus_message_iter_get_basic(&item, &temp);
			data = temp;
		}
		else if constexpr (ljh::is_instance_v<T, std::vector>)
		{
			int count = dbus_message_iter_get_element_count(&item);
			if (count > 0)
			{
				data.reserve(count);
				iter sub{};
				dbus_message_iter_recurse(&item, &sub.item);
				while (sub.get(data.emplace_back()));
			}
		}
		else if constexpr (ljh::is_instance_v<T, std::variant>)
		{
			iter sub{};
			dbus_message_iter_recurse(&item, &sub.item);
			call_times<wrapper_v, std::variant_size_v<T>>(sub, data);
		}
		else if constexpr (ljh::is_instance_v<T, std::tuple>)
		{
			iter sub{};
			dbus_message_iter_recurse(&item, &sub.item);
			call_times<wrapper_t, std::tuple_size_v<T>>(sub, data);
		}
		else if constexpr (ljh::is_instance_v<T, std::map>)
		{
			iter sub{};
			dbus_message_iter_recurse(&item, &sub.item);
			do
			{
				iter member{};
				dbus_message_iter_recurse(&sub.item, &member.item);

				typename T::key_type key;
				typename T::mapped_type value;

				member.get(key);
				member.get(value);

				data[key] = value;
			}
			while (dbus_message_iter_next(&sub.item));
		}
		else
		{
			static_assert(sizeof(T*) == 0, "Can't handle type");
		}
		return dbus_message_iter_next(&item);
	}

	template<typename F>
	object::func object::func::create(connection con, F&& cb)
	{
		using ft = decltype([&cb]{ return std::function{cb}; }());
		using at = typename ljh::function_traits<ft>::argument_types;
		using rt = typename ljh::function_traits<ft>::return_type;

		return std::apply([con, cb]<typename... T>(T... args) {
			return func{
				.callback = [con, cb](message mes) {
					at args{};
					auto i = mes.create_iter();
					std::apply([&i](auto&... a){ (void(i.get(a)), ...); }, args);
					auto reply = mes.create_reply();
					if constexpr (!std::is_same_v<rt, void>)
						reply.args(std::apply(cb, args));
					else
						std::apply(cb, args);
					reply.send();
				},
				.arg_info = gen_info<T...>,
				.return_info = gen_info<rt>,
			};
		}, at{});
	}

	template<typename G, typename S>
	object::prop object::prop::create(connection con, G&& g, S&& s)
	{
		using T = std::decay_t<typename ljh::function_traits<decltype(std::function{g})>::return_type>;
		return prop{
			.getter = [g](message::iter& mes) {
				mes.add(g());
			},
			.setter = [=]{
				if constexpr (!std::is_same_v<std::nullptr_t, S>)
					return [s](message::iter& mes) { T data; mes.get(data); s(data); };
				else
					return nullptr;
			}(),
			.getall = [g](std::string const& key, message::iter::map_builder& map) {
				map.add_item(key, g());
			},
			.arg_info = gen_info<T>,
		};
	}

	template<typename F>
	void object::add_method(std::string interface, std::string name, F&& function)
	{
		interfaces[interface].methods[name] = func::create(_connection, std::forward<F>(function));
	}

	template<typename G, typename S>
	void object::add_prop(std::string interface, std::string name, G&& getter, S&& setter)
		requires std::same_as<std::decay_t<typename ljh::function_traits<decltype(std::function{getter})>::return_type>, std::decay_t<typename ljh::function_traits<decltype(std::function{setter})>::template argument_type<0>>>
	{
		interfaces[interface].properties[name] = prop::create(_connection, std::forward<G>(getter), std::forward<S>(setter));
	}

	template<typename G>
	void object::add_prop(std::string interface, std::string name, G&& getter)
	{
		interfaces[interface].properties[name] = prop::create(_connection, std::forward<G>(getter), nullptr);
	}
}