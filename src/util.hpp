#pragma once

#include <godot_cpp/variant/utility_functions.hpp>

using gd = godot::UtilityFunctions;

class util
{
public:
	static auto to_vector2(const godot::Vector2i &vec2) -> godot::Vector2;

	static auto format_size(int64_t bytes) -> godot::String;

	static auto is_editor() -> bool;

private:
	util() = default;
};
