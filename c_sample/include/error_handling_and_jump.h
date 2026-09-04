#ifndef ERROR_HANDLING_AND_JUMP_H
#define ERROR_HANDLING_AND_JUMP_H

#include <stdbool.h>
#include <setjmp.h>

/**
 * ============================================================================
 * モジュール 08: エラーハンドリング・goto cleanup・setjmp/longjmp
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. goto cleanup イディオム (Linux カーネル標準):
 *    - C言語には RAII や defer、finally がない。
 *    - 複数のリソース（ファイル、メモリ、ソケット）を獲得する際、途中で失敗したら
 *      `goto cleanup;` で確保済みのリソースだけを確実に解放する実務上の最重要イディオム。
 * 
 * 2. errno & strerror:
 *    - 標準Cライブラリ関数のエラー報告方法。スレッドローカル変数 `errno` にエラー番号が入る。
 * 
 * 3. setjmp / longjmp:
 *    - 実行コンテキスト（CPUレジスタ群とスタックポインタ）を保存し、
 *      別関数から一気にジャンプして戻ってくる仕組み。
 *    - C++の例外 (throw / catch) やコルーチンの低レイヤな物理的基礎。
 */

// goto cleanup による多段階リソース確保関数のサンプル
bool process_two_files(const char* file1_name, const char* file2_name);

// setjmp / longjmp のデモ実行
void demonstrate_setjmp_longjmp(void);

// モジュール実行エントリポイント
void run_error_handling_and_jump_demo(void);

#endif // ERROR_HANDLING_AND_JUMP_H
