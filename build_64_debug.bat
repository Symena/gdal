@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
set repodir=%cd%
cd gdal_64_debug

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat" || goto :error

@echo Building 64-bit debug variant
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 WIN64=YES "GDAL_HOME=%repodir%\buildd64" default devinstall || goto :error
copy gdal200.pdb ..\buildd64\bin\ || goto :error
