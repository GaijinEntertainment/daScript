# Deploy dasllama-server as a self-contained JIT bundle - the per-box-tuned alternative to the
# baked `daspkg release` bundle.
#
# The JIT compiles and lld-links the server's [tune]/[llvm_code] kernels for this box at
# startup, so the bundle carries the live daslang toolchain. This mirrors the minimal SDK layout
# the JIT expects (daslang under bin/Release, lld-link under bin/, the runtime import libs under
# lib/Release, LLVM.dll under lib/) plus the daslib/modules the server requires, and the server
# sources at their repo path (utils/dasllama-server, utils/common beside it), so the server's
# relative requires (../common, ../../modules) resolve inside the bundle. getDasRoot() resolves to
# the bundle (daslang lives in bin/Release), so daslib/modules/lib/bin all resolve.
#
# Run:  powershell -File utils/dasllama-server/deploy-jit.ps1 [-Dest E:\dasllama-server]
# Then, from the bundle:  .\watchdog.exe    (its watchdog.json points --cwd at utils/dasllama-server;
# finds bin/Release/daslang.exe beside itself, runs -jit main.das there)
#
# The config (utils/dasllama-server/dasllama-server.toml) is PRESERVED if it already exists.

param(
    [string]$Dest = "E:\dasllama-server"
)

$ErrorActionPreference = "Stop"
$Repo = (Resolve-Path "$PSScriptRoot\..\..").Path
$rc = @("/MIR","/NFL","/NDL","/NJH","/NJS","/NP")   # mirror + quiet

Write-Host "deploy-jit: repo=$Repo -> dest=$Dest"
New-Item -ItemType Directory -Force -Path $Dest | Out-Null

# 1. bin/: the JIT toolchain. daslang + its runtime/LLVM DLLs under bin/Release (so getDasRoot()
#    resolves the bundle root), the linker set (lld-link/lld/llvm-lib) under bin/.
robocopy (Join-Path $Repo "bin\Release") (Join-Path $Dest "bin\Release") *.exe *.dll @rc | Out-Null
foreach ($t in @("lld-link.exe","lld.exe","llvm-lib.exe")) {
    $src = Join-Path $Repo "bin\$t"
    if (Test-Path $src) { New-Item -ItemType Directory -Force -Path (Join-Path $Dest "bin") | Out-Null; Copy-Item $src (Join-Path $Dest "bin") -Force }
}

# 2. lib/: LLVM.dll (dasLLVM binding target) + the runtime import libs the JIT links kernels against.
New-Item -ItemType Directory -Force -Path (Join-Path $Dest "lib") | Out-Null
Copy-Item (Join-Path $Repo "lib\LLVM.dll") (Join-Path $Dest "lib") -Force
robocopy (Join-Path $Repo "lib\Release") (Join-Path $Dest "lib\Release") libDaScriptDyn.lib libDaScriptDyn_runtime.lib @rc | Out-Null

# 3. daslib (whole) + the module dirs in the server's require closure.
robocopy (Join-Path $Repo "daslib") (Join-Path $Dest "daslib") @rc | Out-Null
foreach ($m in @("dasLLAMA","dasLLVM","dasVulkan","dasHV","dasSpirv","dasAudio","dasLiveHost","dasTreeSitter","dasLR1","dasStbImage")) {
    robocopy (Join-Path $Repo "modules\$m") (Join-Path $Dest "modules\$m") @rc | Out-Null
}

# 4. server sources at their repo path - the whole folder minus its tests, so a new sibling the
#    server requires ships without a list to keep - and utils/common, which main.das requires as
#    ../common. The box tune sidecar (main.tune.json, beside main.das) is copied when the repo has
#    one and kept otherwise (the mirror skips it), so first launch
#    runs the tuned kernels instead of re-tuning (which would exit 3 restart-loop under the watchdog).
#    The watchdog (the shared supervisor, the static bin\Release\watchdog.exe) sits at the bundle
#    root beside bin/; its watchdog.json is the server's plus the cwd that points it at main.das.
$Srv = Join-Path $Dest "utils\dasllama-server"
$xt = @("/XD","tests","/XF","test_*.das","_server_rig.das","dasllama-server.toml","main.tune.json")
robocopy $PSScriptRoot $Srv @rc @xt | Out-Null
$tune = Join-Path $PSScriptRoot "main.tune.json"
if (Test-Path $tune) { Copy-Item $tune (Join-Path $Srv "main.tune.json") -Force }
robocopy (Join-Path $Repo "utils\common") (Join-Path $Dest "utils\common") @rc "/XD" "tests" | Out-Null
Copy-Item (Join-Path $Repo "bin\Release\watchdog.exe") $Dest -Force
$wd = Get-Content (Join-Path $PSScriptRoot "watchdog.json") -Raw | ConvertFrom-Json
$wd | Add-Member -NotePropertyName cwd -NotePropertyValue "utils/dasllama-server" -Force
[IO.File]::WriteAllText((Join-Path $Dest "watchdog.json"), ($wd | ConvertTo-Json))   # no BOM: Windows PowerShell 5 writes one under -Encoding UTF8

# 5. config: ship a default only when none exists; never clobber a deployed one.
$cfg = Join-Path $Srv "dasllama-server.toml"
$flat = Join-Path $Dest "dasllama-server.toml"
if ((-not (Test-Path $cfg)) -and (Test-Path $flat)) {
    Move-Item $flat $cfg
    Write-Host "deploy-jit: moved the flat bundle's dasllama-server.toml beside main.das"
}
if (-not (Test-Path $cfg)) {
    Copy-Item (Join-Path $PSScriptRoot "dasllama-server.toml") $cfg -Force
    Write-Host "deploy-jit: shipped default dasllama-server.toml"
} else {
    Write-Host "deploy-jit: preserved existing dasllama-server.toml"
}

# 6. remove any dead baked exe from a prior daspkg release (it would refuse to start now), and the
#    server sources a prior flat bundle left at the root.
$flatSrc = @("main.das","openai_server.das","model_catalog.das","serving_knobs.das","cli.das","cli_args.das","cli_repl.das","control.html","tray.ico",".das_package","main.tune.json")
foreach ($stale in @("dasllama-server.exe","dasllama-server.lib","dasllama-server.map","dasllama-server.tune.json") + $flatSrc) {
    $p = Join-Path $Dest $stale
    if (Test-Path $p) { Remove-Item $p -Force }
}

Write-Host "deploy-jit: done. Launch from $Dest with:  .\watchdog.exe"
