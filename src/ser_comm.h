#ifndef SERCOMM_H
#define SERCOMM_H

#include <godot_cpp/classes/node.hpp>
#include "custom_serial_port_event.h"

namespace godot
{

	class SerComm : public Node
	{
		GDCLASS(SerComm, Node)

		enum BaudRate
		{
			BAUD_110 = 110,
			BAUD_300 = 300,
			BAUD_600 = 600,
			BAUD_1200 = 1200,
			BAUD_2400 = 2400,
			BAUD_4800 = 4800,
			BAUD_9600 = 9600,
			BAUD_14400 = 14400,
			BAUD_19200 = 19200,
			BAUD_38400 = 38400,
			BAUD_57600 = 57600,
			BAUD_115200 = 115200,
			BAUD_128000 = 128000,
			BAUD_256000 = 256000,
		};

	private:
		int baud_rate;

	protected:
		static void _bind_methods();
		static String baudRateToHintString()
		{
			String hint_string = "";
			const int baud_rates[] = {110, 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 128000, 256000};
			for (int i = 0; i < sizeof(baud_rates) / sizeof(baud_rates[0]); ++i)
			{
				if (!hint_string.is_empty())
				{
					hint_string += ", ";
				}
				hint_string += String::num_int64(baud_rates[i]) + ":" + String::num_int64(baud_rates[i]);
			}
			return hint_string;
		};

	public:
		SerComm();
		~SerComm();

		void _process(double delta) override;

		void set_baud_rate(const int p_baud_rate);
		int get_baud_rate() const;
	};
}

#endif