
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_translate_exception.hpp>

#include <ljh/unix/dbus.hpp>

#include <iostream>

CATCH_TRANSLATE_EXCEPTION(const ljh::unix::dbus::error& ex) {
	return "DBus Error (" + ex.name() + "): " + ex.message();
}

TEST_CASE("dbus", "[test_17][dbus]" ) {
	using namespace ljh::unix;
	
	dbus::connection seesion(dbus::bus::SESSION);
	auto notif1 = seesion.get("org.freedesktop.impl.portal.desktop.kde", "/org/freedesktop/portal/desktop", "org.freedesktop.impl.portal.Notification");
	std::map<std::string, std::variant<std::string>> not_info {
		{"title", {"Test title"}},
		{"body", {"Test body"}},
	};
	CHECK(true);
	auto message = notif1.call("AddNotification");
	message.args("a", "a", not_info);
	std::cout << message.signature();
	message.run();

	dbus::connection connection(dbus::bus::SYSTEM);
	auto dbus_obj = connection.get(DBUS_SERVICE_DBUS, DBUS_PATH_DBUS, DBUS_INTERFACE_DBUS);
	CHECK(dbus_obj.call("GetNameOwner").args("org.freedesktop.DBus").run<std::string>() == "org.freedesktop.DBus");
	CHECK(dbus_obj.call("GetId").run<std::string>() == "8ed2eedda742ac1523c70bfa601c9cce");
	std::string storage;
	dbus_obj.call("GetId").run(storage);
	CHECK(storage == "8ed2eedda742ac1523c70bfa601c9cce");
	
	auto dbus_obj2 = connection.get(DBUS_SERVICE_DBUS, DBUS_PATH_DBUS, DBUS_INTERFACE_PROPERTIES);
	CHECK(dbus_obj.get<std::vector<std::string>>("Features") == std::vector<std::string>{"SystemdActivation"});
	CHECK(std::get<0>(dbus_obj2.call("Get").args(DBUS_INTERFACE_DBUS, "Features").run<std::variant<std::vector<std::string>>>()) == std::vector<std::string>{"SystemdActivation"});

	auto systemd = connection.get("org.freedesktop.systemd1", "/org/freedesktop/systemd1", "org.freedesktop.systemd1.Manager");
	auto data = systemd.call("ListUnitFiles").run<std::vector<std::tuple<std::string,std::string>>>();

	CHECK(data != std::vector<std::tuple<std::string,std::string>>{});
}