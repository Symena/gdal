@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
set repodir=%cd%
cd gdal_32_release

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64_x86\vcvarsamd64_x86.bat" || goto :error

@echo Building 32-bit release variant
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 "GDAL_HOME=%repodir%\buildo32" default devinstall || goto :error
copy gdal200.pdb ..\buildo32\bin\ || goto :error
