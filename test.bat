@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
cd gdal\frmts\index

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat" || goto :error

@echo Installing NuGet packages
nuget install gdal-symena -SolutionDirectory . -Source "%cd%\..\..\.." || goto :error
nuget restore index.sln || goto :error

@echo Building whole solution incl. tests (64-bit release)
msbuild index.sln /t:Rebuild /p:Configuration=Release;Platform=x64 /m:6 || goto :error

@echo Running unit tests
x64\Release\indexUnitTests.exe || goto :error
