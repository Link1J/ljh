
//          Copyright Jared Irwin 2020-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/unix/dbus.hpp"
#include "ljh/unreachable.hpp"

ljh::unix::dbus::error::error()
{
	dbus_error_init(&data);
}

ljh::unix::dbus::error::~error()
{
	if (*this)
		dbus_error_free(&data);
}

std::string ljh::unix::dbus::error::name() const
{
	return data.name;
}

std::string ljh::unix::dbus::error::message() const
{
	return data.message;
}

ljh::unix::dbus::error::operator bool() const
{
	return dbus_error_is_set(&data);
}

ljh::unix::dbus::error::error(error&& other)
{
	dbus_error_init(&data);
	if (other)
		dbus_move_error((DBusError*)other, &data);
}

ljh::unix::dbus::error& ljh::unix::dbus::error::operator=(error&& other)
{
	if (other)
		dbus_move_error((DBusError*)other, &data);
	return *this;
}

ljh::unix::dbus::error::operator DBusError*()
{
	return &data;
}

ljh::unix::dbus::connection::connection(DBusConnection* data)
	: data(data)
	, weak(true)
{}

ljh::unix::dbus::connection::connection(bus type)
{
	error _error;
	data = dbus_bus_get(static_cast<DBusBusType>(type), (DBusError*)_error);
	if (_error)
		throw _error;
}

ljh::unix::dbus::connection::~connection()
{
	if (data && !weak)
		dbus_connection_unref(data);
	data = nullptr;
}

ljh::unix::dbus::connection::connection(const connection& other)
{
	data = other.data;
	dbus_connection_ref(data);
}

ljh::unix::dbus::connection& ljh::unix::dbus::connection::operator=(const connection& other)
{
	data = other.data;
	dbus_connection_ref(data);
	return *this;
}

ljh::unix::dbus::connection::connection(connection&& other)
{
	data       = other.data;
	other.data = nullptr;
}

ljh::unix::dbus::connection& ljh::unix::dbus::connection::operator=(connection&& other)
{
	data       = other.data;
	other.data = nullptr;
	return *this;
}

std::string ljh::unix::dbus::connection::unqiue_name() const
{
	return dbus_bus_get_unique_name(data);
}

ljh::unix::dbus::object ljh::unix::dbus::connection::create(std::string path)
{
	return object{*this, path};
}

const DBusObjectPathVTable object_vtable{
	.unregister_function = reinterpret_cast<DBusObjectPathUnregisterFunction>(ljh::unix::dbus::object::_dbus_unregister),
	.message_function    = reinterpret_cast<DBusObjectPathMessageFunction>(ljh::unix::dbus::object::_dbus_message),
};

#include <iostream>
#include <utility>

ljh::unix::dbus::object::object(const connection& _connection, std::string path)
	: _connection(_connection)
{
	dbus_connection_register_object_path(_connection, path.c_str(), &object_vtable, this);

	add_method("org.freedesktop.DBus.Introspectable", "Introspect", [this]() -> std::string { ljh::unreachable(); });
	add_method("org.freedesktop.DBus.Properties", "Get", [](std::string interface, std::string prop) -> std::variant<int> { ljh::unreachable(); });
	add_method("org.freedesktop.DBus.Properties", "Set", [](std::string interface, std::string prop, std::variant<int> value) -> void { ljh::unreachable(); });
	add_method("org.freedesktop.DBus.Properties", "GetAll", [](std::string interface) -> std::map<std::string, std::variant<int>> { ljh::unreachable(); });
}

void ljh::unix::dbus::object::_dbus_unregister(DBusConnection* connection, ljh::unix::dbus::object* user_data)
{}

void handle_introspectable(ljh::unix::dbus::message& mes, ljh::unix::dbus::object& user_data)
{

}

DBusHandlerResult ljh::unix::dbus::object::_dbus_message(DBusConnection* connection, DBusMessage* message, ljh::unix::dbus::object* user_data)
{
	using namespace std::literals;
	dbus::message mes{connection, message, true};

	auto interface_name = dbus_message_get_interface(message);
	auto method_name = dbus_message_get_member(message);

	std::cout << interface_name << " | " << method_name << "\n";

	if (interface_name == "org.freedesktop.DBus.Introspectable"sv)
	{
		if (method_name == "Introspect"sv)
		{
			std::string output = R"(<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN" "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd"><node xmlns:doc="http://www.freedesktop.org/dbus/1.0/doc.dtd">)";
			for (auto& [name, props] : user_data->interfaces)
			{
				output += "<interface name=\"" + name + "\">";

				for (auto [name, props] : props.methods)
				{
					output += "<method name=\"" + name + "\">";
					size_t i = 0;
					for (auto info = props.arg_info(); auto type : info)
						output += "<arg type=\"" + std::string{type} + "\" name=\"a" + std::to_string(i++) + R"(" direction="in" />)";
					i = 0; 
					for (auto info = props.return_info(); auto type : info)
						output += "<arg type=\"" + std::string{type} + "\" name=\"r" + std::to_string(i++) + R"(" direction="out" />)";
					output += "</method>";
				}

				// for (auto [name, props] : props.signals)
				// {
				// 	output += "<method name=\"" + name + "\">";
				// 	if (auto type = props.return_info())
				// 		output += "<arg type=\"" + std::string{type.value()} + R"(" name="r1" direction="out" />)";
				// 
				// 	size_t i = 0;
				// 	for (auto type : props.arg_info())
				// 		output += "<arg type=\"" + std::string{type} + "\" name=\"" + std::to_string(i++) + R"(" direction="in" />)";
				// 	output += "</method>";
				// }

				for (auto [name, props] : props.properties)
				{
					output += "<property name=\"" + name + "\" type=\"" + std::string{props.arg_info()[0]} + "\" access=\"" + (props.setter != nullptr ? "readwrite" : "read") + "\" />";
				}

				output += "</interface>";
			}
			output += "</node>";
			mes.create_reply().args(output).send();
			return DBUS_HANDLER_RESULT_HANDLED;
		}
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (interface_name == "org.freedesktop.DBus.Properties"sv)
	{
		dbus::message mes{connection, message, true};

		auto args  = mes.create_iter();
		auto reply = mes.create_reply();

		std::string interface;
		args.get(interface);
		auto& intf = user_data->interfaces[interface];

		if (method_name == "GetAll"sv)
		{
			{
				auto it = reply.create_iter_append();
				auto map = it.create_map_builder(gen_sig<std::map<std::string, std::variant<int>>>);
				for (auto& [name, prop] : intf.properties)
					prop.getall(name, map);
			}
			reply.send();
			return DBUS_HANDLER_RESULT_HANDLED;
		}

		std::string property;
		args.get(property);
		auto& prop = intf.properties[property];

		if (method_name == "Get"sv)
		{
			auto data = reply.create_iter_append();
			prop.getter(data);
			reply.send();
			return DBUS_HANDLER_RESULT_HANDLED;
		}

		if (method_name == "Set"sv)
		{
			prop.setter(args);
			reply.send();
			return DBUS_HANDLER_RESULT_HANDLED;
		}

		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (auto it1 = user_data->interfaces.find(interface_name); it1 != user_data->interfaces.end())
	{
		auto& interface = it1->second;
		if (auto it2 = interface.methods.find(method_name); it2 != interface.methods.end())
		{
			dbus::message mes{connection, message, true};
			it2->second.callback(mes);
			return DBUS_HANDLER_RESULT_HANDLED;
		}
	}

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

ljh::unix::dbus::connection::operator bool() const
{
	return data != nullptr;
}

ljh::unix::dbus::connection::operator DBusConnection*() const
{
	return data;
}

ljh::unix::dbus::interface ljh::unix::dbus::connection::get(std::string server, std::string path, std::string interface)
{
	return ljh::unix::dbus::interface{*this, server, path, interface};
}

ljh::unix::dbus::interface::interface(const connection& _connection, std::string server, std::string path, std::string interface)
	: _connection(_connection)
	, server(server)
	, path(path)
	, _interface(interface)
{}

ljh::unix::dbus::message ljh::unix::dbus::interface::call(std::string method)
{
	return message{*this, _interface, method};
}

ljh::unix::dbus::message ljh::unix::dbus::interface::get(std::string property)
{
	message temp{*this, DBUS_INTERFACE_PROPERTIES, "Get"};
	temp.args(_interface, property);
	return temp;
}

ljh::unix::dbus::message::message(const class interface& base, std::string interface, std::string method)
	: message(base._connection, dbus_message_new_method_call(base.server.c_str(), base.path.c_str(), interface.c_str(), method.c_str()), false)
{
}


ljh::unix::dbus::message::message(const connection& _connection, DBusMessage* _message, bool weak)
	: _connection(_connection)
	, _message(_message)
	, weak(weak)
{
}

ljh::unix::dbus::message::message(DBusMessage* _message)
	: _message(_message)
{}