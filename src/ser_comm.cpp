#include "ser_comm.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void SerComm::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("refresh_ports"), &SerComm::refresh_ports);
	ClassDB::bind_method(D_METHOD("initialize_ports"), &SerComm::initialize_ports);
	ClassDB::bind_method(D_METHOD("list_serial_ports"), &SerComm::sercomm_list_ports);
	ClassDB::bind_method(D_METHOD("open_serial"), &SerComm::sercomm_open);
	ClassDB::bind_method(D_METHOD("open_serial_with_port"), &SerComm::sercomm_open_specific_serial_port);
	ClassDB::bind_method(D_METHOD("close_serial"), &SerComm::sercomm_close);

	ClassDB::bind_method(D_METHOD("waiting_input_bytes"), &SerComm::sercomm_get_waiting);
	ClassDB::bind_method(D_METHOD("read_serial", "num_bytes"), &SerComm::sercomm_read);
	ClassDB::bind_method(D_METHOD("drain"), &SerComm::sercomm_drain);
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
	_port_enum = 0;
	port = nullptr;
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
	if (!opened || !port) {
		return 0;
	}
	return sp_input_waiting(port);
}

void SerComm::_ready()
{
	// Refresh ports when node enters the scene
	refresh_ports();

	// Auto-open the selected port if ports are available
	if (_ports.size() > 0 && _port_enum >= 0 && _port_enum < _ports.size()) {
		sercomm_open();
	}
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

	if (_ports.size() == 0) {
		UtilityFunctions::print("No ports available! Call initialize_ports() or refresh_ports() first.");
		return false;
	}

	if (_port_enum < 0 || _port_enum >= _ports.size()) {
		UtilityFunctions::print("Invalid port index: ", _port_enum, " (available: ", _ports.size(), ")");
		return false;
	}

	sp_return result = sp_get_port_by_name(_ports[_port_enum].c_str(), &port);
	if (result != SP_OK)
	{
		UtilityFunctions::print("Error getting port '", _ports[_port_enum].c_str(), "': ", sp_last_error_code(), " - ", sp_last_error_message());
		return false;
	}

	result = sp_open(port, SP_MODE_READ_WRITE);
	if (result != SP_OK)
	{
		UtilityFunctions::print("Error opening port '", _ports[_port_enum].c_str(), "': ", sp_last_error_code(), " - ", sp_last_error_message());
		sp_free_port(port);
		return false;
	}

	opened = true;

	// Configure port settings
	sp_set_baudrate(port, baud_rate);
	sp_set_bits(port, 8);
	sp_set_stopbits(port, 1);
	sp_set_parity(port, SP_PARITY_NONE);

	UtilityFunctions::print("Successfully opened port: ", _ports[_port_enum].c_str(), " (", baud_rate, " baud)");
	return true;
}

bool SerComm::sercomm_open_specific_serial_port(const String& port_name)
{
	if (opened) {
		return true;
	}

	std::string port_name_std = port_name.utf8().get_data();

	sp_return result = sp_get_port_by_name(port_name_std.c_str(), &port);
	if (result != SP_OK) {
		UtilityFunctions::print("Error getting port '", port_name_std.c_str(), "': ", sp_last_error_code(), " - ", sp_last_error_message());
		return false;
	}

	result = sp_open(port, SP_MODE_READ_WRITE);
	if (result != SP_OK) {
		UtilityFunctions::print("Error opening port '", port_name_std.c_str(), "': ", sp_last_error_code(), " - ", sp_last_error_message());
		sp_free_port(port);
		return false;
	}

	opened = true;

	// Configure port settings
	sp_set_baudrate(port, baud_rate);
	sp_set_bits(port, 8);
	sp_set_stopbits(port, 1);
	sp_set_parity(port, SP_PARITY_NONE);

	UtilityFunctions::print("Successfully opened port: ", port_name_std.c_str(), " (", baud_rate, " baud)");
	return true;
}

String SerComm::sercomm_read(const int num_bytes)
{
	if (!opened || !port) {
		UtilityFunctions::print("Port not opened for reading");
		return "";
	}

	if (num_bytes <= 0) {
		UtilityFunctions::print("Invalid read size: ", num_bytes);
		return "";
	}

	std::vector<char> read_buffer(num_bytes + 1);

	sp_return result = sp_nonblocking_read(port, read_buffer.data(), num_bytes);

	if (result < 0)
	{
		UtilityFunctions::print("Error reading data from port: ", sp_last_error_code(), " - ", sp_last_error_message());
		return "";
	}

	if (result > 0) {
		read_buffer[result] = '\0';
	}
	String data = String::utf8(read_buffer.data());

	if (data.length() > 0)
	{
		emit_signal("on_message", data);
	}

	return data;
}

void SerComm::sercomm_drain() {
	if (!opened || !port) {
		UtilityFunctions::print("Port not opened for draining");
		return;
	}
	int result = sp_drain(port);
	if (result < 0) {
		UtilityFunctions::print("Error draining port: ", sp_last_error_code(), " - ", sp_last_error_message());
	}
}

void SerComm::sercomm_write(const String &p_message)
{
	if (!opened || !port) {
		UtilityFunctions::print("Port not opened for writing");
		return;
	}

	const char *utf8_data = p_message.utf8().get_data();

	sp_return result = sp_nonblocking_write(port, utf8_data, std::strlen(utf8_data));
	if (result < 0)
	{
		UtilityFunctions::print("Error writing data to port: ", sp_last_error_code(), " - ", sp_last_error_message());
	}
};

void SerComm::refresh_ports()
{
	_ports.clear();
	sp_port **ports;
	sp_return error = sp_list_ports(&ports);

	if (error != SP_OK)
	{
		UtilityFunctions::print("Error listing ports: ", sp_last_error_code(), " - ", sp_last_error_message());
		_err_print_error(__FUNCTION__, __FILE__, __LINE__, "Error listing ports");
		if (ports) {
			sp_free_port_list(ports);
		}
		return;
	}

	UtilityFunctions::print("Found serial ports:");
	int port_count = 0;

	for (sp_port **ptr = ports; *ptr; ++ptr)
	{
		const char *l_port_name = sp_get_port_name(*ptr);
		if (l_port_name) {
			_ports.push_back(l_port_name);

			// Get additional port information
			const char *description = sp_get_port_description(*ptr);
			enum sp_transport transport = sp_get_port_transport(*ptr);

			String port_info = String("  Port ") + String::num(port_count) + String(": ") + String(l_port_name);
			if (description) {
				port_info += String(" (") + String(description) + String(")");
			}
			port_info += String(" - Transport: ");
			switch (transport) {
				case SP_TRANSPORT_NATIVE: port_info += "NATIVE"; break;
				case SP_TRANSPORT_USB: port_info += "USB"; break;
				case SP_TRANSPORT_BLUETOOTH: port_info += "BLUETOOTH"; break;
				default: port_info += "UNKNOWN"; break;
			}
			UtilityFunctions::print(port_info);
			port_count++;
		}
	}

	// Add tty0tty virtual serial ports (not auto-detected by libserialport). For development on Linux
	for (int i = 0; i < 8; ++i) {
		std::string tnt_path = "/dev/tnt" + std::to_string(i);
		FILE *file = fopen(tnt_path.c_str(), "r+");
		if (file) {
			fclose(file);
			_ports.push_back(tnt_path);
			UtilityFunctions::print("  Port ", port_count, ": ", tnt_path.c_str(), " (Virtual - TTY0TTY)");
			port_count++;
		}
	}

	if (port_count == 0) {
		UtilityFunctions::print("  No serial ports found");
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		port_enum_str = VariantHelper::_ports_to_hint_string(_ports);
	}

	sp_free_port_list(ports);
}

void SerComm::initialize_ports()
{
	refresh_ports();
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
