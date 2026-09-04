#include "advanced_templates_and_metaprogramming.hpp"
#include <iostream>
#include <format>
#include <vector>

/**
 * ============================================================================
 * モジュール 09: 高度なテンプレート・メタプログラミング・Fold式・CRTP
 * ============================================================================
 */

namespace modern_cpp {

// CRTP を受け取るジェネリック関数 (テンプレート制約なしでも高速動作)
template <typename T>
void render_shape(const ShapeCRTP<T>& shape) {
    shape.draw();
}

void run_advanced_templates_demo() {
    std::cout << "--- 1. C++17 Fold Expressions (Variadic Templates) ---\n";
    // 単項右畳み込みによる加算
    int int_sum = sum_all(1, 2, 3, 4, 5);
    double dbl_sum = sum_all(1.5, 2.5, 3.0);
    std::cout << std::format("  sum_all(1, 2, 3, 4, 5) -> {}\n", int_sum);
    std::cout << std::format("  sum_all(1.5, 2.5, 3.0) -> {}\n", dbl_sum);

    // カンマ演算子とストリームの畳み込み
    std::cout << "  print_all: ";
    print_all(std::cout, "Values:", 42, 3.14, "C++23", true);

    std::cout << "\n--- 2. Compile-Time Branching via if constexpr & Type Traits ---\n";
    std::cout << "  describe_type(100):      " << describe_type(100) << "\n";
    std::cout << "  describe_type(3.1415):   " << describe_type(3.1415) << "\n";
    std::cout << "  describe_type(\"Modern\"): " << describe_type("Modern") << "\n";
    std::vector<int> dummy_vec{1, 2, 3};
    std::cout << "  describe_type(vector):   " << describe_type(dummy_vec) << "\n";

    std::cout << "\n--- 3. CRTP (Curiously Recurring Template Pattern) ---\n";
    CircleCRTP circle;
    SquareCRTP square;

    std::cout << "  Calling via CRTP generic function (Static Polymorphism):\n";
    render_shape(circle);
    render_shape(square);
}

} // namespace modern_cpp
