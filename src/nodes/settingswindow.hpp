#pragma once

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/menu_button.hpp>

class SettingsWindow: public godot::Window
{
GDCLASS(SettingsWindow, Window)

public:
	SettingsWindow();

	void _ready() override;

	void _on_close_requested();
	void _on_Save_pressed();
	void _on_SizeButton_index_pressed(int index);

protected:
	static void _bind_methods();

private:
	static constexpr real_t min_scale = 0.75;
	static constexpr real_t scale_step = 0.25;

	godot::MenuButton *size_button;
	godot::Button *save;
};
