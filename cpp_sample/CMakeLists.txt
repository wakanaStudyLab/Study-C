# ==============================================================================
# Modern C++ サンプル用 CMakeLists.txt (C++20)
# ==============================================================================

cmake_minimum_required(VERSION 3.20)
project(cpp_sample LANGUAGES CXX)

# 1. C++20 標準規格を要求
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # 標準規格厳格モード

# 2. ソースファイルの定義
set(CPP_SOURCES
    src/main.cpp
    src/raii_and_smart_pointers.cpp
    src/move_semantics_and_classes.cpp
    src/stl_and_ranges.cpp
    src/templates_and_modern_types.cpp
    src/concurrency.cpp
)

# 3. 実行可能バイナリターゲットの作成
add_executable(cpp_sample ${CPP_SOURCES})

# 4. コンパイラフラグ (単独ビルド時にも有効になるようターゲット単位で付与)
if(MSVC)
    target_compile_options(cpp_sample PRIVATE /utf-8 /W4 /EHsc)
else()
    target_compile_options(cpp_sample PRIVATE -Wall -Wextra -pedantic)
endif()

# 5. インクルードディレクトリの指定
target_include_directories(cpp_sample PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 6. 出力先ディレクトリ設定
set_target_properties(cpp_sample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
