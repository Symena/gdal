@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
@echo Removing index packages folder
rmdir /S /Q gdal\frmts\index\packages || goto :error

@echo Building NuGet packages
PowerShell.exe -Command Write-NuGetPackage -Verbose .\gdal.autopkg || goto :error
