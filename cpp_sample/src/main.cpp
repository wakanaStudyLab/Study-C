#include <iostream>
#include "raii_and_smart_pointers.hpp"
#include "move_semantics_and_classes.hpp"
#include "stl_and_ranges.hpp"
#include "templates_and_modern_types.hpp"
#include "concurrency.hpp"

/**
 * ============================================================================
 * Modern C++ 最速習得コース (For Rust / C# / Go Developers) - Main Entry
 * ============================================================================
 */

static void print_banner(const std::string& title) {
    std::cout << "\n================================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "================================================================\n\n";
}

static void print_section(const std::string& title) {
    std::cout << "\n################################################################\n";
    std::cout << "# " << title << "\n";
    std::cout << "################################################################\n\n";
}

int main() {
    print_banner("MODERN C++ (C++20/C++23) CRASH COURSE");

    print_section("01: RAII and Smart Pointers (unique_ptr / shared_ptr)");
    modern_cpp::run_raii_and_smart_pointers_demo();

    print_section("02: Move Semantics (std::move) and Modern Class Design");
    modern_cpp::run_move_semantics_demo();

    print_section("03: STL Containers & C++20 Ranges Pipeline");
    modern_cpp::run_stl_and_ranges_demo();

    print_section("04: C++20 Concepts, std::optional, std::variant (Algebraic Data Types)");
    modern_cpp::run_templates_and_modern_types_demo();

    print_section("05: Concurrency, Async Future, and C++20 jthread");
    modern_cpp::run_concurrency_demo();

    print_banner("ALL C++ TUTORIAL MODULES COMPLETED SUCCESSFULLY!");
    return 0;
}
