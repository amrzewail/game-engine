@echo off
setlocal

:: Clean variable assignment
set "TargetPath=%~1"
set "ProjectDir=%~2"
set "SolutionDir=%~3"

echo %SolutionDir%
echo %ProjectDir%

echo Copying data...
xcopy /i /s /y "%SolutionDir%Moths\data" "%ProjectDir%data"

echo Running program: %TargetPath%
"%TargetPath%"