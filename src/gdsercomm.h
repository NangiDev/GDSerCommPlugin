#ifndef GDSERCOMM_H
#define GDSERCOMM_H

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot
{

    class GDSerComm : public Sprite2D
    {
        GDCLASS(GDSerComm, Sprite2D)

    private:
        double time_passed;
	    double time_emit;
        double amplitude;
	    double speed;

    protected:
        static void _bind_methods();

    public:
        GDSerComm();
        ~GDSerComm();

        void _process(double delta) override;

        void set_amplitude(const double p_amplitude);
        double get_amplitude() const;

        void set_speed(const double p_speed);
        double get_speed() const;
    };

}

#endif