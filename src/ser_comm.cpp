#include "ser_comm.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SerComm::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_baud_rate"), &SerComm::get_baud_rate);
	ClassDB::bind_method(D_METHOD("set_baud_rate", "p_baud_rate"), &SerComm::set_baud_rate);
	ClassDB::add_property("SerComm", PropertyInfo(Variant::INT, "baud_rate", PROPERTY_HINT_ENUM, baudRateToHintString()), "set_baud_rate", "get_baud_rate");

	ClassDB::bind_method(D_METHOD("write_serial_event", "p_event"), &SerComm::write_serial_event);
	ADD_SIGNAL(MethodInfo("read_serial_event", PropertyInfo(Variant::OBJECT, "event")));
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
	Ref<SerialEvent> event = memnew(SerialEvent(message));
	emit_signal("read_serial_event", event);
}

void godot::SerComm::write_serial_event(const Ref<SerialEvent> &p_event)
{
	String test = "Messag from inside: ";
	String message = test + p_event->_to_string();
	_err_print_error(__FUNCTION__, __FILE__, __LINE__, message);
};

void SerComm::set_baud_rate(const int p_baudrate)
{
	baud_rate = p_baudrate;
}

int SerComm::get_baud_rate() const
{
	return baud_rate;
}