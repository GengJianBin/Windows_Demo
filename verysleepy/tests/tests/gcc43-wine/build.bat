@echo off
cd /d "%~dp0"
cmd /c ..\..\scripts\toolchains_download.cmd
call ..\..\scripts\clear_env.cmd

"%~dp0\..\..\toolchains\gcc43\bin\gcc.exe" -g -oprogram program.c
