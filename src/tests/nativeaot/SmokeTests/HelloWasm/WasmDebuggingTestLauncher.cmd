echo ====== WasmDebuggingTestLauncher.cmd ======
REM Skip %scriptPath% and %ExePath%
set "CMD=%*"
set "CMD=!CMD:%1% =!"
set "CMD=!CMD:%2% =!"
echo Running: !CMD!
!CMD!
