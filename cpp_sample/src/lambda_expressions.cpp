#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <functional>
#include <concepts>
#include <ranges>
#include "lambda_expressions.hpp"

namespace modern_cpp {

/**
 * ============================================================================
 * Modern C++ ラムダ式 (C++11 / C++14 / C++17 / C++20) 実践コード
 * ============================================================================
 */

// ---------------------------------------------------------------------------
// 1. 基本構文とキャプチャなしラムダの関数ポインタ変換
// ---------------------------------------------------------------------------
static void demo_basic_and_function_pointer() {
    std::cout << "=== 1. Basic Lambdas & Function Pointer Conversion ===\n";

    // 基本構文
    auto add = [](int a, int b) -> int { return a + b; };
    std::cout << "Add(15, 25): " << add(15, 25) << "\n";

    // キャプチャなしラムダは通常の C 関数ポインタに暗黙変換可能！
    int (*c_func_ptr)(int, int) = [](int a, int b) { return a * b; };
    std::cout << "Function pointer call Multiply(6, 7): " << c_func_ptr(6, 7) << "\n";
}

// ---------------------------------------------------------------------------
// 2. キャプチャモードと mutable
// ---------------------------------------------------------------------------
static void demo_captures_and_mutable() {
    std::cout << "\n=== 2. Captures by Value, Reference, and 'mutable' ===\n";

    int base_val = 100;

    // 1. [=]: 値コピーキャプチャ (元の変数は影響を受けない)
    auto add_base_copy = [base_val](int x) { return x + base_val; };
    std::cout << "Value capture Add(50): " << add_base_copy(50) << "\n";

    // 2. mutable: コピーキャプチャした内部状態の変更を許可
    int counter = 0;
    auto increment_internal = [counter]() mutable {
        counter++;
        std::cout << "  [Inside mutable lambda] counter = " << counter << "\n";
    };
    increment_internal();
    increment_internal();
    std::cout << "  [Outside lambda] counter = " << counter << " (unchanged!)\n";

    // 3. [&]: 参照キャプチャ (元の変数が直接変更される)
    int actual_sum = 0;
    auto accumulate = [&actual_sum](int val) { actual_sum += val; };
    accumulate(10);
    accumulate(20);
    accumulate(30);
    std::cout << "Reference capture actual_sum: " << actual_sum << " (modified!)\n";
}

// ---------------------------------------------------------------------------
// 3. C++14 初期化キャプチャ (Move Only な型の所有権移動)
// ---------------------------------------------------------------------------
static void demo_init_capture_move() {
    std::cout << "\n=== 3. C++14 Init-Capture (Move Semantics in Lambdas) ===\n";

    auto uptr = std::make_unique<std::string>("Heavy Resource Payload");

    // std::unique_ptr はコピーできないため、std::move でラムダ内に所有権を移動
    auto worker = [payload = std::move(uptr)]() {
        std::cout << "  Lambda owns payload: '" << *payload << "'\n";
    };

    std::cout << "Before calling worker, uptr is: "
              << (uptr ? "valid" : "nullptr (moved into lambda)") << "\n";

    worker();
}

// ---------------------------------------------------------------------------
// 4. C++14 ジェネリックラムダ & C++20 テンプレートラムダ
// ---------------------------------------------------------------------------
static void demo_generic_and_template_lambdas() {
    std::cout << "\n=== 4. Generic (C++14) & Template (C++20) Lambdas ===\n";

    // C++14: auto 引数による多相ラムダ
    auto print_any = [](const auto& item) {
        std::cout << "  Generic print: " << item << "\n";
    };
    print_any(42);
    print_any("Modern C++ String");
    print_any(3.14159);

    // C++20: 明示的テンプレートラムダ (型 T の一致をコンパイル時に強制)
    auto safe_add = []<typename T>(T a, T b) -> T {
        return a + b;
    };
    std::cout << "Template lambda int add:    " << safe_add(10, 20) << "\n";
    std::cout << "Template lambda string add: "
              << safe_add(std::string("Hello "), std::string("C++20")) << "\n";
}

// ---------------------------------------------------------------------------
// 5. IIFE (即時実行ラムダ) による const 初期化
// ---------------------------------------------------------------------------
static void demo_iife_const_initialization() {
    std::cout << "\n=== 5. IIFE (Immediately Invoked Function Expression) ===\n";

    bool is_production = false;
    int env_mode = 2;

    // 複雑な分岐を経て決定される設定値を、安全に const として一撃初期化
    const std::string endpoint = [&]() -> std::string {
        if (is_production) {
            return "https://api.production.internal:443";
        }
        switch (env_mode) {
            case 1:  return "http://localhost:8080";
            case 2:  return "http://staging.testnet:8081";
            default: return "http://127.0.0.1:3000";
        }
    }(); // ← 末尾の () で即座に実行

    std::cout << "Const endpoint initialized via IIFE: " << endpoint << "\n";
}

// ---------------------------------------------------------------------------
// 6. ラムダ式の受け渡し：Concepts vs std::function
// ---------------------------------------------------------------------------
// A. ゼロオーバーヘッド版: C++20 Concepts による完全インライン化
template <std::invocable<int> Func>
static void invoke_fast(Func&& f, int arg) {
    f(arg);
}

// B. 型消去版: std::function (柔軟だがヒープ確保や間接呼び出しコストの可能性)
static void invoke_polymorphic(const std::function<void(int)>& f, int arg) {
    f(arg);
}

static void demo_passing_lambdas() {
    std::cout << "\n=== 6. Passing Lambdas: Zero-overhead Concepts vs std::function ===\n";

    int multiplier = 5;
    auto scale = [multiplier](int x) {
        std::cout << "  Computed " << x << " * " << multiplier << " = " << (x * multiplier) << "\n";
    };

    // ゼロオーバーヘッド（推奨）
    invoke_fast(scale, 10);

    // std::function
    invoke_polymorphic(scale, 20);
}

// ---------------------------------------------------------------------------
// モジュールエントリーポイント
// ---------------------------------------------------------------------------
void run_lambda_expressions_demo() {
    demo_basic_and_function_pointer();
    demo_captures_and_mutable();
    demo_init_capture_move();
    demo_generic_and_template_lambdas();
    demo_iife_const_initialization();
    demo_passing_lambdas();
}

} // namespace modern_cpp
