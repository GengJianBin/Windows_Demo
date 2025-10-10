@echo off
setlocal enabledelayedexpansion
if not exist program.exe call build

rem Test 32-bit dbghelpw crash due to incorrect definition of "long"
rem Fixed in wine submodule commit b8af4978ff2a463258d732a52037dafdcc61c423

set SLEEPY_SILENT_CRASH=1

for %%b in (32 64) do for %%d in (wine mingw) do (
	echo Testing %%b-%%d
	if exist program-%%b-%%d.sleepy del program-%%b-%%d.sleepy
	call ..\..\scripts\sleepy%%b --%%d /r:program.exe /o:program-%%b-%%d.sleepy
	if !ERRORLEVEL! neq 0 exit /b 1

	call ..\..\scripts\test_symbol program-%%b-%%d vs_test_fun
	if !ERRORLEVEL! neq 0 echo Symbols not decoded & exit /b 1
)

echo.
echo %~dp0 OK
