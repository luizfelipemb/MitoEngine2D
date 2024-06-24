@echo off

pushd
premake5.exe --file=premake5.lua vs2022
popd
pause