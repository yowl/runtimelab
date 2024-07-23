[CmdletBinding(PositionalBinding=$false)]
param(
    $InstallDir,
    [switch]$CI
)

if (!(Test-Path $InstallDir))
{
    md $InstallDir
}

Set-Location -Path $InstallDir

if (!(Test-Path variable:global:IsWindows))
{
    $IsWindows = [Environment]::OSVersion.Platform -eq [PlatformID]::Win32NT
}

if ($IsWindows)
{
    $WasmtimeTar = "wasmtime-v23.0.1-x86_64-windows.zip"
}
else
{
    $WasmtimeTar = "wasmtime-v23.0.1-x86_64-linux.tar.xz"
}

Invoke-WebRequest -Uri https://github.com/bytecodealliance/wasmtime/releases/download/v23.0.1/$WasmtimeTar -OutFile $WasmtimeTar

if (!(Test-Path wasmtime/bin))
{
    md wasmtime/bin
}

if ($IsWindows)
{
    Expand-Archive -LiteralPath $WasmtimeTar -DestinationPath .
    move wasmtime-v23.0.1-x86_64-windows\wasmtime.exe wasmtime\bin\
    $WasmtimeExecutable = "wasmtime.exe"
}
else
{
    tar -xf $WasmtimeTar
    move wasmtime-v23.0.1-x86_64-linux/wasmtime wasmtime/bin/
    $WasmtimeExecutable = "wasmtime"
}

if ($CI)
{
    Write-Host "Setting WASMTIME_EXECUTABLE to '$InstallDir/wasm-tools/wasmtime/bin/$WasmtimeExecutable'"
    Write-Output "##vso[task.setvariable variable=WASMTIME_EXECUTABLE]$WasmtimeExecutable"
}
