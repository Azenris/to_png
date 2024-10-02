@echo OFF
cls
SET mypath=%~dp0
pushd %mypath%\TEMP\debug\
start to_png.exe
popd