#include "pointers_and_memory.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * C言語 01: ポインタとメモリ管理 (Pointers & Memory Management)
 * ============================================================================
 *
 * 【他言語経験者（Rust, C#, Go）向け要点】
 *
 * 1. ポインタの本質:
 *    - 変数の「メモリアドレス」を保持する整数値（64bit環境なら 8 バイト）。
 *    - `&x`  : x のアドレスを取得（Rust の `&x`、Go の `&x` と同じ）。
 *    - `*p`  : ポインタ p が指すメモリの中身を読み書き（参照外し / Dereference）。
 *    - ポインタ型 `int*` は「int 型の変数が置かれているアドレス」という意味。
 *
 * 2. ポインタ演算 (Pointer Arithmetic):
 *    - `p + 1` は「1バイト進む」のではなく「sizeof(*p) バイト分だけ進む」。
 *      例: int* p に対して p+1 は 4バイト先（32bit int の場合）を指す。
 *    - 配列の添字アクセス `arr[i]` は `*(arr + i)` のシンタックスシュガーに過ぎない。
 *      つまり arr[2] と 2[arr] は C 文法上どちらも合法（後者は読みにくいので書かない）。
 *
 * 3. ヒープと手動メモリ管理 (malloc / free):
 *    - ガベージコレクション (GC) も RAII (Drop) も存在しない。
 *    - `malloc(size)` でヒープからバイト単位でメモリを確保し、
 *      必ずプログラマが明示的に `free(ptr)` で解放する必要がある。
 *    - 【典型的な3大バグ (Rust が型システムで解決した問題)】:
 *        ① メモリリーク (Memory Leak)    : free を忘れる → プロセスが終了するまでメモリが解放されない。
 *        ② Use-After-Free               : free 後の領域にアクセス → 未定義動作 (UB)。クラッシュや脆弱性の温床。
 *        ③ 二重解放 (Double Free)        : 同じポインタを 2 回 free → ヒープ破壊・クラッシュ。
 *    - 対策: free 直後に `ptr = NULL;` を徹底することで ② ③ のリスクを大幅に下げられる。
 *
 * 4. 多重ポインタ (Double Pointer: int**):
 *    - 「関数の中で、呼び出し元のポインタ変数そのものを書き換える」ためのパターン。
 *    - Go で `*T` を渡して関数内で `*p = newValue` するのと同じ概念。
 *    - 出力パラメータ (out parameter) として頻用される。
 *
 * 5. void* (汎用ポインタ):
 *    - 任意の型のアドレスを格納できる「型なしポインタ」。
 *    - printf の %p フォーマット指定子は void* を期待するため、(void*) にキャストして渡す。
 *    - malloc の戻り値も void* なので、C++ では明示的キャストが必要だが C では不要。
 *      （このサンプルでは明示的にキャストしている）
 */

/* ============================================================================
 * swap: ポインタ渡しによる値の入れ替え
 * ============================================================================
 * C言語の関数引数はすべて「値渡し (Pass by Value)」。
 * つまり `void swap(int a, int b)` では a, b はコピーであり呼び出し元の変数は変わらない。
 * 呼び出し元の変数を書き換えるには、アドレス（ポインタ）を渡す必要がある。
 * これは Rust の `fn swap(a: &mut i32, b: &mut i32)` / Go の `func swap(a, b *int)` と同じ発想。
 */
static void swap(int* a, int* b) {
    int temp = *a;   /* *a: ポインタ a が指す先（呼び出し元の変数 x）を読む */
    *a = *b;         /* *a に *b の値を書き込む */
    *b = temp;       /* *b に退避した値を書き込む */
}

/* ============================================================================
 * allocate_array: 多重ポインタ (int**) による配列の動的確保
 * ============================================================================
 * 関数内でヒープ確保し、そのアドレスを呼び出し元のポインタ変数に書き込む。
 *
 * out_ptr は「int* 型の変数のアドレス」。
 * *out_ptr に代入することで、呼び出し元の int* 変数が確保したアドレスを指すようになる。
 *
 * 呼び出し側:
 *   int* arr = NULL;
 *   allocate_array(&arr, 5);   // &arr が int** になる
 *   // arr は確保された配列を指す
 */
static void allocate_array(int** out_ptr, size_t count) {
    /* sizeof(**out_ptr) = sizeof(int)。
     * out_ptr 自体のサイズ (sizeof(int*) = 8 bytes) ではない点に注意。 */
    *out_ptr = (int*)malloc(count * sizeof(int));

    /* malloc は失敗すると NULL を返す（メモリ不足時など）。
     * NULL チェックなしで使うと即座に未定義動作になるため必須。 */
    if (*out_ptr == NULL) {
        perror("malloc failed"); /* エラーメッセージを stderr に出力 */
        return;
    }

    /* 確保したメモリを初期値で埋める (0, 10, 20, ...) */
    for (size_t i = 0; i < count; i++) {
        (*out_ptr)[i] = (int)(i * 10);
        /* ※ *out_ptr[i] と書くと [] の優先順位が高いため (*out_ptr) と括弧が必要 */
    }
}

/* ============================================================================
 * run_pointers_and_memory_demo: デモ実行関数
 * ============================================================================ */
void run_pointers_and_memory_demo(void) {

    /* ====================================================================
     * 1. ポインタの基礎とアドレス演算子
     * ==================================================================== */
    printf("--- 1. Pointers Basics & Address Operations ---\n");
    int x = 42;
    int y = 99;
    int* px = &x; /* px = x のアドレス。px 自体はスタック上の 8 バイト変数（アドレス格納用） */

    /* %p: ポインタ（アドレス）を 16進数で表示。(void*) にキャストするのが標準準拠。 */
    printf("x value: %d, x address (&x): %p\n", x, (void*)&x);
    printf("px value (address): %p, *px (dereference): %d\n", (void*)px, *px);

    /* swap に &x, &y を渡すことで関数内から呼び出し元の x, y を書き換えられる */
    printf("Before Swap: x=%d, y=%d\n", x, y);
    swap(&x, &y);
    printf("After Swap:  x=%d, y=%d\n", x, y);

    /* ====================================================================
     * 2. ポインタ演算と配列
     * ==================================================================== */
    printf("\n--- 2. Pointer Arithmetic and Arrays ---\n");
    int numbers[4] = { 10, 20, 30, 40 };

    /* 配列名 `numbers` は先頭要素 &numbers[0] へのポインタに「暗黙変換 (Decay)」される。
     * ただし sizeof(numbers) は配列全体のサイズ（16 bytes）を返す（decay しない例外的な文脈）。 */
    int* pnum = numbers;

    for (int i = 0; i < 4; i++) {
        /* numbers[i] と *(pnum + i) は C言語において完全に等価な表現。
         * pnum + i のアドレスは pnum + i * sizeof(int) バイト先を指している。 */
        printf("Index %d: numbers[%d]=%d | *(pnum + %d)=%d | Address=%p\n",
               i, i, numbers[i], i, *(pnum + i), (void*)(pnum + i));
    }

    /* ====================================================================
     * 3. 動的メモリ確保 (malloc / free)
     * ==================================================================== */
    printf("\n--- 3. Dynamic Memory Allocation (malloc / free) ---\n");
    size_t size = 5;

    /* malloc(n): n バイトのヒープメモリを確保し、先頭アドレスを返す。
     * 確保した領域の中身は「不定（ゴミ値）」。ゼロ初期化したい場合は calloc() を使う。
     * (int*) へのキャスト: C++ では void* から他のポインタ型への暗黙変換は禁止されるが、
     * C では不要。ただし明示すると C++ との共用コードで役立つ。 */
    int* heap_array = (int*)malloc(size * sizeof(int));

    /* 【重要】malloc は失敗すると NULL を返す。必ず戻り値をチェックすること。
     * NULL チェックなしで *heap_array にアクセスすると即クラッシュ（Segmentation Fault）。 */
    if (heap_array == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    /* 確保したメモリを初期化して使う */
    for (size_t i = 0; i < size; i++) {
        heap_array[i] = (int)((i + 1) * 100); /* 100, 200, 300, 400, 500 */
    }

    printf("Heap Array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", heap_array[i]);
    }
    printf("\n");

    /* free(ptr): malloc で確保したヒープメモリを OS に返却する。
     * この呼び出しを忘れるとプロセスが終了するまでメモリが解放されない（メモリリーク）。 */
    free(heap_array);

    /* 【ベストプラクティス】free 直後に NULL を代入する。
     * こうすると、誤って再アクセスしても NULL の参照外しとなり、
     * 挙動が未定義にならず（大抵はクラッシュとして）問題を早期発見できる。
     * また二重 free も防げる（free(NULL) は安全なノーオペレーション）。 */
    heap_array = NULL;

    /* ====================================================================
     * 4. 多重ポインタ (int**) を使った出力パラメータパターン
     * ==================================================================== */
    printf("\n--- 4. Double Pointer (int**) Allocation ---\n");
    int* dynamic_list = NULL;

    /* &dynamic_list: dynamic_list 自体のアドレス（int** 型）を渡す。
     * 関数内で *out_ptr = malloc(...) することで dynamic_list が確保アドレスを指すようになる。 */
    allocate_array(&dynamic_list, 3);

    if (dynamic_list != NULL) {
        printf("Allocated via double pointer: %d, %d, %d\n",
               dynamic_list[0], dynamic_list[1], dynamic_list[2]); /* 0, 10, 20 */
        free(dynamic_list);
        dynamic_list = NULL; /* 使用後は NULL に戻す */
    }
}
