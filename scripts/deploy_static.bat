@echo off

cd ..\..\scompiler-build-desktop-static
if not exist "deploy" mkdir deploy
copy release\scompiler.exe deploy\
cd deploy

"C:\Program Files\upx\upx.exe" scompiler.exe

set RELEASE_FILENAME=scompiler_qt5_mingw32bit_static.exe
if exist ..\..\%RELEASE_FILENAME% del ..\..\%RELEASE_FILENAME%
copy scompiler.exe ..\..\%RELEASE_FILENAME%

REM pause
