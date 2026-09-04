@echo off
REM Convenience runner for the pfgamesaveutil integration tests.
REM PFSECRETKEY (and a player ID) must be set for the cloud tests to run;
REM otherwise they are skipped with a logged message.

setlocal
cd /d "%~dp0"
py run_tests.py
endlocal
