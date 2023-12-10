#include <godot_cpp/classes/object.hpp>

using namespace godot;

class VariantHelper
{
public:
    static String _baud_rate_to_hint_string()
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

    static String _ports_to_hint_string(const std::vector<std::string> &_ports)
    {
        String types = "No Ports Found";
        for (size_t i = 0; i < _ports.size(); i++)
        {
            if (i == 0)
                types = _ports[i].c_str();
            else
            {
                types += ",";
                types += _ports[i].c_str();
            }
        }

        return types;
    }
};
