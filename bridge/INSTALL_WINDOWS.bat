@echo off
setlocal
cd /d "%~dp0"
echo ===============================================
echo  Quantum Breaks AI - CG50 Bridge Installer
echo ===============================================
where py >nul 2>nul
if %errorlevel%==0 (
  py -m pip install -r requirements.txt
) else (
  python -m pip install -r requirements.txt
)
if errorlevel 1 (
  echo.
  echo Installation failed. Make sure Python 3 is installed.
  pause
  exit /b 1
)
echo.
echo Bridge dependencies installed.
echo Run START_QBAI_WINDOWS.bat when the serial adapter is connected.
pause
