#include <iostream>
#include "raii_and_smart_pointers.hpp"
#include "move_semantics_and_classes.hpp"
#include "stl_and_ranges.hpp"
#include "templates_and_modern_types.hpp"
#include "concurrency.hpp"
#include "lambda_expressions.hpp"

/**
 * ============================================================================
 * Modern C++ 最速習得コース (For Rust / C# / Go Developers) - Main Entry
 * ============================================================================
 *
 * 【このプロジェクトの目的】
 * 他言語（Rust / C# / Go）を使い慣れた開発者が、
 * モダン C++ (C++17/C++20) の重要概念を効率よく把握するためのクラッシュコース。
 *
 * 【各モジュールの対応関係】
 * ┌────────────────────────────────────────────────────────────────┐
 * │ ファイル名                         │ 学習トピック               │
 * ├────────────────────────────────────────────────────────────────┤
 * │ raii_and_smart_pointers.cpp        │ RAII / unique_ptr / shared_ptr │
 * │ move_semantics_and_classes.cpp     │ ムーブセマンティクス / 仮想関数  │
 * │ stl_and_ranges.cpp                 │ STL / C++20 Ranges パイプライン  │
 * │ templates_and_modern_types.cpp     │ Concepts / optional / variant    │
 * │ concurrency.cpp                    │ jthread / async / atomic         │
 * └────────────────────────────────────────────────────────────────┘
 *
 * 【ビルド要件】
 * - C++20 以上 (-std=c++20)
 * - GCC 11+ / Clang 13+ / MSVC 19.29+
 */

// セクション区切りのバナーを標準出力に表示するユーティリティ関数。
// const std::string& で受け取ることで、文字列のコピーを回避する（参照渡し）。
static void print_banner(const std::string& title) {
    std::cout << "\n================================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "================================================================\n\n";
}

// 各デモのセクション見出しを出力するユーティリティ関数。
// static 修飾子により、このファイル内にのみリンクを限定する（内部リンケージ）。
static void print_section(const std::string& title) {
    std::cout << "\n################################################################\n";
    std::cout << "# " << title << "\n";
    std::cout << "################################################################\n\n";
}

int main() {
    // プログラム開始のバナーを表示
    print_banner("MODERN C++ (C++20/C++23) CRASH COURSE");

    // --- モジュール 01: RAII とスマートポインタ ---
    // ・デストラクタによる自動リソース解放 (RAII パターン)
    // ・unique_ptr / shared_ptr の使い方と所有権モデル
    print_section("01: RAII and Smart Pointers (unique_ptr / shared_ptr)");
    modern_cpp::run_raii_and_smart_pointers_demo();

    // --- モジュール 02: ムーブセマンティクスとクラス設計 ---
    // ・std::move によるO(1) のリソース移動
    // ・コピーコンストラクタ vs ムーブコンストラクタの違い
    // ・virtual デストラクタと override による多態的設計
    print_section("02: Move Semantics (std::move) and Modern Class Design");
    modern_cpp::run_move_semantics_demo();

    // --- モジュール 03: STL コンテナと C++20 Ranges ---
    // ・vector / unordered_map などの主要コンテナ
    // ・string_view による非所有のゼロコピー文字列参照
    // ・| パイプ演算子による Ranges パイプライン (C# LINQ 相当)
    print_section("03: STL Containers & C++20 Ranges Pipeline");
    modern_cpp::run_stl_and_ranges_demo();

    // --- モジュール 04: テンプレート・代数的データ型 ---
    // ・C++20 Concepts による型制約付きテンプレート (Rust の trait bounds 相当)
    // ・std::optional<T> によるヌル安全な値の表現 (Rust の Option<T> 相当)
    // ・std::variant<Ts...> + std::visit によるパターンマッチング (Rust の enum + match 相当)
    print_section("04: C++20 Concepts, std::optional, std::variant (Algebraic Data Types)");
    modern_cpp::run_templates_and_modern_types_demo();

    // --- モジュール 05: 並行処理と非同期 ---
    // ・std::jthread による RAII 安全スレッド (C++20)
    // ・std::async / std::future による非同期タスク実行
    // ・std::atomic<T> によるロックフリーカウンター
    print_section("05: Concurrency, Async Future, and C++20 jthread");
    modern_cpp::run_concurrency_demo();

    // --- モジュール 06: ラムダ式と関数型プログラミング ---
    // ・キャプチャ（値・参照・mutable・ムーブ）
    // ・C++14 auto / C++20 テンプレートラムダ
    // ・IIFE による const 初期化イディオム
    // ・Concepts によるゼロオーバーヘッド関数呼び出し
    print_section("06: Lambda Expressions & Functional Modern C++");
    modern_cpp::run_lambda_expressions_demo();

    // 全モジュール完了のバナーを表示
    print_banner("ALL C++ TUTORIAL MODULES COMPLETED SUCCESSFULLY!");

    // main() から 0 を返すことで OS にプログラムが正常終了したことを通知する。
    // C++ では main() のみ return 0 を省略できるが、明示しておくと意図が明確になる。
    return 0;
}
