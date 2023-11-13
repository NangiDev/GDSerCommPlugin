#ifndef GDSERCOMM_H
#define GDSERCOMM_H

#include <godot_cpp/classes/node.hpp>

namespace godot
{

    class GDSerComm : public Node
    {
        GDCLASS(GDSerComm, Node)

    private:
        double time_passed;

    protected:
        static void _bind_methods();

    public:
        GDSerComm();
        ~GDSerComm();

        void _process(double delta);
    };

}

#endif