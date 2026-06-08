@echo off
setlocal enabledelayedexpansion

>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
if '%errorlevel%' NEQ '0' (
    echo restart on administrator...
    powershell -Command "Start-Process '%~f0' -Verb runAs"
    exit /b
)
echo Install or Uninstall(I/U?)
set /p RUN_CMD=

if /I "%RUN_CMD%"=="I" goto Install
if /I "%RUN_CMD%"=="U" goto Uninstall
exit /b 1

:Install
SET regsvr_opt=
SET "regfile=%~dp0Regist.reg"

set "CD=%~dp0"
set "CD_ESCAPED=!CD:\=\\!"

set "IN=%~dp0Regist.src"
set "OUT=%~dp0Regist.reg"
set "OLD=<this_dir>"
set "NEW=!CD_ESCAPED!"

> "%OUT%" type nul

for /f "usebackq delims=" %%L in ("%IN%") do (
    set "LINE=%%L"
    set "LINE=!LINE:%OLD%=%NEW%!"
    echo !LINE!>>"%OUT%"
)

goto Proc

:Uninstall
SET regsvr_opt=/U
SET "regfile=%~dp0Unregist.reg"
goto Proc

:Proc


%windir%\System32\regsvr32 /s %regsvr_opt% "%~dp0SusiePluginCom.dll"
set ret1=%errorlevel%
%windir%\SysWOW64\regsvr32 /s %regsvr_opt% "%~dp0SusiePluginCom32.dll"
set ret2=%errorlevel%
%windir%\System32\reg import "%regfile%"
set ret3=%errorlevel%

if exist "%~dp0Regist.reg" del "%~dp0Regist.reg"
set ret=0
if not "%ret1%"=="0" (
    echo Failed 64bit COM dll process.
    set ret=1
)
if not "%ret2%"=="0" (
    echo Failed 32bit COM dll process.
    set ret=1
)
if not "%ret3%"=="0" (
    echo Failed registory process.
    set ret=1
)

pause

endlocal

exit /b %ret%
