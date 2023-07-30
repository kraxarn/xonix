#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/label.hpp>

class Menu: public godot::Node
{
GDCLASS(Menu, Node)

public:
	Menu();

	void _ready() override;

	void _on_StartGame_pressed();
	void _on_Settings_pressed();
	void _on_QuitGame_pressed();

protected:
	static void _bind_methods();

private:
	godot::Button *start_game;
	godot::Button *settings;
	godot::Button *quit_game;
	godot::Window *settings_window;

	static auto get_system_info() -> godot::String;
};
