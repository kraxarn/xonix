#pragma once

#include <godot_cpp/classes/window.hpp>

class SettingsWindow: public godot::Window
{
GDCLASS(SettingsWindow, Window)

public:
	SettingsWindow();

	void _ready() override;

	void _on_close_requested();

protected:
	static void _bind_methods();
};
