@echo off
echo ========================================
echo Fourth Protocol - Doxygen Documentation Generator
echo ========================================
echo.

REM Check if Doxygen is installed
where doxygen >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Doxygen is not installed or not in PATH!
    echo Please install Doxygen from https://www.doxygen.nl/download.html
    echo.
    pause
    exit /b 1
)

echo Generating documentation...
echo.

REM Run Doxygen
doxygen Doxyfile

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Documentation generated successfully!
    echo ========================================
    echo.
    echo Documentation location: docs\html\index.html
    echo.
    
    REM Ask if user wants to open the documentation
    set /p OPEN="Open documentation in browser? (Y/N): "
    if /i "%OPEN%"=="Y" (
        start docs\html\index.html
    )
) else (
    echo.
    echo ========================================
    echo ERROR: Documentation generation failed!
    echo ========================================
    echo.
    echo Check the Doxyfile configuration and ensure all files are accessible.
)

echo.
pause
