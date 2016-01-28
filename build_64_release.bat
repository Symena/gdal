@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
set repodir=%cd%
cd gdal_64_release

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat" || goto :error

@echo Building 64-bit release variant
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 WIN64=YES "GDAL_HOME=%repodir%\buildo64" default devinstall || goto :error
copy gdal200.pdb ..\buildo64\bin\ || goto :error
