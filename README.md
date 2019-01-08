# sCompiler

Basic parts of a compiler for the subset of C++ language using LR(1) grammar.

What's implemented: lexical, syntactic, and semantic analysis.

## Installation

1. Update paths to your Qt/MinGW
    - in `scripts\deploy_dynamic.bat`
    - and `scripts\windows-build-qt-static.ps1`
1. Install [UPX](https://github.com/upx/upx).
1. Download `clang-format` binary from [this page](https://llvm.org/builds/). Activate `Beautifier` plugin in Qt Creator and customize its options: set "Clang Format"->"Use predefined style" to "File", check "Enable auto format on file save."
1. Configure your builds:

|  | dynamic | static |
|---|---|---|
| build directory | `..\scompiler-build-desktop` | `..\scompiler-build-desktop-static` |
| configurations | debug, release | release |
| extra build step (release) | `cmd /C "%{sourceDir}\scripts\deploy_dynamic.bat"` | `cmd /C "%{sourceDir}\scripts\deploy_static.bat"` |

## Static Builds

Use this guide: https://wiki.qt.io/Building_a_static_Qt_for_Windows_using_MinGW

To compile static Qt, run the following in Windows PowerShell:

```
Set-ExecutionPolicy RemoteSigned
.\scripts\windows-build-qt-static.ps1
o                               // use the Open Source Edition
y                               // accept the license offer
...                             // wait ☕
Set-ExecutionPolicy Restricted  // restore the policy
```

## Todo

- add **new** operator, **void** data type
- comma or block at the end of class methods
- Reverse Polish notation (poliz)
    - cycle 1: syntax, symbol/block tables
    - cycle 2: rpn, semantics

## Links

[C++ Language Tutorials](http://www.cplusplus.com/doc/tutorial/)

[Qt Creator Keyboard Shortcuts](https://wiki.qt.io/Qt_Creator_Keyboard_Shortcuts)

## License

This project is licensed under the terms of the [MIT license](LICENSE.md).
