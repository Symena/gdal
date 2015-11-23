@echo off
goto :begin

:error
exit /b %errorlevel%

:begin
@echo Publishing NuGet packages
nuget push -Source http://sbuild01:8080/nuget/Default -ApiKey Admin:Admin *.nupkg || goto :error
