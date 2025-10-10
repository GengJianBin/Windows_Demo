@echo off

set DEST=toolchains\%FN%
call "%~dp0\download"
if errorlevel 1 exit /b 1

call "%~dp0\unpack_to" "toolchains\%FN%" "toolchains\%DIR%"
if errorlevel 1 exit /b 1
