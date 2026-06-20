@echo off
setlocal enabledelayedexpansion

set "PROJECT_DIR=%~dp0"
set "BUILD_DIR=%PROJECT_DIR%build"

echo === Cleaning prior build artifacts ===
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)

echo === Generating build configuration ===
cmake -S "%PROJECT_DIR%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 exit /b %errorlevel%

echo === Compiling binaries ===
cmake --build "%BUILD_DIR%" --config Release
if %errorlevel% neq 0 exit /b %errorlevel%

echo === Executing pipeline verification ===
if exist "%BUILD_DIR%\Release\hybrid_search_app.exe" (
    "%BUILD_DIR%\Release\hybrid_search_app.exe"
) else if exist "%BUILD_DIR%\hybrid_search_app.exe" (
    "%BUILD_DIR%\hybrid_search_app.exe"
) else (
    echo Error: Executable binary targets not found.
    exit /b 1
)

endlocal
