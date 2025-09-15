echo off

REM create directory struct
REM create the directory of the source code
if not exist "src" (
    call mkdir src
)
if not exist "include" (
    call mkdir include
)
if not exist "libs" (
    call mkdir libs
)

if not exist "build" (
    call mkdir build
)

REM generate 64 bit project
cd build
if not exist "x64" (
    call mkdir x64
)
cd x64
cmake ../..
REM build cmake project
cmake -S ../../ -B build -G "Visual Studio 17 2022" -A x64

REM generate 32 bit project
REM cmake -S . -B build -G "Visual Studio 17 2022" -A Win32

REM compile solution
cmake --build . --config Debug
cd ../..
