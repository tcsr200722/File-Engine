@echo off
title update
set currentPos=%~dp0
set fileName=File-Engine-x64.exe
set tmpFileName=tmp\File-Engine-x64.exe
set oldPath=%currentPos%%fileName%
set newPath=%currentPos%%tmpFileName%
ping -n 2 127.0.0.1 > nul
taskkill /im File-Engine-x64.exe /f
copy "%newPath%" "%oldPath%"
start File-Engine-x64.exe