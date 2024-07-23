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

if ($IsWIndows)
{
    $WasmerTar=wasmer-linux-amd64.tar.gz
}
else
{
    $WasmerTar=wasmer-windows-amd64.tar.gz
}

Invoke-WebRequest -Uri https://github.com/wasmerio/wasmer/releases/download/v3.3.0/$WasmerTar -OutFile $WasmerTar

mkdir wasmer

tar -xzf $WasmerExe -C wasmer

if ($CI)
{
    Write-Host "Setting WASMER_EXECUTABLE to '$InstallDir/wasmer/bin/wasmer'"
    Write-Output "##vso[task.setvariable variable=WASMER_EXECUTABLE]$InstallDir/wasmer/bin/wasmer"
}
