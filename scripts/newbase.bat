@REM Assumes executable installed under newbase/bin

@echo off
set unixPath=%~dp0
set unixPath=%unixPath:\=/%

git -c core.editor="%unixPath%newbase/bin/Main.exe" rebase -i %*
