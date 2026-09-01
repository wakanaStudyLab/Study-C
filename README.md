# C & C++ Crash Course (For Rust, C#, Go Developers) - CMake Edition

Rust, C#, Go などの静的型付け言語を理解しているエンジニアが、**最短で C言語 および Modern C++ (C++20/C++23) をマスターするための体系的なサンプルコードと解説** です。

実務標準の **CMake** を使って、クロスプラットフォーム（Windows / Linux / macOS）で即座にビルド・実行できるよう構成されています。

---

## 📁 ディレクトリ構成

```text
C:\Users\harun\programming\C++\
├── CMakeLists.txt                    # ルート CMake 設定 (C/C++ プロジェクト一括管理)
│
├── c_sample\                         # 【C言語 最速習得コース】
│   ├── CMakeLists.txt                # C言語ターゲット設定 (C11標準)
│   ├── include\                      # ヘッダーファイル
│   │   ├── pointers_and_memory.h     # 01: ポインタ・アドレス演算・malloc/free・多重ポインタ
│   │   ├── structs_and_functions.h   # 02: 構造体・パディング・関数ポインタ・OOP模倣
│   │   └── strings_and_io.h          # 03: ヌル終端文字列・snprintf・ファイルI/O
│   ├── src\                          # C ソースコード
│   │   ├── pointers_and_memory.c
│   │   ├── structs_and_functions.c
│   │   ├── strings_and_io.c
│   │   └── main.c                    # C言語 統合エントリーポイント
│   └── README.md                     # C言語 詳細チートシート & 言語対比表
│
├── cpp_sample\                       # 【Modern C++ (C++20/C++23) 最速習得コース】
│   ├── CMakeLists.txt                # C++ターゲット設定 (C++20標準)
│   ├── include\                      # ヘッダーファイル
│   │   ├── raii_and_smart_pointers.hpp    # 01: RAII・unique_ptr / shared_ptr (RustのBox/Arc相当)
│   │   ├── move_semantics_and_classes.hpp # 02: 移動セマンティクス (std::move)・Rule of 5/0・virtual
│   │   ├── stl_and_ranges.hpp             # 03: STL・string_view・C++20 Ranges (LINQ/Iterator相当)
│   │   ├── templates_and_modern_types.hpp # 04: C++20 Concepts・optional・variant (Rustのenum/match相当)
│   │   └── concurrency.hpp                # 05: 非同期 future・C++20 jthread・atomic
│   ├── src\                          # C++ ソースコード
│   │   ├── raii_and_smart_pointers.cpp
│   │   ├── move_semantics_and_classes.cpp
│   │   ├── stl_and_ranges.cpp
│   │   ├── templates_and_modern_types.cpp
│   │   ├── concurrency.cpp
│   │   └── main.cpp                  # C++ 統合エントリーポイント
│   └── README.md                     # C++ 詳細チートシート & 言語対比表
│
├── build_and_run.ps1                 # CMake 構成・ビルド・実行一括スクリプト
├── CMAKE_GUIDE.md                    # 📘 Modern CMake 完全マスターガイド（構文・作法・設計思想）
└── README.md                         # 本ファイル
```

---

## 🚀 CMake でのビルド & 実行方法

### 1. PowerShell スクリプトで一括実行 (最も簡単)
```powershell
cd C:\Users\harun\programming\C++
.\build_and_run.ps1
```

### 2. 標準的な CMake コマンドで手動ビルド
```powershell
cd C:\Users\harun\programming\C++

# 1. 構成 (Configure): build ディレクトリを作成
cmake -B build -S .

# 2. ビルド (Build): Release 構成でコンパイル
cmake --build build --config Release

# 3. 実行 (Run)
.\build\bin\Release\c_sample.exe    # C言語サンプル
.\build\bin\Release\cpp_sample.exe  # C++言語サンプル
```

### 3. VS Code でのビルド & F5 デバッグ
1. VS Code で `C:\Users\harun\programming\C++`（または `cpp_sample` / `c_sample`）を開きます。
2. 拡張機能 **`C/C++`** (`ms-vscode.cpptools`) と **`CMake Tools`** (`ms-vscode.cmake-tools`) をインストール。
3. **`F5`** を押すだけで、自動で CMake ビルドが走りデバッガー（ブレークポイント等）が起動します。
   * 詳しい設定方法やトラブルシューティングは [CMAKE_GUIDE.md](file:///C:/Users/harun/programming/C++/CMAKE_GUIDE.md#7-vs-code-での-cmake-完全設定ガイド) を参照してください。

---

## 🗺️ 言語対比マッピング早見表 (C vs Modern C++ vs Rust vs C# vs Go)

| 概念・機能 | C言語 | Modern C++ (C++20+) | Rust | C# | Go |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **メモリ管理** | 手動 (`malloc`/`free`) | **RAII** (自動デストラクタ) | `Drop` トレイト | GC | GC |
| **単一所有ヒープ** | 生ポインタ `T*` | `std::unique_ptr<T>` | `Box<T>` | 参照型 | ポインタ |
| **共有所有ヒープ** | 参照カウント自作 | `std::shared_ptr<T>` | `Arc<T>` / `Rc<T>` | GC | GC |
| **所有権の移動** | なし (手動すげ替え) | `std::move(x)` (右辺値参照 `&&`) | デフォルトで Move | なし | なし |
| **ゼロコピー文字列** | `const char*` + 長さ | `std::string_view` | `&str` | `ReadOnlySpan<char>` | `string` |
| **遅延パイプライン** | ループ | **Ranges** (`std::views::filter \| ...`) | `.iter().filter().map()` | `.Where().Select()` | ループ / slices |
| **型制約/ジェネリクス**| なし (`void*` / マクロ) | **Concepts** (`requires`) | `trait bounds` | `where T : ...` | `[T constraints.Ordered]`|
| **代数的データ型 (ADT)**| `union` + tag | `std::variant<A, B>` | `enum Name { A, B }` | `abstract record` | interface + type switch |
| **パターンマッチング**| `switch` (整数のみ) | `std::visit(Overloaded{...}, v)` | `match v { ... }` | `v switch { ... }` | `switch v.(type)` |
| **自動joinスレッド** | なし | `std::jthread` (C++20) | `std::thread::spawn` | `Task` | `go func()` |
