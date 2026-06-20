@echo off
rem uses Open Watcom C and WLink
rem OW needs a helper module: jlmw.obj
wcc386 -mf -zl -zls -s -I..\..\Include hello.c
jwlink format win nt hx ru native file hello.obj, ..\HELLO2\jlmw.obj name hello.exe option start=main_, map
