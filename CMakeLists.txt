# ==============================================================================
# Modern CMake ルート設定ファイル (Root CMakeLists.txt)
# ==============================================================================
# 
# 【Modern CMake の基本思想】
# 従来の「グローバル変数（include_directories, link_libraries 等）を設定する古いスタイル」ではなく、
# 「ターゲット（Target）ごとにスコープ（PRIVATE / PUBLIC / INTERFACE）を閉じて設定する」のが現代の標準です。
# 
# 【他言語ビルドツールとの対応】
#   - Rust: Cargo.toml (ワークスペース [workspace])
#   - C#:   .sln (ソリューションファイル)
#   - Go:   go.work
#   - Java: Gradle (settings.gradle.kts / include("c_sample", "cpp_sample"))
# ==============================================================================

# 1. CMake の最低要求バージョンを指定 (3.20以上を推奨)
cmake_minimum_required(VERSION 3.20)

# 2. プロジェクト名と言語（C と CXX = C++）を定義
#    これにより CMAKE_C_COMPILER や CMAKE_CXX_COMPILER の検出が行われます。
project(Modern_C_Cpp_Course LANGUAGES C CXX)

# ==============================================================================
# グローバルコンパイラ設定 & UTF-8 対応
# ==============================================================================
# MSVC (Visual Studio) と GCC / Clang でコンパイラフラグを条件分岐
if(MSVC)
    # /utf-8 : ソースコードと実行文字セットを UTF-8 に指定（日本語の文字化け・エラーを防止）
    # /W4    : 警告レベル 4（実務で推奨される高レベルな警告検知）
    add_compile_options(/utf-8 /W4)
else()
    # -Wall -Wextra : 主要な警告をすべて有効化
    # -pedantic     : 標準規格に厳密に従っているかを検査
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# ==============================================================================
# サブディレクトリの追加 (add_subdirectory)
# ==============================================================================
# 各サブディレクトリ内の CMakeLists.txt を読み込み、独立したターゲットをビルドツリーに追加します。
add_subdirectory(c_sample)
add_subdirectory(cpp_sample)
