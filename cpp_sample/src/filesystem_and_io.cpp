#include "filesystem_and_io.hpp"
#include <iostream>
#include <fstream>
#include <print>
#include <vector>
#include <system_error>

/**
 * ============================================================================
 * モジュール 12: ファイルシステム・バイナリI/O・C++23 std::print
 * ============================================================================
 */

namespace modern_cpp {

namespace fs = std::filesystem;

void run_filesystem_and_io_demo() {
    std::println("--- 1. C++23 std::print and std::println (Modern Standard I/O) ---");
    std::println("  std::println format demo: Integer={}, Hex=0x{:04X}, Float={:.3f}", 42, 255, 3.14159);

    std::println("\n--- 2. std::filesystem (Path Manipulation & Directory Operations) ---");
    // パス結合演算子 / の使用
    fs::path temp_dir = fs::temp_directory_path() / "cpp_sample_sandbox";
    std::error_code ec;

    // ディレクトリ作成
    fs::create_directories(temp_dir, ec);
    std::println("  Created sandbox directory: {}", temp_dir.string());

    // パス分解
    fs::path sample_file = temp_dir / "sample_data.bin";
    std::println("  Sample file path details:");
    std::println("    - Filename:  {}", sample_file.filename().string());
    std::println("    - Extension: {}", sample_file.extension().string());
    std::println("    - Parent:    {}", sample_file.parent_path().string());

    std::println("\n--- 3. Type-Safe Binary File I/O ---");
    // バイナリデータの書き込み
    struct Packet {
        uint32_t magic;
        uint32_t payload_len;
        char data[16];
    };

    Packet out_packet{0xDEADBEEF, 8, "HelloBin"};
    {
        std::ofstream ofs(sample_file, std::ios::binary);
        if (ofs) {
            ofs.write(reinterpret_cast<const char*>(&out_packet), sizeof(out_packet));
            std::println("  Successfully wrote {} bytes of binary packet to file.", sizeof(out_packet));
        }
    }

    // ファイルサイズ取得
    if (fs::exists(sample_file)) {
        auto sz = fs::file_size(sample_file);
        std::println("  Verification via fs::file_size: {} bytes on disk", sz);
    }

    // バイナリデータの読み込み
    Packet in_packet{};
    {
        std::ifstream ifs(sample_file, std::ios::binary);
        if (ifs) {
            ifs.read(reinterpret_cast<char*>(&in_packet), sizeof(in_packet));
            std::println("  Read Packet: Magic=0x{:08X}, PayloadLen={}, Data='{}'", 
                         in_packet.magic, in_packet.payload_len, in_packet.data);
        }
    }

    // ディレクトリ走査
    std::println("  Scanning contents of sandbox directory:");
    for (const auto& entry : fs::directory_iterator(temp_dir)) {
        std::println("    * Found: {} (Size: {} bytes)", 
                     entry.path().filename().string(), 
                     entry.is_regular_file() ? entry.file_size() : 0);
    }

    // クリーンアップ
    fs::remove_all(temp_dir, ec);
    std::println("  Cleaned up sandbox directory.");
}

} // namespace modern_cpp
