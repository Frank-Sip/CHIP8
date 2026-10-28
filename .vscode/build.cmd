@echo off
setlocal
set "VSCMD_SKIP_SENDTELEMETRY=1"

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -no_logo
if errorlevel 1 exit /b %errorlevel%

if /I "%~1"=="--check" (
    where.exe cl.exe
    exit /b %errorlevel%
)

cl.exe /Zi /EHsc /nologo /Fe"%~dp0..\main.exe" "%~dp0..\main.cpp"
exit /b %errorlevel%
