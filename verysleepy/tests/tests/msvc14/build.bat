@echo off
cd /d "%~dp0"
call ..\..\scripts\clear_env.cmd

rem https://github.com/VerySleepy/verysleepy/issues/28

call "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

cl /MD /Z7 program.c /link /DEBUG:FASTLINK
