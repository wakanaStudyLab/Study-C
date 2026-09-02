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
 *
 * 1. C++20 Concepts (コンセプト):
 *    - Rustの `trait bounds` (`where T: Add + Display`) や Go の `interface` 制約に相当。
 *    - テンプレート引数に型制約をかけ、読みやすいコンパイルエラーを提供する。
 *    - C++20 以前のテンプレートエラー (SFINAE) は数百行の難解なエラーメッセージを出すことがあったが、
 *      Concepts を使えば「このコンセプトを満たしていない」という明確なエラーになる。
 *    - 組み込みコンセプト例: std::integral<T>, std::floating_point<T>, std::copyable<T>,
 *      std::invocable<F, Args...>, std::ranges::range<R> など。
 *
 * 2. std::optional<T> (C++17+):
 *    - Rustの `Option<T>`, Javaの `Optional<T>`, Swift の `T?` に相当。
 *    - 関数が「値を返せないかもしれない」場合に nullptr や -1 などのセンチネル値を使わず、
 *      型システムで「値の有無」を安全に表現する。
 *    - std::nullopt: 値なしを表す定数 (Rust の None, Swift の nil に相当)
 *    - アクセス方法:
 *        opt.has_value()   → bool (値があるかどうか)
 *        if (opt) { ... }  → has_value() の糖衣構文
 *        opt.value()       → T& (値がなければ std::bad_optional_access 例外)
 *        opt->member       → T のメンバへの直接アクセス
 *        opt.value_or(x)   → T (値がなければデフォルト x を返す)
 *
 * 3. std::variant<Ts...> と std::visit (C++17+):
 *    - Rustの `enum` (代数的データ型 ADT)、Haskell の `Either`、Swift の `enum` に相当する型安全な Union。
 *    - C の union と異なり、どの型が格納されているかを型情報として管理するため安全。
 *    - `std::visit` を使って全パターンを網羅的に処理する（Rust の `match`、Haskell の `case of` に相当）。
 *    - `std::get<T>(v)` で特定の型を取り出すことも可能だが、wrong type では例外 (std::bad_variant_access)。
 */

namespace modern_cpp {

// ============================================================================
// 1. Concepts: 型制約付きテンプレート
// ============================================================================

// カスタムコンセプト定義: T が整数型 または 浮動小数点型 のとき true になる
// `concept` キーワードでコンパイル時ブール制約を定義する
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;
// 【注意】std::integral<bool> は true なので、bool 型も Numeric を満たす。
// bool を除外したい場合: concept Numeric = (std::integral<T> && !std::same_as<T, bool>) || std::floating_point<T>;

// Numeric コンセプト制約を持つテンプレート関数。
// `template <Numeric T>` は `template <typename T> requires Numeric<T>` の糖衣構文。
// これにより add("A", "B") のような呼び出しはコンパイルエラーになり、
// エラーメッセージは「T is not Numeric」という明確なものになる。
// Rust の fn add<T: Numeric>(a: T, b: T) -> T に相当。
template <Numeric T>
T add(T a, T b) {
    return a + b;
}

// ============================================================================
// 2. std::optional による null 安全な検索
// ============================================================================
struct User {
    int id;
    std::string name;
};

// 戻り値型 std::optional<User>: ユーザーが見つかれば User を、見つからなければ nullopt を返す。
// 従来の「見つからなければ nullptr を返す」パターンより型安全。
// nullptr を参照した場合はクラッシュするが、optional は has_value() で安全に確認できる。
static std::optional<User> find_user_by_id(int id) {
    // ここでは id == 1 のユーザーのみが存在すると仮定したデータベース模擬
    if (id == 1) {
        // 値ありの optional を返す。User{} は aggregate initialization。
        return User{1, "Haru"};
    }
    // 値なし (Rustの None, Javaの Optional.empty(), Swift の nil)
    return std::nullopt;
}

// ============================================================================
// 3. std::variant による代数的データ型 (Rustの enum 相当)
// ============================================================================

// 各支払い手段を表す構造体。それぞれ固有のデータを持つ。
struct CreditCard   { std::string card_number; };
struct BankTransfer { std::string account_number; };
struct Crypto       { std::string wallet_address; };

// PaymentMethod は CreditCard, BankTransfer, Crypto のいずれかを保持できる型安全 Union。
// Rust の enum PaymentMethod { CreditCard(String), BankTransfer(String), Crypto(String) } に相当。
// sizeof(PaymentMethod) は最大メンバのサイズ + 型タグのオーバーヘッド程度。
using PaymentMethod = std::variant<CreditCard, BankTransfer, Crypto>;

// ============================================================================
// Overloaded ヘルパー: std::visit のためのパターンマッチング糖衣構文
// ============================================================================
// 複数のラムダを継承して、それぞれの operator() を使えるようにする汎用トリック。
// C++17 のテンプレート引数推論 (CTAD) により Overloaded{...} と書くだけで推論される。
//
// 使用例:
//   std::visit(Overloaded{
//       [](const CreditCard& cc) { ... },
//       [](const BankTransfer& bt) { ... },
//       [](const Crypto& cr) { ... }
//   }, payment_method_variant);
//
// Rust で言えば: match payment { CreditCard(cc) => { ... }, BankTransfer(bt) => { ... }, ... }
template <class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
// C++17 の推論ガイド (deduction guide): テンプレート引数を自動推論させる
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

// ============================================================================
// デモ実行関数
// ============================================================================
void run_templates_and_modern_types_demo() {
    // ====================================================================
    // 1. Concepts による型安全なジェネリクス
    // ====================================================================
    std::cout << "--- 1. Type-Safe Generics via C++20 Concepts ---\n";
    // add<int>(10, 20): T = int として型推論される
    std::cout << "  add(10, 20):       " << add(10, 20)     << " (int)\n";
    // add<double>(3.14, 2.71): T = double として型推論される
    std::cout << "  add(3.14, 2.71):   " << add(3.14, 2.71) << " (double)\n";
    // 以下はコンパイルエラー: std::string は Numeric コンセプトを満たさない
    // add("A", "B"); // error: 'std::string' does not satisfy 'Numeric'

    // ====================================================================
    // 2. std::optional<T> による null 安全なアクセス
    // ====================================================================
    std::cout << "\n--- 2. std::optional<T> (Rust Option<T>) ---\n";
    auto user1 = find_user_by_id(1);
    // if (user1) は if (user1.has_value()) と等価
    if (user1) {
        // operator-> でオプショナル内の値に直接アクセス (Rust の if let Some(u) = ... に相当)
        std::cout << "  User 1 Found: " << user1->name << std::endl;
    }

    auto user99 = find_user_by_id(99);
    // 三項演算子で値の有無を確認。value_or() を使うとさらに簡潔に書ける。
    // user99.value_or(User{0, "Guest"}).name でもよい
    std::cout << "  User 99 (value_or): "
              << (user99 ? user99->name : "Guest (Not Found)") << std::endl;

    // ====================================================================
    // 3. std::variant & std::visit によるパターンマッチング
    // ====================================================================
    std::cout << "\n--- 3. std::variant & std::visit (Rust enum & match) ---\n";

    // variant に CreditCard 型の値をセット
    PaymentMethod payment1 = CreditCard{"1234-5678-9012-3456"};
    // variant に Crypto 型の値をセット (BankTransfer はどこにも格納されていない)
    PaymentMethod payment2 = Crypto{"0xabcdef1234567890"};

    // std::visit でパターンマッチングを実行するラムダを定義
    // const PaymentMethod& を受け取り、格納されている型に応じて処理を分岐する。
    // Overloaded に渡した全パターン (CreditCard, BankTransfer, Crypto) を網羅していないと
    // コンパイルエラーになるため、パターン漏れをコンパイル時に防止できる。
    auto print_payment = [](const PaymentMethod& pm) {
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
        }, pm); // pm を variant として渡し、内部の型に応じてオーバーロードが選択される
    };

    print_payment(payment1); // CreditCard のラムダが呼ばれる
    print_payment(payment2); // Crypto のラムダが呼ばれる
}

} // namespace modern_cpp
