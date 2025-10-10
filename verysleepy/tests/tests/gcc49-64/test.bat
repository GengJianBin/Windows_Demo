@echo off
setlocal enabledelayedexpansion
if not exist program.exe call build

rem Test symbol resolution with gcc 4.9.
rem Note: Wine dbghelp can't decode these yet.
rem https://github.com/VerySleepy/verysleepy/issues/9

set SLEEPY_SILENT_CRASH=1

for %%b in (64) do for %%d in (mingw) do (
	echo Testing %%b-%%d
	if exist program-%%b-%%d.sleepy del program-%%b-%%d.sleepy
	call ..\..\scripts\sleepy%%b --%%d /r:program.exe /o:program-%%b-%%d.sleepy
	if !ERRORLEVEL! neq 0 exit /b 1

	call ..\..\scripts\test_symbol program-%%b-%%d vs_test_fun
	if !ERRORLEVEL! neq 0 echo Symbols not decoded & exit /b 1
)

echo %~dp0 OK
