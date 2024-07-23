[CmdletBinding(PositionalBinding=$false)]
param(
    $InstallDir,
    [switch]$CI
)

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

mkdir wasmtime/bin

Expand-Archive -LiteralPath $WasmtimeTar -DestinationPath .
if ($IsWindows)
{
    move wasmtime-v23.0.1-x86_64-windows\wasmtime.exe wasmtime\bin\
}
else
{
    $WasmtimeTar = "wasmtime-v23.0.1-x86_64-linux.tar.xz"
}

if ($CI)
{
    if ($IsWindows)
    {
        $WasmtimeExecutable = "wasmtime-v23.0.1-x86_64-windows/wasmtime.exe"
    }
    else
    {
        $WasmtimeExecutable = "wasmtime-v23.0.1-x86_64-windows/wasmtime.exe"
    }

    Write-Host "Setting WASMTIME_EXECUTABLE to '$WasmtimeExecutable'"
    Write-Output "##vso[task.setvariable variable=WASMTIME_EXECUTABLE]$WasmtimeExecutable"
}
