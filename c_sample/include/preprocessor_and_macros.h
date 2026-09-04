#ifndef PREPROCESSOR_AND_MACROS_H
#define PREPROCESSOR_AND_MACROS_H

#include <stddef.h>

/**
 * ============================================================================
 * モジュール 07: プリプロセッサ極意・マクロ・X-Macros (Preprocessor & Macros)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. do { ... } while(0) イディオム:
 *    - 複数文のマクロを if/else の中で安全に使うための C言語特有の必須パターン。
 * 
 * 2. 文字列化演算子 (#) & トークン連結演算子 (##):
 *    - 引数を文字列リテラルにする `#x`。
 *    - 識別子を結合して新たなシンボルを作る `a ## b`。
 * 
 * 3. X-Macros パターン:
 *    - C言語で最もエレガントな「メタプログラミング手法」。
 *    - データの定義リストを 1 つ作成し、Enumの列挙、文字列配列、
 *      ディスパッチ処理などを DRY (Don't Repeat Yourself) に自動生成する。
 * 
 * 4. C11 _Static_assert:
 *    - コンパイル時に条件（構造体サイズや型サイズ）を検証する。
 */

// 1. 安全な複数文マクロ (do-while(0) イディオム)
#define SAFE_SWAP(a, b, type) do { \
    type _temp = (a); \
    (a) = (b); \
    (b) = _temp; \
} while(0)

// 2. 文字列化 (#) とトークン連結 (##)
#define LOG_VAR(var) printf("  [LOG] " #var " = %d\n", var)
#define MAKE_FUNC(name) void run_func_##name(void) { printf("  Called dynamic func: %s\n", #name); }

// 3. X-Macros: エラーコード定義リスト
// FORMAT: X(ENUM_NAME, ERROR_STRING)
#define HTTP_STATUS_TABLE(X) \
    X(HTTP_OK,                    200, "OK") \
    X(HTTP_BAD_REQUEST,           400, "Bad Request") \
    X(HTTP_UNAUTHORIZED,          401, "Unauthorized") \
    X(HTTP_NOT_FOUND,             404, "Not Found") \
    X(HTTP_INTERNAL_SERVER_ERROR, 500, "Internal Server Error")

// X-Macros による Enum の自動生成
#define AS_ENUM(enum_name, code, msg) enum_name = code,
typedef enum {
    HTTP_STATUS_TABLE(AS_ENUM)
} HttpStatus;
#undef AS_ENUM

// HttpStatus を文字列に変換する関数
const char* http_status_to_string(HttpStatus status);

// モジュール実行エントリポイント
void run_preprocessor_and_macros_demo(void);

#endif // PREPROCESSOR_AND_MACROS_H
