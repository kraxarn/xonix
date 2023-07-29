#pragma once

#include <godot_cpp/classes/area2d.hpp>

class Player: public godot::Area2D
{
GDCLASS(Player, Area2D)

public:
	Player();

protected:
	static void _bind_methods();

private:
};
