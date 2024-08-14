#include "ser_comm.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SerComm::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("list_serial_ports"), &SerComm::sercomm_list_ports);
	ClassDB::bind_method(D_METHOD("open_serial"), &SerComm::sercomm_open);
	ClassDB::bind_method(D_METHOD("close_serial"), &SerComm::sercomm_close);
	ClassDB::bind_method(D_METHOD("read_serial"), &SerComm::sercomm_read);
	ClassDB::bind_method(D_METHOD("write_serial", "p_message"), &SerComm::sercomm_write);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "port"), "set_port", "get_port");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "toggle_to_refresh"), "set_toggle_to_refresh", "get_toggle_to_refresh");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "baud_rate", PROPERTY_HINT_ENUM, VariantHelper::_baud_rate_to_hint_string()), "set_baud_rate", "get_baud_rate");
	ADD_SIGNAL(MethodInfo("read_serial", PropertyInfo(Variant::STRING, "message")));
}

void SerComm::set_port(const int id) {
	_port_enum = id; 
}

int SerComm::get_port() const {
	return _port_enum;
}

void SerComm::set_baud_rate(const int b) {
	baud_rate = b;
}

int SerComm::get_baud_rate() const {
	return baud_rate;
}

SerComm::SerComm()
{
	baud_rate = BAUD_9600;
	toggle_to_refresh = false;
	std::string port_name = "No port found!";
	refresh_ports();
}

SerComm::~SerComm()
{
	// Add your cleanup here.
	sercomm_close();
}

godot::Array SerComm::SerComm::sercomm_list_ports()
{
	struct sp_port **port_list;
	sp_return error = sp_list_ports(&port_list);

	if (error != SP_OK)
	{
		_err_print_error(__FUNCTION__, __FILE__, __LINE__, "Error listening ports");
		sp_free_port_list(port_list);
		return {};
	}

	godot::TypedArray<String> res_names = {};
	for (sp_port **ptr = port_list; *ptr; ++ptr)
	{
		const char *l_port_name = sp_get_port_name(*ptr);
		godot::String s = l_port_name;
		res_names.push_back(s);
	}

	sp_free_port_list(port_list);
	return res_names;
}

void godot::SerComm::sercomm_flush()
{
}

void SerComm::_process(double delta)
{
	if (toggle_to_refresh)
	{
		refresh_ports();
		notify_property_list_changed();
	}

	// if (Engine::get_singleton()->is_editor_hint())
	// 	return;

	// open_port();
	// sercomm_read();
}

void SerComm::sercomm_close()
{
	sp_close(port);
	sp_free_port(port);
}

bool SerComm::sercomm_open()
{
	sp_return result = sp_get_port_by_name(_ports[_port_enum].c_str(), &port);
	if (result != SP_OK)
	{
		std::cerr << "Error getting port" << std::endl;
		return false;
	}

	result = sp_open(port, SP_MODE_READ_WRITE);
	if (result != SP_OK)
	{
		std::cerr << "Error opening port" << std::endl;
		sp_free_port(port);
		return false;
	}

	std::cout << "Success opening port!" << std::endl;
	sp_set_baudrate(port, baud_rate);
	sp_set_bits(port, 8);
	sp_set_stopbits(port, 1);
	sp_set_parity(port, SP_PARITY_NONE);
	return true;
}

String SerComm::sercomm_read()
{
	char read_buffer[1024];
	sp_return result = sp_nonblocking_read(port, read_buffer, sizeof(read_buffer) - 1);

	if (result < 0)
	{
		std::cerr << "Error reading data from port" << std::endl;
		return "";
	}

	read_buffer[result] = '\0';
	String data = String::utf8(read_buffer);

	if (data.length() > 0)
	{
		// std::cout << "Reading input: " << read_buffer << std::endl;
		emit_signal("read_serial_message", data);
		return data;
	}

	return "";
}

void SerComm::sercomm_write(const String &p_message)
{
	const char *utf8_data = p_message.utf8().get_data();

	sp_return result = sp_nonblocking_write(port, utf8_data, std::strlen(utf8_data));
	if (result < 0)
	{
		std::cerr << "Error writing data to port" << std::endl;
	}

	// std::cout << "Writing output: " << utf8_data << std::endl;
};

void SerComm::set_toggle_to_refresh(const bool p_is_toggled)
{
	toggle_to_refresh = p_is_toggled;
}

bool SerComm::get_toggle_to_refresh() const
{
	return toggle_to_refresh;
}

void SerComm::refresh_ports()
{
	_ports.clear();
	sp_port **ports;
	sp_return error = sp_list_ports(&ports);

	if (error != SP_OK)
	{
		_err_print_error(__FUNCTION__, __FILE__, __LINE__, "Error listening ports");
		sp_free_port_list(ports);
		toggle_to_refresh = false;
		return;
	}

	for (sp_port **ptr = ports; *ptr; ++ptr)
	{
		const char *l_port_name = sp_get_port_name(*ptr);
		_ports.push_back(l_port_name);
	}

	sp_free_port_list(ports);
	toggle_to_refresh = false;
}

void SerComm::_get_property_list(List<PropertyInfo> *r_list) const
{
	r_list->push_back(PropertyInfo(Variant::BOOL, "toggle_to_refresh"));
	r_list->push_back(PropertyInfo(Variant::INT, "port", PROPERTY_HINT_ENUM, VariantHelper::_ports_to_hint_string(_ports)));
	r_list->push_back(PropertyInfo(Variant::INT, "baud_rate", PROPERTY_HINT_ENUM, VariantHelper::_baud_rate_to_hint_string()));
}

bool SerComm::_get(const StringName &p_name, Variant &r_value) const
{
	if (p_name == StringName("port"))
	{
		r_value = get_port();
		return true;
	}
	else if (p_name == StringName("baud_rate"))
	{
		r_value = get_baud_rate();
		return true;
	}
	else if (p_name == StringName("toggle_to_refresh"))
	{
		r_value = get_toggle_to_refresh();
		return true;
	}

	return false;
}

bool SerComm::_set(const StringName &p_name, const Variant &p_value)
{
	if (p_name == StringName("port"))
	{
		set_port(p_value);
		return true;
	}
	else if (p_name == StringName("baud_rate"))
	{
		set_baud_rate(p_value);
		return true;
	}
	else if (p_name == StringName("toggle_to_refresh"))
	{
		set_toggle_to_refresh(p_value);
		return true;
	}

	return false;
}
