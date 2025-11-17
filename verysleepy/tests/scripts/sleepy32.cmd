@echo off
if not defined CONFIGURATION set CONFIGURATION=Release
if not defined SLEEPY32 set SLEEPY32=%~dp0\..\..\obj\Win32\%CONFIGURATION%\sleepy.exe

%SLEEPY32% %*
