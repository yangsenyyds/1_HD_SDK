::@echo off
::cls

set LOCAL_PATH=%~dp0
set PRJ_PATH=..\Download
echo start download ROM code...
cd "%PRJ_PATH%"
call download.bat
cd "%LOCAL_PATH%"
echo download ROM code OK

