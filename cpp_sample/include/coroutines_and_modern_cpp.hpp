#ifndef COROUTINES_AND_MODERN_CPP_HPP
#define COROUTINES_AND_MODERN_CPP_HPP

#include <coroutine>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

/**
 * ============================================================================
 * モジュール 07: コルーチン・コンパイル時計算・C++20/23 (Coroutines & Modern C++)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Python, Go）向け要点】
 * 1. C++20 コルーチン (Coroutines: co_yield, co_await, co_return):
 *    - 関数が実行途中で一時停止 (Suspend) し、後から再開 (Resume) できる仕組み。
 *    - スタックレスコルーチンであり、promise_type と coroutine_handle で制御。
 * 
 * 2. consteval (C++20 即時関数):
 *    - constexpr よりも厳格で、「必ずコンパイル時に評価されなければならない」関数。
 *    - コンパイル時ハッシュ計算や文字列検証に最適。
 * 
 * 3. std::span<T> (C++20 ゼロコピー安全参照):
 *    - Rust の &[T]、Go の slice、C# の Span<T> に相当。
 *    - ポインタと要素数のペアであり、vector や 生配列を安全にスライシング。
 */

namespace modern_cpp {

// 1. C++20 コルーチンによる遅延ジェネレータ
template <typename T>
class Generator {
public:
    struct promise_type {
        T current_value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) noexcept {
            current_value = value;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    explicit Generator(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~Generator() {
        if (handle_) handle_.destroy();
    }

    // コピー禁止・ムーブのみ許可
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    // 次の値を取得 (コルーチンの再開)
    bool next() {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }

    T value() const {
        return handle_.promise().current_value;
    }

private:
    std::coroutine_handle<promise_type> handle_ = nullptr;
};

// コルーチン関数の宣言
Generator<uint64_t> generate_fibonacci();

// 2. consteval: 完全コンパイル時強制の FNV-1a ハッシュ計算
consteval uint64_t fnv1a_hash_consteval(std::string_view str) {
    uint64_t hash = 14695981039346656037ULL;
    for (char c : str) {
        hash ^= static_cast<uint8_t>(c);
        hash *= 1099511628211ULL;
    }
    return hash;
}

// 3. std::span を受け取る安全な集計関数
int64_t sum_elements(std::span<const int> numbers);

// モジュール実行エントリポイント
void run_coroutines_and_modern_cpp_demo();

} // namespace modern_cpp

#endif // COROUTINES_AND_MODERN_CPP_HPP
