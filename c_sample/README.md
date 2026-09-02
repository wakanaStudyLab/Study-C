# C Language Crash Course (For Rust, C#, Go Developers)

Rust, C#, Go 経験者向けに特化した、**C言語のコア概念（ポインタ、メモリレイアウト、手動メモリ管理、関数ポインタ）最速習得ガイド**です。

---

## 🚀 CMake でのビルド & 実行方法

ルートディレクトリ（`C:\Users\harun\programming\C++`）から CMake で一括ビルドできます：

```powershell
cd C:\Users\harun\programming\C++

# 構成 & ビルド
cmake -B build -S .
cmake --build build --config Release --target c_sample

# 実行
.\build\bin\Release\c_sample.exe
```

---

## 🗺️ 言語対比マッピング早見表 (C vs Rust vs Go vs C#)

| 概念・機能 | C言語 | Rust | Go | C# |
| :--- | :--- | :--- | :--- | :--- |
| **メモリアドレス取得** | `&x` | `&x` | `&x` | `fixed` / `unsafe { &x }` |
| **ポインタ参照外し** | `*p` | `*p` (`unsafe`時) / 参照 | `*p` | `*p` (`unsafe`時) |
| **ヒープメモリ確保** | `malloc(size)` | `Box::new(val)` | `new(Type)` / `make` | `new Class()` |
| **メモリ解放** | `free(ptr)` | `Drop` による自動解放 | GC による自動回収 | GC による自動回収 |
| **構造体** | `typedef struct { ... } T;`| `struct T { ... }` | `type T struct { ... }`| `struct T { ... }` |
| **メンバアクセス** | 実体: `.`, ポインタ: `->` | `.` (自動参照外し) | `.` (自動参照外し) | `.` |
| **文字列型** | `char*` / `char[]` (ヌル終端 `\0`) | `&str` / `String` | `string` | `string` |
| **関数ポインタ / コールバック** | `void (*fn)(int)` | `fn(i32)` / `impl Fn(i32)` | `func(int)` | `Action<int>` / `delegate` |
| **エラーハンドリング** | 戻り値 + `errno` | `Result<T, E>` | `(T, error)` | `try-catch` |
| **ポリモーフィズム** | 構造体 + 関数ポインタ | `trait` (vtable) | `interface` (itab) | `interface` / `virtual` |

---

## 📁 ソースファイル構成

- [`include/pointers_and_memory.h`](file:///C:/Users/harun/programming/C++/sample/c_sample/include/pointers_and_memory.h) / [`src/pointers_and_memory.c`](file:///C:/Users/harun/programming/C++/sample/c_sample/src/pointers_and_memory.c): ポインタ演算・動的確保・多重ポインタ
- [`include/structs_and_functions.h`](file:///C:/Users/harun/programming/C++/sample/c_sample/include/structs_and_functions.h) / [`src/structs_and_functions.c`](file:///C:/Users/harun/programming/C++/sample/c_sample/src/structs_and_functions.c): パディング・アライメント・関数ポインタ
- [`include/strings_and_io.h`](file:///C:/Users/harun/programming/C++/sample/c_sample/include/strings_and_io.h) / [`src/strings_and_io.c`](file:///C:/Users/harun/programming/C++/sample/c_sample/src/strings_and_io.c): ヌル終端文字列・snprintf・ファイルI/O
- [`include/lambda_and_callbacks.h`](file:///C:/Users/harun/programming/C++/sample/c_sample/include/lambda_and_callbacks.h) / [`src/lambda_and_callbacks.c`](file:///C:/Users/harun/programming/C++/sample/c_sample/src/lambda_and_callbacks.c): コールバック・void*状態キャプチャ・クロージャ構造体
- [`src/main.c`](file:///C:/Users/harun/programming/C++/sample/c_sample/src/main.c): 統合エントリーポイント

> 📖 **C言語におけるラムダ式・クロージャの深層理解**:  
> C言語で状態キャプチャや高階関数、クロージャをどう実現するかの完全解説は [**`LAMBDA.md`**](file:///C:/Users/harun/programming/C++/sample/c_sample/LAMBDA.md) を参照してください。現代の高水準言語が裏で生成しているマシン語の物理的実体を学べます。
