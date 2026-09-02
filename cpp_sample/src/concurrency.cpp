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
 *
 * 1. std::jthread (C++20+):
 *    - RAII スレッド。デストラクタで自動的に `join()`（および停止要求）を呼ぶ。
 *    - 従来の `std::thread` で頻発した「join 忘れによる std::terminate クラッシュ」を完全防止。
 *    - 停止要求: `stop_source` / `stop_token` によって外部からスレッドに停止を要求できる。
 *      ※ このデモでは停止要求は使用していないが、長時間タスクの協調的キャンセルに使う。
 *    - C#の `Task` + `CancellationToken`、Go のコルーチン + `context.Context` に近い概念。
 *
 * 2. std::async と std::future (C++11+):
 *    - C# の `Task.Run` + `await`、JavaScript の `Promise`、Rust の `tokio::spawn` に相当。
 *    - `std::launch::async`: 別スレッドで即座に非同期実行 (対して `std::launch::deferred` は lazy)。
 *    - `future.get()`: 結果が準備できるまでブロックして待機し、値を取り出す (C# の `.Result`)。
 *    - `future.get()` は 1 回しか呼べない点に注意 (2回目は未定義動作)。
 *    - 例外伝播: 非同期タスク内で例外が発生した場合、`future.get()` を呼んだ側に再スローされる。
 *
 * 3. 排他制御 (std::mutex & std::scoped_lock):
 *    - `std::mutex`: 相互排他ロック。`lock()` / `unlock()` を手動で呼ぶのは危険 (例外時に unlock 漏れ)。
 *    - `std::scoped_lock` (C++17+): RAII ロック。スコープを抜けると自動 unlock。
 *      複数の mutex を一度にロックでき、デッドロックを自動回避するアルゴリズムを使う。
 *      → C# の `lock(...)` / Go の `mu.Lock()` + `defer mu.Unlock()` に相当。
 *    - `std::lock_guard<std::mutex>`: 単一 mutex の RAII ラッパー (C++11 から)。
 *
 * 4. ロックフリー操作 (std::atomic<T>):
 *    - CPU の Compare-And-Swap (CAS) 命令を用いた超高速な並行カウンター。
 *    - mutex によるロックと異なり、スレッドのブロッキングが発生しない。
 *    - `fetch_add`: アトミックに加算し、加算前の値を返す (Rust の `AtomicI32::fetch_add`)。
 *    - メモリオーダリング (std::memory_order):
 *        memory_order_relaxed  : 順序保証なし。カウンターの単純な加算に最適。最速。
 *        memory_order_seq_cst  : 全スレッドで一貫した順序を保証 (デフォルト)。最も安全だが低速。
 *        memory_order_acquire/release: 生産者・消費者パターンで使用。
 */

namespace modern_cpp {

void run_concurrency_demo() {
    // ====================================================================
    // 1. std::async と std::future による非同期タスク
    // ====================================================================
    std::cout << "--- 1. Asynchronous Tasks with std::async and std::future ---\n";
    {
        // std::launch::async: 別スレッドで即座に非同期実行を開始する。
        // ラムダ内の処理が別スレッドで走り、戻り値は future に格納される。
        // C# の Task.Run(() => { return 42; }) に相当。
        std::future<int> task1 = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 重い処理の模擬
            return 42; // この返り値が future に格納される
        });

        // task1 と task2 は並行して実行される (2スレッドが同時に走っている)
        std::future<std::string> task2 = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            return std::string("Task2 Completed");
        });

        // この時点でメインスレッドは両タスクと並行して他の処理ができる
        std::cout << "  Main thread waiting for async tasks to complete..." << std::endl;

        // future.get() でブロッキング待機。タスクが完了していれば即座に返る。
        // task1 が 50ms、task2 が 30ms なので合計約 50ms で両方取得できる（直列なら 80ms かかる）
        int res1 = task1.get();         // task1 の完了を待って値を取得
        std::string res2 = task2.get(); // task2 の完了を待って値を取得 (既に完了済みの可能性が高い)
        std::cout << "  Task1 result: " << res1 << ", Task2 result: " << res2 << std::endl;
    }

    // ====================================================================
    // 2. std::jthread (C++20 の RAII 安全スレッド)
    // ====================================================================
    std::cout << "\n--- 2. std::jthread (C++20 RAII Auto-Joining Thread) ---\n";
    {
        // jthread のコンストラクタでスレッドを開始する。
        // 従来の std::thread と違い、スコープを抜けるとデストラクタで自動的に join() が呼ばれる。
        // std::thread の場合、join() を忘れると std::terminate() が呼ばれプログラムがクラッシュする。
        std::jthread worker([]() {
            std::cout << "  [jthread] Background worker thread running..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            // このスレッドが終了するとスレッドリソースが解放される
        });

        // メインスレッドはすぐにここに到達する (worker と並行)
        std::cout << "  [Main] Leaving scope: worker.join() will be called automatically" << std::endl;
        // } ← スコープ終了時に worker のデストラクタが worker.join() を呼ぶ
        //    → worker スレッドの完了を待ってからデストラクタが返る
    }

    // ====================================================================
    // 3. std::atomic<T> によるロックフリー並行カウンター
    // ====================================================================
    std::cout << "\n--- 3. std::atomic<T> (Lock-Free Concurrent Counter) ---\n";
    {
        // std::atomic<int>: 複数スレッドから安全に読み書きできる整数型。
        // 通常の int に複数スレッドから同時アクセスするとデータ競合 (UB) になるが、
        // atomic は CPU のハードウェア命令で原子性 (atomicity) を保証する。
        std::atomic<int> counter{0};
        constexpr int NUM_THREADS  = 4;    // スレッド数
        constexpr int ITERATIONS   = 2500; // 各スレッドがカウンターを加算する回数

        // 4スレッドそれぞれが 2500 回加算 → 合計 4 × 2500 = 10000 が期待値
        std::vector<std::jthread> threads;
        for (int i = 0; i < NUM_THREADS; i++) {
            // [&counter]: counter を参照キャプチャ (コピーではなく同じ atomic オブジェクトを共有)
            threads.emplace_back([&counter]() {
                for (int j = 0; j < ITERATIONS; j++) {
                    // fetch_add: アトミックに 1 を加算 (ロック不要、CAS 命令使用)
                    // memory_order_relaxed: 他の操作との順序保証は不要 (カウンタのみ最速)
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }

        // threads.clear() で全 jthread のデストラクタが呼ばれ、全スレッドが join される。
        // clear() が完了するまでメインスレッドはブロックされる。
        // （ここは jthread のスコープアウトによる自動 join と同じ効果）
        threads.clear();

        // 全スレッドが完了した後の最終値を読む
        // counter.load() は現在の値を安全に読み取る (memory_order_seq_cst がデフォルト)
        std::cout << "  Atomic Counter Final Result (Expected 10000): " << counter.load() << std::endl;
        // 注: 通常の int で同じことをすると、データ競合により 10000 以下の不定値になる
    }
}

} // namespace modern_cpp
