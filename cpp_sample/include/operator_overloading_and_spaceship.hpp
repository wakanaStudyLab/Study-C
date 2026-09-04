#ifndef OPERATOR_OVERLOADING_AND_SPACESHIP_HPP
#define OPERATOR_OVERLOADING_AND_SPACESHIP_HPP

#include <compare>
#include <iostream>
#include <vector>
#include <string>

/**
 * ============================================================================
 * モジュール 10: 演算子オーバーロードと C++20 宇宙船演算子 (<=>)
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. C++20 三方比較演算子 (Spaceship Operator <=> / Rust の Ord::cmp 相当):
 *    - `<=>` を 1 つ定義する（または `= default` にする）だけで、
 *      6 つの比較演算子（==, !=, <, <=, >, >=）がすべてコンパイラにより自動導出される！
 *    - 比較結果は std::strong_ordering (同値, 未満, 超過) などの型を返す。
 * 
 * 2. 算術演算子とストリーム出力:
 *    - `operator+`, `operator+=` を定義することで自然な数式表現が可能。
 *    - `operator<<` をオーバーロードして `std::cout` に直接流し込める。
 * 
 * 3. カスタムイテレータと Range-based for:
 *    - `begin()` と `end()` を実装するだけで、自作コレクションが
 *      `for (auto& item : my_collection)` や Ranges パイプラインで利用可能になる。
 */

namespace modern_cpp {

// 1. 3Dベクトルクラス (算術演算子 + ストリーム演算子)
class Vector3D {
public:
    double x, y, z;

    Vector3D(double x_val = 0.0, double y_val = 0.0, double z_val = 0.0)
        : x(x_val), y(y_val), z(z_val) {}

    // 加算代入演算子 (+=)
    Vector3D& operator+=(const Vector3D& rhs);

    // 二項加算演算子 (+)
    friend Vector3D operator+(Vector3D lhs, const Vector3D& rhs);

    // 出力ストリーム演算子 (<<)
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v);
};

// 2. C++20 宇宙船演算子 (<=>) による自動比較導出クラス
class PlayerScore {
public:
    std::string name;
    int level;
    int points;

    PlayerScore(std::string n, int lvl, int pts)
        : name(std::move(n)), level(lvl), points(pts) {}

    // = default により、メンバ定義順（name -> level -> points）で
    // 辞書順の完全な三方比較と等価性比較が自動生成される！
    auto operator<=>(const PlayerScore& other) const = default;
};

// 3. 簡易自作コレクションとカスタムイテレータ
template <typename T>
class SimpleBuffer {
private:
    std::vector<T> data_;

public:
    void add(T val) { data_.push_back(std::move(val)); }

    // begin / end を提供することで Range-based for に対応
    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }

    // 添字演算子 [] のオーバーロード
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    size_t size() const { return data_.size(); }
};

// モジュール実行エントリポイント
void run_operator_overloading_demo();

} // namespace modern_cpp

#endif // OPERATOR_OVERLOADING_AND_SPACESHIP_HPP
