#pragma once

#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/label.hpp>

class PerfMon: public godot::GridContainer
{
GDCLASS(PerfMon, GridContainer)

public:
	PerfMon();

	void _ready() override;
	void _process(double delta) override;

protected:
	static void _bind_methods();

private:
	godot::Label *fps;
	godot::Label *process;
	godot::Label *object_count;
	godot::Label *resource_count;
	godot::Label *node_count;
};
