@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
@echo Removing AircomMap packages folder
rmdir /S /Q gdal\frmts\aircom_map\packages || goto :error

@echo Building NuGet packages
PowerShell.exe -Command Write-NuGetPackage -Verbose .\gdal.autopkg || goto :error
