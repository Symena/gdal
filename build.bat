
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64\vcvars64.bat"
set repodir=%cd%
cd gdal
@echo Building debug variant
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd devinstall
copy gdal200.pdb ..\buildd\bin\
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%repodir%\buildd clean

@echo Building release variant
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 WIN64=YES GDAL_HOME=%repodir%\buildo devinstall
copy gdal200.pdb ..\buildo\bin\
