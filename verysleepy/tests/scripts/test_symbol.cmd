@echo off

if exist %1 rmdir /S /Q %1
call %~dp0\unpack_to %1.sleepy %1 > nul
if errorlevel 1 exit 1

set FOUND=0
for /F "tokens=3" %%a in (%1\Symbols.txt) do if "%%~a" == "%~2" set FOUND=1

if %FOUND% == 0 exit /b 1
