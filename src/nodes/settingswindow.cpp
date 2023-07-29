#include "nodes/settingswindow.hpp"

SettingsWindow::SettingsWindow()
{
}

void SettingsWindow::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("_on_close_requested"),
		&SettingsWindow::_on_close_requested);
}

void SettingsWindow::_ready()
{
	Node::_ready();

	connect("close_requested", {this, "_on_close_requested"});
}

void SettingsWindow::_on_close_requested()
{
	hide();
}
