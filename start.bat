@echo off
setlocal enabledelayedexpansion

echo ================================
echo Cleaning previous build...
echo ================================

if exist build (
    rmdir /S /Q build
)

mkdir build

echo.
echo ================================
echo Building Shadow Empire...
echo ================================

set SOURCES=
for /R src %%f in (*.cpp) do (
    set SOURCES=!SOURCES! "%%f"
)

g++ -std=c++17 -Wall -Wextra -Iinclude !SOURCES! -o build\shadow_empire.exe

if errorlevel 1 (
    echo.
    echo Build failed.
    pause
    exit /b 1
)

echo.
echo Build successful.
echo ================================
echo Starting Shadow Empire...
echo ================================
echo.

build\shadow_empire.exe

echo.
pause