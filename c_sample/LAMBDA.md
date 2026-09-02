# C言語 コールバック・擬似ラムダ・クロージャ 完全理解ガイド (C Lambdas & Closures Deep Dive)

C言語（C11 / C17 / C23）には、C++ や Java、Rust、C# のような言語組み込みの `[](){}` 構文（第一級のラムダ式）は**直接存在しません**。

しかし、**「なぜC言語にはないのか？」「現代の高水準言語のラムダ式は、マシン語やC言語レベルでどのように実現されているのか？」** を理解することは、全プログラマにとってラムダ式の本質を掴む最大の武器になります。

本ドキュメントでは、C言語における関数ポインタ、状態キャプチャ（`void* user_data` パターン）、高階関数、そしてコンパイラ拡張によるラムダ式を徹底解説します。

---

## 📑 目次

1. [なぜC言語にはラムダ式がないのか？（歴史と設計思想）](#1-なぜc言語にはラムダ式がないのか歴史と設計思想)
2. [基本：関数ポインタによる処理の受け渡し](#2-基本関数ポインタによる処理の受け渡し)
3. [最大の壁：「状態のキャプチャ」と `void* user_data` パターン](#3-最大の壁状態のキャプチャと-void-user_data-パターン)
4. [C言語で実装する「クロージャ（Closure）」の物理的実体](#4-c言語で実装するクロージャclosureの物理的実体)
5. [高階関数の自作 (`filter`, `map`, `for_each`)](#5-高階関数の自作-filter-map-for_each)
6. [コンパイラ拡張による「本物のラムダ式」 (Apple Blocks / GCC Nested Functions)](#6-コンパイラ拡張による本物のラムダ式-apple-blocks--gcc-nested-functions)
7. [C11 `_Generic` とプリプロセッサによる擬似ラムダ](#7-c11-_generic-とプリプロセッサによる擬似ラムダ)
8. [他言語のラムダ式はC言語レベルでどう動いているか？](#8-他言語のラムダ式はc言語レベルでどう動いているか)
9. [実務における重大な落とし穴とセキュリティ (ダングリングポインタ)](#9-実務における重大な落とし穴とセキュリティ-ダングリングポインタ)
10. [理解度チェッククイズ](#10-理解度チェッククイズ)

---

## 1. なぜC言語にはラムダ式がないのか？（歴史と設計思想）

### 1-1. C言語の基本哲学：「隠された処理をしない (No Hidden Mechanism)」
- C言語（1972年誕生）は、アセンブリ言語に代わる「薄い抽象化レイヤ」として設計されました。
- ラムダ式が外部のローカル変数をキャプチャする場合、言語ランタイムは裏でメモリ確保（ヒープ退避）や暗黙の構造体生成を行う必要があります。
- C言語は「プログラマが明示しないメモリ確保や型生成は一切行わない」という設計思想を持つため、言語コアに自動キャプチャ機能を含むラムダ式が存在しません。

### 1-2. しかし実務では「ラムダ式と同等の機能」が必須だった
OS のシグナル処理、GUI のイベントリスナー、マルチスレッド（`pthread`）、非同期I/O（`libuv`）、ソート（`qsort`）など、あらゆるC言語ライブラリで「振る舞い（コールバック）を渡す」必要がありました。
その解決策として確立されたのが、**「関数ポインタ」＋「コンテキストポインタ（`void*`）」** です。

---

## 2. 基本：関数ポインタによる処理の受け渡し

### 2-1. 関数ポインタの構文
C言語では、関数名はその関数のコード領域（テキストセグメント）上のメモリアドレスを指します。

```c
// 戻り値の型 (*変数名)(引数型リスト);
int (*operation)(int, int);

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

operation = add;
printf("10 + 20 = %d\n", operation(10, 20)); // 30

operation = multiply;
printf("10 * 20 = %d\n", operation(10, 20)); // 200
```

### 2-2. `typedef` による可読性の向上
関数ポインタの構文は複雑になりやすいため、実務では必ず `typedef`（または C23 の `using`）を使います。

```c
// BinaryOp という型名で「int を2つ取って int を返す関数ポインタ型」を定義
typedef int (*BinaryOp)(int, int);

// 高階関数（関数を受け取って実行する関数）
int execute(int a, int b, BinaryOp op) {
    return op(a, b);
}
```

### 2-3. 標準ライブラリ `qsort` の例
C言語標準の `qsort` は、比較ロジックを関数ポインタとして受け取る代表例です。

```c
int compare_ints(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

int numbers[] = { 42, 13, 99, 7 };
qsort(numbers, 4, sizeof(int), compare_ints);
```

---

## 3. 最大の壁：「状態のキャプチャ」と `void* user_data` パターン

### 3-1. 単純な関数ポインタの致命的弱点
単純な関数ポインタは、**「外側のローカル変数の値」を一切受け取ることができません**。

```c
void filter_numbers(int* arr, int size, int (*predicate)(int)) { ... }

void do_something(void) {
    int threshold = 50; // 外側のローカル変数
    
    // ❌ できない！
    // C言語にはローカル変数をキャプチャする構文がないため、
    // threshold を比較関数に渡す術がない（グローバル変数を使うとマルチスレッドで壊れる）
}
```

### 3-2. 世界標準の解決策: `(Callback, void* user_data)`
この問題を解決するため、POSIX、Win32 API、Linux カーネル、GUI ツールキットなど、**実務のすべての優れたC言語ライブラリは、必ずコールバックに関脈コンテキスト `void* user_data`（または `void* context`）をペアで渡します**。

```
呼び出し元 (Caller)                ライブラリ (Callee)
┌───────────────────────┐         ┌─────────────────────────────────┐
│ int threshold = 50;   │         │ void for_each(..., callback,    │
│                       │         │               void* user_data)  │
│ callback(item, &ctx)  │ ──────> │ {                               │
│                       │         │     callback(item, user_data);  │
│                       │         │ }                               │
└───────────────────────┘         └─────────────────────────────────┘
```

#### 実装例：コンテキスト付き述語関数
```c
// user_data を受け取れるコールバックの型定義
typedef bool (*PredicateWithContext)(int value, void* user_data);

// フィルタ関数
void filter_with_context(const int* src, int size, PredicateWithContext pred, void* user_data) {
    for (int i = 0; i < size; i++) {
        if (pred(src[i], user_data)) {
            printf("Matched: %d\n", src[i]);
        }
    }
}

// 述語関数の実装（user_data からコンテキストを復元する）
bool is_greater_than(int value, void* user_data) {
    int threshold = *(const int*)user_data; // キャプチャした変数を参照
    return value > threshold;
}

// 呼び出し側
int threshold = 30;
filter_with_context(numbers, 5, is_greater_than, &threshold);
```

> **💡 核心**: 高水準言語（Java, C++, Rust, C#）がラムダ式で「ローカル変数をキャプチャ」するとき、**裏のランタイムが自動生成しているのはまさにこの `void* user_data` の仕組み**です。

---

## 4. C言語で実装する「クロージャ（Closure）」の物理的実体

ラムダ式／クロージャの本質とは、**「関数ポインタ」＋「キャプチャされた環境（環境データ）」のペア**です。
C言語でこれを明示的なデータ構造として定義すると、以下のようになります。

```c
// クロージャ構造体
typedef struct {
    void (*invoke)(void* env, int arg); // 関数コード
    void* env;                           // キャプチャされた環境
} IntConsumerClosure;

// クロージャを呼び出すヘルパー
void invoke_closure(IntConsumerClosure* closure, int arg) {
    closure->invoke(closure->env, arg);
}
```

### キャプチャ環境の作成例
```c
// キャプチャしたい環境構造体
typedef struct {
    const char* prefix;
    int multiplier;
} PrintEnv;

// 実行ロジック
void print_logic(void* raw_env, int val) {
    PrintEnv* env = (PrintEnv*)raw_env;
    printf("%s: %d\n", env->prefix, val * env->multiplier);
}

// 使用
PrintEnv env = { .prefix = "Result", .multiplier = 10 };
IntConsumerClosure my_lambda = {
    .invoke = print_logic,
    .env = &env
};

invoke_closure(&my_lambda, 5); // "Result: 50" と出力
```

---

## 5. 高階関数の自作 (`filter`, `map`, `for_each`)

C言語でも、コンテキスト付きコールバックを使えば Stream API や LINQ のようなパイプライン処理を構築できます。

```c
typedef void (*ConsumerFn)(int item, void* ctx);
typedef int  (*MapFn)(int item, void* ctx);
typedef bool (*FilterFn)(int item, void* ctx);

// for_each 実装
void c_for_each(const int* arr, size_t len, ConsumerFn action, void* ctx) {
    for (size_t i = 0; i < len; ++i) {
        action(arr[i], ctx);
    }
}

// map 実装
void c_map(const int* src, int* dst, size_t len, MapFn mapper, void* ctx) {
    for (size_t i = 0; i < len; ++i) {
        dst[i] = mapper(src[i], ctx);
    }
}
```

---

## 6. コンパイラ拡張による「本物のラムダ式」

標準C規格外ですが、主要コンパイラにはラムダ式を記述できる拡張機能が存在します。

### 6-1. Clang / Apple Blocks 拡張（`^` 構文）
macOS / iOS の Objective-C や C/C++ で使われる拡張です。`-fblocks` オプションで有効化できます。

```c
// Apple Blocks 記法 (キャプチャ可能)
int multiplier = 3;
int (^my_block)(int) = ^int(int x) {
    return x * multiplier; // 外部変数を直接キャプチャ可能！
};

printf("%d\n", my_block(10)); // 30
```
- 外側の変数を書き換えるには `__block int count = 0;` のように `__block` ストレージ修飾子を付与します。

### 6-2. GCC Nested Functions（ネストした関数）
GCC 独自拡張として、関数の中に関数を宣言できます。

```c
void foo(void) {
    int offset = 100;
    
    // GCCネスト関数（外側の offset を暗黙キャプチャ）
    int add_offset(int x) {
        return x + offset;
    }
    
    printf("%d\n", add_offset(5)); // 105
}
```
> **⚠️ セキュリティ警告（DEP / NXビット）**:  
> GCC のネストした関数ポインタを外部に渡す場合、GCC はスタック上に**トランポリンコード（実行可能マシン語）**を動的生成します。これは現代のセキュリティ機構（DEP: Data Execution Prevention / W^X: 書き込み可能かつ実行可能なスタックの禁止）に違反するため、実務のプロダクションコードでは**絶対に使用してはいけません**。

---

## 7. C11 `_Generic` とプリプロセッサによる擬似ラムダ

C11 で導入された `_Generic`（コンパイル時型分岐）とステートメント式（GCC/Clang拡張 `({...})`）を組み合わせることで、擬似的な即時無名処理をマクロ化するテクニックがあります。

```c
// ステートメント式 ({ ... }) によるインライン評価マクロ
#define LAMBDA(return_type, body_and_args) \
    ({ return_type __fn__ body_and_args __fn__; })

// 使用例 (GCC/Clang環境)
int (*square)(int) = LAMBDA(int, (int x) { return x * x; });
```

---

## 8. 他言語のラムダ式はC言語レベルでどう動いているか？

| 言語 | ラムダ式の正体 | C言語で書いた場合の実装実体 | メモリ確保 |
| :--- | :--- | :--- | :--- |
| **C言語 (標準)** | 関数ポインタ + `void*` | `struct { void (*fn)(void*, ...); void* env; }` | プログラマが明示的に管理 |
| **C++** | 無名クラスの `operator()` | キャプチャ変数をメンバに持つ `struct __lambda { ... }` | スタック（ゼロオーバーヘッド） |
| **Rust** | `Fn` / `FnMut` / `FnOnce` | キャプチャ変数を保持する `struct`（トレイトオブジェクト時は fat pointer）| スタックまたは `Box` |
| **Java** | `invokedynamic` + SAM | 初回のみ `LambdaMetafactory` で無名クラスのリンケージを動的生成 | ヒープ（JVM管理） |
| **Go / JS** | 第一級クロージャ | 関数ポインタ + キャプチャ環境ヒープポインタ（Fat Pointer） | エスケープ解析によりヒープ退避 |

---

## 9. 実務における重大な落とし穴とセキュリティ (ダングリングポインタ)

C言語でコールバックとコンテキストを使う際、最も致命的なバグが **スタック変数のダングリングポインタ（Use-After-Free）** です。

```c
// ❌ 致命的なバグ：非同期処理にローカルスタックのアドレスを渡してしまう
void setup_async_task(void) {
    int temporary_id = 999;
    
    // 別スレッドで遅れて実行される処理に関脈アドレス &temporary_id を渡す
    start_background_thread(worker_callback, &temporary_id);
    
} // ← ここで setup_async_task のスタックフレームが破棄される！
  //   worker_callback が呼ばれる頃には temporary_id のメモリはゴミデータに！
```

### 正しい対策（ヒープ確保とライフサイクル管理）
```c
// ⭕ 安全なパターン：コンテキストをヒープに malloc し、コールバック完了時に free する
typedef struct {
    int id;
} TaskContext;

void worker_callback(void* user_data) {
    TaskContext* ctx = (TaskContext*)user_data;
    printf("Processing ID: %d\n", ctx->id);
    
    // 責任を持って解放する
    free(ctx);
}

void setup_async_task_safe(void) {
    TaskContext* ctx = (TaskContext*)malloc(sizeof(TaskContext));
    ctx->id = 999;
    start_background_thread(worker_callback, ctx);
}
```

---

## 10. 理解度チェッククイズ

### Q1. C言語の標準規格（C11/C17/C23）において、ラムダ式に最も近い標準機能は何ですか？
- A. `lambda` キーワード
- B. 関数ポインタとコンテキストポインタ（`void* user_data`）の組み合わせ
- C. アロー演算子 `->`
- D. マクロ `#define`

<details>
<summary>▶ 解答と解説</summary>

**正解: B**
C言語には直接のラムダ構文はありませんが、関数ポインタに関脈データポインタ（`void*`）を添えることで、他言語のクロージャと全く同じ状態保持型コールバックを実現します。
</details>

### Q2. なぜ非同期コールバックの `user_data` にローカル変数のアドレス `&x` を渡してはいけないのですか？
<details>
<summary>▶ 解答と解説</summary>

**解答**: 関数がリターンした瞬間にローカル変数のスタックフレームが解放（消滅）するため、後から別スレッドでコールバックが実行された際に不正なメモリアクセス（ダングリングポインタ / 未定義動作）を引き起こすからです。非同期コールバックでは `malloc` したヒープメモリを渡す必要があります。
</details>

---

## まとめ

1. **C言語のコールバックは「関数ポインタ」＋「`void* user_data`」が鉄則**。
2. **クロージャの物理的本質は「関数ポインタと構造体ポインタのペア」である**。
3. **高水準言語が隠蔽しているメモリのライフサイクル（スタック vs ヒープ）を意識することが、安全なシステムプログラミングの基礎となる**。
