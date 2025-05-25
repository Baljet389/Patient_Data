@echo on
setlocal

:: CONFIGURE THESE PATHS
set "QT_DIR=C:\Qt\6.7.3\mingw_64"
set "BUILD_DIR=build"

:: Set CMake prefix path
set "CMAKE_PREFIX_PATH=%QT_DIR%\lib\cmake"
set "PATH=%QT_DIR%\bin;%PATH%"

:: Create and enter build directory
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

:: Generate build files 
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%CMAKE_PREFIX_PATH%" ..

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b %errorlevel%
)

:: Build project
mingw32-make -j %NUMBER_OF_PROCESSORS%
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    exit /b %errorlevel%
)

:: Deploy Qt DLLs
echo [INFO] Running windeployqt...
"%QT_DIR%\bin\windeployqt.exe" --compiler-runtime Patient_Data_Project.exe

if %errorlevel% neq 0 (
    echo [WARNING] windeployqt encountered an issue.
) else (
    echo [SUCCESS] Deployment complete.
)

endlocal
