@echo off
set srcdate=%~t1
set dstdate=%~t2
if "%srcdate%" GTR "%dstdate%" (
    goto compile
)
if "%3"=="force" (
    goto compile
)

goto :eof

:compile
"C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/VC/Auxiliary/Build/vcvars64.bat" > nul ^
    && cl /nologo /EHsc /W4 /utf-8 /D_CRT_SECURE_NO_WARNINGS %1
