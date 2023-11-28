#include "ser_comm.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SerComm::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("write_serial_message", "p_message"), &SerComm::write_serial_message);
	ADD_SIGNAL(MethodInfo("read_serial_message", PropertyInfo(Variant::STRING, "message")));
}

SerComm::SerComm()
{
	baud_rate = BAUD_9600;
	toggle_to_refresh = false;
	is_port_open = false;
	std::string port_name = "No port found!";
	refresh_ports();

	//_err_print_error(__FUNCTION__, __FILE__, __LINE__, message);
}

SerComm::~SerComm()
{
	// Add your cleanup here.
	close_port();
}

void SerComm::_process(double delta)
{
	if (toggle_to_refresh)
	{
		refresh_ports();
		notify_property_list_changed();
	}

	if (Engine::get_singleton()->is_editor_hint())
		return;

	open_port();
	read_serial_message();
}

bool SerComm::open_port()
{
	if (!is_port_open)
	{
		open_serial();
		is_port_open = true;
		return true;
	}
	return false; // Port is already open
}

void SerComm::close_port()
{
	if (is_port_open)
	{
		sp_close(port);
		sp_free_port(port);
		is_port_open = false;
	}
}

void SerComm::open_serial()
{
	if (is_port_open)
		return;

	result = sp_get_port_by_name(_ports[_port].c_str(), &port);
	if (result != SP_OK)
	{
		std::cerr << "Error getting port" << std::endl;
		return;
	}

	result = sp_open(port, SP_MODE_READ_WRITE);
	if (result != SP_OK)
	{
		std::cerr << "Error opening port" << std::endl;
		sp_free_port(port);
		return;
	}

	std::cout << "Success opening port!" << std::endl;
	sp_set_baudrate(port, baud_rate);
	sp_set_bits(port, 8);
	sp_set_stopbits(port, 1);
	sp_set_parity(port, SP_PARITY_NONE);
}

void SerComm::read_serial_message()
{
	if (!is_port_open)
		return;

	char read_buffer[1024];
	result = sp_nonblocking_read(port, read_buffer, sizeof(read_buffer) - 1);

	if (result < 0)
	{
		std::cerr << "Error reading data from port" << std::endl;
		return;
	}

	read_buffer[result] = '\0';
	String data = String::utf8(read_buffer);

	if (data.length() > 0)
	{
		std::cout << "Reading input: " << read_buffer << std::endl;
		emit_signal("read_serial_message", read_buffer);
	}
}

void SerComm::write_serial_message(const String &p_message)
{
	if (!is_port_open)
		return;

	const char *utf8_data = p_message.utf8().get_data();

	result = sp_nonblocking_write(port, utf8_data, std::strlen(utf8_data));
	if (result < 0)
	{
		std::cerr << "Error writing data to port" << std::endl;
	}

	std::cout << "Writing output: " << utf8_data << std::endl;

	sp_drain(port);
};

void SerComm::set_toggle_to_refresh(const bool p_is_toggled)
{
	toggle_to_refresh = p_is_toggled;
}

bool SerComm::get_toggle_to_refresh()
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
		r_value = _port;
		return true;
	}
	else if (p_name == StringName("baud_rate"))
	{
		r_value = baud_rate;
		return true;
	}
	else if (p_name == StringName("toggle_to_refresh"))
	{
		r_value = toggle_to_refresh;
		return true;
	}

	return false;
}

bool SerComm::_set(const StringName &p_name, const Variant &p_value)
{
	if (p_name == StringName("port"))
	{
		_port = p_value;
		return true;
	}
	else if (p_name == StringName("baud_rate"))
	{
		baud_rate = p_value;
		return true;
	}
	else if (p_name == StringName("toggle_to_refresh"))
	{
		toggle_to_refresh = p_value;
		return true;
	}

	return false;
}
