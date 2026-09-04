#ifndef MODERN_C11_FEATURES_H
#define MODERN_C11_FEATURES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * ============================================================================
 * モジュール 05: Modern C11/C23 言語機能 (Modern C11/C23 Features)
 * ============================================================================
 * 
 * 【他言語経験者向け要点】
 * 1. _Generic 選択式 (C11):
 *    - C言語には関数の多重定義（オーバーロード）がないが、C11 で導入された
 *      `_Generic` により、引数の「型」に基づいてコンパイル時に呼び出す関数を切り替え可能。
 * 
 * 2. 匿名構造体・共用体 (Anonymous Structs & Unions):
 *    - 名前なしでネストでき、`vec.x` と `vec.raw[0]` の両方で同一メモリ領域にアクセス可能。
 * 
 * 3. フレキシブル配列メンバ (Flexible Array Member):
 *    - 構造体の末尾に `char data[]` を宣言し、ヘッダとペイロードを単一の malloc で連続確保。
 * 
 * 4. _Static_assert (C11):
 *    - コンパイル時アサーションによる型サイズ・アライメントの事前検証。
 */

// 1. _Generic による疑似オーバーロードマクロ
const char* format_type_int(int val);
const char* format_type_double(double val);
const char* format_type_string(const char* val);
const char* format_type_default(void);

#define describe_type(X) _Generic((X), \
    int: format_type_int,              \
    double: format_type_double,        \
    char*: format_type_string,         \
    const char*: format_type_string,   \
    default: format_type_default       \
)(X)

// 2. 匿名構造体と共用体 (3D ベクトル)
typedef struct {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float raw[3]; // 配列としてもインデックスアクセス可能
    };
} Vector3;

// 3. フレキシブル配列メンバを持つパケット構造体
typedef struct {
    uint32_t packet_id;
    size_t payload_len;
    uint8_t payload[]; // サイズ未定の末尾配列 (構造体自体のサイズには含まれない)
} NetworkPacket;

NetworkPacket* packet_create(uint32_t id, const uint8_t* data, size_t len);
void packet_free(NetworkPacket* packet);

// モジュール実行エントリポイント
void run_modern_c11_features(void);

#endif // MODERN_C11_FEATURES_H
