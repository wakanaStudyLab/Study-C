#ifndef ADVANCED_CONCURRENCY_HPP
#define ADVANCED_CONCURRENCY_HPP

#include <mutex>
#include <condition_variable>
#include <queue>
#include <semaphore>
#include <latch>
#include <barrier>
#include <thread>

/**
 * ============================================================================
 * モジュール 13: 高度な並行処理・同期プリミティブ (Advanced Concurrency)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. std::scoped_lock (C++17):
 *    - 複数のミューテックスをデッドロックを起こさずに一括取得するRAIIロック。
 *    - C# の lock ステートメントのネストや、手動のロック順序制御のバグを根本排除。
 * 
 * 2. std::condition_variable:
 *    - イベント駆動型の待機・通知（Producer-Consumer パターン）。
 * 
 * 3. C++20 新同期プリミティブ:
 *    - `std::counting_semaphore`: 同時アクセス数を N 個に制限（DBコネクションプール等）。
 *    - `std::latch`: 指定されたカウントがゼロになるまでスレッド群を待機させる使い捨てバリア。
 *    - `std::barrier`: 複数スレッドがフェーズごとに足並みを揃えて繰り返し同期できるバリア。
 */

namespace modern_cpp {

// スレッドセーフなブロッキングキュー (Producer-Consumer パターン)
template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    bool finished_ = false;

public:
    void push(T val) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            queue_.push(std::move(val));
        }
        cv_.notify_one();
    }

    bool pop(T& out_val) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return !queue_.empty() || finished_; });

        if (queue_.empty() && finished_) {
            return false;
        }

        out_val = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void finish() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            finished_ = true;
        }
        cv_.notify_all();
    }
};

// モジュール実行エントリポイント
void run_advanced_concurrency_demo();

} // namespace modern_cpp

#endif // ADVANCED_CONCURRENCY_HPP
