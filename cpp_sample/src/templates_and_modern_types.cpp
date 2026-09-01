#include "templates_and_modern_types.hpp"
#include <iostream>
#include <string>
#include <optional>
#include <variant>
#include <concepts>

/**
 * ============================================================================
 * C++ 04: テンプレート・Concepts・代数的データ型 (Templates, Concepts, Optional & Variant)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. C++20 Concepts (コンセプト):
 *    - Rustの `trait bounds` (`where T: Add + Display`) や Goの `interface` 制約に相当。
 *    - テンプレート引数に型制約をかけ、読みやすいコンパイルエラーを提供します。
 * 
 * 2. std::optional<T> (C++17+):
 *    - Rustの `Option<T>`, Javaの `Optional<T>`。値の存在・非存在を型安全に表現。
 *    - `value_or()`, `has_value()`。
 * 
 * 3. std::variant<Ts...> と std::visit (C++17+):
 *    - Rustの `enum` (代数的データ型 ADT) に相当する型安全な Union。
 *    - `std::visit` を使って全パターンを網羅（パターンマッチング `match`）。
 */

namespace modern_cpp {

// --- 1. Concepts: 数値型 (整数または浮動小数点) に限定する制約 ---
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Numeric コンセプトを満たす型しか受け付けないテンプレート関数 (Rustの fn add<T: Numeric>(a: T, b: T) -> T)
template <Numeric T>
T add(T a, T b) {
    return a + b;
}

// --- 2. std::optional による null 安全な検索 ---
struct User {
    int id;
    std::string name;
};

static std::optional<User> find_user_by_id(int id) {
    if (id == 1) {
        return User{1, "Haru"};
    }
    return std::nullopt; // 値なし (Rustの None, Javaの Optional.empty())
}

// --- 3. std::variant による代数的データ型 (Rustの enum 相当) ---
struct CreditCard { std::string card_number; };
struct BankTransfer { std::string account_number; };
struct Crypto { std::string wallet_address; };

// 支払い方法の代数的データ型 (Rustの enum PaymentMethod { CreditCard(...), BankTransfer(...), Crypto(...) })
using PaymentMethod = std::variant<CreditCard, BankTransfer, Crypto>;

// std::visit 用のオーバーロードヘルパー構造体 (パターンマッチング糖衣構文)
template <class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

void run_templates_and_modern_types_demo() {
    std::cout << "--- 1. Type-Safe Generics via C++20 Concepts ---\n";
    std::cout << "  add(10, 20):       " << add(10, 20) << " (int)\n";
    std::cout << "  add(3.14, 2.71):   " << add(3.14, 2.71) << " (double)\n";
    // add("A", "B"); // コンパイルエラー: 文字列は Numeric を満たさない

    std::cout << "\n--- 2. std::optional<T> (Rust Option<T>) ---\n";
    auto user1 = find_user_by_id(1);
    if (user1) {
        std::cout << "  User 1 Found: " << user1->name << std::endl;
    }

    auto user99 = find_user_by_id(99);
    std::cout << "  User 99 (value_or): " 
              << (user99 ? user99->name : "Guest (Not Found)") << std::endl;

    std::cout << "\n--- 3. std::variant & std::visit (Rust enum & match) ---\n";
    PaymentMethod payment1 = CreditCard{"1234-5678-9012-3456"};
    PaymentMethod payment2 = Crypto{"0xabcdef1234567890"};

    auto print_payment = [](const PaymentMethod& pm) {
        // std::visit でパターンマッチングを実行
        std::visit(Overloaded{
            [](const CreditCard& cc) {
                std::cout << "  [Payment] CreditCard Payment: " << cc.card_number << std::endl;
            },
            [](const BankTransfer& bt) {
                std::cout << "  [Payment] BankTransfer Payment: " << bt.account_number << std::endl;
            },
            [](const Crypto& cr) {
                std::cout << "  [Payment] Crypto Payment: " << cr.wallet_address << std::endl;
            }
        }, pm);
    };

    print_payment(payment1);
    print_payment(payment2);
}

} // namespace modern_cpp
