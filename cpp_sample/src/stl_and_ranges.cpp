#include "stl_and_ranges.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>

/**
 * ============================================================================
 * C++ 03: STL コンテナ & C++20 Ranges (LINQ / Rust Iterator 相当)
 * ============================================================================
 *
 * 【他言語経験者（Rust, C#, Go）向け要点】
 *
 * 1. std::string_view (C++17+):
 *    - Rustの `&str`、Goの `string` スライスに相当する「非所有の文字列参照」。
 *    - 文字列のコピーを一切発生させずに O(1) で部分文字列や関数引数を扱えます。
 *    - ポインタと長さのペアとして実装されており、メモリ確保は行わない。
 *    - 【注意】参照先の文字列 (std::string or リテラル) の寿命が尽きると dangling 参照になるため、
 *      string_view 自体をオブジェクトのフィールドとして長期間保持するのは危険。
 *
 * 2. C++20 Ranges パイプライン (|):
 *    - C# の LINQ (`.Where().Select()`)、Rust の Iterator (`.filter().map()`) と同様の
 *      「遅延評価 (Lazy Evaluation)」パイプライン。
 *    - `std::views::filter` や `std::views::transform` を `|` (パイプ演算子) で連結可能。
 *    - 遅延評価: パイプラインを構築しただけでは実行されず、for ループなどで消費されたとき
 *      初めて各要素が評価される。→ 不要な中間配列を生成しないため非常に効率的。
 *    - Rust の .iter().filter().map().collect() と比較すると、collect() なしでも
 *      for ループで直接消費できる点が特徴的。
 *
 * 3. ラムダ式のキャプチャ構文:
 *    ┌──────────┬────────────────────────────────────────────────────────┐
 *    │ 構文     │ 意味                                                   │
 *    ├──────────┼────────────────────────────────────────────────────────┤
 *    │ []       │ キャプチャなし (外部変数にアクセスしない)              │
 *    │ [=]      │ 外部変数を全て「値（コピー）」でキャプチャ             │
 *    │ [&]      │ 外部変数を全て「参照」でキャプチャ (Rust の借用クロージャ) │
 *    │ [x, &y]  │ x は値コピー、y は参照でキャプチャ (個別指定)         │
 *    │ [this]   │ クラスメソッド内でメンバ変数にアクセスするため        │
 *    └──────────┴────────────────────────────────────────────────────────┘
 *
 * 4. 主要 STL コンテナの対応関係:
 *    ┌────────────────────────┬────────────────────────────────────────────┐
 *    │ C++                    │ 他言語                                     │
 *    ├────────────────────────┼────────────────────────────────────────────┤
 *    │ std::vector<T>         │ Rust Vec<T> / Go []T / C# List<T>         │
 *    │ std::unordered_map<K,V>│ Rust HashMap<K,V> / Go map[K]V / C# Dictionary │
 *    │ std::unordered_set<T>  │ Rust HashSet<T> / Go map[T]struct{}        │
 *    │ std::map<K,V>          │ Rust BTreeMap<K,V> (ソート済みマップ)      │
 *    │ std::deque<T>          │ 両端キュー。push_front が O(1)             │
 *    └────────────────────────┴────────────────────────────────────────────┘
 */

namespace modern_cpp {

// 商品データを表す集約 (aggregate) 構造体。
// C++ の struct はデフォルトで全メンバが public。class との違いはアクセス修飾子のデフォルトのみ。
struct Item {
    std::string name;     // 商品名
    std::string category; // カテゴリ ("Electronics", "Books" 等)
    int price;            // 価格 (円)
    bool in_stock;        // 在庫有無
};

// string_view を引数に取ることで、std::string でも "リテラル" でもコピーゼロで受け取れる。
// もし const std::string& にすると、リテラルから std::string が構築されコピーが発生する。
static void print_category_header(std::string_view title) {
    std::cout << "=== Category: " << title << " ===" << std::endl;
}

void run_stl_and_ranges_demo() {
    print_category_header("Overview");

    // ====================================================================
    // 1. STL コンテナと string_view
    // ====================================================================
    std::cout << "--- 1. STL Containers & string_view (Zero-Copy String) ---\n";

    // std::vector<Item>: 動的配列。連続したメモリ領域に確保されるためキャッシュ効率が高い。
    // 初期化子リスト {} による構築 (aggregate initialization)
    std::vector<Item> items = {
        {"MacBook Pro",         "Electronics", 250000, true },
        {"Mechanical Keyboard", "Electronics",  18000, false},
        {"Rust Book",           "Books",         4200, true },
        {"C++ Primer",          "Books",         5800, true },
        {"Coffee Mug",          "Misc",          1500, true }
    };

    // std::unordered_map: ハッシュテーブルによる O(1) 平均の検索・挿入
    // Go の map、Rust の HashMap、C# の Dictionary に相当
    std::unordered_map<std::string, int> stock_counts;
    for (const auto& item : items) {
        // operator[] は存在しないキーに対してデフォルト値(0)で自動挿入する
        stock_counts[item.category] += (item.in_stock ? 1 : 0);
    }

    // C++17 構造化束縛 (Structured Binding): Rust の let (k, v) = ... に相当
    // const auto& [category, count] で map の各エントリをキーと値に分解して受け取る
    for (const auto& [category, count] : stock_counts) {
        std::cout << "  " << category << " (In Stock Count): " << count << std::endl;
    }

    // ====================================================================
    // 2. C++20 Ranges パイプライン (遅延評価)
    // ====================================================================
    std::cout << "\n--- 2. C++20 Ranges Pipeline (LINQ / Rust Iterator) ---\n";

    // namespace エイリアス: `std::views` を `views` と短縮して読みやすくする
    namespace views = std::views;

    // パイプライン構築 (この時点では評価されない。遅延評価。)
    // ① items の全要素から in_stock == true のものをフィルタ (C# .Where / Rust .filter)
    // ② さらに category == "Books" のものだけに絞り込む
    // ③ 各 Item を "名前 (税込価格)" という文字列に変換 (C# .Select / Rust .map)
    auto filtered_pipeline = items
        | views::filter([](const Item& it) { return it.in_stock; })
        | views::filter([](const Item& it) { return it.category == "Books"; })
        | views::transform([](const Item& it) {
            // static_cast<int> で double を int に明示的に変換 (暗黙の縮小変換を避ける)
            return it.name + " (Tax incl: " + std::to_string(static_cast<int>(it.price * 1.1)) + " JPY)";
        });

    // ここで初めてパイプラインが実行される (消費のタイミング)
    // for ループが各要素を要求するたびに、パイプラインが 1 要素分だけ評価される
    std::cout << "> In-Stock Books (Ranges Pipeline Result):\n";
    for (const auto& display_text : filtered_pipeline) {
        std::cout << "  * " << display_text << std::endl;
    }

    // ====================================================================
    // 3. std::ranges アルゴリズム
    // ====================================================================
    std::cout << "\n--- 3. std::ranges Algorithms (Sort & Query) ---\n";

    // std::ranges::sort: 従来の std::sort(begin, end, ...) と異なり、
    // イテレータペアではなくコンテナ全体を直接渡せる (C++20 以降)。
    // ラムダ式でカスタム比較関数を指定: price の昇順 (小さい方が先)
    std::ranges::sort(items, [](const Item& a, const Item& b) {
        return a.price < b.price; // true を返す側が「より小さい」とみなされる
    });

    std::cout << "> Sorted by Price (Ascending):\n";
    for (const auto& it : items) {
        std::cout << "  - " << it.name << " (" << it.price << " JPY)" << std::endl;
    }
}

} // namespace modern_cpp
