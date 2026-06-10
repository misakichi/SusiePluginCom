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
goto Proc

:Uninstall
SET regsvr_opt=/U
SET "regfile=%~dp0Unregist.reg"
goto Proc

:Proc

pushd %~dp0
echo %windir%\SysWOW64\regsvr32 /s %regsvr_opt% "SusiePluginCom32.dll"
%windir%\SysWOW64\regsvr32 /s %regsvr_opt% "SusiePluginCom32.dll"
set ret2=%errorlevel%
echo %windir%\System32\regsvr32 /s %regsvr_opt% "SusiePluginCom.dll"
%windir%\System32\regsvr32 /s %regsvr_opt% "SusiePluginCom.dll"
set ret1=%errorlevel%
popd

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

pause

endlocal

exit /b %ret%
