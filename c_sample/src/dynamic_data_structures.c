#include "dynamic_data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ============================================================================
 * モジュール 09: 動的データ構造 (Dynamic Data Structures & container_of)
 * ============================================================================
 */

IntVector* int_vector_create(size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 4;
    IntVector* vec = (IntVector*)malloc(sizeof(IntVector));
    if (!vec) return NULL;

    vec->data = (int*)malloc(initial_capacity * sizeof(int));
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    vec->size = 0;
    vec->capacity = initial_capacity;
    return vec;
}

bool int_vector_push_back(IntVector* vec, int value) {
    if (!vec) return false;

    // 容量がいっぱいになったら 2 倍に拡張 (Amortized O(1))
    if (vec->size >= vec->capacity) {
        size_t new_capacity = vec->capacity * 2;
        // 重要: realloc の戻り値を直接 vec->data に受けてはならない！
        // 失敗した場合に元のポインタが NULL で上書きされメモリリークするため。
        int* new_data = (int*)realloc(vec->data, new_capacity * sizeof(int));
        if (!new_data) {
            return false; // メモリ不足
        }
        vec->data = new_data;
        vec->capacity = new_capacity;
    }

    vec->data[vec->size++] = value;
    return true;
}

int int_vector_get(const IntVector* vec, size_t index) {
    if (!vec || index >= vec->size) return -1;
    return vec->data[index];
}

void int_vector_free(IntVector* vec) {
    if (vec) {
        free(vec->data);
        free(vec);
    }
}

void run_dynamic_data_structures_demo(void) {
    printf("--- 1. Manual Dynamic Array (Vector) with Capacity Doubling ---\n");
    IntVector* vec = int_vector_create(2);
    printf("  Initial vector: size = %zu, capacity = %zu\n", vec->size, vec->capacity);

    for (int i = 1; i <= 6; ++i) {
        int_vector_push_back(vec, i * 100);
        printf("  Pushed %d -> size = %zu, capacity = %zu\n", i * 100, vec->size, vec->capacity);
    }

    printf("  Vector contents: [ ");
    for (size_t i = 0; i < vec->size; ++i) {
        printf("%d ", int_vector_get(vec, i));
    }
    printf("]\n");
    int_vector_free(vec);
    printf("  Vector memory successfully freed.\n");

    printf("\n--- 2. Intrusive Linked List Pattern (Linux Kernel 'container_of') ---\n");
    // スタック上に 3 つのタスクを作成
    UserTask task1 = {101, "Compile Kernel", {NULL}};
    UserTask task2 = {102, "Run Tests", {NULL}};
    UserTask task3 = {103, "Deploy Binary", {NULL}};

    // 侵入型ノード同士をチェーン
    ListNode* head = &task1.list_node;
    task1.list_node.next = &task2.list_node;
    task2.list_node.next = &task3.list_node;
    task3.list_node.next = NULL;

    // リストをトラバースしながら container_of で親構造体を復元！
    ListNode* curr = head;
    while (curr != NULL) {
        // ListNode* のアドレスから、親の UserTask* の先頭アドレスを逆算
        UserTask* task = container_of(curr, UserTask, list_node);
        printf("  [Task #%d]: '%s' (Offset of list_node in UserTask: %zu bytes)\n",
               task->user_id, task->name, offsetof(UserTask, list_node));
        curr = curr->next;
    }
}
