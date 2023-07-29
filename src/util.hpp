#pragma once

#include <godot_cpp/variant/utility_functions.hpp>

using gd = godot::UtilityFunctions;

class util
{
public:
	static auto to_vector2(const godot::Vector2i &vec2) -> godot::Vector2;

private:
	util() = default;
};
