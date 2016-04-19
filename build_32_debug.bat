@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
set repodir=%cd%
cd gdal_32_debug

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64_x86\vcvarsamd64_x86.bat" || goto :error

@echo Building 32-bit debug variant
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 "GDAL_HOME=%repodir%\buildd32" default devinstall || goto :error
copy gdal200.pdb ..\buildd32\bin\ || goto :error
