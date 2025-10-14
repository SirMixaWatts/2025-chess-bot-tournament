# PowerShell script to move build artifacts into src/py/bin/
param()
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$Root = Resolve-Path "$ScriptDir/.."
$PyDir = Join-Path $Root
$BinDir = Join-Path $PyDir "bin"

if (-not (Test-Path $BinDir)) { New-Item -ItemType Directory -Path $BinDir | Out-Null }

$artifacts = @('chess_engine.exe','libchess.dll','libchess.a','engine_out.txt')
foreach ($f in $artifacts) {
    $src = Join-Path $PyDir $f
    if (Test-Path $src) { Move-Item -Path $src -Destination $BinDir -Force }
}
Get-ChildItem -Path $PyDir -Filter '*.o' -File -ErrorAction SilentlyContinue | Move-Item -Destination $BinDir -Force
Write-Output "Build artifacts moved to $BinDir"