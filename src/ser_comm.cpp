#include "ser_comm.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SerComm::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("refresh_ports"), &SerComm::refresh_ports);
	ClassDB::bind_method(D_METHOD("list_serial_ports"), &SerComm::sercomm_list_ports);
	ClassDB::bind_method(D_METHOD("open_serial"), &SerComm::sercomm_open);
	ClassDB::bind_method(D_METHOD("close_serial"), &SerComm::sercomm_close);

	ClassDB::bind_method(D_METHOD("waiting_input_bytes"), &SerComm::sercomm_get_waiting);
	ClassDB::bind_method(D_METHOD("read_serial", "num_bytes"), &SerComm::sercomm_read);
	ClassDB::bind_method(D_METHOD("write_serial", "p_message"), &SerComm::sercomm_write);
	ADD_SIGNAL(MethodInfo("on_message", PropertyInfo(Variant::STRING, "message")));

	ClassDB::bind_method(D_METHOD("get_port"), &SerComm::get_port);
	ClassDB::bind_method(D_METHOD("set_port", "id"), &SerComm::set_port);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "port", PROPERTY_HINT_ENUM, "", PROPERTY_USAGE_STORAGE), "set_port", "get_port");

	ClassDB::bind_method(D_METHOD("get_open"), &SerComm::get_open);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_open", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_READ_ONLY), "", "get_open");
	
	ClassDB::bind_method(D_METHOD("get_baud_rate"), &SerComm::get_baud_rate);
	ClassDB::bind_method(D_METHOD("set_baud_rate", "b"), &SerComm::set_baud_rate);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "baud_rate", PROPERTY_HINT_ENUM, VariantHelper::_baud_rate_to_hint_string()), "set_baud_rate", "get_baud_rate");
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

bool SerComm::get_open() const {
	return opened;
}

SerComm::SerComm()
{
	baud_rate = BAUD_9600;
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
	godot::TypedArray<String> res_names = {};
	for (auto port : _ports)
	{
		godot::String s(port.c_str());
		res_names.push_back(s);
	}

	return res_names;
}

void godot::SerComm::sercomm_flush()
{
}

int SerComm::sercomm_get_waiting() {
	return sp_input_waiting(port);
}

void SerComm::_process(double delta)
{
	if (opened) {
		int bytes = sercomm_get_waiting();
		if (bytes > 0) {
			sercomm_read(bytes);
		} 
	}
}

void SerComm::sercomm_close()
{
	if (opened) {
		sp_close(port);
		sp_free_port(port);
	}
	opened = false;
}

bool SerComm::sercomm_open()
{
	if (opened) {
		return true;
	}
	
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

	opened = true;

	std::cout << "Success opening port!" << std::endl;
	sp_set_baudrate(port, baud_rate);
	sp_set_bits(port, 8);
	sp_set_stopbits(port, 1);
	sp_set_parity(port, SP_PARITY_NONE);
	return true;
}

void SerComm::sercomm_read(const int num_bytes)
{
	std::vector<char> read_buffer(num_bytes);
	sp_return result = sp_nonblocking_read(port, read_buffer.data(), num_bytes);

	if (result < 0)
	{
		std::cerr << "Error reading data from port" << std::endl;
		return;
	}

	read_buffer[result] = '\0';
	String data = String::utf8(read_buffer.data());

	if (data.length() > 0)
	{
		emit_signal("on_message", data);
	}
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

void SerComm::refresh_ports()
{
	_ports.clear();
	sp_port **ports;
	sp_return error = sp_list_ports(&ports);

	if (error != SP_OK)
	{
		_err_print_error(__FUNCTION__, __FILE__, __LINE__, "Error listening ports");
		sp_free_port_list(ports);
		return;
	}

	for (sp_port **ptr = ports; *ptr; ++ptr)
	{
		const char *l_port_name = sp_get_port_name(*ptr);
		_ports.push_back(l_port_name);
	}
	
	if (Engine::get_singleton()->is_editor_hint()) {
		port_enum_str = VariantHelper::_ports_to_hint_string(_ports);
	}

	sp_free_port_list(ports);
}

void SerComm::_get_property_list(List<PropertyInfo> *r_list) const
{
	r_list->push_back(PropertyInfo(Variant::INT, "port", PROPERTY_HINT_ENUM, port_enum_str, PROPERTY_USAGE_EDITOR));
	r_list->push_back(PropertyInfo(Variant::BOOL, "toggle_to_refresh", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR));
}

bool SerComm::_get(const StringName &p_name, Variant &r_value) const
{
	if (p_name == StringName("port"))
	{
		r_value = get_port();
		return true;
	} else if (p_name == StringName("toggle_to_refresh")) {
		r_value = false;
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
	} else if (p_name == StringName("toggle_to_refresh")) {
		refresh_ports();
		notify_property_list_changed();
		return true;
	}

	return false;
}
