#include "custom_serial_port_event.h"

using namespace godot;

void CustomSerialPortEvent::_bind_methods() {
}


CustomSerialPortEvent::CustomSerialPortEvent() {
	// Initialize any variables here.
}

CustomSerialPortEvent::~CustomSerialPortEvent() {
	// Add your cleanup here.
}

String CustomSerialPortEvent::_to_string() const {
    return "CustomSerialPortEvent, Message: " + m_Message;
}
// Implement your custom properties and methods here