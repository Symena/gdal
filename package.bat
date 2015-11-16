@echo Removing index packages folder 
rmdir /S /Q gdal\frmts\index\packages

@echo Building NuGet packages
PowerShell.exe -Command Write-NuGetPackage -Verbose .\gdal.autopkg

@echo Publishing NuGet packages
nuget push -Source http://sbuild01:8080/nuget/Default -ApiKey Admin:Admin *.nupkg
