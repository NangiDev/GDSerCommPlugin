#ifndef SERCOMM_H
#define SERCOMM_H

#include <godot_cpp/classes/node.hpp>
#include <libserialport.h>
#include "variant_helper.h"

namespace godot
{
	using namespace std;

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
		sp_port *port;
		int _port_enum;
		std::vector<std::string> _ports{};
		godot::String port_enum_str;

		bool opened = false;

	protected:
		static void _bind_methods();

	public:
		SerComm();
		~SerComm();

		void _process(double delta) override;

		int sercomm_get_waiting();
		String sercomm_read(const int num_bytes);
		
		void set_port(const int p);
		int get_port() const;

		void set_baud_rate(const int b);
		int get_baud_rate() const;

		bool get_open() const;

		godot::Array sercomm_list_ports();
		bool sercomm_open();

		void sercomm_close();
		void sercomm_flush();

		void sercomm_write(const String &p_message);

		void refresh_ports();

		void _get_property_list(List<PropertyInfo> *r_list) const;
		bool _get(const StringName &p_name, Variant &r_value) const;
		bool _set(const StringName &p_name, const Variant &p_value);
	};
}

#endif // SERCOMM_H
