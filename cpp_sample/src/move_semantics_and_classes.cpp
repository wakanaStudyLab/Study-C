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
 * 1. 移動セマンティクス (Move Semantics - C++11+):
 *    - Rustの「所有権の移動 (Move)」をC++に導入したもの。
 *    - 一時オブジェクトや不要になったオブジェクトのヒープバッファ（ポインタ）を、
 *      ディープコピーせずに「ポインタをすげ替える（盗む）」ことでO(1)で高速移動します。
 *    - `std::move(x)` は実際には何も移動せず、「x を右辺値参照 (x&&) にキャストする」だけの関数です。
 * 
 * 2. Rule of Zero (ゼロの法則):
 *    - 生ポインタを持たず、`std::string` や `std::vector`, `std::unique_ptr` をメンバに持つクラスは、
 *      デストラクタ・コピー・ムーブの5つ (Rule of 5) を自分で書く必要がありません（コンパイラが完璧に自動生成）。
 * 
 * 3. 仮想関数とインターフェース:
 *    - 基底クラスのデストラクタには必ず `virtual ~Base() = default;` を付ける（多態的破棄時の未定義動作防止）。
 *    - 派生クラスのオーバーライドには必ず `override` を明示する。
 */

namespace modern_cpp {

// 内部でバッファを持つクラス（ムーブの挙動を可視化するための手動実装）
class DynamicBuffer {
private:
    size_t size_;
    int* data_;

public:
    // 通常コンストラクタ
    explicit DynamicBuffer(size_t size) : size_(size), data_(new int[size]) {
        std::cout << "  [Buffer] Allocated (Size: " << size_ << ")" << std::endl;
        for (size_t i = 0; i < size_; i++) data_[i] = (int)(i + 1);
    }

    // デストラクタ
    ~DynamicBuffer() {
        if (data_ != nullptr) {
            std::cout << "  [Buffer] Freed (Size: " << size_ << ")" << std::endl;
            delete[] data_;
        } else {
            std::cout << "  [Buffer] Freed (Already moved, no-op)" << std::endl;
        }
    }

    // コピーコンストラクタ (ディープコピー: 高コスト)
    DynamicBuffer(const DynamicBuffer& other) : size_(other.size_), data_(new int[other.size_]) {
        std::cout << "  [Buffer] Copy Constructor (Deep Copy occurred!)" << std::endl;
        for (size_t i = 0; i < size_; i++) data_[i] = other.data_[i];
    }

    // ムーブコンストラクタ (右辺値参照 && : ポインタを奪うだけで超高速 O(1))
    DynamicBuffer(DynamicBuffer&& other) noexcept : size_(other.size_), data_(other.data_) {
        std::cout << "  [Buffer] Move Constructor (Transferred ownership O(1)!)" << std::endl;
        // 元のオブジェクトを空にする (二重解放防止)
        other.size_ = 0;
        other.data_ = nullptr;
    }

    // コピー代入演算子
    DynamicBuffer& operator=(const DynamicBuffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            for (size_t i = 0; i < size_; i++) data_[i] = other.data_[i];
        }
        return *this;
    }

    // ムーブ代入演算子
    DynamicBuffer& operator=(DynamicBuffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    size_t size() const { return size_; }
};

// ============================================================================
// ポリモーフィズム (インターフェースと override)
// ============================================================================
class Shape {
public:
    virtual ~Shape() = default; // 【超重要】基底クラスのデストラクタは必ず virtual
    virtual double area() const = 0; // 純粋仮想関数 (Rustのtrait method / C#のabstract method)
    virtual std::string name() const = 0;
};

class Rectangle : public Shape {
private:
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    double area() const override { return width_ * height_; }
    std::string name() const override { return "Rectangle"; }
};

class Circle : public Shape {
private:
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.1415926535 * radius_ * radius_; }
    std::string name() const override { return "Circle"; }
};

void run_move_semantics_demo() {
    std::cout << "--- 1. Copy vs Move (std::move) Performance ---\n";
    {
        std::cout << "> Creating buf1:\n";
        DynamicBuffer buf1(1000);

        std::cout << "\n> Copy creating buf2 (from buf1):\n";
        DynamicBuffer buf2 = buf1; // ディープコピー

        std::cout << "\n> Move creating buf3 (from std::move(buf1)):\n";
        DynamicBuffer buf3 = std::move(buf1); // ポインタのすげ替えのみ

        std::cout << "  buf1 size (after move): " << buf1.size() << std::endl;
        std::cout << "  buf3 size:              " << buf3.size() << std::endl;
        std::cout << "\n> Leaving scope:\n";
    }

    std::cout << "\n--- 2. Class Polymorphism (virtual & override) ---\n";
    {
        // unique_ptr による多態的オブジェクトのコレクション (Rustの Vec<Box<dyn Shape>> 相当)
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Rectangle>(10.0, 5.0));
        shapes.push_back(std::make_unique<Circle>(3.0));

        for (const auto& s : shapes) {
            std::cout << "  Shape: " << s->name() << ", Area: " << s->area() << std::endl;
        }
    }
}

} // namespace modern_cpp
