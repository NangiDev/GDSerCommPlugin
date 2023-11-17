#ifndef SERIAL_EVENT_H
#define SERIAL_EVENT_H

#include <godot_cpp/classes/ref_counted.hpp>

using namespace std;
using namespace godot;

class SerialEvent : public RefCounted
{
    GDCLASS(SerialEvent, RefCounted);

public:
    SerialEvent();
    SerialEvent(string p_Message);
    ~SerialEvent();

    String _to_string() const;

protected:
    static void _bind_methods();

private:
    string m_Message;
};

#endif // SERIAL_EVENT_H
