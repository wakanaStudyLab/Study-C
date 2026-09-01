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
 * 1. RAII (Resource Acquisition Is Initialization):
 *    - C++の最も基本的かつ強力な設計思想（Rustの所有権・Dropシステムのベース）。
 *    - 「コンストラクタでリソースを獲得し、スコープを抜けるときにデストラクタ (~ClassName) で自動解放する」。
 *    - 明示的な `free()` や `delete`、`try-finally` は現代C++では一切書きません。
 * 
 * 2. 生ポインタ vs スマートポインタ:
 *    - `std::unique_ptr<T>`: 単一所有権 (Rustの `Box<T>`)。コピー不可、ムーブ (`std::move`) のみ可能。ゼロオーバーヘッド。
 *    - `std::shared_ptr<T>`: 参照カウント共有所有権 (Rustの `Arc<T>` / `Rc<T>`)。
 *    - `std::weak_ptr<T>`: 循環参照を防ぐための非所有の弱参照。
 * 
 * 3. 推奨ファクトリ関数:
 *    - `new` は直接使わず、`std::make_unique<T>()` または `std::make_shared<T>()` を使います。
 */

namespace modern_cpp {

// RAII を体現するリソース管理クラスの例 (ファイルやソケットの模擬)
class DatabaseConnection {
private:
    std::string connection_id;

public:
    explicit DatabaseConnection(std::string id) : connection_id(std::move(id)) {
        std::cout << "  [DB] Connection established: " << connection_id << std::endl;
    }

    ~DatabaseConnection() {
        // スコープを抜けた瞬間（または例外が投げられた際）に確実に自動実行される (Rustの Drop)
        std::cout << "  [DB] Connection closed (Destructor): " << connection_id << std::endl;
    }

    void query(const std::string& sql) const {
        std::cout << "  [DB] Executing query (" << connection_id << "): " << sql << std::endl;
    }
};

void run_raii_and_smart_pointers_demo() {
    std::cout << "--- 1. RAII (Automatic Scope Resource Management) ---\n";
    {
        DatabaseConnection conn("local-postgres-01");
        conn.query("SELECT * FROM users");
        std::cout << "  Leaving scope..." << std::endl;
    } // ここでデストラクタが自動的に呼ばれる

    std::cout << "\n--- 2. std::unique_ptr (Single Ownership / Rust Box<T>) ---\n";
    {
        // make_unique で安全にヒープ確保
        auto u1 = std::make_unique<DatabaseConnection>("unique-db-conn");
        u1->query("INSERT INTO logs VALUES ('login')");

        // std::unique_ptr はコピー禁止 (コンパイルエラーになる)
        // auto u2 = u1; // ERROR!

        // 所有権の移動 (Move Ownership / Rustと同様)
        std::unique_ptr<DatabaseConnection> u2 = std::move(u1);
        std::cout << "  After move: u1 is " << (u1 ? "valid" : "nullptr (empty)") << std::endl;
        u2->query("SELECT COUNT(*) FROM logs");
    } // u2 が破棄され、ここでデストラクタ呼び出し

    std::cout << "\n--- 3. std::shared_ptr (Reference Counting / Rust Arc<T>) ---\n";
    {
        std::shared_ptr<DatabaseConnection> s1 = std::make_shared<DatabaseConnection>("shared-db-conn");
        std::cout << "  Reference count (after s1 creation): " << s1.use_count() << std::endl;

        {
            std::shared_ptr<DatabaseConnection> s2 = s1; // コピー可能 (参照カウント+1)
            std::cout << "  Reference count (after s2 copy):     " << s1.use_count() << std::endl;
            s2->query("UPDATE settings SET theme = 'dark'");
        } // s2 がスコープアウト (参照カウント-1)

        std::cout << "  Reference count (after s2 scope out):" << s1.use_count() << std::endl;
    } // 参照カウントが0になり、ここでデストラクタ呼び出し
}

} // namespace modern_cpp
