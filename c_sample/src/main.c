#include <stdio.h>
#include "pointers_and_memory.h"
#include "structs_and_functions.h"
#include "strings_and_io.h"

/**
 * ============================================================================
 * C言語 最速習得コース (For Rust / C# / Go Developers) - Main Entry
 * ============================================================================
 */

static void print_banner(const char* title) {
    printf("\n================================================================\n");
    printf("  %s\n", title);
    printf("================================================================\n\n");
}

static void print_section(const char* title) {
    printf("\n################################################################\n");
    printf("# %s\n", title);
    printf("################################################################\n\n");
}

int main(void) {
    print_banner("C LANGUAGE CRASH COURSE (For Rust / C# / Go Developers)");

    // セクションタイトルを出力 (Terminal output in English to prevent encoding issues)
    print_section("01: Pointers, Pointer Arithmetic, Manual Memory Management");
    run_pointers_and_memory_demo();

    print_section("02: Structs, Memory Padding, Function Pointers (OOP Emulation)");
    run_structs_and_functions_demo();

    print_section("03: Null-Terminated Strings and Safe File I/O");
    run_strings_and_io_demo();

    print_banner("ALL C TUTORIAL MODULES COMPLETED SUCCESSFULLY!");
    return 0;
}
