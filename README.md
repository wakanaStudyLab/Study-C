# C & C++ Crash Course (For Rust, C#, Go Developers) - CMake Edition

Rust, C#, Go などの静的型付け言語を理解しているエンジニアが、**最短で C言語 および Modern C++ (C++20/C++23) をマスターするための体系的なサンプルコードと解説集**です。

---

## 📂 プロジェクト全体構成 (Directory Structure)

```text
sample/
├── CMakeLists.txt                    # ルート CMake 設定 (C/C++ プロジェクト一括管理)
│
├── c_sample\                         # 【C言語 最速習得コース (全9モジュール)】
│   ├── CMakeLists.txt                # C言語ターゲット設定 (C11標準)
│   ├── include\                      # ヘッダーファイル
│   │   ├── pointers_and_memory.h     # 01: ポインタ・アドレス演算・malloc/free・多重ポインタ
│   │   ├── structs_and_functions.h   # 02: 構造体・パディング・関数ポインタ・OOP模倣
│   │   ├── strings_and_io.h          # 03: ヌル終端文字列・snprintf・ファイルI/O
│   │   ├── lambda_and_callbacks.h    # 04: 関数ポインタ・クロージャ模倣・void*状態キャプチャ
│   │   ├── modern_c11_features.h     # 05: C11 _Generic型分岐・匿名構造体/共用体・可変長配列メンバ
│   │   ├── bitwise_and_binary.h      # 06: ビット演算・ビットフィールド・エンディアン・バイトスワップ
│   │   ├── preprocessor_and_macros.h # 07: 安全マクロ(do-while)・文字列化(#)・連結(##)・X-Macros
│   │   ├── error_handling_and_jump.h # 08: goto cleanup イディオム・errno・setjmp/longjmp
│   │   └── dynamic_data_structures.h # 09: 動的配列(Vector)・Linux侵入型リスト(container_of)
│   ├── src\                          # C ソースコード
│   │   ├── pointers_and_memory.c
│   │   ├── structs_and_functions.c
│   │   ├── strings_and_io.c
│   │   ├── lambda_and_callbacks.c
│   │   ├── modern_c11_features.c
│   │   ├── bitwise_and_binary.c
│   │   ├── preprocessor_and_macros.c
│   │   ├── error_handling_and_jump.c
│   │   ├── dynamic_data_structures.c
│   │   └── main.c                    # C言語 統合エントリーポイント
│   ├── POINTERS.md                   # ポインタを完全に理解できるかもしれないファイル
│   ├── LAMBDA.md                     # C言語による高階関数・クロージャ完全理解ガイド
│   └── README.md                     # C言語 詳細チートシート & 言語対比表
│
├── cpp_sample\                       # 【Modern C++ (C++20/C++23) 最速習得コース (全13モジュール)】
│   ├── CMakeLists.txt                # C++ターゲット設定 (C++23標準)
│   ├── include\                      # ヘッダーファイル
│   │   ├── raii_and_smart_pointers.hpp    # 01: RAII・unique_ptr / shared_ptr (RustのBox/Arc相当)
│   │   ├── move_semantics_and_classes.hpp # 02: 移動セマンティクス (std::move)・Rule of 5/0・virtual
│   │   ├── stl_and_ranges.hpp             # 03: STL・string_view・C++20 Ranges (LINQ/Iterator相当)
│   │   ├── templates_and_modern_types.hpp # 04: C++20 Concepts・optional・variant (Rustのenum/match相当)
│   │   ├── concurrency.hpp                # 05: 非同期 future・C++20 jthread・atomic
│   │   ├── lambda_expressions.hpp         # 06: ラムダ式 (キャプチャ・mutable・ムーブ・C++20テンプレート)
│   │   ├── coroutines_and_modern_cpp.hpp  # 07: C++20 Coroutines (co_yield)・consteval・std::span・std::format
│   │   ├── error_handling_and_expected.hpp# 08: C++23 std::expected (Result型)・モナディック操作・noexcept
│   │   ├── advanced_templates_and_metaprogramming.hpp # 09: Fold式・if constexpr・CRTP(静的ポリモーフィズム)
│   │   ├── operator_overloading_and_spaceship.hpp     # 10: 演算子オーバーロード・宇宙船演算子(<=>)・自作Iterator
│   │   ├── memory_and_low_level.hpp       # 11: weak_ptr循環解決・std::pmr(バンプアロケータ)・bit_cast・alignas
│   │   ├── filesystem_and_io.hpp          # 12: std::filesystem・バイナリI/O・C++23 std::print/println
│   │   └── advanced_concurrency.hpp       # 13: std::scoped_lock・CondVar・counting_semaphore・std::latch
│   ├── src\                          # C++ ソースコード
│   │   ├── raii_and_smart_pointers.cpp
│   │   ├── move_semantics_and_classes.cpp
│   │   ├── stl_and_ranges.cpp
│   │   ├── templates_and_modern_types.cpp
│   │   ├── concurrency.cpp
│   │   ├── lambda_expressions.cpp
│   │   ├── coroutines_and_modern_cpp.cpp
│   │   ├── error_handling_and_expected.cpp
│   │   ├── advanced_templates_and_metaprogramming.cpp
│   │   ├── operator_overloading_and_spaceship.cpp
│   │   ├── memory_and_low_level.cpp
│   │   ├── filesystem_and_io.cpp
│   │   ├── advanced_concurrency.cpp
│   │   └── main.cpp                  # C++ 統合エントリーポイント
│   ├── LAMBDA.md                     # Modern C++ ラムダ式完全理解ガイド
│   └── README.md                     # C++ 詳細チートシート & 言語対比表
│
├── build_and_run.ps1                 # CMake 構成・ビルド・実行一括スクリプト
├── CMAKE_GUIDE.md                    # 📘 Modern CMake 完全マスターガイド（構文・作法・設計思想）
└── README.md                         # 本ファイル
```

---

## 🛠️ ビルドに必要な環境・要件 (Prerequisites)

本環境では、**Clang (`clang.exe` / `clang++.exe`) + Ninja** を用いた高速ビルドおよび **MSVC (cppvsdbg)** によるデバッグ構成を採用しています。

### 1. 必要なツール一覧

| ツール | 用途 | インストール先（本環境例） | 入手先 / 導入方法 |
| :--- | :--- | :--- | :--- |
| **LLVM / Clang**<br>(`clang.exe`, `clang++.exe`) | C/C++ コンパイラ<br>(標準 GNU 互換インターフェース) | `C:\Program Files\LLVM\bin\` | [LLVM Releases (GitHub)](https://github.com/llvm/llvm-project/releases)<br>（Windows 64bit用インストーラ） |
| **Ninja**<br>(`ninja.exe`) | 高速ビルドジェネレータ | `C:\ninja\ninja.exe` | [Ninja Releases (GitHub)](https://github.com/ninja-build/ninja/releases)<br>（実行ファイルを配置） |
| **CMake**<br>(`cmake.exe`) | ビルド構成生成ツール | `C:\Program Files\CMake\bin\` | [cmake.org (Previous Releases)](https://cmake.org/download/#previous) |
| **Windows SDK**<br>(`rc.exe`) | リソースコンパイラ | `C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\` | Visual Studio Installer または<br>[Windows SDK 公式](https://developer.microsoft.com/ja-jp/windows/downloads/windows-sdk/) |
| **MSVC CRT**<br>(C/C++ Build Tools) | C/C++ ランタイムライブラリ<br>(Clang がリンク) | Visual Studio インストール配下 | [Visual Studio Build Tools](https://visualstudio.microsoft.com/ja/downloads/)<br>「C++ によるデスクトップ開発」 |

### 2. VS Code 推奨拡張機能

| 拡張機能 | 識別子 | 用途 |
| :--- | :--- | :--- |
| **C/C++** | `ms-vscode.cpptools` | IntelliSense、シンタックスハイライト、デバッグ (`cppvsdbg`) |
| **CMake Tools** | `ms-vscode.cmake-tools` | VS Code 統合 CMake ビルド & テスト |

---

## 🚀 CMake でのビルド & 実行方法

### 1. PowerShell スクリプトで一括実行 (最も簡単)
```powershell
.\build_and_run.ps1
```

### 2. コマンドラインでの手動ビルド (Ninja + Clang)
```powershell
# 1. 構成 (Configure)
cmake -B build -S . -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

# 2. ビルド (Build)
cmake --build build

# 3. 実行 (Run)
.\build\bin\c_sample.exe
.\build\bin\cpp_sample.exe
```

---

## 🗺️ 言語対比マッピング早見表 (C vs Modern C++ vs Rust vs C# vs Go)

| 概念・機能 | C言語 | Modern C++ (C++20/23) | Rust | C# | Go |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **メモリ管理** | 手動 (`malloc`/`free`) | **RAII** (自動デストラクタ) | `Drop` トレイト | GC | GC |
| **単一所有ヒープ** | 生ポインタ `T*` | `std::unique_ptr<T>` | `Box<T>` | 参照型 | ポインタ |
| **共有所有ヒープ** | 参照カウント自作 | `std::shared_ptr<T>` | `Arc<T>` / `Rc<T>` | GC | GC |
| **弱参照 (循環防止)**| なし | `std::weak_ptr<T>` | `Weak<T>` | `WeakReference<T>`| なし |
| **所有権の移動** | なし (手動すげ替え) | `std::move(x)` (右辺値参照 `&&`) | デフォルトで Move | なし | なし |
| **ゼロコピー文字列** | `const char*` + 長さ | `std::string_view` | `&str` | `ReadOnlySpan<char>`| `string` |
| **遅延パイプライン** | 手動ループ | **Ranges** (`views::filter \| ...`)| `.iter().filter().map()` | `.Where().Select()` | 手動ループ / slices |
| **型制約/ジェネリクス**| なし (`void*` / マクロ) | **Concepts** (`requires`) | `trait bounds` | `where T : ...` | `[T constraints.Ordered]`|
| **Result型エラー処理**| 戻り値 + `errno` | **`std::expected<T, E>`** | `Result<T, E>` | なし (例外) | `(T, error)` |
| **三方比較演算子** | なし | **宇宙船演算子 `<=>`** | `PartialOrd` / `Ord` | `IComparable<T>` | `cmp.Compare` |
| **代数的データ型 (ADT)**| `union` + tag | `std::variant<A, B>` | `enum Name { A, B }` | `abstract record` | interface + type switch |
| **パターンマッチング**| `switch` (整数のみ) | `std::visit(Overloaded{...}, v)` | `match v { ... }` | `v switch { ... }` | `switch v.(type)` |
| **高速I/O** | `printf` (型危険) | **`std::println`** (高速・型安全) | `println!` | `Console.WriteLine` | `fmt.Println` |
| **自動joinスレッド** | なし | `std::jthread` (C++20) | `std::thread::spawn` | `Task` | `go func()` |
| **複数デッドロック回避**| なし | `std::scoped_lock` (C++17) | ロック順序手動制御 | `lock(a) { lock(b) }`| `sync.Mutex` |
| **カウントダウン同期** | なし | `std::latch` (C++20) | `Barrier` | `CountdownEvent` | `sync.WaitGroup` |
| **コルーチン** | なし | `co_yield` (C++20) | `gen` / `async-stream` | `yield return` | goroutine + chan |
| **侵入型リスト復元** | **`container_of`** | なし | なし | なし | なし |
| **メタプログラミング** | **X-Macros** | **Fold式** / `if constexpr` / **CRTP** | マクロ / トレイト | リフレクション / ジェネリクス | リフレクション |

---

## 📚 推薦学習ロードマップ

1. **第1段階: C言語の基礎と低レイヤの解剖 (`c_sample`)**
   - ポインタ、メモリレイアウト、パディング、アロー演算子
   - 関数ポインタとコールバック、クロージャの物理的実体 (`void* user_data`)
   - ビット演算、ビットフィールド、エンディアン判定
   - 安全マクロ、X-Macros、Linux カーネルの `goto cleanup` & `container_of`
2. **第2段階: Modern C++ への昇華 (`cpp_sample` 01〜06)**
   - 生ポインタを捨て、RAII とスマートポインタ（`unique_ptr` / `shared_ptr` / `weak_ptr`）へ
   - コピーからゼロコストのムーブセマンティクス（右辺値参照、`std::move`）へ
   - STL コンテナと C++20 Ranges パイプライン
   - C++20 Concepts と代数的データ型（`optional` / `variant` / `visit`）
   - ラムダ式の完全攻略（キャプチャ、ムーブ、テンプレートラムダ、IIFE）
3. **第3段階: 最先端 Modern C++ (C++20/C++23) と実務アーキテクチャ (`cpp_sample` 07〜13)**
   - コルーチン (`co_yield` ジェネレータ)、`consteval` コンパイル時計算、`std::span`
   - C++23 `std::expected` によるモナディックエラーハンドリング
   - 可変引数テンプレート、C++17 畳み込み式 (Fold Expressions)、CRTP 静的ポリモーフィズム
   - C++20 宇宙船演算子 (`<=>`) による比較の自動化と自作イテレータ
   - `std::pmr` バンプアロケータ、`std::bit_cast`、ハードウェアアライメント
   - `std::filesystem` と C++23 `std::println`
   - `std::scoped_lock`、`std::condition_variable`、`std::counting_semaphore`、`std::latch`
