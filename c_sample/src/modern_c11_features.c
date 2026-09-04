#include "modern_c11_features.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 静的アサーション (C11 _Static_assert)
// 64bit システムでのポインタサイズ検証
_Static_assert(sizeof(void*) == 8, "This codebase requires a 64-bit architecture!");

// Vector3 のサイズが float 3つ分 (12バイト) であることをコンパイル時に保証
_Static_assert(sizeof(Vector3) == sizeof(float) * 3, "Vector3 layout must be tightly packed!");

const char* format_type_int(int val) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "[int: %d]", val);
    return buf;
}

const char* format_type_double(double val) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "[double: %.4f]", val);
    return buf;
}

const char* format_type_string(const char* val) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "[string: \"%s\"]", val);
    return buf;
}

const char* format_type_default(void) {
    return "[unknown type]";
}

NetworkPacket* packet_create(uint32_t id, const uint8_t* data, size_t len) {
    // 構造体ヘッダサイズ + ペイロード分を一括 malloc で確保
    NetworkPacket* p = (NetworkPacket*)malloc(sizeof(NetworkPacket) + len);
    if (!p) return NULL;

    p->packet_id = id;
    p->payload_len = len;
    if (data && len > 0) {
        memcpy(p->payload, data, len);
    }
    return p;
}

void packet_free(NetworkPacket* packet) {
    // 単一の malloc で確保されたため、free も1回で完了 (キャッシュローカリティが高い)
    free(packet);
}

void run_modern_c11_features(void) {
    printf("================================================================\n");
    printf("  Module 05: Modern C11/C23 Features (_Generic, Anonymous Structs)\n");
    printf("================================================================\n\n");

    // 1. C11 _Generic による型分岐
    printf("--- 1. C11 _Generic Compile-time Type Dispatch ---\n");
    int i = 42;
    double d = 3.14159;
    const char* str = "Modern C11";

    printf("  describe_type(i):   %s\n", describe_type(i));
    printf("  describe_type(d):   %s\n", describe_type(d));
    printf("  describe_type(str): %s\n", describe_type(str));

    // 2. 匿名構造体 & 匿名共用体
    printf("\n--- 2. Anonymous Structs and Unions (Vector3 Access) ---\n");
    Vector3 v = { .x = 10.0f, .y = 20.0f, .z = 30.0f };
    printf("  Direct member access: x=%.1f, y=%.1f, z=%.1f\n", v.x, v.y, v.z);

    // 配列経由でのアクセス (同一アドレス)
    printf("  Array indexing access: raw[0]=%.1f, raw[1]=%.1f, raw[2]=%.1f\n",
           v.raw[0], v.raw[1], v.raw[2]);
    v.raw[1] = 99.0f; // 配列経由で書き換えると .y も変わる
    printf("  After modifying raw[1]: v.y is now %.1f\n", v.y);

    // 3. フレキシブル配列メンバ
    printf("\n--- 3. Flexible Array Members (Single-Allocation Packet) ---\n");
    const char* payload_text = "PING_ACK_OK";
    size_t payload_size = strlen(payload_text) + 1;

    NetworkPacket* pkt = packet_create(1001, (const uint8_t*)payload_text, payload_size);
    if (pkt) {
        printf("  Packet ID: %u, Payload Len: %zu\n", pkt->packet_id, pkt->payload_len);
        printf("  Payload Data: \"%s\"\n", (const char*)pkt->payload);
        printf("  Total packet allocation size: %zu bytes (Header: %zu, Payload: %zu)\n",
               sizeof(NetworkPacket) + pkt->payload_len, sizeof(NetworkPacket), pkt->payload_len);
        packet_free(pkt);
    }
}
