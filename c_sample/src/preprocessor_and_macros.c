#include "preprocessor_and_macros.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/**
 * ============================================================================
 * モジュール 07: プリプロセッサ極意・マクロ・X-Macros (Preprocessor & Macros)
 * ============================================================================
 */

// C11 _Static_assert によるコンパイル時検査
_Static_assert(sizeof(uint32_t) == 4, "uint32_t must be exactly 4 bytes");

// MAKE_FUNC マクロで関数を自動生成
MAKE_FUNC(alpha)
MAKE_FUNC(beta)

// X-Macros による switch-case 文字列化関数の自動生成
const char* http_status_to_string(HttpStatus status) {
    switch (status) {
        #define AS_CASE(enum_name, code, msg) case enum_name: return msg;
        HTTP_STATUS_TABLE(AS_CASE)
        #undef AS_CASE
        default:
            return "Unknown Status";
    }
}

void run_preprocessor_and_macros_demo(void) {
    printf("--- 1. Safe Multi-statement Macro (do-while(0) idiom) ---\n");
    int x = 10, y = 20;
    printf("  Before SAFE_SWAP: x = %d, y = %d\n", x, y);
    
    // if文で波括弧を省略しても、else との構文崩れが発生しない
    if (x < y)
        SAFE_SWAP(x, y, int);
    else
        printf("  No swap needed\n");

    printf("  After SAFE_SWAP:  x = %d, y = %d\n", x, y);

    printf("\n--- 2. Stringification (#) and Token Pasting (##) ---\n");
    int sensor_temperature = 42;
    LOG_VAR(sensor_temperature); // #sensor_temperature で変数名が文字列になる

    // ## で生成された関数を呼び出す
    run_func_alpha();
    run_func_beta();

    printf("\n--- 3. X-Macros Table Pattern (DRY Enum + String Generator) ---\n");
    HttpStatus codes[] = {HTTP_OK, HTTP_NOT_FOUND, HTTP_INTERNAL_SERVER_ERROR};
    for (size_t i = 0; i < sizeof(codes) / sizeof(codes[0]); ++i) {
        printf("  HTTP %d -> %s\n", codes[i], http_status_to_string(codes[i]));
    }
}
