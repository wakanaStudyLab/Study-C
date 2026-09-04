#ifndef ERROR_HANDLING_AND_EXPECTED_HPP
#define ERROR_HANDLING_AND_EXPECTED_HPP

#include <expected>
#include <string>
#include <string_view>

/**
 * ============================================================================
 * モジュール 08: エラーハンドリング・例外安全・C++23 std::expected
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. C++23 std::expected<T, E>:
 *    - Rust の `Result<T, E>`、Go の `(T, error)` に相当する待望の標準機能。
 *    - 例外をスローせず、値かエラーかを型安全に保持（スタック巻き戻しのオーバーヘッドなし）。
 *    - 成功値は `*res` や `res.value()`、エラーは `res.error()` で取得。
 * 
 * 2. モナディック操作 (and_then / transform / or_else):
 *    - Rust の `and_then()` / `map()` と同様、エラーチェックを連鎖的に行える。
 * 
 * 3. 従来の例外 (try / catch) と noexcept:
 *    - ドメインエラー（パース失敗等）は `std::expected` が推奨される。
 *    - 致命的・想定外のエラー（メモリ枯渇等）には例外が使われる。
 *    - `noexcept` 修飾されたムーブコンストラクタは `std::vector` 再確保時に高速パスが選ばれる。
 */

namespace modern_cpp {

// カスタムエラー型の定義
enum class ParseErrorCode {
    EmptyInput,
    InvalidCharacter,
    Overflow
};

// エラーコードを文字列表現に変換するヘルパー関数
std::string_view to_string(ParseErrorCode code);

// 1. C++23 std::expected による整数パース関数 (Rust の Result<int, ParseErrorCode> 相当)
std::expected<int, ParseErrorCode> parse_integer(std::string_view input);

// 2. モナディック操作のデモ関数
std::expected<double, ParseErrorCode> safe_square_root(int value);

// モジュール実行エントリポイント
void run_error_handling_demo();

} // namespace modern_cpp

#endif // ERROR_HANDLING_AND_EXPECTED_HPP
