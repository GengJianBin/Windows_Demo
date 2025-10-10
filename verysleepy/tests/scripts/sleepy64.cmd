@echo off
if not defined CONFIGURATION set CONFIGURATION=Release
if not defined SLEEPY64 set SLEEPY64=%~dp0\..\..\obj\x64\%CONFIGURATION%\sleepy.exe

%SLEEPY64% %*
