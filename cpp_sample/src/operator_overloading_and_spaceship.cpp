#include "operator_overloading_and_spaceship.hpp"
#include <iostream>
#include <format>
#include <algorithm>

/**
 * ============================================================================
 * モジュール 10: 演算子オーバーロードと C++20 宇宙船演算子 (<=>)
 * ============================================================================
 */

namespace modern_cpp {

Vector3D& Vector3D::operator+=(const Vector3D& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3D operator+(Vector3D lhs, const Vector3D& rhs) {
    lhs += rhs;
    return lhs;
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
    os << std::format("Vector3D({:.1f}, {:.1f}, {:.1f})", v.x, v.y, v.z);
    return os;
}

void run_operator_overloading_demo() {
    std::cout << "--- 1. Arithmetic & Stream Operator Overloading ---\n";
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    Vector3D v3 = v1 + v2;
    std::cout << "  v1: " << v1 << "\n";
    std::cout << "  v2: " << v2 << "\n";
    std::cout << "  v1 + v2 = " << v3 << "\n";

    std::cout << "\n--- 2. C++20 Spaceship Operator (<=>) and Comparison Derivation ---\n";
    PlayerScore alice("Alice", 10, 500);
    PlayerScore bob("Bob", 8, 800);
    PlayerScore alice_clone("Alice", 10, 500);

    // operator<=> により、全比較演算子がコンパイル可能
    std::cout << std::format("  alice == alice_clone: {}\n", (alice == alice_clone) ? "true" : "false");
    std::cout << std::format("  alice < bob:          {}\n", (alice < bob) ? "true (Alice is lexicographically before Bob)" : "false");
    std::cout << std::format("  alice > bob:          {}\n", (alice > bob) ? "true" : "false");

    // std::sort で <=> の比較がそのまま利用される
    std::vector<PlayerScore> players = {
        {"Charlie", 12, 100},
        {"Alice", 10, 500},
        {"Bob", 8, 800}
    };
    std::sort(players.begin(), players.end());

    std::cout << "  Sorted Players (by name -> level -> points):\n";
    for (const auto& p : players) {
        std::cout << std::format("    - {}: Level {}, Points {}\n", p.name, p.level, p.points);
    }

    std::cout << "\n--- 3. Custom Container with begin()/end() and Range-based for ---\n";
    SimpleBuffer<std::string> buffer;
    buffer.add("Modern");
    buffer.add("C++23");
    buffer.add("Rocks");

    std::cout << "  Iterating over SimpleBuffer via Range-based for:\n";
    for (const auto& item : buffer) {
        std::cout << std::format("    * Item: {}\n", item);
    }
    std::cout << std::format("  Subscript operator buffer[1] -> {}\n", buffer[1]);
}

} // namespace modern_cpp
