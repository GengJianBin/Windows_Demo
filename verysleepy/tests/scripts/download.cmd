@echo off

rem Download %URL% to %DEST%.

if exist %DEST% goto :eof

for %%a in (powershell.exe) do if not [%%~$PATH:a] == [] powershell -Command "(New-Object Net.WebClient).DownloadFile('%URL%', '%DEST%')" & goto :eof
for %%a in (curl.exe) do if not [%%~$PATH:a] == [] curl "%URL%" -O "%DEST%" --location & goto :eof
for %%a in (wget.exe) do if not [%%~$PATH:a] == [] wget "%URL%" -o "%DEST%" --max-redirect=5 & goto :eof
for %%a in (bitsadmin.exe) do if not [%%~$PATH:a] == [] start /wait "MinGW download" bitsadmin /transfer "MinGW" "%URL%" "%DEST%" & goto :eof
echo No download utilities available - please download file at %URL% and save to %DEST% manually
exit /b 1
