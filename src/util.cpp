#include "util.hpp"

auto util::to_vector2(const godot::Vector2i &vec2) -> godot::Vector2
{
	return {
		static_cast<real_t>(vec2.x),
		static_cast<real_t>(vec2.y),
	};
}
