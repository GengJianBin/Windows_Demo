@echo off

cd "%~dp0"

for /D %%d in (*) do (
	echo Running test %%d
	pushd %%d
	cmd /c test.bat
	if errorlevel 1 exit /b 1
	popd
	echo Test %%d OK
)

echo All tests OK!
