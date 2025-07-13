@echo off
setlocal

REM === CONFIGURE YOUR SDL3 LOCATION HERE ===
set SDL3_PATH=C:\programming\libs\SDL3-3.2.16\x86_64-w64-mingw32
set SDL3_IMAGE_PATH=C:\programming\libs\SDL_Image\build

REM === Clean previous build ===
echo Cleaning build folder...
rmdir /s /q build
mkdir build
cd build

REM === Run CMake ===
echo Running CMake...
cmake -G "MinGW Makefiles" -DCMAKE_CXX_STANDARD=17 ..

IF %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed.
    exit /b %ERRORLEVEL%
)

REM === Run mingw32-make ===
echo Building with mingw32-make...
mingw32-make

IF %ERRORLEVEL% NEQ 0 (
    echo Build failed.
    exit /b %ERRORLEVEL%
)

REM === Copy SDL3.dll if needed ===
echo Copying DLLs...
copy "%SDL3_PATH%\lib\SDL3.dll" . >nul
copy "%SDL3_IMAGE_PATH%\SDL3_image.dll" . >nul

echo Build complete!
cd ..
./run.bat

endlocal
pause
