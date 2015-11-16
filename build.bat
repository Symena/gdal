@echo off
GOTO :begin

:error
exit /b %errorlevel%

:begin
set repodir=%cd%
cd gdal

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat" || GOTO :error

@echo Building 64-bit debug variant
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd64 || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd64 devinstall || GOTO :error
copy gdal200.pdb ..\buildd64\bin\ || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd64 clean || GOTO :error

@echo Building 64-bit release variant
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo64 || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo64 devinstall || GOTO :error
copy gdal200.pdb ..\buildo64\bin\ || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo64 clean || GOTO :error

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64_x86\vcvarsamd64_x86.bat" || GOTO :error

@echo Building 32-bit debug variant
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 GDAL_HOME=%repodir%\buildd32 || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 GDAL_HOME=%repodir%\buildd32 devinstall || GOTO :error
copy gdal200.pdb ..\buildd32\bin\ || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 DEBUG=1 GDAL_HOME=%repodir%\buildd32 clean || GOTO :error

@echo Building 32-bit release variant
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 GDAL_HOME=%repodir%\buildo32 || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 GDAL_HOME=%repodir%\buildo32 devinstall || GOTO :error
copy gdal200.pdb ..\buildo32\bin\ || GOTO :error
nmake -f makefile.vc MSVC_VER=1900 WITH_PDB=1 GDAL_HOME=%repodir%\buildo32 clean || GOTO :error
