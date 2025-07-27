@echo off
setlocal

set arg=%1

REM === CONFIGURE YOUR INSTALL LOCATION HERE ===
set INSTALL_DIR=C:\programming\libs\install

REM === Clean previous build ===
echo Cleaning build folder...
rmdir /s /q build
mkdir build
cd build

REM === Run CMake with Ninja and prefix path to install ===
echo Running CMake...
cmake -G "Ninja" -DCMAKE_CXX_STANDARD=17 -DCMAKE_PREFIX_PATH=%INSTALL_DIR% ..

IF %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed.
    exit /b %ERRORLEVEL%
)

REM === Build with Ninja ===
echo Building with Ninja...
ninja

IF %ERRORLEVEL% NEQ 0 (
    echo Build failed.
    exit /b %ERRORLEVEL%
)

echo Build complete!
cd ..

if /I "%arg%"=="run" (
    echo Running...
    call run.bat
)

endlocal