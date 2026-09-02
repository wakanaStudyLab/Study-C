#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lambda_and_callbacks.h"

/**
 * ============================================================================
 * C言語における「ラムダ式」「クロージャ」「コールバック」の実践実装
 * ============================================================================
 * 
 * C言語には言語仕様としてのラムダ式はありませんが、
 * 「関数ポインタ」と「コンテキストポインタ (void* user_data)」を組み合わせることで、
 * 現代の高水準言語のクロージャと全く同じ動作をマシン語レベルで実現できます。
 */

/* ------------------------------------------------------------------------- */
/* 1. 基本：ステートレスな関数ポインタ (Stateless Function Pointer)          */
/* ------------------------------------------------------------------------- */
typedef int (*BinaryOp)(int, int);

static int op_add(int a, int b) { return a + b; }
static int op_multiply(int a, int b) { return a * b; }

static int compute(int a, int b, BinaryOp op) {
    return op(a, b);
}

static void demo_stateless_function_pointers(void) {
    printf("=== 1. Basic Function Pointers (Stateless) ===\n");
    printf("Compute Add(10, 20):      %d\n", compute(10, 20, op_add));
    printf("Compute Multiply(10, 20): %d\n", compute(10, 20, op_multiply));
}

/* ------------------------------------------------------------------------- */
/* 2. 状態のキャプチャ：void* user_data パターン (Stateful Callback)          */
/* ------------------------------------------------------------------------- */
/* コールバックが外側の変数（環境）にアクセスするための標準シグネチャ */
typedef bool (*PredicateWithContext)(int value, void* user_data);

/* フィルタ処理を行う高階関数 */
static void filter_with_context(const int* src, size_t len,
                                PredicateWithContext pred, void* user_data,
                                int* out_dst, size_t* out_len) {
    size_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        if (pred(src[i], user_data)) {
            out_dst[count++] = src[i];
        }
    }
    *out_len = count;
}

/* 「しきい値より大きいか」判定する述語関数（threshold をキャプチャ） */
static bool is_greater_than(int val, void* user_data) {
    int threshold = *(const int*)user_data;
    return val > threshold;
}

/* 「特定の倍数か」判定する述語関数（divisor をキャプチャ） */
static bool is_multiple_of(int val, void* user_data) {
    int divisor = *(const int*)user_data;
    return (val % divisor) == 0;
}

static void demo_contextual_callbacks(void) {
    printf("\n=== 2. State Capture via void* user_data ===\n");
    int data[] = { 5, 12, 18, 25, 30, 42, 55 };
    size_t data_len = sizeof(data) / sizeof(data[0]);

    int result[7];
    size_t result_len = 0;

    // 外側のローカル変数（キャプチャ対象）
    int threshold = 20;
    filter_with_context(data, data_len, is_greater_than, &threshold, result, &result_len);

    printf("Numbers > %d: [", threshold);
    for (size_t i = 0; i < result_len; ++i) {
        printf("%d%s", result[i], (i + 1 < result_len) ? ", " : "");
    }
    printf("]\n");

    int divisor = 6;
    filter_with_context(data, data_len, is_multiple_of, &divisor, result, &result_len);

    printf("Multiples of %d: [", divisor);
    for (size_t i = 0; i < result_len; ++i) {
        printf("%d%s", result[i], (i + 1 < result_len) ? ", " : "");
    }
    printf("]\n");
}

/* ------------------------------------------------------------------------- */
/* 3. クロージャの物理構造体 (Closure struct: Function Pointer + Env)        */
/* ------------------------------------------------------------------------- */
/* 他言語のクロージャが裏で生成しているデータ構造そのもの */
typedef struct {
    void (*invoke)(void* env, const char* message);
    void* env;
} StringConsumerClosure;

/* キャプチャしたい環境 */
typedef struct {
    char prefix[32];
    int call_count;
} LoggerEnv;

static void logger_impl(void* raw_env, const char* message) {
    LoggerEnv* env = (LoggerEnv*)raw_env;
    env->call_count++;
    printf("  %s [#%d]: %s\n", env->prefix, env->call_count, message);
}

static void demo_closure_structure(void) {
    printf("\n=== 3. Explicit Closure Struct (fn pointer + env) ===\n");

    LoggerEnv env;
    snprintf(env.prefix, sizeof(env.prefix), "%s", "[API Gateway]");
    env.call_count = 0;

    StringConsumerClosure logger = {
        .invoke = logger_impl,
        .env = &env
    };

    logger.invoke(logger.env, "Incoming request: GET /users");
    logger.invoke(logger.env, "Authenticated user ID: 42");
    logger.invoke(logger.env, "Response sent: 200 OK");
    printf("Total logs dispatched: %d\n", env.call_count);
}

/* ------------------------------------------------------------------------- */
/* 4. 安全な非同期・遅延実行ライフサイクル (Heap Allocation Pattern)         */
/* ------------------------------------------------------------------------- */
typedef struct {
    int task_id;
    char payload[64];
} TaskContext;

typedef void (*TaskCallback)(TaskContext* ctx);

static void execute_delayed_task(TaskCallback cb, TaskContext* ctx) {
    // 擬似的な遅延実行 / 別スレッド実行のシミュレーション
    cb(ctx);
    // 実行完了後に責任を持ってヒープ解放
    free(ctx);
}

static void my_task_handler(TaskContext* ctx) {
    printf("  Executing Heap Task ID=%d, Payload='%s'\n", ctx->task_id, ctx->payload);
}

static void demo_heap_context_lifecycle(void) {
    printf("\n=== 4. Safe Context Lifecycle (Heap Pattern) ===\n");

    // スタックのアドレス &ctx ではなく、ヒープに確保して渡すことでダングリングを防止
    TaskContext* ctx = (TaskContext*)malloc(sizeof(TaskContext));
    if (ctx != NULL) {
        ctx->task_id = 1001;
        snprintf(ctx->payload, sizeof(ctx->payload), "Background Sync Operation");
        execute_delayed_task(my_task_handler, ctx);
    }
}

/* ------------------------------------------------------------------------- */
/* モジュールエントリーポイント                                              */
/* ------------------------------------------------------------------------- */
void run_lambda_and_callbacks_demo(void) {
    demo_stateless_function_pointers();
    demo_contextual_callbacks();
    demo_closure_structure();
    demo_heap_context_lifecycle();
}
