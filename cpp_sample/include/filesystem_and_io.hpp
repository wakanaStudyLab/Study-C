#ifndef FILESYSTEM_AND_IO_HPP
#define FILESYSTEM_AND_IO_HPP

#include <filesystem>
#include <string>
#include <string_view>

/**
 * ============================================================================
 * モジュール 12: ファイルシステム・バイナリI/O・C++23 std::print
 * ============================================================================
 * 
 * 【他言語経験者（Rust, C#, Go）向け要点】
 * 1. std::filesystem (C++17 / Rust std::fs, Go os/filepath 相当):
 *    - OS 依存のファイルパス区切り文字（Windows の `\`、Linux の `/`）を
 *      演算子 `/` でプラットフォーム非依存に自動吸収。
 *    - ディレクトリの再帰走査やファイル属性チェックを簡単・安全に行える。
 * 
 * 2. C++23 std::print / std::println:
 *    - `std::cout` のストリーム構文（`<<` のタイプ量が多くバッファリングが遅い）と
 *      C言語の `printf`（型安全性がなくバッファオーバーランの危険）の双方の欠点を解消。
 *    - Python の f-string や Rust の `println!` に匹敵する高速・型安全な最新標準出力。
 */

namespace modern_cpp {

// モジュール実行エントリポイント
void run_filesystem_and_io_demo();

} // namespace modern_cpp

#endif // FILESYSTEM_AND_IO_HPP
