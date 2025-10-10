@echo off

pushd "%~dp0"
cd ..

if not exist toolchains mkdir toolchains

rem We must pass the URL as an environment variable, to avoid cmd.exe's "The input line is too long."

rem GCC 4.9.1

set DIR=i686-4.9.1-release-posix-dwarf-rt_v3-rev0
set FN=%DIR%.7z
set URL=https://sourceforge.net/projects/mingw-w64/files/Toolchains%%20targetting%%20Win32/Personal%%20Builds/mingw-builds/4.9.1/threads-posix/dwarf/%FN%/download
call scripts\toolchains_download_one
if errorlevel 1 exit /b 1

set DIR=x86_64-4.9.1-release-posix-seh-rt_v3-rev0
set FN=%DIR%.7z
set URL=https://sourceforge.net/projects/mingw-w64/files/Toolchains%%20targetting%%20Win64/Personal%%20Builds/mingw-builds/4.9.1/threads-posix/seh/%FN%/download
call scripts\toolchains_download_one
if errorlevel 1 exit /b 1

rem GCC 4.3.0

set DIR=gcc43

set FN=binutils-2.19-2-mingw32-bin.tar.gz
set URL=https://sourceforge.net/projects/mingw/files/MinGW/Base/binutils/binutils-2.19/binutils-2.19-2-mingw32-bin.tar.gz/download
call scripts\toolchains_download_one
if errorlevel 1 exit /b 1

set FN=gcc-4.3.0-20080502-mingw32-alpha-bin.7z
set URL=https://sourceforge.net/projects/mingw/files/MinGW/Base/gcc/Version4/Previous%%20Testing_%%20gcc-4.3.0-20080502-mingw32-alpha/gcc-4.3.0-20080502-mingw32-alpha-bin.7z/download
call scripts\toolchains_download_one
if errorlevel 1 exit /b 1

set FN=mingwrt-3.18-mingw32-dev.tar.gz
set URL=https://sourceforge.net/projects/mingw/files/MinGW/Base/mingwrt/mingwrt-3.18/mingwrt-3.18-mingw32-dev.tar.gz/download
call scripts\toolchains_download_one
if errorlevel 1 exit /b 1

set FN=w32api-3.14-mingw32-dev.tar.gz
set URL=https://sourceforge.net/projects/mingw/files/MinGW/Base/w32api/w32api-3.14/w32api-3.14-mingw32-dev.tar.gz/download
call scripts\toolchains_download_one
if errorlevel 1 exit /b 1

rem DMD 2.073

set DIR=dmd.2.073.0.windows
set FN=%DIR%.7z
set URL=http://downloads.dlang.org/releases/2.x/2.073.0/%FN%
call scripts\toolchains_download_one
if errorlevel 1 exit /b 1

popd
