@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64\vcvars64.bat"
cd %1\gdal\gdal
echo Building debug variant
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%1\gdal\buildd
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%1\gdal\buildd devinstall
copy %1\gdal\gdal\gdal200.pdb %1\gdal\buildd\bin\
nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=YES GDAL_HOME=%1\gdal\buildd clean

echo Building release variant
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 WIN64=YES GDAL_HOME=%1\gdal\buildo
nmake -f makefile.vc MSVC_VER=1800 WITH_PDB=1 WIN64=YES GDAL_HOME=%1\gdal\buildo devinstall
copy %1\gdal\gdal\gdal200.pdb %1\gdal\buildo\bin\

echo Building NuGet packages
cd ..
PowerShell.exe -Command Write-NuGetPackage .\gdal.autopkg

echo Publishing NuGet packages
nuget push -Source http://sbuild01:8080/nuget/Default -ApiKey Admin:Admin *.nupkg
