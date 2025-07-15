@echo off
:: 设置 Qt 工具链路径（根据实际路径调整）
set QT_DIR=D:\Programs\qt6.8.3
set PATH=%QT_DIR%\Tools\mingw1310_64\bin;%QT_DIR%\6.8.3\mingw_64\bin;%PATH%
set CMAKE=%QT_DIR%\Tools\CMake_64\bin\cmake.exe
set MINGW_MAKE=%QT_DIR%\Tools\mingw1310_64\bin\mingw32-make.exe
set GXX=%QT_DIR%\Tools\mingw1310_64\bin\g++.exe
set PROJECT_NAME=Painter

:: 清理旧构建
@REM rm -r ./build
@REM mkdir build

:: 使用 Qt 的 CMake 和 MinGW
cd build
%CMAKE% -G "MinGW Makefiles" ^
  -D CMAKE_CXX_COMPILER=%GXX% ^
  ..
%MINGW_MAKE%

  @REM -D CMAKE_BUILD_TYPE=Release ^