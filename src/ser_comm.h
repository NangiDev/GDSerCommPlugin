#ifndef SERCOMM_H
#define SERCOMM_H

#include <godot_cpp/classes/sprite2d.hpp>
#include "custom_serial_port_event.h"

namespace godot
{
	class SerComm : public Sprite2D
	{
		GDCLASS(SerComm, Sprite2D)

	private:
		double time_passed;
		double time_emit;
		double amplitude;
		double speed;

	protected:
		static void _bind_methods();
		virtual void _serial(const godot::Ref<CustomSerialPortEvent>& event) {};

	public:
		SerComm();
		~SerComm();

		void _process(double delta) override;

		void set_amplitude(const double p_amplitude);
		double get_amplitude() const;

		void set_speed(const double p_speed);
		double get_speed() const;
	};

}

#endif