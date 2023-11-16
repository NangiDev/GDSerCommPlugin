#include "ser_comm.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SerComm::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_amplitude"), &SerComm::get_amplitude);
	ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"), &SerComm::set_amplitude);
	ClassDB::add_property("SerComm", PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude", "get_amplitude");

	ClassDB::bind_method(D_METHOD("get_speed"), &SerComm::get_speed);
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &SerComm::set_speed);
	ClassDB::add_property("SerComm", PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,20,0.01"), "set_speed", "get_speed");

	ADD_SIGNAL(MethodInfo("position_changed", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::VECTOR2, "new_pos")));
	ADD_SIGNAL(MethodInfo("serial_event_received", PropertyInfo(Variant::OBJECT, "event")));
}

SerComm::SerComm()
{
	// Initialize any variables here.
	time_passed = 0.0;
	time_emit = 0.0;
	amplitude = 10.0;
	speed = 1.0;
}

SerComm::~SerComm()
{
	// Add your cleanup here.
}

void SerComm::_process(double delta)
{
	time_passed += speed * delta;

	Vector2 new_position = Vector2(
		amplitude + (amplitude * sin(time_passed * 2.0)),
		amplitude + (amplitude * cos(time_passed * 1.5)));

	set_position(new_position);

	time_emit += delta;
	if (time_emit > 1.0)
	{
		Ref<CustomSerialPortEvent> event = memnew(CustomSerialPortEvent("This just in!"));
		emit_signal("serial_event_received", event);
		emit_signal("position_changed", this, new_position);

		time_emit = 0.0;
	}
}

void SerComm::set_amplitude(const double p_amplitude)
{
	amplitude = p_amplitude;
}

double SerComm::get_amplitude() const
{
	return amplitude;
}

void SerComm::set_speed(const double p_speed)
{
	speed = p_speed;
}

double SerComm::get_speed() const
{
	return speed;
}