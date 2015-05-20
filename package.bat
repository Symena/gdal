if not defined GO_STAGE_COUNTER (set GO_STAGE_COUNTER=0)
if not defined GO_PIPELINE_COUNTER (set GO_PIPELINE_COUNTER=0)

@echo Building NuGet packages
PowerShell.exe -Command Write-NuGetPackage -Verbose .\gdal.autopkg

@echo Publishing NuGet packages
nuget push -Source http://sbuild01:8080/nuget/Default -ApiKey Admin:Admin *.nupkg
