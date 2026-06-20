@echo off
rem uses MS VC and MS Link
\msvc71\bin\cl -c -I..\..\Include hello.c
link /LIBPATH:\msvc71\lib hello.obj /subsystem:native /fixed:no /out:hello.exe /entry:main
\hx\bin\patchpe hello.exe
