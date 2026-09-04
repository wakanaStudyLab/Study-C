#include "memory_and_low_level.hpp"
#include <iostream>
#include <format>
#include <memory_resource>
#include <array>
#include <chrono>

/**
 * ============================================================================
 * モジュール 11: 高度メモリ管理・PMR・低レイヤ操作 (Memory & Low-Level)
 * ============================================================================
 */

namespace modern_cpp {

Node::~Node() {
    std::cout << std::format("    [Node Destructor] Node {} successfully freed!\n", id);
}

// 64バイトアライメント（CPUキャッシュライン偽共有防止用）の構造体
struct alignas(64) CacheAlignedData {
    uint64_t counter;
    char padding[56];
};

void run_memory_and_low_level_demo() {
    std::cout << "--- 1. Breaking Reference Cycles via std::weak_ptr ---\n";
    {
        std::cout << "  Creating Node A and Node B in nested scope...\n";
        auto nodeA = std::make_shared<Node>(1);
        auto nodeB = std::make_shared<Node>(2);

        nodeA->next = nodeB;
        nodeB->prev = nodeA; // weak_ptr なので nodeA の shared_ptr 参照カウントは増えない！

        std::cout << std::format("  Node A use_count: {}\n", nodeA.use_count());
        std::cout << std::format("  Node B use_count: {}\n", nodeB.use_count());

        // weak_ptr から shared_ptr を昇格して安全にアクセス
        if (auto lockedA = nodeB->prev.lock()) {
            std::cout << std::format("  Accessing Node A from Node B via weak_ptr: Node ID = {}\n", lockedA->id);
        }
        std::cout << "  Leaving nested scope (both nodes should be destroyed):\n";
    }

    std::cout << "\n--- 2. High-Performance Bump Allocation via std::pmr (C++17) ---\n";
    {
        // スタック上に 1024 バイトのメモリバッファを用意
        std::array<std::byte, 1024> stack_buffer{};
        // バンプアロケータ（単調増加リソース）を初期化
        std::pmr::monotonic_buffer_resource mem_pool(
            stack_buffer.data(), stack_buffer.size(), std::pmr::null_memory_resource());

        // ヒープアロケーション(malloc/new)を一切行わず、スタックバッファからメモリを確保する pmr::vector
        std::pmr::vector<int> pmr_vec(&mem_pool);

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 50; ++i) {
            pmr_vec.push_back(i * 10);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << std::format("  std::pmr::vector allocated {} elements in {} ns without heap syscalls!\n",
                                 pmr_vec.size(), elapsed_ns);
        std::cout << std::format("  First: {}, Last: {}\n", pmr_vec.front(), pmr_vec.back());
    }

    std::cout << "\n--- 3. Type-Safe Bit Reinterpretation via std::bit_cast (C++20) ---\n";
    {
        float f = 1.0f;
        // float (IEEE 754) のバイナリを型安全に uint32_t として読み取る
        uint32_t f_bits = std::bit_cast<uint32_t>(f);
        std::cout << std::format("  float 1.0f as raw hex bits: 0x{:08X} (IEEE 754 standard)\n", f_bits);

        // 逆変換
        float reconstructed = std::bit_cast<float>(f_bits);
        std::cout << std::format("  Reconstructed float from bits: {:.1f}\n", reconstructed);
    }

    std::cout << "\n--- 4. Hardware Alignment & std::byte (alignas / alignof) ---\n";
    {
        std::cout << std::format("  alignof(int):              {} bytes\n", alignof(int));
        std::cout << std::format("  alignof(double):           {} bytes\n", alignof(double));
        std::cout << std::format("  alignof(CacheAlignedData): {} bytes (Matches CPU cache-line!)\n", alignof(CacheAlignedData));
        std::cout << std::format("  sizeof(CacheAlignedData):  {} bytes\n", sizeof(CacheAlignedData));
    }
}

} // namespace modern_cpp
