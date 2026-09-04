# CMake を用いた C & C++ サンプルの構成・ビルド・実行スクリプト
$ErrorActionPreference = "Stop"

# UTF-8 出力を設定
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

Write-Host "===> 1. CMake Configure..." -ForegroundColor Cyan
if (Get-Command ninja -ErrorAction SilentlyContinue) {
    cmake -B build -S . -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
} else {
    cmake -B build -S .
}

Write-Host "`n===> 2. CMake Build..." -ForegroundColor Cyan
cmake --build build --config Release

# バイナリパスの自動解決 (Ninja: build/bin, MSVC: build/bin/Release)
function Find-Executable($name) {
    $paths = @(
        ".\build\bin\$name.exe",
        ".\build\bin\Release\$name.exe",
        ".\build\bin\Debug\$name.exe"
    )
    foreach ($p in $paths) {
        if (Test-Path $p) { return $p }
    }
    throw "Executable $name.exe not found!"
}

$c_bin = Find-Executable "c_sample"
$cpp_bin = Find-Executable "cpp_sample"

Write-Host "`n===================== [ RUNNING C SAMPLE ] =====================" -ForegroundColor Green
& $c_bin

Write-Host "`n==================== [ RUNNING C++ SAMPLE ] ====================" -ForegroundColor Green
& $cpp_bin
