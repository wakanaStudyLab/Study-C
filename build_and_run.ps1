# CMake を用いた C & C++ サンプルの構成・ビルド・実行スクリプト
$ErrorActionPreference = "Stop"

Write-Host "===> 1. CMake Configure (build ディレクトリの生成)..." -ForegroundColor Cyan
cmake -B build -S .

Write-Host "`n===> 2. CMake Build (Release 構成でコンパイル)..." -ForegroundColor Cyan
cmake --build build --config Release

Write-Host "`n===================== [ RUNNING C SAMPLE ] =====================" -ForegroundColor Green
& ".\build\bin\Release\c_sample.exe"

Write-Host "`n==================== [ RUNNING C++ SAMPLE ] ====================" -ForegroundColor Green
& ".\build\bin\Release\cpp_sample.exe"
