#ifndef CUSTOM_SERIAL_PORT_EVENT_H
#define CUSTOM_SERIAL_PORT_EVENT_H

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class CustomSerialPortEvent : public RefCounted {
    GDCLASS(CustomSerialPortEvent, RefCounted);

public:
	CustomSerialPortEvent();
	~CustomSerialPortEvent();

    String CustomSerialPortEvent::_to_string() const;

protected:
    static void _bind_methods();

private:
    String m_Message;
};

#endif // CUSTOM_SERIAL_PORT_EVENT_H