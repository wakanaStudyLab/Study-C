#include "coroutines_and_modern_cpp.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <format>
#include <numeric>

namespace modern_cpp {

// C++20 コルーチン実装 (co_yield による無限フィボナッチ数列)
Generator<uint64_t> generate_fibonacci() {
    uint64_t a = 0;
    uint64_t b = 1;
    while (true) {
        co_yield a;
        uint64_t next = a + b;
        a = b;
        b = next;
    }
}

// std::span<const int> の実装
int64_t sum_elements(std::span<const int> numbers) {
    // 境界外アクセスの心配がなく、std::vector でも C 配列でも std::array でも同一シグネチャで受け取れる
    return std::accumulate(numbers.begin(), numbers.end(), int64_t{0});
}

void run_coroutines_and_modern_cpp_demo() {
    std::cout << "================================================================\n";
    std::cout << "  Module 07: Coroutines, Consteval, Span, and std::format\n";
    std::cout << "================================================================\n\n";

    // 1. C++20 コルーチン (co_yield による遅延ストリーム)
    std::cout << "--- 1. C++20 Coroutines (co_yield Lazy Generator) ---\n";
    auto fib = generate_fibonacci();
    std::cout << "First 10 Fibonacci numbers via Coroutine: [ ";
    for (int i = 0; i < 10; ++i) {
        if (fib.next()) {
            std::cout << fib.value() << (i == 9 ? "" : ", ");
        }
    }
    std::cout << " ]\n";

    // 2. C++20 consteval (完全コンパイル時計算)
    std::cout << "\n--- 2. C++20 consteval (Guaranteed Compile-Time Execution) ---\n";
    // コンパイル時に値が計算され、アセンブリ内には即値定数として直接埋め込まれる！
    constexpr uint64_t hash_api = fnv1a_hash_consteval("api/v1/users");
    constexpr uint64_t hash_auth = fnv1a_hash_consteval("api/v1/auth");

    std::cout << std::format("  Hash for 'api/v1/users': 0x{:016X}\n", hash_api);
    std::cout << std::format("  Hash for 'api/v1/auth':  0x{:016X}\n", hash_auth);

    // 3. C++20 std::span (ゼロコピー安全メモリ参照)
    std::cout << "\n--- 3. C++20 std::span (Zero-Copy Non-Owning Slice View) ---\n";
    std::vector<int> vec = {10, 20, 30, 40, 50};
    int raw_arr[] = {1, 2, 3, 4, 5};
    std::array<int, 3> std_arr = {100, 200, 300};

    // すべてコピーなしで同一関数に渡せる
    std::cout << std::format("  Sum of std::vector (len={}): {}\n", vec.size(), sum_elements(vec));
    std::cout << std::format("  Sum of C raw array (len={}): {}\n", sizeof(raw_arr) / sizeof(int), sum_elements(raw_arr));
    std::cout << std::format("  Sum of std::array  (len={}): {}\n", std_arr.size(), sum_elements(std_arr));

    // サブスライスの安全な切り出し (vec[1..4])
    std::span<const int> subspan = std::span{vec}.subspan(1, 3);
    std::cout << std::format("  Sum of Subspan vec[1..4] (len={}): {}\n", subspan.size(), sum_elements(subspan));

    // 4. std::format によるモダンフォーマット
    std::cout << "\n--- 4. C++20 std::format (Type-Safe Fast Formatting) ---\n";
    std::string formatted = std::format("User ID: {:06d}, Performance Score: {:.2f}%, Status: {}\n",
                                        42, 99.8765, "ACTIVE");
    std::cout << "  " << formatted;
}

} // namespace modern_cpp
