#include "move_semantics_and_classes.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <utility>

/**
 * ============================================================================
 * C++ 02: 移動セマンティクスとモダンクラス設計 (Move Semantics & Modern OOP)
 * ============================================================================
 *
 * 【他言語経験者（Rust, C#, Go）向け要点】
 *
 * 1. 移動セマンティクス (Move Semantics - C++11+):
 *    - Rustの「所有権の移動 (Move)」をC++に導入したもの。
 *    - 一時オブジェクトや不要になったオブジェクトのヒープバッファ（ポインタ）を、
 *      ディープコピーせずに「ポインタをすげ替える（盗む）」ことでO(1)で高速移動します。
 *    - `std::move(x)` は実際には何も移動せず、「x を右辺値参照 (x&&) にキャストする」だけ。
 *      実際の移動処理はムーブコンストラクタ/ムーブ代入演算子で実装する必要があります。
 *    - ムーブ後の元オブジェクトは「有効だが不定状態 (valid but unspecified state)」。
 *      ポインタを nullptr にしておくのはベストプラクティス（二重解放防止）。
 *
 * 2. Rule of Zero / Rule of Five:
 *    - 【Rule of Zero】生ポインタを直接保持せず、std::string・std::vector・std::unique_ptr
 *      などのリソース管理クラスをメンバに使えば、コンパイラが以下の5つを完璧に自動生成する。
 *        ① デストラクタ
 *        ② コピーコンストラクタ
 *        ③ コピー代入演算子
 *        ④ ムーブコンストラクタ
 *        ⑤ ムーブ代入演算子
 *    - 【Rule of Five】生ポインタ (int*) を持つ場合など、自分でリソース管理が必要なら
 *      必ず上記の5つ全てを実装する（1つでも書いたら残り4つも書くこと）。
 *
 * 3. 仮想関数とインターフェース:
 *    - 基底クラスのデストラクタには必ず `virtual ~Base() = default;` を付ける。
 *      （理由: Base* ptr で派生クラスを delete した場合、virtual でないと派生クラスの
 *       デストラクタが呼ばれず未定義動作 (UB) になる。）
 *    - 派生クラスのオーバーライドには必ず `override` を明示する。
 *      （理由: 基底クラスのシグネチャと少しでも異なると、新規仮想関数が追加されてしまう。
 *       `override` があればコンパイラが検出してくれる。）
 */

namespace modern_cpp {

// ============================================================================
// DynamicBuffer: 生ポインタで内部バッファを管理するクラス (Rule of Five の見本)
// ============================================================================
// 【教育目的】通常は std::vector<int> を使えばよい。
// ここではムーブセマンティクスの内部動作を可視化するために、
// あえて生ポインタ (int*) を使って手動実装している。
class DynamicBuffer {
private:
    size_t size_;   // バッファのサイズ (要素数)
    int* data_;     // ヒープ上に確保された配列の先頭ポインタ

public:
    // --- 通常コンストラクタ ---
    // size 個の int 配列をヒープ (free store) に確保し、1〜size で初期化する。
    // explicit: 暗黙の型変換を防ぐ (DynamicBuffer buf = 100; のような誤用を禁止)
    explicit DynamicBuffer(size_t size) : size_(size), data_(new int[size]) {
        std::cout << "  [Buffer] Allocated (Size: " << size_ << ")" << std::endl;
        for (size_t i = 0; i < size_; i++) data_[i] = (int)(i + 1);
    }

    // --- デストラクタ ---
    // data_ が nullptr でなければ（まだムーブされていなければ）ヒープメモリを解放する。
    // delete[] と delete の違い: 配列は delete[]、単一オブジェクトは delete を使う。
    ~DynamicBuffer() {
        if (data_ != nullptr) {
            std::cout << "  [Buffer] Freed (Size: " << size_ << ")" << std::endl;
            delete[] data_; // ヒープ配列の解放
        } else {
            // ムーブ後の元オブジェクトは data_ == nullptr なので、解放不要
            std::cout << "  [Buffer] Freed (Already moved, no-op)" << std::endl;
        }
    }

    // --- コピーコンストラクタ (ディープコピー: O(N) で高コスト) ---
    // const DynamicBuffer& を受け取り、メモリを新たに確保して要素を全コピーする。
    // サイズ N のバッファに対して O(N) のコストがかかる（避けられる場面ではムーブを使うべき）。
    DynamicBuffer(const DynamicBuffer& other) : size_(other.size_), data_(new int[other.size_]) {
        std::cout << "  [Buffer] Copy Constructor (Deep Copy occurred!)" << std::endl;
        for (size_t i = 0; i < size_; i++) data_[i] = other.data_[i];
    }

    // --- ムーブコンストラクタ (右辺値参照 &&: ポインタを奪うだけで O(1) 超高速) ---
    // other から data_ ポインタを「盗む」だけなので、バッファサイズに関係なく O(1)。
    // noexcept: ムーブは例外を投げないことを保証する。これにより vector の再確保時に
    //           コピーの代わりにムーブが選ばれ、パフォーマンスが向上する（重要）。
    DynamicBuffer(DynamicBuffer&& other) noexcept : size_(other.size_), data_(other.data_) {
        std::cout << "  [Buffer] Move Constructor (Transferred ownership O(1)!)" << std::endl;
        // 元のオブジェクト (other) を空にする → デストラクタで二重解放されないよう null に
        other.size_ = 0;
        other.data_ = nullptr;
    }

    // --- コピー代入演算子 ---
    // 自己代入 (buf = buf) をチェックし、既存バッファを解放してからディープコピーする。
    DynamicBuffer& operator=(const DynamicBuffer& other) {
        if (this != &other) {  // 自己代入ガード
            delete[] data_;    // 現在保持しているバッファを先に解放
            size_ = other.size_;
            data_ = new int[size_];
            for (size_t i = 0; i < size_; i++) data_[i] = other.data_[i];
        }
        return *this; // 代入チェーン (a = b = c) を可能にするために自身の参照を返す
    }

    // --- ムーブ代入演算子 ---
    // 既存バッファを解放し、other からポインタを奪い取る。
    // noexcept は前述の理由で非常に重要。
    DynamicBuffer& operator=(DynamicBuffer&& other) noexcept {
        if (this != &other) {  // 自己代入ガード (std::move(x) = x のような稀なケースへの対応)
            delete[] data_;    // 現在のバッファを解放
            size_ = other.size_;
            data_ = other.data_; // ポインタをすげ替えるだけ (O(1))
            other.size_ = 0;
            other.data_ = nullptr; // 元オブジェクトを空に
        }
        return *this;
    }

    // const 修飾: このメソッドはメンバを変更しない（読み取り専用）
    size_t size() const { return size_; }
};

// ============================================================================
// ポリモーフィズム (インターフェースと override)
// ============================================================================
// 純粋仮想関数 (= 0) を持つ抽象基底クラス。Rustの trait / C#の abstract class / Javaの interface に相当。
// インスタンスを直接生成することはできない。
class Shape {
public:
    // 【超重要】基底クラスのデストラクタは必ず virtual にする。
    // `= default` はコンパイラが生成するデフォルト実装を使うことを明示する。
    virtual ~Shape() = default;

    // 純粋仮想関数: 派生クラスで必ず実装されなければならない (= 0)
    // const: このメソッド呼び出しによってオブジェクトの状態を変更しない
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

// Shape インターフェースを実装する長方形クラス
class Rectangle : public Shape {
private:
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}

    // override キーワード: 基底クラスの仮想関数を正しくオーバーライドしていることをコンパイラが検証する
    double area() const override { return width_ * height_; }
    std::string name() const override { return "Rectangle"; }
};

// Shape インターフェースを実装する円クラス
class Circle : public Shape {
private:
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.1415926535 * radius_ * radius_; }
    std::string name() const override { return "Circle"; }
};

// ============================================================================
// デモ実行関数
// ============================================================================
void run_move_semantics_demo() {
    // ====================================================================
    // 1. コピー vs ムーブ の性能比較
    // ====================================================================
    std::cout << "--- 1. Copy vs Move (std::move) Performance ---\n";
    {
        // サイズ 1000 のバッファをヒープに確保 (通常コンストラクタ)
        std::cout << "> Creating buf1:\n";
        DynamicBuffer buf1(1000);

        // コピーコンストラクタが呼ばれる: 1000 個の int を全コピー (O(N) コスト)
        // buf1 の所有権は保持されたまま
        std::cout << "\n> Copy creating buf2 (from buf1):\n";
        DynamicBuffer buf2 = buf1; // ディープコピー

        // ムーブコンストラクタが呼ばれる: ポインタのすげ替えのみ (O(1) コスト)
        // buf1 は空 (nullptr) になる
        std::cout << "\n> Move creating buf3 (from std::move(buf1)):\n";
        DynamicBuffer buf3 = std::move(buf1); // ポインタのすげ替えのみ

        // buf1 は移動後なので size は 0 になっている
        std::cout << "  buf1 size (after move): " << buf1.size() << std::endl;
        std::cout << "  buf3 size:              " << buf3.size() << std::endl;

        // スコープアウト時の順番 (逆順): buf3 → buf2 → buf1 の順でデストラクタが呼ばれる
        std::cout << "\n> Leaving scope:\n";
    }

    // ====================================================================
    // 2. ポリモーフィズム (仮想関数 & override)
    // ====================================================================
    std::cout << "\n--- 2. Class Polymorphism (virtual & override) ---\n";
    {
        // unique_ptr<Shape> により多態的オブジェクトをコレクション管理する。
        // これは Rust の Vec<Box<dyn Shape>> に相当する。
        // ヒープ上に派生クラスのオブジェクトを置き、基底クラスのポインタで参照する。
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Rectangle>(10.0, 5.0)); // 10x5 の長方形
        shapes.push_back(std::make_unique<Circle>(3.0));           // 半径3の円

        // const auto& でコピーせずに各要素を参照する（auto& にすると unique_ptr をムーブしてしまう恐れがある）
        for (const auto& s : shapes) {
            // s->area() は仮想関数呼び出し (vtable 経由) → 実行時に正しい派生クラスの実装が呼ばれる
            std::cout << "  Shape: " << s->name() << ", Area: " << s->area() << std::endl;
        }
        // shapes がスコープアウト → unique_ptr が各 Shape オブジェクトを自動 delete
        // Shape の仮想デストラクタにより、派生クラスのデストラクタも正しく呼ばれる
    }
}

} // namespace modern_cpp
