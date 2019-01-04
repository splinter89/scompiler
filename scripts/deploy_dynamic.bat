@echo off
cd ..\..\scompiler-build-desktop
if not exist "deploy" mkdir deploy
copy release\scompiler.exe deploy\
set PATH=C:\Qt\5.11.3\mingw53_32\bin;C:/Qt/Tools/mingw530_32\bin;%PATH%
C:\Qt\5.11.3\mingw53_32\bin\windeployqt.exe deploy\scompiler.exe
upx deploy\scompiler.exe
REM pause
