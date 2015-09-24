
set repodir=%cd%
cd gdal

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64\vcvars64.bat"

@echo Building 64-bit debug variant
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd64
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd64 devinstall
copy gdal200.pdb ..\buildd64\bin\
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd64 clean

@echo Building 64-bit release variant
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo64
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo64 devinstall
copy gdal200.pdb ..\buildo64\bin\
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo64 clean

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64_x86\vcvarsamd64_x86.bat"

@echo Building 32-bit debug variant
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 GDAL_HOME=%repodir%\buildd32
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 GDAL_HOME=%repodir%\buildd32 devinstall
copy gdal200.pdb ..\buildd32\bin\
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 GDAL_HOME=%repodir%\buildd32 clean

@echo Building 32-bit release variant
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 GDAL_HOME=%repodir%\buildo32
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 GDAL_HOME=%repodir%\buildo32 devinstall
copy gdal200.pdb ..\buildo32\bin\
