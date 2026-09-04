#ifndef BITWISE_AND_BINARY_H
#define BITWISE_AND_BINARY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * ============================================================================
 * モジュール 06: ビット演算・エンディアン・バイナリ操作 (Bitwise & Binary)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. ビット演算とビットマスク:
 *    - フラグ管理 (AND/OR/XOR/NOT/Shift) は組み込み・OS・ネットワークプロトコルで必須。
 *    - Rust の `bitflags` クレートや C# の `[Flags] enum` の物理的実体。
 * 
 * 2. ビットフィールド (Bitfields):
 *    - 構造体のメンバに対して「ビット幅」を指定可能 (`unsigned int flag : 1;`)。
 *    - 1バイトや数ビット単位でパックされたハードウェアレジスタをマッピングできる。
 * 
 * 3. エンディアン (Endianness) とバイトスワップ:
 *    - CPU（x86_64 はリトルエンディアン）とネットワーク（ビッグエンディアン）の相互変換。
 *    - ポインタキャストによるバイトダンプ。
 */

// フラグ定義 (ビットマスク)
#define PERM_READ    (1U << 0) // 0b0001 = 1
#define PERM_WRITE   (1U << 1) // 0b0010 = 2
#define PERM_EXECUTE (1U << 2) // 0b0100 = 4
#define PERM_ADMIN   (1U << 3) // 0b1000 = 8

// ビットフィールド構造体 (わずか 1 バイトに 3 つのフラグと 4 ビット数値を格納)
typedef struct {
    unsigned int is_active   : 1; // 1 bit (0 or 1)
    unsigned int is_verified : 1; // 1 bit (0 or 1)
    unsigned int is_admin    : 1; // 1 bit (0 or 1)
    unsigned int access_tier : 5; // 5 bits (0 ~ 31)
} UserStatusFlags;

// エンディアン判定関数 (true: リトルエンディアン, false: ビッグエンディアン)
bool is_system_little_endian(void);

// 32ビット整数のエンディアンバイトスワップ
uint32_t swap_endian32(uint32_t val);

// メモリ領域のバイト列を16進数でダンプ表示
void print_hex_dump(const void* ptr, size_t size);

// モジュール実行エントリポイント
void run_bitwise_and_binary_demo(void);

#endif // BITWISE_AND_BINARY_H
