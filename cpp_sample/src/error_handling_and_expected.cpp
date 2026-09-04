#include "error_handling_and_expected.hpp"
#include <iostream>
#include <format>
#include <cmath>
#include <stdexcept>
#include <vector>

/**
 * ============================================================================
 * モジュール 08: エラーハンドリング・例外安全・C++23 std::expected
 * ============================================================================
 */

namespace modern_cpp {

std::string_view to_string(ParseErrorCode code) {
    switch (code) {
        case ParseErrorCode::EmptyInput:
            return "Input string is empty";
        case ParseErrorCode::InvalidCharacter:
            return "Contains non-digit characters";
        case ParseErrorCode::Overflow:
            return "Value exceeds integer range";
    }
    return "Unknown error";
}

// 1. C++23 std::expected: 失敗する可能性のある処理を型安全に表現
std::expected<int, ParseErrorCode> parse_integer(std::string_view input) {
    if (input.empty()) {
        return std::unexpected(ParseErrorCode::EmptyInput);
    }

    int64_t result = 0;
    bool negative = false;
    size_t i = 0;

    if (input[0] == '-') {
        negative = true;
        i = 1;
        if (input.size() == 1) {
            return std::unexpected(ParseErrorCode::InvalidCharacter);
        }
    } else if (input[0] == '+') {
        i = 1;
        if (input.size() == 1) {
            return std::unexpected(ParseErrorCode::InvalidCharacter);
        }
    }

    for (; i < input.size(); ++i) {
        char c = input[i];
        if (c < '0' || c > '9') {
            return std::unexpected(ParseErrorCode::InvalidCharacter);
        }
        result = result * 10 + (c - '0');
        if (result > INT32_MAX || result < INT32_MIN) {
            return std::unexpected(ParseErrorCode::Overflow);
        }
    }

    if (negative) {
        result = -result;
    }

    return static_cast<int>(result);
}

// 2. モナディック操作連携用: 正の数の平方根を計算
std::expected<double, ParseErrorCode> safe_square_root(int value) {
    if (value < 0) {
        return std::unexpected(ParseErrorCode::InvalidCharacter); // 便宜上のエラーコード
    }
    return std::sqrt(static_cast<double>(value));
}

// noexcept の重要性を説明するサンプルクラス
class FastMover {
public:
    int id;
    explicit FastMover(int i) : id(i) {}

    // noexcept を明示することで、std::vector の再確保時にコピーではなくムーブが使われる
    FastMover(FastMover&& other) noexcept : id(other.id) {
        other.id = -1;
    }
    FastMover& operator=(FastMover&& other) noexcept {
        if (this != &other) {
            id = other.id;
            other.id = -1;
        }
        return *this;
    }

    FastMover(const FastMover& other) : id(other.id) {
        std::cout << "    [FastMover] Copy occurred (Slow path!)\n";
    }
    FastMover& operator=(const FastMover& other) {
        id = other.id;
        return *this;
    }
};

void run_error_handling_demo() {
    std::cout << "--- 1. C++23 std::expected<T, E> (Rust Result<T, E> / Go (T, error)) ---\n";
    
    // 成功ケース
    auto ok_res = parse_integer("1024");
    if (ok_res) {
        std::cout << std::format("  parse_integer(\"1024\"): SUCCESS -> {}\n", *ok_res);
    }

    // 失敗ケース (無効文字)
    auto err_res1 = parse_integer("42abc");
    if (!err_res1) {
        std::cout << std::format("  parse_integer(\"42abc\"): FAILED -> {} ({})\n", 
                                 static_cast<int>(err_res1.error()), 
                                 to_string(err_res1.error()));
    }

    // value_or: 失敗時のフォールバック値
    int fallback_val = parse_integer("invalid").value_or(-1);
    std::cout << std::format("  parse_integer(\"invalid\").value_or(-1) -> {}\n", fallback_val);

    std::cout << "\n--- 2. Monadic Operations (.and_then(), .transform(), .or_else()) ---\n";
    // パース -> 平方根計算 -> 文字列フォーマットをパイプラインのように連鎖
    auto chained_pipeline = [](std::string_view input) {
        return parse_integer(input)
            .and_then([](int n) -> std::expected<double, ParseErrorCode> {
                return safe_square_root(n);
            })
            .transform([](double root) {
                return std::format("Sqrt computed: {:.2f}", root);
            });
    };

    auto pipeline_ok = chained_pipeline("144");
    if (pipeline_ok) {
        std::cout << std::format("  Pipeline Success for '144': {}\n", *pipeline_ok);
    }

    auto pipeline_err = chained_pipeline("xyz");
    if (!pipeline_err) {
        std::cout << std::format("  Pipeline Error for 'xyz': {}\n", to_string(pipeline_err.error()));
    }

    std::cout << "\n--- 3. Traditional Exceptions (try-catch) and noexcept Guarantee ---\n";
    try {
        std::cout << "  Attempting operation that throws std::runtime_error...\n";
        throw std::runtime_error("Simulated catastrophic network failure");
    } catch (const std::exception& ex) {
        std::cout << std::format("  Caught std::exception: {}\n", ex.what());
    }

    // noexcept の型トレイト検査
    std::cout << std::format("  Is FastMover nothrow move constructible? {}\n",
                             std::is_nothrow_move_constructible_v<FastMover> ? "YES (Fast vector reallocation)" : "NO");
}

} // namespace modern_cpp
