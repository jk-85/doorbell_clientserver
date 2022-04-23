@echo off
:while
client.exe 192.168.1.38
timeout 2
goto :while
pause