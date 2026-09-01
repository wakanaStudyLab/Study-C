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
 * 1. std::string_view (C++17+):
 *    - Rustの `&str`、Goの `string` スライスに相当する「非所有の文字列参照」。
 *    - 文字列のコピーを一切発生させずに O(1) で部分文字列や関数引数を扱えます。
 * 
 * 2. C++20 Ranges パイプライン (|):
 *    - C#の LINQ (`.Where().Select()`)、Rustの Iterator (`.filter().map()`) と同様の
 *      「遅延評価 (Lazy Evaluation)」パイプライン。
 *    - `std::views::filter` や `std::views::transform` を `|` (パイプ演算子) で連結可能。
 * 
 * 3. ラムダ式のキャプチャ構文:
 *    - `[=]`: 外部変数を値（コピー）でキャプチャ
 *    - `[&]`: 外部変数を参照でキャプチャ (Rustの借用クロージャ)
 *    - `[x, &y]`: 個別指定
 */

namespace modern_cpp {

struct Item {
    std::string name;
    std::string category;
    int price;
    bool in_stock;
};

// string_view を引数に取ることで、std::string でも "リテラル" でもコピーゼロで受け取れる
static void print_category_header(std::string_view title) {
    std::cout << "=== Category: " << title << " ===" << std::endl;
}

void run_stl_and_ranges_demo() {
    print_category_header("Overview");
    std::cout << "--- 1. STL Containers & string_view (Zero-Copy String) ---\n";
    std::vector<Item> items = {
        {"MacBook Pro", "Electronics", 250000, true},
        {"Mechanical Keyboard", "Electronics", 18000, false},
        {"Rust Book", "Books", 4200, true},
        {"C++ Primer", "Books", 5800, true},
        {"Coffee Mug", "Misc", 1500, true}
    };

    // ハッシュマップ (Goの map, Rustの HashMap, C#の Dictionary 相当)
    std::unordered_map<std::string, int> stock_counts;
    for (const auto& item : items) {
        stock_counts[item.category] += (item.in_stock ? 1 : 0);
    }
    for (const auto& [category, count] : stock_counts) { // C++17 構造化束縛 (Structured Binding)
        std::cout << "  " << category << " (In Stock Count): " << count << std::endl;
    }

    std::cout << "\n--- 2. C++20 Ranges Pipeline (LINQ / Rust Iterator) ---\n";
    // クエリ: 在庫がある商品の名前を大文字にし、価格に消費税 (10%) を加算して表示
    namespace views = std::views;

    auto filtered_pipeline = items 
        | views::filter([](const Item& it) { return it.in_stock; }) // C# Where / Rust filter
        | views::filter([](const Item& it) { return it.category == "Books"; })
        | views::transform([](const Item& it) {                      // C# Select / Rust map
            return it.name + " (Tax incl: " + std::to_string(static_cast<int>(it.price * 1.1)) + " JPY)";
        });

    std::cout << "> In-Stock Books (Ranges Pipeline Result):\n";
    for (const auto& display_text : filtered_pipeline) {
        std::cout << "  * " << display_text << std::endl;
    }

    std::cout << "\n--- 3. std::ranges Algorithms (Sort & Query) ---\n";
    // 価格の昇順にインプレースソート
    std::ranges::sort(items, [](const Item& a, const Item& b) {
        return a.price < b.price;
    });

    std::cout << "> Sorted by Price (Ascending):\n";
    for (const auto& it : items) {
        std::cout << "  - " << it.name << " (" << it.price << " JPY)" << std::endl;
    }
}

} // namespace modern_cpp
