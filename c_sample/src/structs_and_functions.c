#include "structs_and_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ============================================================================
 * C言語 02: 構造体と関数ポインタ (Structs & Function Pointers)
 * ============================================================================
 *
 * 【他言語経験者（Rust, C#, Go）向け要点】
 *
 * 1. 構造体 (struct):
 *    - C# の struct / Go の struct / Rust の struct の元祖。
 *    - C言語では `struct TypeName { ... };` と書くと `struct TypeName` が型名になる。
 *      `typedef struct { ... } TypeName;` で `TypeName` だけで使えるエイリアスを定義するのが慣例。
 *    - メンバアクセス:
 *        実体 (value)  → `.` 演算子  (例: s.id)
 *        ポインタ      → `->` 演算子 (例: p->id、`(*p).id` の糖衣構文)
 *
 * 2. メモリレイアウトとパディング (Padding & Alignment):
 *    - CPU は「アライメント (Alignment)」に従ったアドレスへのアクセスが最速。
 *      例: int (4バイト) は 4 の倍数アドレスに配置される必要がある。
 *    - コンパイラはこの制約を満たすため、メンバ間に自動的に「パディング（詰め物）」を挿入する。
 *    - フィールドの宣言順序によって sizeof(struct) が変わる。
 *      大きい型から順に並べると無駄なパディングを最小化できる。
 *    - #pragma pack(1) や __attribute__((packed)) でパディングを抑制することも可能だが、
 *      パフォーマンス低下やアーキテクチャ依存の問題が生じるため注意が必要。
 *
 * 3. 関数ポインタ (Function Pointer):
 *    - 「関数の実行コードが置かれているメモリアドレス」を保持するポインタ。
 *    - 宣言構文: `戻り値型 (*変数名)(引数型, ...);`
 *      例: `int (*fp)(int, int);` → int を 2 つ受け取り int を返す関数へのポインタ。
 *    - typedef で型エイリアスを作ると格段に読みやすくなる。
 *    - 対応言語機能: C# のデリゲート / Go の `func(int) int` 型 / Rust の `fn(i32) -> i32`。
 *    - コールバック・プラグイン・戦略パターン (Strategy Pattern) などに使われる。
 *
 * 4. 関数ポインタを struct に埋め込む = VTable (仮想関数テーブル) の手動実装:
 *    - C++ の virtual 関数、Go の interface、Rust の trait object (dyn Trait) の内部実装原理。
 *    - Linux カーネルの `struct file_operations` など、低レイヤシステムプログラミングで多用される。
 */

/* ============================================================================
 * メモリレイアウト比較用の構造体
 * ============================================================================ */

/* 非効率な並び: char → int → char の順に宣言するとパディングが多く発生する
 *
 * メモリ上のレイアウト（64bit GCC 典型例）:
 *   [0] a (1 byte)
 *   [1][2][3] padding (3 bytes) ← int の 4byte 境界に合わせるため
 *   [4][5][6][7] b (4 bytes)
 *   [8] c (1 byte)
 *   [9][10][11] padding (3 bytes) ← 構造体全体のサイズが int のサイズの倍数になるよう末尾調整
 *   合計: 12 bytes
 */
typedef struct {
    char a;     /* 1 byte */
    int  b;     /* 4 bytes（直前に 3 bytes のパディングが挿入される） */
    char c;     /* 1 byte（直後に 3 bytes のパディングが挿入される） */
} InefficientLayout; /* sizeof = 12 bytes! */

/* 効率的な並び: 大きい型を前、小さい型を後ろに並べると無駄なパディングが減る
 *
 * メモリ上のレイアウト:
 *   [0][1][2][3] b (4 bytes)
 *   [4] a (1 byte)
 *   [5] c (1 byte)
 *   [6][7] padding (2 bytes) ← 末尾調整のみ
 *   合計: 8 bytes（12 bytes から 4 bytes 削減）
 */
typedef struct {
    int  b;     /* 4 bytes（先頭に置くことでパディング不要） */
    char a;     /* 1 byte */
    char c;     /* 1 byte */
} EfficientLayout; /* sizeof = 8 bytes */

/* ============================================================================
 * 実務で使う構造体の例
 * ============================================================================ */
typedef struct {
    int    id;       /* 学生 ID */
    char   name[32]; /* 固定長の名前バッファ（ヌル終端を含むため最大 31 文字） */
    double score;    /* スコア（浮動小数点） */
} Student;

/* 構造体ポインタを受け取る関数。
 * 「値渡し (by value)」にすると struct 全体がコピーされる（大きい struct では高コスト）。
 * ポインタで渡せばアドレス（8 bytes）のコピーだけで済む。
 * const Student* s: 「s が指す先を変更しない」ことをコンパイラに明示（読み取り専用参照）。
 * これは Rust の `fn print_student(s: &Student)` / C# の `in Student s` に相当する。 */
static void print_student(const Student* s) {
    /* NULL チェック: 無効なポインタを参照外しするとセグメンテーション違反 (SIGSEGV) になる */
    if (s == NULL) return;
    /* s->name は (*s).name のシンタックスシュガー
     * %.1f: 小数点以下 1 桁の浮動小数点表示 */
    printf("Student[ID=%d, Name=%s, Score=%.1f]\n", s->id, s->name, s->score);
}

/* ============================================================================
 * 関数ポインタ & コールバック
 * ============================================================================ */

/* typedef で関数ポインタ型を宣言する（可読性のため強く推奨）。
 * TransformFunc は「int を受け取り int を返す関数へのポインタ型」。
 * typedef なしで同じことを書くと: `int (*)(int)` という読みにくい表記になる。 */
typedef int (*TransformFunc)(int);

/* 変換関数の実体（関数ポインタ経由で呼ばれる）。
 * 関数名そのものはそのアドレスに評価されるため、& なしで関数ポインタ変数に代入できる。 */
static int square(int x)    { return x * x; }    /* 二乗 */
static int double_val(int x) { return x * 2; }   /* 2倍 */

/* 高階関数 (Higher-Order Function): 関数ポインタを引数に受け取る。
 * 配列の各要素に transform を適用してインプレース変換する（Rust の .iter_mut().map() 相当）。
 * transform(arr[i]): 関数ポインタ経由の呼び出し（通常の関数呼び出しと同じ構文で書ける）。 */
static void map_array(int* arr, size_t len, TransformFunc transform) {
    for (size_t i = 0; i < len; i++) {
        arr[i] = transform(arr[i]); /* 関数ポインタを介して動的に呼び出す関数を切り替える */
    }
}

/* ============================================================================
 * C言語で OOP（仮想関数テーブル VTable）を手動実装する
 * ============================================================================
 * C++ の virtual 関数は内部的に以下と同等のことをコンパイラが自動生成している。
 * Go の interface / Rust の dyn Trait も概念的には同じ「関数ポインタのテーブル」を使う。
 *
 * Greeter 構造体に「挨拶データ (greeting_word)」と
 * 「振る舞い（どう挨拶するかの関数ポインタ: greet）」を両方持たせることで、
 * 異なる実装を同じ型として扱えるポリモーフィズムを実現する。
 *
 * C++ で書けば:
 *   class Greeter {
 *   public:
 *       virtual void greet(const char* target) const = 0;
 *   };
 */
typedef struct Greeter {
    const char* greeting_word;
    /* greet: 「Greeter 自身へのポインタ」と「あいさつ相手の名前」を受け取る関数ポインタ。
     * self を渡す理由: C++の `this` ポインタに相当。
     * 関数ポインタが greeting_word などのフィールドを参照するために必要。 */
    void (*greet)(const struct Greeter* self, const char* target);
} Greeter;

/* Greeter の「英語版」実装 */
static void english_greet(const Greeter* self, const char* target) {
    /* self->greeting_word には "Hello" が格納されている */
    printf("%s, %s!\n", self->greeting_word, target);
}

/* Greeter の「フランス語版」実装 */
static void french_greet(const Greeter* self, const char* target) {
    /* self->greeting_word には "Bonjour" が格納されている */
    printf("%s, %s!\n", self->greeting_word, target);
}

/* ============================================================================
 * run_structs_and_functions_demo: デモ実行関数
 * ============================================================================ */
void run_structs_and_functions_demo(void) {

    /* ====================================================================
     * 1. 構造体のメモリレイアウト（パディング）
     * ==================================================================== */
    printf("--- 1. Structs and Memory Layout (Padding) ---\n");
    /* sizeof は配列やパディングを含む実際のメモリ占有サイズを返す（コンパイル時定数）。
     * 実行結果は環境・コンパイラ・アーキテクチャによって異なる場合がある。 */
    printf("InefficientLayout size: %zu bytes (char, int, char)\n", sizeof(InefficientLayout));
    printf("EfficientLayout size:   %zu bytes (int, char, char)\n",   sizeof(EfficientLayout));

    /* ====================================================================
     * 2. 構造体の操作と -> 演算子
     * ==================================================================== */
    printf("\n--- 2. Struct Operations and Arrow Operator (->) ---\n");

    /* スタック上に Student を構築（集成体初期化: フィールド順に値を並べる）。
     * name フィールドは char[32] なのでリテラル "Alice" がコピーされる（ポインタではない）。 */
    Student s1 = { 101, "Alice", 92.5 };
    print_student(&s1); /* &s1 で Student* を渡す */

    /* ヒープ上に構造体を確保する場合（動的なオブジェクト生成、Rust の Box::new(Student{...}) 相当）。
     * malloc は確保領域をゼロ初期化しないため、各フィールドを明示的に設定する必要がある。 */
    Student* s2 = (Student*)malloc(sizeof(Student));
    if (s2 != NULL) {
        s2->id = 102;
        /* 【重要】C言語の配列フィールドには直接代入できない（s2->name = "Bob" はコンパイルエラー）。
         * snprintf を使って安全に文字列をコピーする（詳細は strings_and_io.c を参照）。 */
        snprintf(s2->name, sizeof(s2->name), "Bob");
        s2->score = 88.0;
        print_student(s2);
        free(s2);       /* ヒープに確保した構造体は必ず free する */
        s2 = NULL;
    }

    /* ====================================================================
     * 3. 関数ポインタと高階関数（コールバック）
     * ==================================================================== */
    printf("\n--- 3. Function Pointers and Higher-Order Functions (Callbacks) ---\n");

    int data1[4] = { 1, 2, 3, 4 };
    /* square 関数のアドレスを TransformFunc 型として渡す。
     * map_array 内では transform(arr[i]) → square(arr[i]) として実行される。 */
    map_array(data1, 4, square);
    printf("After square: ");
    for (int i = 0; i < 4; i++) printf("%d ", data1[i]); /* 1, 4, 9, 16 */
    printf("\n");

    int data2[4] = { 1, 2, 3, 4 };
    /* 同じ map_array に double_val を渡すだけで、別の変換が適用できる（戦略パターン）。 */
    map_array(data2, 4, double_val);
    printf("After double: ");
    for (int i = 0; i < 4; i++) printf("%d ", data2[i]); /* 2, 4, 6, 8 */
    printf("\n");

    /* ====================================================================
     * 4. 関数ポインタによるポリモーフィズム（VTable エミュレーション）
     * ==================================================================== */
    printf("\n--- 4. Polymorphism via Function Pointers (VTable Emulation) ---\n");

    /* en_greeter と fr_greeter は同じ Greeter 型だが、greet フィールドに
     * 異なる関数ポインタが格納されている。 */
    Greeter en_greeter = { "Hello",   english_greet };
    Greeter fr_greeter = { "Bonjour", french_greet  };

    /* Greeter* の配列として扱うことで、C++ の基底クラスポインタ配列と同等の操作が可能。 */
    Greeter* greeters[2] = { &en_greeter, &fr_greeter };
    for (int i = 0; i < 2; i++) {
        /* greeters[i]->greet: 関数ポインタを取り出して呼び出す（仮想関数呼び出しの手動版）。
         * greeters[i] 自体を self として渡すことで、関数内から greeting_word にアクセスできる。 */
        greeters[i]->greet(greeters[i], "Haru");
    }
}
