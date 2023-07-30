#include "util.hpp"

#include <godot_cpp/classes/engine.hpp>

auto util::to_vector2(const godot::Vector2i &vec2) -> godot::Vector2
{
	return {
		static_cast<real_t>(vec2.x),
		static_cast<real_t>(vec2.y),
	};
}

auto util::format_size(int64_t bytes) -> godot::String
{
	constexpr int64_t kilo = 1'000;
	constexpr int64_t mega = 1'000'000;
	constexpr int64_t giga = 1'000'000'000;

	godot::Dictionary values;

	if (bytes >= giga)
	{
		values["size"] = bytes / giga;
		values["unit"] = "GB";
	}
	else if (bytes >= mega)
	{
		values["size"] = bytes / mega;
		values["unit"] = "MB";
	}
	else if (bytes >= kilo)
	{
		values["size"] = bytes / kilo;
		values["unit"] = "KB";
	}
	else
	{
		values["size"] = bytes;
		values["unit"] = "B";
	}

	return godot::String("{size} {unit}").format(values);
}

auto util::is_editor() -> bool
{
	const auto *engine = godot::Engine::get_singleton();
	return engine->is_editor_hint();
}
