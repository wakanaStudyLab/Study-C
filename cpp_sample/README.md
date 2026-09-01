# Modern C++ Crash Course (For Rust, C#, Go Developers)

Rust, C#, Go 経験者向けに特化した、**モダン C++ (C++20 / C++23) の設計思想と実践イディオム最速習得ガイド**です。

---

## 🚀 CMake でのビルド & 実行方法

ルートディレクトリ（`C:\Users\harun\programming\C++`）から CMake で一括ビルドできます：

```powershell
cd C:\Users\harun\programming\C++

# 構成 & ビルド (C++20規格)
cmake -B build -S .
cmake --build build --config Release --target cpp_sample

# 実行
.\build\bin\Release\cpp_sample.exe
```

---

## 🗺️ 言語対比マッピング早見表 (Modern C++ vs Rust vs C# vs Go)

| 概念・機能 | Modern C++ (C++20+) | Rust | C# | Go |
| :--- | :--- | :--- | :--- | :--- |
| **自動リソース解放** | **RAII** (デストラクタ `~T()`) | `Drop` トレイト | `using` / `IDisposable` | `defer r.Close()` |
| **単一所有ヒープ** | `std::unique_ptr<T>` | `Box<T>` | 参照型 (`new`) | ポインタ (`new`) |
| **参照カウント共有** | `std::shared_ptr<T>` | `Arc<T>` / `Rc<T>` | GC に委譲 | GC に委譲 |
| **所有権の移動** | `std::move(x)` (右辺値参照 `&&`) | デフォルトで Move | なし (参照コピー) | なし (値コピー) |
| **ゼロコピー文字列** | `std::string_view` | `&str` | `ReadOnlySpan<char>` | `string` |
| **パイプライン処理** | `std::views::filter \| ...` | `.iter().filter().map()` | `.Where().Select()` | ループ / slices |
| **型制約 / ジェネリクス**| **Concepts** (`requires`) | `trait bounds` | `where T : ...` | `[T constraints.Ordered]`|
| **Null 安全性** | `std::optional<T>` | `Option<T>` | `T?` / Nullable | `(T, bool)` / `nil` |
| **代数的データ型** | `std::variant<A, B>` | `enum Name { A, B }` | `abstract record` | interface + type switch |
| **パターンマッチング** | `std::visit(Overloaded{...}, v)` | `match v { ... }` | `v switch { ... }` | `switch v.(type)` |
| **非同期 Future** | `std::async` + `std::future` | `Future<T>` | `Task<T>` | チャネル + goroutine |
| **RAIIスレッド** | `std::jthread` (自動join) | `std::thread::spawn` | `Thread` / `Task` | `go func()` |

---

## 📁 ソースファイル構成

- [`include/raii_and_smart_pointers.hpp`](file:///C:/Users/harun/programming/C++/cpp_sample/include/raii_and_smart_pointers.hpp) / [`src/raii_and_smart_pointers.cpp`](file:///C:/Users/harun/programming/C++/cpp_sample/src/raii_and_smart_pointers.cpp): RAII・unique_ptr / shared_ptr
- [`include/move_semantics_and_classes.hpp`](file:///C:/Users/harun/programming/C++/cpp_sample/include/move_semantics_and_classes.hpp) / [`src/move_semantics_and_classes.cpp`](file:///C:/Users/harun/programming/C++/cpp_sample/src/move_semantics_and_classes.cpp): 右辺値参照・std::move・Rule of 5/0・virtual
- [`include/stl_and_ranges.hpp`](file:///C:/Users/harun/programming/C++/cpp_sample/include/stl_and_ranges.hpp) / [`src/stl_and_ranges.cpp`](file:///C:/Users/harun/programming/C++/cpp_sample/src/stl_and_ranges.cpp): string_view・C++20 Ranges パイプライン・sort
- [`include/templates_and_modern_types.hpp`](file:///C:/Users/harun/programming/C++/cpp_sample/include/templates_and_modern_types.hpp) / [`src/templates_and_modern_types.cpp`](file:///C:/Users/harun/programming/C++/cpp_sample/src/templates_and_modern_types.cpp): C++20 Concepts・optional・variant & visit
- [`include/concurrency.hpp`](file:///C:/Users/harun/programming/C++/cpp_sample/include/concurrency.hpp) / [`src/concurrency.cpp`](file:///C:/Users/harun/programming/C++/cpp_sample/src/concurrency.cpp): 非同期 async/future・C++20 jthread・atomic
- [`src/main.cpp`](file:///C:/Users/harun/programming/C++/cpp_sample/src/main.cpp): 統合エントリーポイント
