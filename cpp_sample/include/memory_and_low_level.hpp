#ifndef MEMORY_AND_LOW_LEVEL_HPP
#define MEMORY_AND_LOW_LEVEL_HPP

#include <memory>
#include <vector>
#include <cstddef>
#include <bit>

/**
 * ============================================================================
 * モジュール 11: 高度メモリ管理・PMR・低レイヤ操作 (Memory & Low-Level)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. std::weak_ptr による循環参照解決:
 *    - shared_ptr 同士が相互参照すると参照カウントが 0 にならずメモリリークする。
 *    - weak_ptr はカウントを増やさない非所有参照（Rust の Weak<T> 相当）。
 * 
 * 2. std::pmr (Polymorphic Memory Resources - C++17):
 *    - ヒープ確保（malloc/new）はシステムコールやロックを伴い遅い。
 *    - `std::pmr::monotonic_buffer_resource` を使うと、スタック上に置いた
 *      固定長バッファから超高速（ポインタ加算のみ）にメモリを切り出せる（バンプアロケータ）。
 *    - ゲームエンジンやHFT（高頻度取引）で必須の技術。
 * 
 * 3. std::bit_cast (C++20):
 *    - `reinterpret_cast` や memcpy の未定義動作を回避し、
 *      同じサイズの型のバイナリ表現をコンパイル時にも型安全に再解釈する。
 * 
 * 4. alignas & alignof:
 *    - CPU のキャッシュライン（通常64バイト）や SIMD 命令に合わせた
 *      メモリアライメントの指定と検証。
 */

namespace modern_cpp {

// 循環参照デモ用の構造体
struct Node {
    int id;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev; // weak_ptr にすることで循環参照を防止！

    explicit Node(int val) : id(val) {}
    ~Node();
};

// モジュール実行エントリポイント
void run_memory_and_low_level_demo();

} // namespace modern_cpp

#endif // MEMORY_AND_LOW_LEVEL_HPP
