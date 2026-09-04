#include "bitwise_and_binary.h"
#include <stdio.h>

/**
 * ============================================================================
 * モジュール 06: ビット演算・エンディアン・バイナリ操作 (Bitwise & Binary)
 * ============================================================================
 */

bool is_system_little_endian(void) {
    // 16ビット値 0x0001 の先頭1バイトが 1 ならリトルエンディアン
    uint16_t test = 0x0001;
    uint8_t* byte_ptr = (uint8_t*)&test;
    return *byte_ptr == 1;
}

uint32_t swap_endian32(uint32_t val) {
    return ((val & 0x000000FFU) << 24) |
           ((val & 0x0000FF00U) << 8)  |
           ((val & 0x00FF0000U) >> 8)  |
           ((val & 0xFF000000U) >> 24);
}

void print_hex_dump(const void* ptr, size_t size) {
    const uint8_t* bytes = (const uint8_t*)ptr;
    printf("[Hex Dump (%zu bytes)]: ", size);
    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

void run_bitwise_and_binary_demo(void) {
    printf("--- 1. Bitwise Flags Manipulation (Bitmasks) ---\n");
    uint32_t permissions = 0;

    // フラグのセット (OR)
    permissions |= PERM_READ | PERM_WRITE;
    printf("  Initial perms (Read | Write): 0x%02X\n", permissions);

    // フラグの判定 (AND)
    if (permissions & PERM_READ) {
        printf("  - Has READ permission: YES\n");
    }
    if (!(permissions & PERM_EXECUTE)) {
        printf("  - Has EXECUTE permission: NO\n");
    }

    // フラグの反転 (XOR)
    permissions ^= PERM_WRITE; // Write を OFF
    printf("  After toggling WRITE (XOR): 0x%02X (Has write: %s)\n", 
           permissions, (permissions & PERM_WRITE) ? "YES" : "NO");

    // フラグのクリア (AND NOT)
    permissions &= ~PERM_READ; // Read をクリア
    printf("  After clearing READ (AND NOT): 0x%02X\n", permissions);

    printf("\n--- 2. C Bitfields (Hardware Register / Protocol Header Simulation) ---\n");
    UserStatusFlags flags = {0};
    flags.is_active = 1;
    flags.is_verified = 0;
    flags.is_admin = 1;
    flags.access_tier = 25; // 5bit (最大31)

    printf("  sizeof(UserStatusFlags): %zu byte(s) (Packed into minimal space)\n", sizeof(UserStatusFlags));
    printf("  Flags state: active=%u, verified=%u, admin=%u, tier=%u\n",
           flags.is_active, flags.is_verified, flags.is_admin, flags.access_tier);

    printf("\n--- 3. Endianness Detection & Byte Swapping ---\n");
    bool is_le = is_system_little_endian();
    printf("  Host CPU Architecture: %s-Endian\n", is_le ? "Little" : "Big");

    uint32_t original_val = 0x12345678;
    printf("  Original 32-bit integer: 0x%08X\n", original_val);
    printf("  Memory layout of original: ");
    print_hex_dump(&original_val, sizeof(original_val));

    uint32_t swapped = swap_endian32(original_val);
    printf("  Swapped 32-bit integer:  0x%08X\n", swapped);
    printf("  Memory layout of swapped:  ");
    print_hex_dump(&swapped, sizeof(swapped));
}
