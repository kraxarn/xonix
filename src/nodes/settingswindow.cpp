#include "nodes/settingswindow.hpp"
#include "util.hpp"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/popup_menu.hpp>

SettingsWindow::SettingsWindow()
	: size_button(nullptr),
	save(nullptr)
{
}

void SettingsWindow::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("_on_close_requested"),
		&SettingsWindow::_on_close_requested);

	godot::ClassDB::bind_method(godot::D_METHOD("_on_Save_pressed"),
		&SettingsWindow::_on_Save_pressed);

	godot::ClassDB::bind_method(godot::D_METHOD("_on_SizeButton_index_pressed"),
		&SettingsWindow::_on_SizeButton_index_pressed);
}

void SettingsWindow::_ready()
{
	Node::_ready();

	connect("close_requested", {this, "_on_close_requested"});

	size_button = get_node<godot::MenuButton>("Container/Grid/SizeButton");

	const auto screen_size = godot::DisplayServer::get_singleton()->screen_get_size();
	const auto root_size = util::to_vector2(get_tree()->get_root()->get_size());
	auto scale = min_scale;

	while (true)
	{
		const auto size = root_size * scale;
		if (size > screen_size)
		{
			break;
		}

		scale += scale_step;

		godot::Dictionary values;
		values["x"] = size.x;
		values["y"] = size.y;

		const auto label = godot::String("{x}x{y}").format(values);
		size_button->get_popup()->add_item(label);
	}

	size_button->get_popup()->connect("index_pressed",
		{this, "_on_SizeButton_index_pressed"});

	save = get_node<godot::Button>("Container/Save");
	save->connect("pressed", {this, "_on_Save_pressed"});
}

void SettingsWindow::_on_close_requested()
{
	hide();
}

void SettingsWindow::_on_Save_pressed()
{
	hide();
}

void SettingsWindow::_on_SizeButton_index_pressed(int index)
{
	const auto text = size_button->get_popup()->get_item_text(index);
	size_button->set_text(text);

	const auto parts = text.split("x");
	const auto width = parts[0].to_int();
	const auto height = parts[1].to_int();

	get_tree()->get_root()->set_size({
		static_cast<int32_t>(width),
		static_cast<int32_t>(height),
	});
}
