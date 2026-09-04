#ifndef DYNAMIC_DATA_STRUCTURES_H
#define DYNAMIC_DATA_STRUCTURES_H

#include <stddef.h>
#include <stdbool.h>

/**
 * ============================================================================
 * モジュール 09: 動的データ構造 (Dynamic Data Structures & container_of)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. 動的配列 (Vector) の手動実装:
 *    - C言語には std::vector や slices、List<T> がない。
 *    - 容量 (capacity) が足りなくなったら倍々に拡張 (doubling) し、
 *      `realloc` で安全に再確保するアルゴリズムを理解する。
 *    - `realloc` の戻り値を直接元のポインタに代入すると、失敗時に元のメモリがリークする罠。
 * 
 * 2. 侵入型データ構造 (Intrusive Data Structures & container_of):
 *    - Linux カーネルなどの高性能 C プログラミングで標準的に使われる設計。
 *    - ノード構造体（next/prev）をデータ構造体の「内部メンバ」として埋め込み、
 *      `offsetof` マクロを使って親構造体のポインタを逆算する。
 */

// 1. 動的整数配列 (Vector 相当)
typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} IntVector;

IntVector* int_vector_create(size_t initial_capacity);
bool int_vector_push_back(IntVector* vec, int value);
int int_vector_get(const IntVector* vec, size_t index);
void int_vector_free(IntVector* vec);

// 2. 侵入型リストノード (Intrusive List Node)
typedef struct ListNode {
    struct ListNode* next;
} ListNode;

// container_of マクロの定義 (Linux カーネル由来)
// メンバポインタ ptr から、そのメンバを内包する親構造体 type の先頭アドレスを算出する
#define container_of(ptr, type, member) \
    ((type*)((char*)(ptr) - offsetof(type, member)))

// 侵入型ノードを含むユーザーデータ構造体
typedef struct {
    int user_id;
    char name[32];
    ListNode list_node; // 侵入型ノードを埋め込む！
} UserTask;

// モジュール実行エントリポイント
void run_dynamic_data_structures_demo(void);

#endif // DYNAMIC_DATA_STRUCTURES_H
