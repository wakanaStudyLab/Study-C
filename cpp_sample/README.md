# Modern C++ Crash Course (For Rust, C#, Go Developers) - C++20/C++23 Edition

Rust, C#, Go 経験者向けに特化した、**モダン C++ (C++20 / C++23) の設計思想と実践イディオム最速習得ガイド**です。
ゼロコスト抽象化、RAII、ムーブセマンティクス、代数的データ型、並行・非同期、メタプログラミング、ファイルI/Oまで全13モジュールで網羅しています。

---

## 🚀 CMake でのビルド & 実行方法

ルートディレクトリ（`C:\Users\harun\programming\C++\sample`）から CMake で一括ビルド・実行できます：

```powershell
cd C:\Users\harun\programming\C++\sample

# 構成 & ビルド (C++23規格)
cmake -B build -S .
cmake --build build --config Release --target cpp_sample

# 実行
.\build\bin\Release\cpp_sample.exe  # または .\build\bin\cpp_sample.exe (Ninja使用時)
```

---

## 🗺️ 言語対比マッピング早見表 (Modern C++ vs Rust vs C# vs Go)

| 概念・機能 | Modern C++ (C++20/C++23) | Rust | C# | Go |
| :--- | :--- | :--- | :--- | :--- |
| **自動リソース解放** | **RAII** (デストラクタ `~T()`) | `Drop` トレイト | `using` / `IDisposable` | `defer r.Close()` |
| **単一所有ヒープ** | `std::unique_ptr<T>` | `Box<T>` | 参照型 (`new`) | ポインタ (`new`) |
| **参照カウント共有** | `std::shared_ptr<T>` | `Arc<T>` / `Rc<T>` | GC に委譲 | GC に委譲 |
| **弱参照 (循環参照防止)**| `std::weak_ptr<T>` | `Weak<T>` | `WeakReference<T>` | なし |
| **所有権の移動** | `std::move(x)` (右辺値参照 `&&`) | デフォルトで Move | なし (参照コピー) | なし (値コピー) |
| **ゼロコピー文字列** | `std::string_view` | `&str` | `ReadOnlySpan<char>` | `string` |
| **パイプライン処理** | `std::views::filter \| ...` | `.iter().filter().map()` | `.Where().Select()` | ループ / slices |
| **型制約 / ジェネリクス**| **Concepts** (`requires`) | `trait bounds` | `where T : ...` | `[T constraints.Ordered]`|
| **Null 安全性** | `std::optional<T>` | `Option<T>` | `T?` / Nullable | `(T, bool)` / `nil` |
| **代数的データ型** | `std::variant<A, B>` | `enum Name { A, B }` | `abstract record` | interface + type switch |
| **パターンマッチング** | `std::visit(Overloaded{...}, v)` | `match v { ... }` | `v switch { ... }` | `switch v.(type)` |
| **Result 型エラー処理**| `std::expected<T, E>` (C++23) | `Result<T, E>` | なし (例外) | `(T, error)` |
| **モナディック操作** | `.and_then()`, `.transform()` | `.and_then()`, `.map()` | LINQ SelectMany | なし |
| **三方比較 / 順序付け** | **宇宙船演算子 `<=>`** | `PartialOrd` / `Ord` | `IComparable<T>` | `cmp.Compare` |
| **非同期 Future** | `std::async` + `std::future` | `Future<T>` | `Task<T>` | チャネル + goroutine |
| **RAIIスレッド** | `std::jthread` (自動join) | `std::thread::spawn` | `Thread` / `Task` | `go func()` |
| **コルーチン (Generator)** | `co_yield` (C++20) | `gen` / `async-stream` | `yield return` | goroutine + chan |
| **非所有スライスビュー** | `std::span<T>` (C++20) | `&[T]` | `ReadOnlySpan<T>` / `Span<T>` | `[]T` |
| **高速バンプアロケータ**| `std::pmr` (monotonic_buffer) | バンプアロケータ crate | `ArrayPool<T>` | sync.Pool |
| **安全なビット再解釈** | `std::bit_cast` (C++20) | `transmute` | `Unsafe.As` | `unsafe.Pointer` |
| **コンパイル時強制評価** | `consteval` (C++20) | `const fn` | `const` | なし (定数リテラル) |
| **型安全・高速I/O** | `std::print` / `println` (C++23) | `println!` | `Console.WriteLine` | `fmt.Println` |
| **複数デッドロックフリー**| `std::scoped_lock` (C++17) | `lock()` 手動順序 | `lock(a) { lock(b) }` | `sync.Mutex` |
| **カウントダウン同期** | `std::latch` / `barrier` (C++20) | `Barrier` | `CountdownEvent` | `sync.WaitGroup` |

---

## 📁 全13モジュール構成一覧

| # | ヘッダー / 実装 | 主な学習トピック |
| :-: | :--- | :--- |
| **01** | [`raii_and_smart_pointers.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/raii_and_smart_pointers.hpp)<br>[`raii_and_smart_pointers.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/raii_and_smart_pointers.cpp) | デストラクタによる自動リソース管理、`std::unique_ptr` (単一所有権)、`std::shared_ptr` (参照カウント)、`make_unique` / `make_shared` |
| **02** | [`move_semantics_and_classes.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/move_semantics_and_classes.hpp)<br>[`move_semantics_and_classes.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/move_semantics_and_classes.cpp) | 右辺値参照 (`&&`)、`std::move` によるO(1)移動、Rule of 5/0、`virtual` デストラクタ、`override` |
| **03** | [`stl_and_ranges.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/stl_and_ranges.hpp)<br>[`stl_and_ranges.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/stl_and_ranges.cpp) | `vector` / `unordered_map`、`std::string_view` (ゼロコピー文字列)、C++20 Ranges パイプライン (`\|`)、`std::ranges::sort` |
| **04** | [`templates_and_modern_types.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/templates_and_modern_types.hpp)<br>[`templates_and_modern_types.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/templates_and_modern_types.cpp) | C++20 Concepts (`requires`)、`std::optional<T>`、代数的データ型 `std::variant<Ts...>`、パターンマッチ `std::visit` |
| **05** | [`concurrency.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/concurrency.hpp)<br>[`concurrency.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/concurrency.cpp) | `std::async` & `std::future` 非同期タスク、C++20 `std::jthread` (自動joinスレッド)、`std::atomic<T>` ロックフリー同期 |
| **06** | [`lambda_expressions.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/lambda_expressions.hpp)<br>[`lambda_expressions.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/lambda_expressions.cpp) | キャプチャリスト (`[=]`, `[&]`, `mutable`)、C++14 ムーブキャプチャ、C++20 テンプレートラムダ、IIFE イディオム |
| **07** | [`coroutines_and_modern_cpp.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/coroutines_and_modern_cpp.hpp)<br>[`coroutines_and_modern_cpp.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/coroutines_and_modern_cpp.cpp) | C++20 コルーチン (`co_yield` による遅延ジェネレータ)、`consteval` 完全コンパイル時計算、`std::span<T>` スライス、`std::format` |
| **08** | [`error_handling_and_expected.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/error_handling_and_expected.hpp)<br>[`error_handling_and_expected.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/error_handling_and_expected.cpp) | **C++23 `std::expected<T, E>`** (Rust `Result` 相当)、モナディック操作 (`.and_then()`, `.transform()`)、`noexcept` 保証 |
| **09** | [`advanced_templates_and_metaprogramming.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/advanced_templates_and_metaprogramming.hpp)<br>[`advanced_templates_and_metaprogramming.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/advanced_templates_and_metaprogramming.cpp) | 可変引数テンプレート、C++17 畳み込み式 (Fold Expressions: `(... + args)`)、`if constexpr`、`<type_traits>`、**CRTP** (静的ポリモーフィズム) |
| **10** | [`operator_overloading_and_spaceship.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/operator_overloading_and_spaceship.hpp)<br>[`operator_overloading_and_spaceship.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/operator_overloading_and_spaceship.cpp) | 算術・添字・ストリーム出力オーバーロード、**C++20 宇宙船演算子 (`<=>`)** による6大比較演算子の自動導出、自作イテレータ |
| **11** | [`memory_and_low_level.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/memory_and_low_level.hpp)<br>[`memory_and_low_level.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/memory_and_low_level.cpp) | `std::weak_ptr` 循環参照解決、**`std::pmr`** バンプアロケータ (ヒープ不要の高速確保)、`std::bit_cast`、`alignas` / `alignof` |
| **12** | [`filesystem_and_io.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/filesystem_and_io.hpp)<br>[`filesystem_and_io.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/filesystem_and_io.cpp) | **`std::filesystem`** (パス結合 `/`, 再帰走査)、バイナリファイルI/O、**C++23 `std::print` / `std::println`** |
| **13** | [`advanced_concurrency.hpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/include/advanced_concurrency.hpp)<br>[`advanced_concurrency.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/advanced_concurrency.cpp) | **`std::scoped_lock`** (デッドロックフリー)、`std::condition_variable` (Producer-Consumer)、C++20 `std::counting_semaphore` & `std::latch` |

- [`src/main.cpp`](file:///C:/Users/harun/programming/C++/sample/cpp_sample/src/main.cpp): 全13モジュールを順序よく実演する統合エントリーポイント

> 📖 **Modern C++ ラムダ式の完全理解ガイド**:  
> C++におけるラムダ式の構文解剖からキャプチャリスト（値・参照・ムーブ・`mutable`）、IIFE、コンパイラが裏で生成する無名ファンクタクラスの正体まで完全網羅した解説は [**`LAMBDA.md`**](file:///C:/Users/harun/programming/C++/sample/cpp_sample/LAMBDA.md) を参照してください。
