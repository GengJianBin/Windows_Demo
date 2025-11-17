@echo off
cd /d "%~dp0"
cmd /c ..\..\scripts\toolchains_download.cmd
call ..\..\scripts\clear_env.cmd

set PATH=%~dp0\..\..\toolchains\i686-4.9.1-release-posix-dwarf-rt_v3-rev0\mingw32\bin;%PATH%
gcc -g -oprogram program.c 
