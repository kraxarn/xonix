#include "nodes/menu.hpp"
#include "util.hpp"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

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

	if (!util::is_editor())
	{
		auto *debug_info = get_node<godot::Label>("DebugInfo");
		debug_info->set_text(get_system_info());
	}
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

auto Menu::get_system_info() -> godot::String
{
	const auto *os_info = godot::OS::get_singleton();
	auto *render_info = godot::RenderingServer::get_singleton();

	godot::Dictionary values;
	values["name"] = os_info->get_name();
	values["distribution_name"] = os_info->get_distribution_name();
	values["locale_language"] = os_info->get_locale_language();
	values["processor_count"] = os_info->get_processor_count();
	values["processor_name"] = os_info->get_processor_name();

	values["video_adapter_name"] = render_info->get_video_adapter_name();
	values["video_adapter_vendor"] = render_info->get_video_adapter_vendor();
	values["video_adapter_api_version"] = render_info->get_video_adapter_api_version();

	if (os_info->is_debug_build())
	{
		values["title"] = "Debug Mode";
	}

	return godot::String(
		"{title}\n"
		"OS: {name} ({distribution_name})\n"
		"CPU: {processor_name} ({processor_count} cores)\n"
		"GPU: {video_adapter_vendor} - {video_adapter_name} (OpenGL {video_adapter_api_version})\n"
	).format(values);
}
