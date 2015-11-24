@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
cd gdal\frmts\aircom_map

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat" || goto :error

@echo Installing NuGet packages
nuget install gdal-symena -SolutionDirectory . -Source "%cd%\..\..\.." || goto :error
nuget restore AircomMap.sln || goto :error

@echo Building whole solution incl. tests (64-bit release)
msbuild AircomMap.sln /t:Rebuild /p:Configuration=Release;Platform=x64 /m:6 || goto :error

@echo Running unit tests
x64\Release\UnitTests.exe || goto :error

@echo Running integration tests
x64\Release\IntegrationTests.exe || goto :error
