#pragma once

#include <string>

namespace ember::perf {

void enable(bool v);
bool is_enabled();

void start_frame();
void end_frame();

void start_section(const std::string& name);
void end_section();

} // namespace perf
