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
|   ├── POINTERS.md                   #ポインタを完全に理解できるかもしれないファイル
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

## 🛠️ ビルドに必要な環境・要件 (Prerequisites)

本環境では、**Clang (`clang-cl`) + Ninja** を用いた高速ビルドおよび **MSVC (cppvsdbg)** によるデバッグ構成を採用しています。新規マシン等でビルド・デバッグを行うには以下の環境が必要です。

### 1. 必要なツール一覧

| ツール | 用途 | インストール先（本環境例） | 入手先 / 導入方法 |
| :--- | :--- | :--- | :--- |
| **LLVM / Clang**<br>(`clang-cl.exe`) | C/C++ コンパイラ<br>(MSVC互換モード) | `C:\Program Files\LLVM\bin\` | [LLVM Releases (GitHub)](https://github.com/llvm/llvm-project/releases)<br>（Windows 64bit用インストーラ） |
| **Ninja**<br>(`ninja.exe`) | 高速ビルドジェネレータ | `C:\ninja\ninja.exe` | [Ninja Releases (GitHub)](https://github.com/ninja-build/ninja/releases)<br>（実行ファイルを配置） |
| **CMake**<br>(`cmake.exe`) | ビルド構成生成ツール | `C:\Program Files\CMake\bin\` | [cmake.org (Previous Releases)](https://cmake.org/download/#previous) |
| **Windows SDK**<br>(`rc.exe`, `mt.exe`) | リソースコンパイラ /<br>マニフェストツール | `C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\` | Visual Studio Installer または<br>[Windows SDK 公式](https://developer.microsoft.com/ja-jp/windows/downloads/windows-sdk/) |
| **MSVC CRT**<br>(C/C++ Build Tools) | C/C++ ランタイムライブラリ<br>(`clang-cl` がリンク) | Visual Studio インストール配下 | [Visual Studio Build Tools](https://visualstudio.microsoft.com/ja/downloads/)<br>「C++ によるデスクトップ開発」 |

### 2. VS Code 推奨拡張機能

| 拡張機能 | 識別子 | 用途 |
| :--- | :--- | :--- |
| **C/C++** | `ms-vscode.cpptools` | IntelliSense、シンタックスハイライト、デバッグ (`cppvsdbg`) |
| **CMake Tools** | `ms-vscode.cmake-tools` | VS Code 統合 CMake ビルド & テスト |

> [!IMPORTANT]
> **MSVC Build Tools (CRT) の必須性:**  
> Windows 上で `clang-cl` を使う場合、起動エントリ（`mainCRTStartup` 等）や標準ライブラリ（`libcmt`, `msvcrt` 等）は MSVC のランタイムを参照します。Visual Studio または C++ Build Tools が未導入の場合、リンクエラーが発生します。

---

## 🚀 CMake でのビルド & 実行方法

### 1. PowerShell スクリプトで一括実行 (最も簡単)
```powershell
cd C:\Users\harun\programming\C++
.\build_and_run.ps1
```

### 2. コマンドラインでの手動ビルド

#### A. Ninja + Clang-cl（本プロジェクト標準構成）
```powershell
# 1. 構成 (Configure): Release または Debug を指定
cmake -B build -S . -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_MAKE_PROGRAM="C:/ninja/ninja.exe" `
  -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" `
  -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" `
  -DCMAKE_RC_COMPILER="C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/rc.exe" `
  -DCMAKE_MT="C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/mt.exe"

# 2. ビルド (Build)
cmake --build build

# 3. 実行 (Run)
.\build\bin\c_sample.exe
.\build\bin\cpp_sample.exe
```

#### B. Visual Studio ジェネレータ（マルチ構成）
```powershell
cmake -B build -S .
cmake --build build --config Release
.\build\bin\Release\c_sample.exe
.\build\bin\Release\cpp_sample.exe
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
