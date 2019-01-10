@echo off

cd ..\..\scompiler-build-desktop
if not exist "deploy" mkdir deploy
copy release\scompiler.exe deploy\
cd deploy

set PATH=C:\Qt\5.11.3\mingw53_32\bin;C:/Qt/Tools/mingw530_32\bin;%PATH%
C:\Qt\5.11.3\mingw53_32\bin\windeployqt.exe scompiler.exe

"C:\Program Files\upx\upx.exe" scompiler.exe

set RELEASE_FILENAME=scompiler_qt5_mingw32bit_dynamic.zip
if exist ..\..\%RELEASE_FILENAME% del ..\..\%RELEASE_FILENAME%
"C:\Program Files\WinRAR\WinRAR.exe" a -afzip -r ..\..\%RELEASE_FILENAME% .

REM pause
