@echo off
setlocal enabledelayedexpansion

rem Unpack %1 to the current directory.

if not defined 7ZIP for %%a in (7z.exe) do if not [%%~$PATH:a] == [] set 7ZIP="%%~$PATH:a"
if not defined 7ZIP if exist "!ProgramFiles!\7-Zip\7z.exe" set 7ZIP="!ProgramFiles!\7-Zip\7z.exe"
if not defined 7ZIP if exist "!ProgramFiles(x86)!\7-Zip\7z.exe" set 7ZIP="!ProgramFiles(x86)!\7-Zip\7z.exe"
if not defined 7ZIP if exist "!SystemDrive!\7-Zip\7z.exe" set 7ZIP="!SystemDrive!\7-Zip\7z.exe"
if not defined 7ZIP echo drmingw_build: Can't find 7-Zip installation - please add it to PATH or specify the full path to 7z.exe in the 7ZIP environment variable. & exit /b 1
::echo Found 7-Zip at !7ZIP!

!7ZIP! x -y %1
if errorlevel 1 exit /b 1

set FN=%~1

if [%FN:~-7%] == [.tar.gz] (
	set TAR=%~n1
	if not exist "%TAR%" echo Expected .tar archive: %~n1
	!7ZIP! x -y "%~n1"
	del "%~n1"
)
