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
 * 1. 構造体 (struct):
 *    - C#の struct, Goの struct, Rustの struct の元祖。
 *    - `typedef struct { ... } TypeName;` で型エイリアスを定義するのが慣例。
 *    - メンバアクセス: 実体には `.` (dot)、ポインタには `->` (arrow) を使用。
 * 
 * 2. メモリレイアウトとパディング (Padding & Alignment):
 *    - CPUのメモリアクセス効率のため、メンバ間に見えない隙間（パディング）が挿入されます。
 *    - フィールドの並び順によって `sizeof(Struct)` が変わるため、大きな型から順に並べるのがコツ。
 * 
 * 3. 関数ポインタ (Function Pointer):
 *    - 関数の実行コードがあるメモリアドレスを保持するポインタ。
 *    - C#のデリゲート、Goの `func(int) string` 型、Rustの `fn(i32) -> String` に相当。
 *    - コールバック関数や、構造体に関数ポインタを持たせることで「ポリモーフィズム / インターフェース」を実現可能。
 */

// --- 構造体の定義とパディングの検証 ---
typedef struct {
    char a;     // 1 byte
    // 3 bytes padding (int の 4 byte 境界に合わせるため)
    int b;      // 4 bytes
    char c;     // 1 byte
    // 3 bytes padding
} InefficientLayout; // 合計 12 bytes!

typedef struct {
    int b;      // 4 bytes
    char a;     // 1 byte
    char c;     // 1 byte
    // 2 bytes padding
} EfficientLayout;   // 合計 8 bytes!

// --- 実務構造体 ---
typedef struct {
    int id;
    char name[32];
    double score;
} Student;

// 構造体ポインタを受け取る関数 (ポインタ渡しでコピーコストをゼロにする / C#の in ref, Rustの &Student)
static void print_student(const Student* s) {
    if (s == NULL) return;
    // s->name は (*s).name のシンタックスシュガー
    printf("Student[ID=%d, Name=%s, Score=%.1f]\n", s->id, s->name, s->score);
}

// ============================================================================
// 関数ポインタ & コールバック
// ============================================================================
// typedef で関数ポインタ型を宣言: 「int を受け取り int を返す関数へのポインタ」
typedef int (*TransformFunc)(int);

static int square(int x) { return x * x; }
static int double_val(int x) { return x * 2; }

// 高階関数 (Higher-Order Function): 関数ポインタを受け取って配列各要素を変換 (Rustの .map 相当)
static void map_array(int* arr, size_t len, TransformFunc transform) {
    for (size_t i = 0; i < len; i++) {
        arr[i] = transform(arr[i]); // 関数ポインタ経由で呼び出し
    }
}

// ============================================================================
// C言語でのオブジェクト指向（仮想関数テーブル VTable の手動実装）
// Linux カーネルや組み込み、Goの interface 内部表現の原型
// ============================================================================
typedef struct Greeter {
    const char* greeting_word;
    void (*greet)(const struct Greeter* self, const char* target);
} Greeter;

static void english_greet(const Greeter* self, const char* target) {
    printf("%s, %s!\n", self->greeting_word, target);
}

static void french_greet(const Greeter* self, const char* target) {
    printf("%s, %s!\n", self->greeting_word, target);
}

void run_structs_and_functions_demo(void) {
    printf("--- 1. Structs and Memory Layout (Padding) ---\n");
    printf("InefficientLayout size: %zu bytes (char, int, char)\n", sizeof(InefficientLayout));
    printf("EfficientLayout size:   %zu bytes (int, char, char)\n", sizeof(EfficientLayout));

    printf("\n--- 2. Struct Operations and Arrow Operator (->) ---\n");
    Student s1 = { 101, "Alice", 92.5 };
    print_student(&s1);

    // ヒープ上に構造体を確保
    Student* s2 = (Student*)malloc(sizeof(Student));
    if (s2 != NULL) {
        s2->id = 102;
        // 文字列の代入は strcpy_s や snprintf を使う (C言語では配列への直接代入 s2->name = "Bob" は不可)
        snprintf(s2->name, sizeof(s2->name), "Bob");
        s2->score = 88.0;
        print_student(s2);
        free(s2);
        s2 = NULL;
    }

    printf("\n--- 3. Function Pointers and Higher-Order Functions (Callbacks) ---\n");
    int data1[4] = { 1, 2, 3, 4 };
    map_array(data1, 4, square);
    printf("After square: ");
    for (int i = 0; i < 4; i++) printf("%d ", data1[i]);
    printf("\n");

    int data2[4] = { 1, 2, 3, 4 };
    map_array(data2, 4, double_val);
    printf("After double: ");
    for (int i = 0; i < 4; i++) printf("%d ", data2[i]);
    printf("\n");

    printf("\n--- 4. Polymorphism via Function Pointers (VTable Emulation) ---\n");
    Greeter en_greeter = { "Hello", english_greet };
    Greeter fr_greeter = { "Bonjour", french_greet };

    Greeter* greeters[2] = { &en_greeter, &fr_greeter };
    for (int i = 0; i < 2; i++) {
        greeters[i]->greet(greeters[i], "Haru");
    }
}
