@echo off
setlocal enabledelayedexpansion

set arg=%1

if /I "%arg%"=="build" (
    echo Building First...
    call build.bat
)

:: === CONFIG ===
set PROJECT_NAME=HumanAimbot
set BUILD_DIR=build
set DIST_DIR=dist
set SDL_DIR=C:\programming\libs\SDL3-3.2.16\x86_64-w64-mingw32
set SDL_IMAGE_DIR=C:\programming\libs\SDL_image
set SDL_TTF_DIR=C:\programming\libs\SDL_ttf
set ASSET_DIR=assets

:: === CLEAN DIST ===
echo Cleaning previous %DIST_DIR%...
rd /s /q %DIST_DIR%
mkdir %DIST_DIR%

:: === COPY GAME EXE ===
echo Copying game executable...
copy %BUILD_DIR%\%PROJECT_NAME%.exe %DIST_DIR%\

:: === COPY DLLs ===
echo Copying DLLs...
copy "%BUILD_DIR%\*.dll" %DIST_DIR%

:: === COPY ASSETS ===
echo Copying Assets...
xcopy /E /I /Y %ASSET_DIR% %DIST_DIR%\assets\

:: === MAKE Installer ===
echo Creating Installer...
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" inno.iss 
