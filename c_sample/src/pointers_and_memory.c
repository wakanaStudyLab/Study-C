#include "pointers_and_memory.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * C言語 01: ポインタとメモリ管理 (Pointers & Memory Management)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. ポインタの本質:
 *    - 変数の「メモリアドレス」を保持する整数値（64bit環境なら8バイト）。
 *    - `&x`: xのアドレスを取得 (Rustの `&x`, Goの `&x`)。
 *    - `*p`: ポインタpが指すメモリの中身を読み書き (参照外し / Dereference)。
 * 
 * 2. ポインタ演算 (Pointer Arithmetic):
 *    - `p + 1` は「1バイト進む」のではなく、「型サイズ (sizeof(*p)) 分だけ進む」。
 *    - 配列の添字アクセス `arr[i]` は `*(arr + i)` のシンタックスシュガーに過ぎない。
 * 
 * 3. ヒープと手動メモリ管理 (malloc / free):
 *    - ガベージコレクション (GC) や RAII (Drop) はありません。
 *    - `malloc(size)` で確保したメモリは、必ずプログラマが明示的に `free(ptr)` する必要があります。
 *    - 【典型的な3大バグ (Rustが解決した問題)】:
 *      ① メモリリーク (Memory Leak): free を忘れる。
 *      ② ダングリングポインタ / Use-After-Free: free 後の領域にアクセスする。
 *      ③ 二重解放 (Double Free): 同じポインタを2回 free する。
 * 
 * 4. 多重ポインタ (Double Pointer: `int**`):
 *    - 「関数の引数で、呼び出し元のポインタ変数そのものを書き換える（再代入する）」ために必須。
 */

// 関数の引数はすべて「値渡し (Pass by Value)」。
// ポインタを渡すことで、呼び出し元の変数を書き換える（Rustの &mut, Goの *int 相当）。
static void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 多重ポインタ: 関数の内部で動的確保し、呼び出し元のポインタにアドレスをセットする
static void allocate_array(int** out_ptr, size_t count) {
    // sizeof(*out_ptr) ではなく sizeof(**out_ptr) なので注意
    *out_ptr = (int*)malloc(count * sizeof(int));
    if (*out_ptr == NULL) {
        perror("malloc failed");
        return;
    }
    for (size_t i = 0; i < count; i++) {
        (*out_ptr)[i] = (int)(i * 10);
    }
}

void run_pointers_and_memory_demo(void) {
    printf("--- 1. Pointers Basics & Address Operations ---\n");
    int x = 42;
    int y = 99;
    int* px = &x; // px は x のメモリアドレスを指す

    printf("x value: %d, x address (&x): %p\n", x, (void*)&x);
    printf("px value (address): %p, *px (dereference): %d\n", (void*)px, *px);

    // ポインタ渡しによる値の入れ替え
    printf("Before Swap: x=%d, y=%d\n", x, y);
    swap(&x, &y);
    printf("After Swap:  x=%d, y=%d\n", x, y);

    printf("\n--- 2. Pointer Arithmetic and Arrays ---\n");
    int numbers[4] = { 10, 20, 30, 40 };
    int* pnum = numbers; // 配列名は先頭要素へのポインタ &numbers[0] に暗黙変換される

    for (int i = 0; i < 4; i++) {
        // numbers[i] と *(pnum + i) は完全に同等
        printf("Index %d: numbers[%d]=%d | *(pnum + %d)=%d | Address=%p\n",
               i, i, numbers[i], i, *(pnum + i), (void*)(pnum + i));
    }

    printf("\n--- 3. Dynamic Memory Allocation (malloc / free) ---\n");
    size_t size = 5;
    // ヒープ領域からメモリをバイト単位で確保
    int* heap_array = (int*)malloc(size * sizeof(int));
    
    // 【重要作法】malloc は失敗すると NULL を返すため、必ずチェックする
    if (heap_array == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    for (size_t i = 0; i < size; i++) {
        heap_array[i] = (int)((i + 1) * 100);
    }

    printf("Heap Array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", heap_array[i]);
    }
    printf("\n");

    // 必ず free する
    free(heap_array);
    // 【ベストプラクティス】free 後は直ちに NULL を代入してダングリングポインタ（不正アクセス）を防ぐ
    heap_array = NULL;

    printf("\n--- 4. Double Pointer (int**) Allocation ---\n");
    int* dynamic_list = NULL;
    allocate_array(&dynamic_list, 3);
    if (dynamic_list != NULL) {
        printf("Allocated via double pointer: %d, %d, %d\n",
               dynamic_list[0], dynamic_list[1], dynamic_list[2]);
        free(dynamic_list);
        dynamic_list = NULL;
    }
}
