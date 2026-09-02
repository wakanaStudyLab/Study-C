#include "raii_and_smart_pointers.hpp"
#include <iostream>
#include <memory>
#include <string>

/**
 * ============================================================================
 * C++ 01: RAII とスマートポインタ (RAII & Smart Pointers)
 * ============================================================================
 *
 * 【他言語経験者（Rust, C#, Go）向け要点】
 *
 * 1. RAII (Resource Acquisition Is Initialization):
 *    - C++の最も基本的かつ強力な設計思想（Rustの所有権・Dropシステムのベース）。
 *    - 「コンストラクタでリソースを獲得し、スコープを抜けるときにデストラクタ (~ClassName) で自動解放する」。
 *    - 明示的な `free()` や `delete`、`try-finally` は現代C++では一切書きません。
 *    - 例外が発生してスタックが巻き戻される (stack unwind) 場合でもデストラクタは確実に呼ばれます。
 *
 * 2. 生ポインタ vs スマートポインタ:
 *    ┌──────────────────────┬──────────────────────────────────────────────────────────┐
 *    │ 型                   │ 説明                                                     │
 *    ├──────────────────────┼──────────────────────────────────────────────────────────┤
 *    │ std::unique_ptr<T>   │ 単一所有権 (Rustの Box<T>)。コピー不可・ムーブのみ。   │
 *    │                      │ ランタイムオーバーヘッドは生ポインタとほぼ同等 (ゼロコスト) │
 *    ├──────────────────────┼──────────────────────────────────────────────────────────┤
 *    │ std::shared_ptr<T>   │ 参照カウント共有所有権 (Rustの Arc<T> / Rc<T>)。         │
 *    │                      │ コピー時に参照カウントがインクリメントされる。            │
 *    │                      │ カウントが 0 になった瞬間にデストラクタ自動呼び出し。    │
 *    ├──────────────────────┼──────────────────────────────────────────────────────────┤
 *    │ std::weak_ptr<T>     │ shared_ptr の所有権を持たない弱参照。                    │
 *    │                      │ 循環参照 (A→B→A) によるメモリリーク防止に使う。          │
 *    └──────────────────────┴──────────────────────────────────────────────────────────┘
 *
 * 3. 推奨ファクトリ関数:
 *    - `new` は直接使わず、`std::make_unique<T>()` または `std::make_shared<T>()` を使います。
 *    - 理由① 例外安全性: `new T` と `unique_ptr` の構築が一つの式に統合され、
 *      例外が挟まってメモリリークする余地がない。
 *    - 理由② shared_ptr の場合、`make_shared` はオブジェクトと制御ブロック（参照カウント）を
 *      1回のアロケーションでまとめて確保できるため、パフォーマンスが向上する。
 */

namespace modern_cpp {

// ============================================================================
// RAII を体現するリソース管理クラスの例 (ファイルやソケット、DB接続の模擬)
// ============================================================================
// このクラスはファイルオープン/クローズ、ネットワーク接続/切断など
// 「獲得→使用→解放」のライフサイクルを持つリソースを模擬している。
class DatabaseConnection {
private:
    // 接続を識別するためのID文字列（std::string は自分でメモリ管理してくれる）
    std::string connection_id;

public:
    // コンストラクタ: リソースの「獲得」を担当する
    // std::move(id) により、引数の文字列をコピーせず内部へ「移動」している（効率化）
    explicit DatabaseConnection(std::string id) : connection_id(std::move(id)) {
        std::cout << "  [DB] Connection established: " << connection_id << std::endl;
    }

    // デストラクタ: スコープを抜けた瞬間（または例外が投げられた際）に確実に自動実行される
    // これが RAII の核心。Rust の Drop::drop() と同等の役割を持つ。
    // 「~」がデストラクタのシグネチャ。仮想関数ではないが、基底クラスでは virtual にすること（後述）。
    ~DatabaseConnection() {
        std::cout << "  [DB] Connection closed (Destructor): " << connection_id << std::endl;
    }

    // const 修飾子: このメソッドはオブジェクトのメンバを変更しないことを保証する
    // Rust の &self (不変参照レシーバ) に相当
    void query(const std::string& sql) const {
        std::cout << "  [DB] Executing query (" << connection_id << "): " << sql << std::endl;
    }
};

void run_raii_and_smart_pointers_demo() {
    // ====================================================================
    // 1. RAII - 自動スコープ管理
    // ====================================================================
    std::cout << "--- 1. RAII (Automatic Scope Resource Management) ---\n";
    {
        // スタック上に DatabaseConnection を構築。コンストラクタでDB接続が確立される。
        DatabaseConnection conn("local-postgres-01");
        conn.query("SELECT * FROM users");

        std::cout << "  Leaving scope..." << std::endl;
        // } ← このブレースでスコープが終了し、conn のデストラクタが「自動」で呼ばれる。
        // delete conn; などは一切不要。例外が発生した場合でも確実に解放される。
    }

    // ====================================================================
    // 2. std::unique_ptr - 単一所有権スマートポインタ (Rust の Box<T>)
    // ====================================================================
    std::cout << "\n--- 2. std::unique_ptr (Single Ownership / Rust Box<T>) ---\n";
    {
        // make_unique でヒープ上に DatabaseConnection を安全に確保。
        // 型推論 (auto) により `std::unique_ptr<DatabaseConnection>` と明記しなくてよい。
        auto u1 = std::make_unique<DatabaseConnection>("unique-db-conn");
        u1->query("INSERT INTO logs VALUES ('login')");

        // 【コンパイルエラー例】unique_ptr はコピー禁止。所有権が一意であるため。
        // auto u2 = u1; // ERROR: use of deleted function (copy constructor)

        // 所有権の「移動」(Move Ownership)
        // std::move(u1) は u1 を「右辺値参照」にキャストする。
        // これにより u2 が所有権を引き継ぎ、u1 は nullptr になる。
        // Rust の「所有権の move」と全く同じ考え方。
        std::unique_ptr<DatabaseConnection> u2 = std::move(u1);
        std::cout << "  After move: u1 is " << (u1 ? "valid" : "nullptr (empty)") << std::endl;
        u2->query("SELECT COUNT(*) FROM logs");
    } // u2 がスコープアウト → デストラクタが自動呼び出され、DB接続が解放される

    // ====================================================================
    // 3. std::shared_ptr - 参照カウント共有所有権 (Rust の Arc<T>)
    // ====================================================================
    std::cout << "\n--- 3. std::shared_ptr (Reference Counting / Rust Arc<T>) ---\n";
    {
        // make_shared はオブジェクトと参照カウント制御ブロックを1回のアロケーションで確保する（高速）。
        std::shared_ptr<DatabaseConnection> s1 = std::make_shared<DatabaseConnection>("shared-db-conn");
        std::cout << "  Reference count (after s1 creation): " << s1.use_count() << std::endl; // 期待値: 1

        {
            // コピーによって同じオブジェクトを s2 でも指す（参照カウント +1）。
            // Rust の Arc::clone() と同様に、内部でアトミックインクリメントが発生する。
            std::shared_ptr<DatabaseConnection> s2 = s1;
            std::cout << "  Reference count (after s2 copy):     " << s1.use_count() << std::endl; // 期待値: 2
            s2->query("UPDATE settings SET theme = 'dark'");
        } // s2 がスコープアウト → 参照カウント -1 になるが、まだ s1 が保持しているので解放されない

        std::cout << "  Reference count (after s2 scope out):" << s1.use_count() << std::endl; // 期待値: 1
    } // s1 がスコープアウト → 参照カウントが 0 になり、ここで初めてデストラクタが呼ばれる
}

} // namespace modern_cpp
