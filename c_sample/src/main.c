#include <stdio.h>
#include "pointers_and_memory.h"
#include "structs_and_functions.h"
#include "strings_and_io.h"
#include "lambda_and_callbacks.h"
#include "modern_c11_features.h"
#include "bitwise_and_binary.h"
#include "preprocessor_and_macros.h"
#include "error_handling_and_jump.h"
#include "dynamic_data_structures.h"

/**
 * ============================================================================
 * C言語 最速習得コース (For Rust / C# / Go Developers) - Main Entry
 * ============================================================================
 *
 * 【このプロジェクトの目的】
 * 他言語（Rust / C# / Go）を使い慣れた開発者が、C言語の根本を理解するためのコース。
 * C は Rust・Go・C++ すべての「祖先」であり、ポインタ・手動メモリ管理・構造体など
 * 低レイヤの仕組みを理解することで、他言語の設計思想がより深く分かるようになる。
 *
 * 【各モジュールの対応関係】
 * ┌────────────────────────────┬──────────────────────────────────────────┐
 * │ ファイル名                 │ 学習トピック                             │
 * ├────────────────────────────┼──────────────────────────────────────────┤
 * │ pointers_and_memory.c      │ ポインタ / ポインタ演算 / malloc・free   │
 * │ structs_and_functions.c    │ 構造体 / パディング / 関数ポインタ       │
 * │ strings_and_io.c           │ ヌル終端文字列 / snprintf / ファイルI/O  │
 * └────────────────────────────┴──────────────────────────────────────────┘
 *
 * 【ビルド要件】
 * - C11 以上 (-std=c11)
 * - GCC 5+ / Clang 3.4+ / MSVC 19+
 */

/* セクション区切りのバナーを標準出力に表示するユーティリティ関数。
 * C言語では const char* で文字列を受け渡す（std::string は存在しない）。
 * static 修飾子によりこのファイル内にのみリンクを限定する（内部リンケージ）。 */
static void print_banner(const char* title) {
    printf("\n================================================================\n");
    printf("  %s\n", title);
    printf("================================================================\n\n");
}

/* 各デモのセクション見出しを出力するユーティリティ関数。
 * C言語のブロックコメントは ... で囲む形式を使う（// 形式はC99以降で可）。
 * ブロックコメントのネスト（コメント内にコメント）はC言語では禁止されている点に注意。 */
static void print_section(const char* title) {
    printf("\n################################################################\n");
    printf("# %s\n", title);
    printf("################################################################\n\n");
}

/* C言語の main は必ず int を返す。
 * `void main()` は C標準では未定義動作（MSVC で動いても移植性がない）。
 * `(void)` 引数宣言: argc/argv を使わない場合に「引数なし」を明示する慣例。 */
int main(void) {
    print_banner("C LANGUAGE CRASH COURSE (For Rust / C# / Go Developers)");

    /* --- モジュール 01: ポインタとメモリ管理 ---
     * ・ポインタの基礎とアドレス演算子 (&) / 間接参照演算子 (*)
     * ・ポインタ演算 (arr[i] == *(arr + i))
     * ・malloc / free による手動ヒープ管理
     * ・多重ポインタ (int**) による出力パラメータパターン */
    /* セクションタイトルを出力 (Terminal output in English to prevent encoding issues) */
    print_section("01: Pointers, Pointer Arithmetic, Manual Memory Management");
    run_pointers_and_memory_demo();

    /* --- モジュール 02: 構造体と関数ポインタ ---
     * ・struct とメモリレイアウト（パディングによる sizeof の変化）
     * ・-> 演算子（ポインタ経由のメンバアクセス）
     * ・関数ポインタ typedef による高階関数（コールバック）
     * ・関数ポインタを struct に持たせる VTable 模擬（OOP エミュレーション） */
    print_section("02: Structs, Memory Padding, Function Pointers (OOP Emulation)");
    run_structs_and_functions_demo();

    /* --- モジュール 03: 文字列とファイルI/O ---
     * ・ヌル終端文字列の構造と strlen の O(N) コスト
     * ・const char* vs char[] の書き換え可否の違い
     * ・snprintf / fgets など「サイズ指定付き安全関数」の使い方
     * ・FILE* によるファイルの書き込み・読み込み */
    print_section("03: Null-Terminated Strings and Safe File I/O");
    run_strings_and_io_demo();

    /* --- モジュール 04: コールバックと擬似ラムダ・クロージャ ---
     * ・関数ポインタによるステートレスな高階関数
     * ・void* user_data による状態キャプチャ (クロージャの物理構造)
     * ・ヒープ確保による安全な非同期コールバックライフサイクル */
    print_section("04: Callbacks, State Capture (void* user_data), and Closures");
    run_lambda_and_callbacks_demo();

    /* --- モジュール 05: Modern C11/C23 言語機能 ---
     * ・_Generic による型ディスパッチ（疑似オーバーロード）
     * ・匿名構造体 & 匿名共用体
     * ・フレキシブル配列メンバ */
    print_section("05: Modern C11/C23 Features (_Generic, Anonymous Structs/Unions)");
    run_modern_c11_features();

    /* --- モジュール 06: ビット演算・エンディアン・バイナリ操作 ---
     * ・ビットマスク・ビットシフトによるフラグ管理
     * ・ビットフィールドによる最小メモリレイアウト
     * ・エンディアン判定とバイトスワップ */
    print_section("06: Bitwise Operations, Bitfields, and Endianness");
    run_bitwise_and_binary_demo();

    /* --- モジュール 07: プリプロセッサ極意・マクロ・X-Macros ---
     * ・do-while(0) による安全な複数文マクロ
     * ・文字列化 (#) とトークン連結 (##)
     * ・X-Macros パターンによる Enum/文字列の一元自動生成 */
    print_section("07: Preprocessor Macros, Stringification, and X-Macros");
    run_preprocessor_and_macros_demo();

    /* --- モジュール 08: エラーハンドリング・goto cleanup・setjmp/longjmp ---
     * ・Linux カーネル標準の goto cleanup イディオム
     * ・errno とエラー処理
     * ・setjmp / longjmp による非局所大域脱出 (例外エミュレーション) */
    print_section("08: Error Handling (goto cleanup) and setjmp/longjmp");
    run_error_handling_and_jump_demo();

    /* --- モジュール 09: 動的データ構造 ---
     * ・容量自動倍増 (Doubling) による動的配列 (Vector) 実装
     * ・Linux カーネルスタイル侵入型リストと container_of マクロの物理構造 */
    print_section("09: Dynamic Data Structures (Vector and container_of Intrusive List)");
    run_dynamic_data_structures_demo();

    print_banner("ALL C TUTORIAL MODULES COMPLETED SUCCESSFULLY!");

    /* return 0: プログラムが正常終了したことを OS に伝える。
     * C言語では main() でも return 0 の省略は推奨しない（移植性のために明示する）。 */
    return 0;
}
