#include "nodes/menu.hpp"

#include <godot_cpp/classes/scene_tree.hpp>

Menu::Menu()
	: start_game(nullptr),
	settings(nullptr),
	quit_game(nullptr)
{
}

void Menu::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("_on_StartGame_pressed"),
		&Menu::_on_StartGame_pressed);

	godot::ClassDB::bind_method(godot::D_METHOD("_on_Settings_pressed"),
		&Menu::_on_Settings_pressed);

	godot::ClassDB::bind_method(godot::D_METHOD("_on_QuitGame_pressed"),
		&Menu::_on_QuitGame_pressed);
}

void Menu::_ready()
{
	Node::_ready();

	start_game = get_node<godot::Button>("Buttons/StartGame");
	start_game->connect("pressed", {this, "_on_StartGame_pressed"});

	settings = get_node<godot::Button>("Buttons/Settings");
	settings->connect("pressed", {this, "_on_Settings_pressed"});

	quit_game = get_node<godot::Button>("Buttons/QuitGame");
	quit_game->connect("pressed", {this, "_on_QuitGame_pressed"});

	settings_window = get_node<godot::Window>("SettingsWindow");
}

void Menu::_on_StartGame_pressed()
{
}

void Menu::_on_Settings_pressed()
{
	settings_window->show();
}

void Menu::_on_QuitGame_pressed()
{
	get_tree()->quit();
}
