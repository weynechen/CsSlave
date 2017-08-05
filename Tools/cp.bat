@echo off
start cmd /c "powershell set-ExecutionPolicy RemoteSigned&&powershell -file cp.ps1&&powershell set-ExecutionPolicy Restricted"