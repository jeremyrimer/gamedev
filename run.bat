@echo off
setlocal

REM Path to your build output
set EXE_PATH=build\HumanAimbot.exe

REM Path to SDL3.dll (if needed at runtime)
set SDL_DLL=build\SDL3.dll

REM Check if exe exists
if not exist "%EXE_PATH%" (
    echo ERROR: HumanAimbot.exe not found. Did you forget to build?
    pause
    exit /b 1
)

REM Check if SDL3.dll is present
if not exist "%SDL_DLL%" (
    echo WARNING: SDL3.dll not found next to exe. Game may not run.
)

echo Running HumanAimbot...
echo ==========================
"%EXE_PATH%" 2>&1
echo ==========================
echo Game exited.
@REM pause
endlocal
