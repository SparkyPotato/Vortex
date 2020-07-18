@echo off
pushd %~dp0\..\
call ThirdParty\Premake\premake5-windows.exe vs2019
popd
PAUSE