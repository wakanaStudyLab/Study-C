#include <iostream>
#include "raii_and_smart_pointers.hpp"
#include "move_semantics_and_classes.hpp"
#include "stl_and_ranges.hpp"
#include "templates_and_modern_types.hpp"
#include "concurrency.hpp"
#include "lambda_expressions.hpp"
#include "coroutines_and_modern_cpp.hpp"
#include "error_handling_and_expected.hpp"
#include "advanced_templates_and_metaprogramming.hpp"
#include "operator_overloading_and_spaceship.hpp"
#include "memory_and_low_level.hpp"
#include "filesystem_and_io.hpp"
#include "advanced_concurrency.hpp"

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

    // --- モジュール 07: コルーチン・コンパイル時計算・C++20/23 ---
    // ・co_yield による遅延ジェネレータ
    // ・consteval による完全コンパイル時ハッシュ計算
    // ・std::span によるゼロコピー安全スライス
    // ・std::format による現代的フォーマット
    print_section("07: Coroutines (co_yield), Consteval, Span, and std::format");
    modern_cpp::run_coroutines_and_modern_cpp_demo();

    // --- モジュール 08: エラーハンドリング・例外・C++23 std::expected ---
    // ・std::expected<T, E> による Result 型エラー表現 (Rust 相当)
    // ・モナディック操作 (and_then, transform, or_else)
    // ・noexcept 保証とムーブ最適化
    print_section("08: Error Handling, Exception Safety, and C++23 std::expected");
    modern_cpp::run_error_handling_demo();

    // --- モジュール 09: 高度なテンプレート・メタプログラミング ---
    // ・可変引数テンプレート & C++17 Fold 式
    // ・if constexpr と Type Traits
    // ・CRTP によるゼロオーバーヘッド静的ポリモーフィズム
    print_section("09: Advanced Templates, Fold Expressions, and CRTP");
    modern_cpp::run_advanced_templates_demo();

    // --- モジュール 10: 演算子オーバーロードと宇宙船演算子 (<=>) ---
    // ・基本演算子のオーバーロード
    // ・C++20 三方比較演算子 (<=>) による比較演算子の自動一括導出
    // ・自作コンテナとカスタムイテレータ (Range-based for)
    print_section("10: Operator Overloading and C++20 Spaceship Operator (<=>)");
    modern_cpp::run_operator_overloading_demo();

    // --- モジュール 11: 高度メモリ管理・PMR・低レイヤ操作 ---
    // ・std::weak_ptr による循環参照解決
    // ・std::pmr によるスタックバンプアロケータ (ヒープ確保なし)
    // ・std::bit_cast による安全なビット再解釈
    // ・alignas / alignof によるハードウェアアライメント
    print_section("11: Memory Management, std::pmr, bit_cast, and Low-Level");
    modern_cpp::run_memory_and_low_level_demo();

    // --- モジュール 12: ファイルシステム・最新I/O ---
    // ・C++23 std::print / std::println
    // ・std::filesystem によるパス操作・ディレクトリ走査
    // ・バイナリファイルの読み書き
    print_section("12: Filesystem Operations, Binary I/O, and C++23 std::print");
    modern_cpp::run_filesystem_and_io_demo();

    // --- モジュール 13: 高度な並行処理・同期プリミティブ ---
    // ・std::scoped_lock によるデッドロックフリーな複数ロック
    // ・std::condition_variable による Producer-Consumer キュー
    // ・std::counting_semaphore による同時アクセス制限
    // ・std::latch によるカウントダウン同期
    print_section("13: Advanced Concurrency, Mutex, Semaphore, and Latch");
    modern_cpp::run_advanced_concurrency_demo();

    // 全モジュール完了のバナーを表示
    print_banner("ALL C++ TUTORIAL MODULES COMPLETED SUCCESSFULLY!");

    // main() から 0 を返すことで OS にプログラムが正常終了したことを通知する。
    // C++ では main() のみ return 0 を省略できるが、明示しておくと意図が明確になる。
    return 0;
}
