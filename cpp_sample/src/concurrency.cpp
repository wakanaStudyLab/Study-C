#include "concurrency.hpp"
#include <iostream>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>

/**
 * ============================================================================
 * C++ 05: 並行処理・非同期 Future・アトミック変数 (Concurrency & Future)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. std::jthread (C++20+):
 *    - RAIIスレッド。デストラクタで自動的に `join()`（および停止要求）を呼ぶ。
 *    - 従来の `std::thread` で頻発した「join忘れによる std::terminate クラッシュ」を完全防止。
 * 
 * 2. std::async と std::future (C++11+):
 *    - C#の `Task.Run`、JavaScriptの `Promise`、Rustの `Future` に相当。
 *    - バックグラウンドで非同期タスクを実行し、`future.get()` で結果を取得（ブロック待機）。
 * 
 * 3. 排他制御 (std::mutex & std::scoped_lock):
 *    - ロックの獲得と解放を RAII で行う。デッドロックを防ぐため `std::scoped_lock` (C++17+) を推奨。
 * 
 * 4. ロックフリー操作 (std::atomic<T>):
 *    - CPUのCAS命令を用いた超高速な並行カウンター。
 */

namespace modern_cpp {

void run_concurrency_demo() {
    std::cout << "--- 1. Asynchronous Tasks with std::async and std::future ---\n";
    {
        // 2つの重い処理を並行実行
        std::future<int> task1 = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return 42;
        });

        std::future<std::string> task2 = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            return std::string("Task2 Completed");
        });

        std::cout << "  Main thread waiting for async tasks to complete..." << std::endl;
        int res1 = task1.get();         // 結果をブロック取得
        std::string res2 = task2.get(); // 結果をブロック取得
        std::cout << "  Task1 result: " << res1 << ", Task2 result: " << res2 << std::endl;
    }

    std::cout << "\n--- 2. std::jthread (C++20 RAII Auto-Joining Thread) ---\n";
    {
        // スコープを抜けるときに自動で join されるため安全
        std::jthread worker([]() {
            std::cout << "  [jthread] Background worker thread running..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        });
        std::cout << "  [Main] Leaving scope: worker.join() will be called automatically" << std::endl;
    }

    std::cout << "\n--- 3. std::atomic<T> (Lock-Free Concurrent Counter) ---\n";
    {
        std::atomic<int> counter{0};
        constexpr int NUM_THREADS = 4;
        constexpr int ITERATIONS = 2500;

        std::vector<std::jthread> threads;
        for (int i = 0; i < NUM_THREADS; i++) {
            threads.emplace_back([&counter]() {
                for (int j = 0; j < ITERATIONS; j++) {
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        // 全 jthread がスコープ終了時に自動 join される
        threads.clear();

        std::cout << "  Atomic Counter Final Result (Expected 10000): " << counter.load() << std::endl;
    }
}

} // namespace modern_cpp
