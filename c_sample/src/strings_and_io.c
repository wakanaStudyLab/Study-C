#include "strings_and_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ============================================================================
 * C言語 03: 文字列操作とファイル入出力 (Strings & File I/O)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. C言語の文字列の本質:
 *    - 文字列型 (String) は存在せず、単なる「末尾にヌル文字 `\0` (ASCII 0) を含む `char` 配列」。
 *    - 文字列の長さを保持していないため、`strlen()` は `\0` に出会うまでメモリを線形探索します (O(N))。
 *    - ヌル終端を忘れると、メモリの境界を越えて暴走（Buffer Overread）します。
 * 
 * 2. `const char*` vs `char[]`:
 *    - `const char* s = "Hello";` -> 実行ファイルの読み取り専用データセクション (.rodata)。書き換え不可（SIGSEGV / クラッシュ）。
 *    - `char s[] = "Hello";`      -> スタック上にコピーされた配列。個別の文字を書き換え可能。
 * 
 * 3. セキュリティとバッファオーバーフロー対策:
 *    - `strcpy`, `strcat`, `gets`, `sprintf` はバッファサイズを検証しないため【使用禁止 (Deprecated)】。
 *    - 代わりに `snprintf`, `strncpy_s`, `fgets` などの「サイズ指定付き安全関数」を使用します。
 * 
 * 4. ファイルI/O:
 *    - `FILE*` 構造体ハンドルを介してストリーム操作。
 *    - 使用後は必ず `fclose(f)` でOSリソースを解放する。
 */

void run_strings_and_io_demo(void) {
    printf("--- 1. String Structure and Null-Terminator (\\0) ---\n");
    // "Hi" はメモリ上では {'H', 'i', '\0'} の3バイト
    char greeting[] = "Hi!";
    printf("greeting: %s, sizeof: %zu bytes, strlen: %zu chars\n",
           greeting, sizeof(greeting), strlen(greeting));

    // 文字列の書き換え (char[] だから可能)
    greeting[0] = 'B'; // "Bi!" になる
    printf("Modified greeting: %s\n", greeting);

    printf("\n--- 2. Safe String Formatting (snprintf) ---\n");
    char buffer[64];
    const char* user = "Haru";
    int score = 100;
    
    // snprintf はバッファサイズ (sizeof(buffer)) を超えないように安全に切り詰める
    int written = snprintf(buffer, sizeof(buffer), "User: %s (Score: %d)", user, score);
    printf("Formatted string: %s (length: %d)\n", buffer, written);

    // 文字列の比較 (strcmp: 一致すれば 0)
    // C言語で s1 == s2 と書くと「ポインタ（アドレス）比較」になるため厳禁！
    if (strcmp(user, "Haru") == 0) {
        printf("User is Haru!\n");
    }

    printf("\n--- 3. File I/O ---\n");
    const char* filename = "temp_c_demo.txt";

    // ファイル書き込み (fopen_s は MSVC の安全版、標準なら fopen)
    FILE* fp = NULL;
#if defined(_MSC_VER)
    fopen_s(&fp, filename, "w");
#else
    fp = fopen(filename, "w");
#endif

    if (fp != NULL) {
        fprintf(fp, "Hello from C File I/O!\nLine 2: Pointers and Memory are awesome.\n");
        fclose(fp);
        printf("File write successful: %s\n", filename);
    } else {
        perror("Failed to open file");
    }

    // ファイル読み込み
#if defined(_MSC_VER)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif

    if (fp != NULL) {
        printf("--- File Read Result ---\n");
        char line[128];
        while (fgets(line, sizeof(line), fp) != NULL) {
            printf("[Read] %s", line);
        }
        fclose(fp);
        // 一時ファイルを削除
        remove(filename);
    }
}
