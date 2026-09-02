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
 *
 * 1. C言語の文字列の本質:
 *    - Rust の `String` / `&str`、Go の `string`、C# の `string` に相当する「文字列型」は存在しない。
 *    - C言語の文字列は単なる「末尾にヌル文字 `\0`（ASCII コード 0）を含む char 配列」に過ぎない。
 *    - 文字列の「長さ」はどこにも保存されていないため、`strlen()` は `\0` に出会うまで
 *      メモリを線形探索する O(N) 処理になる（巨大な文字列のループ内での呼び出しは避けること）。
 *    - メモリ上のイメージ（"Hi!" の場合）:
 *        アドレス: [0x00] [0x01] [0x02] [0x03]
 *        内容:     'H'    'i'    '!'    '\0'
 *        sizeof("Hi!") = 4（ヌル文字含む）
 *        strlen("Hi!")  = 3（ヌル文字を除く文字数）
 *
 * 2. `const char*` vs `char[]`:
 *    ┌───────────────────────┬──────────────────────────────────────────────────────┐
 *    │ 宣言                  │ 挙動                                                 │
 *    ├───────────────────────┼──────────────────────────────────────────────────────┤
 *    │ const char* s = "Hi"; │ 実行ファイルの読み取り専用セクション (.rodata) を指す │
 *    │                       │ s[0] = 'h'; は未定義動作 → SIGSEGV でクラッシュ      │
 *    ├───────────────────────┼──────────────────────────────────────────────────────┤
 *    │ char s[] = "Hi";      │ スタック上に {'H','i','\0'} がコピーされた配列       │
 *    │                       │ s[0] = 'h'; は安全に書き換えられる                  │
 *    └───────────────────────┴──────────────────────────────────────────────────────┘
 *
 * 3. セキュリティとバッファオーバーフロー対策:
 *    - 以下の関数はバッファサイズを検証しないため【現代では使用禁止】:
 *        gets()     → fgets() で代替
 *        strcpy()   → strncpy() / snprintf() で代替
 *        strcat()   → strncat() / snprintf() で代替
 *        sprintf()  → snprintf() で代替
 *    - 安全関数の鉄則: 「バッファサイズを必ず引数で渡す」。
 *      snprintf(buf, sizeof(buf), ...) のように sizeof を直接渡すのが最も安全。
 *    - MSVC 環境では fopen_s, strcpy_s など `_s` サフィックス付きの安全版が利用できる
 *      (ISO C11 Annex K 相当)。ただし POSIX 環境では非対応のため #if で分岐する。
 *
 * 4. ファイルI/O:
 *    - FILE* 構造体ハンドルを介してストリーム操作。
 *    - fopen(filename, mode): ファイルを開いて FILE* を返す（失敗すると NULL）。
 *        "r"  : 読み込み専用
 *        "w"  : 書き込み（ファイルが存在すれば上書き、なければ新規作成）
 *        "a"  : 追記
 *        "rb" : バイナリ読み込み
 *    - 使用後は必ず fclose(fp) で OS リソース（ファイルディスクリプタ）を解放する。
 *    - fprintf(fp, ...): FILE* に対して printf 同様のフォーマット出力。
 *    - fgets(buf, n, fp): 最大 n-1 文字を 1 行読み込む安全関数（\0 を自動付与）。
 */

void run_strings_and_io_demo(void) {

    /* ====================================================================
     * 1. 文字列の構造とヌル終端文字 (\0)
     * ==================================================================== */
    printf("--- 1. String Structure and Null-Terminator (\\0) ---\n");

    /* char[] で宣言: スタック上に {'H','i','!','\0'} の 4 バイト配列がコピーされる。
     * 文字の書き換えが可能（後述）。 */
    char greeting[] = "Hi!";

    /* sizeof(greeting) = 4（ヌル文字 \0 を含む配列の全バイト数）
     * strlen(greeting) = 3（\0 を除いた文字数、O(N) 処理）
     * %zu: size_t 型（符号なし整数）の書式指定子 */
    printf("greeting: %s, sizeof: %zu bytes, strlen: %zu chars\n",
           greeting, sizeof(greeting), strlen(greeting));

    /* char[] はスタック上のコピーなので個別の文字を書き換えられる。
     * 一方 `const char* s = "Hi!";` の s[0] = 'B'; は実行時クラッシュになる。 */
    greeting[0] = 'B'; /* "Hi!" → "Bi!" に変更 */
    printf("Modified greeting: %s\n", greeting);

    /* ====================================================================
     * 2. 安全な文字列フォーマット (snprintf)
     * ==================================================================== */
    printf("\n--- 2. Safe String Formatting (snprintf) ---\n");
    char buffer[64]; /* フォーマット結果を格納するバッファ（スタック上） */
    const char* user = "Haru";
    int score = 100;

    /* snprintf(dest, n, format, ...):
     * - dest に最大 n-1 文字（+ \0 で合計 n バイト）を書き込む。
     * - バッファを絶対に溢れないため、sprintf の安全な置き換えとして常に使うべき。
     * - 戻り値: 書き込もうとした文字数（\0 を除く）。n より大きければ切り詰めが発生している。 */
    int written = snprintf(buffer, sizeof(buffer), "User: %s (Score: %d)", user, score);
    printf("Formatted string: %s (length: %d)\n", buffer, written);

    /* 文字列の比較には strcmp() を使う。
     * 【重要】C言語で `user == "Haru"` と書くと「ポインタ（アドレス）の比較」になり、
     * 文字列の内容を比較したことにならない。必ず strcmp() を使うこと。
     * strcmp(s1, s2) の戻り値:
     *   0     → 文字列が一致
     *   負の値 → s1 が s2 より辞書順で小さい
     *   正の値 → s1 が s2 より辞書順で大きい */
    if (strcmp(user, "Haru") == 0) {
        printf("User is Haru!\n");
    }

    /* ====================================================================
     * 3. ファイルI/O (FILE* ストリーム)
     * ==================================================================== */
    printf("\n--- 3. File I/O ---\n");
    const char* filename = "temp_c_demo.txt";

    /* --- ファイル書き込み ---
     * fopen(filename, "w"): 書き込みモードでファイルを開く。
     * MSVC と GCC/Clang では安全版 API が異なるため、プリプロセッサマクロで分岐する。
     * _MSC_VER: MSVC でのみ定義されるマクロ（バージョン番号）。 */
    FILE* fp = NULL;
#if defined(_MSC_VER)
    /* fopen_s: MSVC の安全版。FILE** を第1引数に受け取る（出力パラメータパターン）。 */
    fopen_s(&fp, filename, "w");
#else
    /* POSIX 標準の fopen: 戻り値として FILE* を返す。失敗すると NULL。 */
    fp = fopen(filename, "w");
#endif

    if (fp != NULL) {
        /* fprintf(fp, format, ...): fp が指すファイルストリームに printf 形式で書き込む。
         * \n はテキストモード ("w") では OS 改行コードに自動変換される（Windows なら \r\n）。 */
        fprintf(fp, "Hello from C File I/O!\nLine 2: Pointers and Memory are awesome.\n");

        /* fclose: バッファをフラッシュし、ファイルを閉じて OS リソースを解放する。
         * fclose を忘れると書き込みバッファが flush されず、内容がディスクに書かれない場合がある。 */
        fclose(fp);
        fp = NULL; /* 再利用前に NULL に戻しておく（dangling FILE* 防止） */
        printf("File write successful: %s\n", filename);
    } else {
        /* perror: errno に設定されたエラーコードに対応するメッセージを stderr に出力する。
         * 例: "Failed to open file: No such file or directory" */
        perror("Failed to open file");
    }

    /* --- ファイル読み込み --- */
#if defined(_MSC_VER)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif

    if (fp != NULL) {
        printf("--- File Read Result ---\n");
        char line[128]; /* 1行分を格納するバッファ */

        /* fgets(buf, n, fp): fp から最大 n-1 文字を読み込み buf に格納する。
         * - 改行文字 (\n) が読めたら、そこで停止して \n も buf に含める。
         * - EOF またはエラーの場合は NULL を返す → while 条件でループを終了。
         * - gets() と違いバッファサイズを指定するため安全（gets は廃止済み）。 */
        while (fgets(line, sizeof(line), fp) != NULL) {
            /* %s で出力すると \n まで含まれるため、printf に追加の \n は不要 */
            printf("[Read] %s", line);
        }

        fclose(fp);
        fp = NULL;

        /* remove(filename): ファイルシステムからファイルを削除する（一時ファイルのクリーンアップ）。
         * 成功すれば 0、失敗すれば非 0 を返す（今回はエラーチェック省略）。 */
        remove(filename);
    }
}
