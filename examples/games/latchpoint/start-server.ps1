param(
    [string]$Config = "$PSScriptRoot/server.local.toml"
)
$ErrorActionPreference = 'Stop'
$repoRoot = (Resolve-Path "$PSScriptRoot/../../..").Path
$runtime = Join-Path $repoRoot 'bin/Release/daslang.exe'
$runRoot = Join-Path $repoRoot 'logs/latchpoint-server'
if (!(Test-Path -LiteralPath $runtime)) { throw "Build this checkout's daslang runtime first: $runtime" }
if (!(Test-Path -LiteralPath $Config)) { throw "Create $Config from server.example.toml with your model paths." }
New-Item -ItemType Directory -Force $runRoot | Out-Null
$env:DAS_JOBQUE_THREADS = '8'
$env:DASLLAMA_GPU = '0'
python "$repoRoot/utils/watchdog/watchdog.py" --name latchpoint --daslang $runtime --script "$repoRoot/utils/dasllama-server/main.das" --cwd $runRoot --log "$runRoot/watchdog.log" --pid-file "$runRoot/watchdog.pid" --health-url 'http://127.0.0.1:18082/v1/models' --shutdown-url 'http://127.0.0.1:18082/shutdown' -- --config $Config
exit $LASTEXITCODE
