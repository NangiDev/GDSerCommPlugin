#include "ser_comm.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SerComm::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_baud_rate"), &SerComm::get_baud_rate);
	ClassDB::bind_method(D_METHOD("set_baud_rate", "p_baud_rate"), &SerComm::set_baud_rate);

	// String hint_string = "110:110, 300:300, 600:600, 1200:1200, 2400:2400, 4800:4800, 9600:9600, 14400:14400, 19200:19200, 38400:38400, 57600:57600, 115200:115200, 128000:128000, 256000:256000";
	ClassDB::add_property("SerComm", PropertyInfo(Variant::INT, "baud_rate", PROPERTY_HINT_ENUM, baudRateToHintString()), "set_baud_rate", "get_baud_rate");

	ADD_SIGNAL(MethodInfo("serial_event_received", PropertyInfo(Variant::OBJECT, "event")));
}

SerComm::SerComm()
{
	// Initialize any variables here.
	baud_rate = BAUD_9600;
}

SerComm::~SerComm()
{
	// Add your cleanup here.
}

void SerComm::_process(double delta)
{
	std::string message = "This just in: " + std::to_string(baud_rate);
	Ref<CustomSerialPortEvent> event = memnew(CustomSerialPortEvent(message.c_str()));
	emit_signal("serial_event_received", event);
}

void SerComm::set_baud_rate(const int p_baudrate)
{
	baud_rate = p_baudrate;
}

int SerComm::get_baud_rate() const
{
	return baud_rate;
}