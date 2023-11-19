#define FMT_HEADER_ONLY

#include "fmt/format.h"
#include "serial_event.h"

using namespace std;
using namespace godot;

void SerialEvent::_bind_methods()
{
}

SerialEvent::SerialEvent()
{
	m_Message = string();
}

SerialEvent::SerialEvent(string p_Message)
{
	m_Message = p_Message;
}

SerialEvent::~SerialEvent()
{
	// Add your cleanup here.
}

String SerialEvent::_to_string() const
{
	return fmt::format("SerialEvent - [ Message: {} ]", m_Message).c_str();
}
// Implement your custom properties and methods here