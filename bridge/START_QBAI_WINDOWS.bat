@echo off
setlocal
cd /d "%~dp0"
where py >nul 2>nul
if %errorlevel%==0 (
  py qb_launch.py
) else (
  python qb_launch.py
)
if errorlevel 1 (
  echo.
  echo The bridge stopped with an error.
)
pause
