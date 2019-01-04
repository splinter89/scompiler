@echo off
cd ..\..\scompiler-build-desktop-static
if not exist "deploy" mkdir deploy
copy release\scompiler.exe deploy\
upx deploy\scompiler.exe
REM pause
