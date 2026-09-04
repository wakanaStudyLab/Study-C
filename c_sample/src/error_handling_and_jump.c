#include "error_handling_and_jump.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * ============================================================================
 * モジュール 08: エラーハンドリング・goto cleanup・setjmp/longjmp
 * ============================================================================
 */

// グローバルなジャンプバッファ
static jmp_buf g_exception_env;

// setjmp / longjmp デモ用の下請け関数
static void risky_operation(int level) {
    printf("    [Worker] Entering risky_operation(level=%d)...\n", level);
    if (level < 0) {
        printf("    [Worker] Negative level detected! Longjmping back to safety...\n");
        longjmp(g_exception_env, 101); // 戻り値 101 で setjmp の位置へ大域ジャンプ
    }
    printf("    [Worker] Operation succeeded for level %d\n", level);
}

bool process_two_files(const char* file1_name, const char* file2_name) {
    FILE* f1 = NULL;
    FILE* f2 = NULL;
    char* buffer = NULL;
    bool success = false;

    // リソース1: ヒープメモリの確保
    buffer = (char*)malloc(128);
    if (!buffer) {
        perror("  Failed to allocate memory");
        goto cleanup;
    }
    printf("    [Resource 1] Memory allocated at %p\n", (void*)buffer);

    // リソース2: ファイル1 のオープン
    f1 = fopen(file1_name, "w");
    if (!f1) {
        printf("  Failed to open %s: %s\n", file1_name, strerror(errno));
        goto cleanup;
    }
    printf("    [Resource 2] File 1 opened: %s\n", file1_name);

    // リソース3: ファイル2 のオープン (故意に無効なパスで失敗させることも可能)
    f2 = fopen(file2_name, "w");
    if (!f2) {
        printf("  Failed to open %s: %s\n", file2_name, strerror(errno));
        goto cleanup;
    }
    printf("    [Resource 3] File 2 opened: %s\n", file2_name);

    // すべて成功した場合の処理
    fputs("Hello File 1\n", f1);
    fputs("Hello File 2\n", f2);
    success = true;

cleanup:
    // 逆順（LIFO）で確実にリソース解放（確保されたものだけを安全にクリーンアップ）
    printf("    [Cleanup Phase] Releasing resources in reverse order...\n");
    if (f2) {
        fclose(f2);
        printf("    - File 2 closed\n");
    }
    if (f1) {
        fclose(f1);
        printf("    - File 1 closed\n");
    }
    if (buffer) {
        free(buffer);
        printf("    - Memory freed\n");
    }

    return success;
}

void demonstrate_setjmp_longjmp(void) {
    // setjmp は初回呼び出し時には 0 を返す
    int val = setjmp(g_exception_env);

    if (val == 0) {
        printf("  [Try block] Registered exception context via setjmp(). Calling workers...\n");
        risky_operation(1);
        risky_operation(-5); // ここで longjmp が発動
        printf("  [Try block] This line will never execute!\n");
    } else {
        // longjmp から復帰した場合は第2引数の値 (101) が val に入る
        printf("  [Catch block] Caught exception via longjmp! Error code: %d\n", val);
    }
}

void run_error_handling_and_jump_demo(void) {
    printf("--- 1. Linux Kernel 'goto cleanup' Pattern (Safe Multi-Resource Teardown) ---\n");
    // 正常系テスト
    printf("  Test A: All valid paths\n");
    process_two_files("temp_res1.tmp", "temp_res2.tmp");
    remove("temp_res1.tmp");
    remove("temp_res2.tmp");

    // 異常系テスト (無効なファイル名)
    printf("\n  Test B: Second file has invalid path (triggers cleanup on step 3)\n");
    process_two_files("temp_res1.tmp", "Z:\\invalid_dir_9999\\fail.tmp");
    remove("temp_res1.tmp");

    printf("\n--- 2. Non-Local Jumps via setjmp / longjmp (Exception Emulation) ---\n");
    demonstrate_setjmp_longjmp();
}
