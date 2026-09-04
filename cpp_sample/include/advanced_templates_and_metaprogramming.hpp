#ifndef ADVANCED_TEMPLATES_AND_METAPROGRAMMING_HPP
#define ADVANCED_TEMPLATES_AND_METAPROGRAMMING_HPP

#include <iostream>
#include <string>
#include <type_traits>

/**
 * ============================================================================
 * モジュール 09: 高度なテンプレート・メタプログラミング・Fold式・CRTP
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. 可変引数テンプレート (Variadic Templates) & Fold Expressions (C++17):
 *    - 任意の数の引数を型安全に受け取る仕組み。
 *    - Fold Expressions `(... + args)` により、再帰関数なしで一括加算や処理が可能。
 * 
 * 2. if constexpr (コンパイル時分岐):
 *    - 偽となったブランチはコンパイルすらされない（型チェックは通る）。
 *    - かつての複雑な SFINAE (Substitution Failure Is Not An Error) がほぼ不要に。
 * 
 * 3. CRTP (Curiously Recurring Template Pattern):
 *    - 派生クラスが基底クラスのテンプレート引数になるイディオム。
 *    - vtable（仮想関数テーブル）のポインタ参照オーバーヘッドをゼロにし、
 *      コンパイル時にインライン展開可能な静的ポリモーフィズムを実現。
 */

namespace modern_cpp {

// 1. Fold Expressions (畳み込み式) による可変引数合計計算
template <typename... Args>
auto sum_all(Args... args) {
    return (... + args);
}

// 2. Fold Expressions による一括プリント
template <typename... Args>
void print_all(std::ostream& os, const Args&... args) {
    ((os << args << " "), ...) << "\n";
}

// 3. if constexpr と <type_traits> による型に応じたコンパイル時分岐
template <typename T>
std::string describe_type(const T& val) {
    using DecayedT = std::decay_t<T>;
    if constexpr (std::is_integral_v<DecayedT>) {
        return "Integral: " + std::to_string(val);
    } else if constexpr (std::is_floating_point_v<DecayedT>) {
        return "Floating point: " + std::to_string(val);
    } else if constexpr (std::is_same_v<DecayedT, std::string> || std::is_same_v<DecayedT, const char*>) {
        return "String value: " + std::string(val);
    } else {
        return "Custom or unsupported type";
    }
}

// 4. CRTP (Curiously Recurring Template Pattern) 静的ポリモーフィズム
template <typename Derived>
class ShapeCRTP {
public:
    void draw() const {
        // static_cast で派生クラスの型へキャストし、直接メンバ関数を呼び出す
        // 仮想関数テーブル(vtable)参照のオーバーヘッドが完全にゼロ (インライン展開可能)
        static_cast<const Derived*>(this)->draw_impl();
    }
};

class CircleCRTP : public ShapeCRTP<CircleCRTP> {
public:
    void draw_impl() const {
        std::cout << "    [CRTP Circle] Drawing static circle (No vtable overhead!)\n";
    }
};

class SquareCRTP : public ShapeCRTP<SquareCRTP> {
public:
    void draw_impl() const {
        std::cout << "    [CRTP Square] Drawing static square (No vtable overhead!)\n";
    }
};

// モジュール実行エントリポイント
void run_advanced_templates_demo();

} // namespace modern_cpp

#endif // ADVANCED_TEMPLATES_AND_METAPROGRAMMING_HPP
