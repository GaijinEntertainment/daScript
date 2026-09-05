$ErrorActionPreference = 'Stop'
$repoRoot = (Resolve-Path "$PSScriptRoot/../../..").Path
$runtime = Join-Path $repoRoot 'bin/Release/daslang-live.exe'
$runRoot = Join-Path $repoRoot 'logs/latchpoint-scene'
if (!(Test-Path -LiteralPath $runtime)) { throw "Build this checkout's daslang-live first: $runtime" }
New-Item -ItemType Directory -Force $runRoot | Out-Null
$sceneReady = $false
try { $null = Invoke-RestMethod 'http://127.0.0.1:19091/status' -TimeoutSec 2; $sceneReady = $true } catch { }
if (!$sceneReady) {
    Start-Process $runtime -ArgumentList @('-no-module-cache', "$PSScriptRoot/main.das", '--live-port', '19091') -WorkingDirectory $runRoot -WindowStyle Hidden -RedirectStandardOutput "$runRoot/game-live.stdout.log" -RedirectStandardError "$runRoot/game-live.stderr.log"
}
Write-Output 'Latchpoint is a native daslang game: inference server :18082 and live development :19091. No Python bridge.'
