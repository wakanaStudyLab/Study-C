# C Language Crash Course (For Rust, C#, Go Developers) - C11/C23 Edition

Rust, C#, Go 経験者向けに特化した、**C言語のコア概念（ポインタ、メモリレイアウト、手動メモリ管理、関数ポインタ、ビット演算、X-Macros、侵入型データ構造）最速習得ガイド**です。
低レイヤ・システムプログラミングの土台となる全9モジュールを網羅しています。

---

## 🚀 CMake でのビルド & 実行方法

ルートディレクトリ（`..`）から CMake で一括ビルド・実行できます：

```powershell
cd ..

# 構成 & ビルド
cmake -B build -S .
cmake --build build --config Release --target c_sample

# 実行
.\build\bin\Release\c_sample.exe  # または .\build\bin\c_sample.exe (Ninja使用時)
```

---

## 🗺️ 言語対比マッピング早見表 (C vs Rust vs Go vs C#)

| 概念・機能 | C言語 | Rust | Go | C# |
| :--- | :--- | :--- | :--- | :--- |
| **メモリアドレス取得** | `&x` | `&x` | `&x` | `fixed` / `unsafe { &x }` |
| **ポインタ参照外し** | `*p` | `*p` (`unsafe`時) / 参照 | `*p` | `*p` (`unsafe`時) |
| **ヒープメモリ確保** | `malloc(size)` | `Box::new(val)` | `new(Type)` / `make` | `new Class()` |
| **メモリ再確保** | `realloc(ptr, new_sz)` | `Vec::reserve` | スライスの自動拡張 | `Array.Resize` |
| **メモリ解放** | `free(ptr)` | `Drop` による自動解放 | GC による自動回収 | GC による自動回収 |
| **多段階リソース解放** | `goto cleanup;` | `Drop` の逆順自動解放 | `defer` | `using` / `try-finally` |
| **構造体** | `typedef struct { ... } T;`| `struct T { ... }` | `type T struct { ... }`| `struct T { ... }` |
| **メンバアクセス** | 実体: `.`, ポインタ: `->` | `.` (自動参照外し) | `.` (自動参照外し) | `.` |
| **ビットフィールド** | `unsigned int flag : 1;` | `bitflags` crate | 手動シフト/マスク | 手動シフト/マスク |
| **文字列型** | `char*` / `char[]` (ヌル終端 `\0`) | `&str` / `String` | `string` | `string` |
| **関数ポインタ / コールバック** | `void (*fn)(int)` | `fn(i32)` / `impl Fn(i32)` | `func(int)` | `Action<int>` / `delegate` |
| **状態キャプチャ** | `void* user_data` イディオム | クロージャキャプチャ環境 | クロージャ環境 | ラムダ式キャプチャ |
| **型ディスパッチ** | `_Generic` (C11) | トレイト解決 | 型アサーション | メソッドオーバーロード |
| **非局所大域ジャンプ** | `setjmp` / `longjmp` | `panic!` / `catch_unwind`| `panic` / `recover` | `throw` / `catch` |
| **侵入型リスト復元** | `container_of` マクロ | なし (スマートポインタ) | なし | なし |

---

## 📁 全9モジュール構成一覧

| # | ヘッダー / 実装 | 主な学習トピック |
| :-: | :--- | :--- |
| **01** | [`pointers_and_memory.h`](./c_sample/include/pointers_and_memory.h)<br>[`pointers_and_memory.c`](./c_sample/src/pointers_and_memory.c) | ポインタの基礎、アドレス演算子 (`&`)、ポインタ演算 (`*(arr + i)`)、`malloc` / `free`、二重ポインタ (`int**`) 出力引数パターン |
| **02** | [`structs_and_functions.h`](./c_sample/include/structs_and_functions.h)<br>[`structs_and_functions.c`](./c_sample/src/structs_and_functions.c) | 構造体メモリレイアウト、パディング・アライメント (`sizeof`)、アロー演算子 (`->`)、関数ポインタによるコールバックと OOP 模倣 (VTable) |
| **03** | [`strings_and_io.h`](./c_sample/include/strings_and_io.h)<br>[`strings_and_io.c`](./c_sample/src/strings_and_io.c) | ヌル終端文字列 (`\0`)、`strlen` の計算量、サイズ指定付き安全関数 (`snprintf`)、`FILE*` によるファイル書き込み・読み込み |
| **04** | [`lambda_and_callbacks.h`](./c_sample/include/lambda_and_callbacks.h)<br>[`lambda_and_callbacks.c`](./c_sample/src/lambda_and_callbacks.c) | ステートレス関数ポインタ、`void* user_data` による状態キャプチャ（クロージャの物理的実体）、ヒープ確保による非同期ライフサイクル管理 |
| **05** | [`modern_c11_features.h`](./c_sample/include/modern_c11_features.h)<br>[`modern_c11_features.c`](./c_sample/src/modern_c11_features.c) | C11 `_Generic` 型ディスパッチ (疑似オーバーロード)、匿名構造体 & 匿名共用体、フレキシブル配列メンバ (可変長パケット単一アロケーション) |
| **06** | [`bitwise_and_binary.h`](./c_sample/include/bitwise_and_binary.h)<br>[`bitwise_and_binary.c`](./c_sample/src/bitwise_and_binary.c) | ビットマスクによるフラグ制御 (AND/OR/XOR/NOT)、**ビットフィールド** による省メモリ設計、**エンディアン判定とバイトスワップ**、HEXダンプ |
| **07** | [`preprocessor_and_macros.h`](./c_sample/include/preprocessor_and_macros.h)<br>[`preprocessor_and_macros.c`](./c_sample/src/preprocessor_and_macros.c) | `do-while(0)` 安全マクロ、文字列化 (`#`) とトークン連結 (`##`)、**X-Macros** (Enum と文字列を単一テーブルから自動生成する最高峰イディオム)、`_Static_assert` |
| **08** | [`error_handling_and_jump.h`](./c_sample/include/error_handling_and_jump.h)<br>[`error_handling_and_jump.c`](./c_sample/src/error_handling_and_jump.c) | Linux カーネル標準 **`goto cleanup;`** イディオム (確実な逆順リソースクリーンアップ)、`errno` / `strerror`、**`setjmp` / `longjmp`** (例外エミュレーション) |
| **09** | [`dynamic_data_structures.h`](./c_sample/include/dynamic_data_structures.h)<br>[`dynamic_data_structures.c`](./c_sample/src/dynamic_data_structures.c) | 容量倍増 (Doubling) による動的配列 (**Vector** 実装と `realloc` の安全な作法)、Linux カーネルスタイル侵入型リスト (**`container_of`** マクロの解剖) |

- [`src/main.c`](./c_sample/src/main.c): 全9モジュールを順序よく実演する統合エントリーポイント

> 📖 **詳細ガイドドキュメント**:
> - [**`POINTERS.md`**](./c_sample/POINTERS.md): ポインタ・アドレス演算の完全図解
> - [**`LAMBDA.md`**](./c_sample/LAMBDA.md): C言語による高階関数・クロージャ完全理解ガイド
