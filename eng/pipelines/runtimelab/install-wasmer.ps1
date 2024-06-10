[CmdletBinding(PositionalBinding=$false)]
param(
    $InstallDir,
    [switch]$CI
)

Set-Location -Path $InstallDir

Invoke-WebRequest -Uri https://github.com/wasmerio/wasmer/releases/download/v3.3.0/wasmer-windows-amd64.tar.gz -OutFile wasmer-windows-amd64.tar.gz

mkdir wasmer

tar -xzf wasmer-windows-amd64.tar.gz -C wasmer

if ($CI)
{
    Write-Host "Setting WASMER_EXECUTABLE to '$InstallDir/wasmer/bin/wasmer.exe'"
    Write-Output "##vso[task.setvariable variable=WASMER_EXECUTABLE]$InstallDir/wasmer/bin/wasmer.exe"
}
