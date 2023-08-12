#include "nodes/perfmon.hpp"
#include "util.hpp"

#include <godot_cpp/classes/performance.hpp>

PerfMon::PerfMon()
	: fps(nullptr),
	process(nullptr),
	object_count(nullptr),
	resource_count(nullptr),
	node_count(nullptr)
{
}

void PerfMon::_bind_methods()
{
}

void PerfMon::_ready()
{
	Node::_ready();

	fps = get_node<godot::Label>("Fps");
	process = get_node<godot::Label>("Process");
	object_count = get_node<godot::Label>("ObjectCount");
	resource_count = get_node<godot::Label>("ResourceCount");
	node_count = get_node<godot::Label>("NodeCount");
}

void PerfMon::_process(double delta)
{
	Node::_process(delta);

	if (util::is_editor())
	{
		return;
	}

	constexpr int milli = 1'000;
	auto *performance = godot::Performance::get_singleton();

	godot::Dictionary values;
	values["fps"] = performance->get_monitor(godot::Performance::TIME_FPS);
	values["process"] = performance->get_monitor(godot::Performance::TIME_PROCESS) * milli;
	values["object_count"] = performance->get_monitor(godot::Performance::OBJECT_COUNT);
	values["resource_count"] = performance->get_monitor(godot::Performance::OBJECT_RESOURCE_COUNT);
	values["node_count"] = performance->get_monitor(godot::Performance::OBJECT_NODE_COUNT);

	fps->set_text(godot::String("{fps}").format(values));
	process->set_text(godot::String("{process} ms").format(values));
	object_count->set_text(godot::String("{object_count}").format(values));
	resource_count->set_text(godot::String("{resource_count}").format(values));
	node_count->set_text(godot::String("{node_count}").format(values));
}
