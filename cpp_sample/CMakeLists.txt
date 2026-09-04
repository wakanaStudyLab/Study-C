# ==============================================================================
# Modern C++ サンプル用 CMakeLists.txt (C++20)
# ==============================================================================

cmake_minimum_required(VERSION 3.20)
project(cpp_sample LANGUAGES CXX)

# 1. C++23 標準規格を要求
set(CMAKE_CXX_STANDARD 23)
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
    src/lambda_expressions.cpp
    src/coroutines_and_modern_cpp.cpp
    src/error_handling_and_expected.cpp
    src/advanced_templates_and_metaprogramming.cpp
    src/operator_overloading_and_spaceship.cpp
    src/memory_and_low_level.cpp
    src/filesystem_and_io.cpp
    src/advanced_concurrency.cpp
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
