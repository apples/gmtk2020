#include "perf.hpp"

#include <unordered_map>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>

namespace ember::perf {

namespace {

using clock = std::chrono::high_resolution_clock;

struct section_data {
    int hits = 0;
    double total_time = 0;
    clock::time_point start;
};

bool enabled = false;
bool should_enable = false;
std::unordered_map<std::string, section_data> sections;
std::vector<std::string> section_stack;
int frame_count = 0;

} // namespace

void enable(bool v) {
    should_enable = v;
}

bool is_enabled() {
    return enabled;
}

void start_frame() {
    enabled = should_enable;
    if (enabled) {
        section_stack.clear();
        start_section("[FRAME]");
    }
}

void end_frame() {
    if (enabled) {
        end_section();
        ++frame_count;
        if (frame_count >= 120 || sections["[FRAME]"].total_time > 2) {
            std::vector<decltype(sections)::const_iterator> sorted_sections;
            for (auto i = begin(sections); i != end(sections); ++i) {
                sorted_sections.push_back(i);
            }
            std::sort(begin(sorted_sections), end(sorted_sections), [](auto& a, auto& b) {
                return a->second.total_time > b->second.total_time;
            });
            std::cout << "PERF (" << frame_count << " frames)\n";
            for (const auto& iter : sorted_sections) {
                const auto& [name, sec] = *iter;
                auto avg = sec.total_time / sec.hits;
                std::cout << "  " << name << ": hits = " << sec.hits << ", total = " << std::fixed
                          << std::setprecision(3) << sec.total_time << ", avg = " << avg << "\n";
            }
            frame_count = 0;
            sections.clear();
        }
    }
}

void start_section(const char* name) {
    if (enabled) {
        auto& sec = sections[name];
        sec.hits += 1;
        sec.start = clock::now();
        section_stack.push_back(name);
    }
}

void end_section() {
    if (enabled) {
        auto& sec = sections[section_stack.back()];
        sec.total_time += std::chrono::duration_cast<std::chrono::duration<double>>(clock::now() - sec.start).count();
        section_stack.pop_back();
    }
}

} // namespace perf
