@echo off

rem Unpack archive %1 to directory %2, if this was not done already.

if not exist %2 mkdir %2

set FILE=%~dpnx1

pushd %2

if not exist "%~nx1.unpacked" (
	call "%~dp0\unpack" "%FILE%"
	if errorlevel 1 exit /b 1

	echo.>"%~nx1.unpacked"
)

popd
