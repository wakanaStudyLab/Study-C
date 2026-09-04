#include "advanced_concurrency.hpp"
#include <iostream>
#include <format>
#include <vector>
#include <chrono>

/**
 * ============================================================================
 * モジュール 13: 高度な並行処理・同期プリミティブ (Advanced Concurrency)
 * ============================================================================
 */

namespace modern_cpp {

// 銀行口座送金デモ（デッドロック回避用 scoped_lock）
class Account {
public:
    int id;
    int balance;
    std::mutex mtx;

    Account(int i, int b) : id(i), balance(b) {}
};

void transfer(Account& from, Account& to, int amount) {
    // std::scoped_lock により、from.mtx と to.mtx をデッドロックフリーなアルゴリズムで同時獲得！
    std::scoped_lock lock(from.mtx, to.mtx);
    from.balance -= amount;
    to.balance += amount;
    std::cout << std::format("    [Transfer] Transferred {} from Acc {} to Acc {}\n", amount, from.id, to.id);
}

void run_advanced_concurrency_demo() {
    std::cout << "--- 1. Deadlock-Free Multi-Locking via std::scoped_lock (C++17) ---\n";
    Account acc1(1, 1000);
    Account acc2(2, 500);

    // 逆順のロック取得を別スレッドで行ってもデッドロックしない
    std::jthread t1([&] { transfer(acc1, acc2, 200); });
    std::jthread t2([&] { transfer(acc2, acc1, 100); });
    t1.join();
    t2.join();

    std::cout << std::format("  Final balances -> Acc 1: {}, Acc 2: {}\n", acc1.balance, acc2.balance);

    std::cout << "\n--- 2. Producer-Consumer Pattern via std::condition_variable ---\n";
    ThreadSafeQueue<int> task_queue;
    int sum_consumed = 0;

    std::jthread consumer([&] {
        int item = 0;
        while (task_queue.pop(item)) {
            sum_consumed += item;
        }
    });

    std::jthread producer([&] {
        for (int i = 1; i <= 5; ++i) {
            task_queue.push(i * 10);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        task_queue.finish();
    });

    producer.join();
    consumer.join();
    std::cout << std::format("  Consumer finished processing queue. Total sum: {} (Expected: 150)\n", sum_consumed);

    std::cout << "\n--- 3. Resource Limiting via std::counting_semaphore (C++20) ---\n";
    // 同時に2スレッドのみ許可するセマフォ
    std::counting_semaphore<2> sem(2);
    auto access_resource = [&](int worker_id) {
        sem.acquire(); // リソース獲得 (枠がなければ待機)
        std::cout << std::format("    [Worker {}] Acquired slot in critical section\n", worker_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << std::format("    [Worker {}] Releasing slot\n", worker_id);
        sem.release(); // リソース返却
    };

    std::vector<std::jthread> sem_workers;
    for (int i = 1; i <= 4; ++i) {
        sem_workers.emplace_back(access_resource, i);
    }
    for (auto& w : sem_workers) {
        w.join();
    }

    std::cout << "\n--- 4. Countdown Synchronization via std::latch (C++20) ---\n";
    const int num_workers = 3;
    std::latch init_latch(num_workers);

    std::vector<std::jthread> latch_workers;
    for (int i = 1; i <= num_workers; ++i) {
        latch_workers.emplace_back([&, i] {
            std::this_thread::sleep_for(std::chrono::milliseconds(5 * i));
            std::cout << std::format("    Worker {} initialized.\n", i);
            init_latch.count_down(); // カウントを減らし、待機せずに続行
        });
    }

    std::cout << "  Main thread waiting for all workers to complete init...\n";
    init_latch.wait(); // カウントが 0 になるまでブロック
    std::cout << "  All workers initialized! Main thread resuming processing.\n";
    for (auto& w : latch_workers) {
        w.join();
    }
}

} // namespace modern_cpp
