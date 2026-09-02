# ==============================================================================
# C言語 サンプル用 CMakeLists.txt
# ==============================================================================

cmake_minimum_required(VERSION 3.20)
project(c_sample LANGUAGES C)

# 1. C言語の標準規格を指定 (C11 / C17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF) # 標準規格厳格モード

# 2. ソースファイルの定義
set(C_SOURCES
    src/main.c
    src/pointers_and_memory.c
    src/structs_and_functions.c
    src/strings_and_io.c
    src/lambda_and_callbacks.c
)

# 3. 実行可能バイナリターゲットの作成
add_executable(c_sample ${C_SOURCES})

# 4. コンパイラフラグ (単独ビルド時にも有効になるようターゲット単位で付与)
if(MSVC)
    target_compile_options(c_sample PRIVATE /utf-8 /W4)
else()
    target_compile_options(c_sample PRIVATE -Wall -Wextra -pedantic)
endif()

# 5. インクルードディレクトリの指定
target_include_directories(c_sample PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 6. 出力先ディレクトリ設定
set_target_properties(c_sample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
