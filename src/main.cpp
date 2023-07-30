#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "nodes/player.hpp"
#include "nodes/menu.hpp"
#include "nodes/settingswindow.hpp"
#include "nodes/perfmon.hpp"

void initialize_module(godot::ModuleInitializationLevel level)
{
	if (level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}

	godot::ClassDB::register_class<Player>();
	godot::ClassDB::register_class<Menu>();
	godot::ClassDB::register_class<SettingsWindow>();
	godot::ClassDB::register_class<PerfMon>();
}

void terminate_module(godot::ModuleInitializationLevel level)
{
	// ...
}

extern "C" {
auto GDE_EXPORT library_init(GDExtensionInterfaceGetProcAddress address,
	const GDExtensionClassLibraryPtr library,
	GDExtensionInitialization *initialization) -> GDExtensionBool
{
	const godot::GDExtensionBinding::InitObject init(address, library, initialization);
	init.register_initializer(initialize_module);
	init.register_terminator(terminate_module);
	init.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
	return init.init();
}
}
