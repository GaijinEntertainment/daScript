# Deploy dasllama-server as a self-contained JIT bundle.
#
# dasllama-server is JIT-only (per-box [tune]/[llvm_code] kernels), so it cannot be a baked
# daspkg -exe — it needs the live daslang toolchain to JIT-compile + lld-link its kernels at
# startup. This mirrors the minimal SDK layout the JIT expects (daslang under bin/Release,
# lld-link under bin/, the runtime import libs under lib/Release, LLVM.dll under lib/) plus the
# daslib/modules the server requires and the server sources. Launched from the bundle root,
# getDasRoot() resolves to the bundle (daslang lives in bin/Release), so daslib/modules/lib/bin
# all resolve.
#
# Run:  powershell -File utils/dasllama-server/deploy-jit.ps1 [-Dest E:\dasllama-server]
# Then, from the bundle:  python watchdog.py    (finds bin/Release/daslang.exe, runs -jit main.das)
#
# The config (dasllama-server.toml) is PRESERVED if it already exists.

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
foreach ($m in @("dasLLAMA","dasLLVM","dasVulkan","dasHV","dasSpirv","dasAudio","dasLiveHost","dasTreeSitter")) {
    robocopy (Join-Path $Repo "modules\$m") (Join-Path $Dest "modules\$m") @rc | Out-Null
}

# 4. server sources + control page + watchdog at the bundle root (main.das's require siblings
#    live beside it; SERVE_FILE reads control.html from the main.das dir). The watchdog is the
#    shared supervisor from utils/watchdog; watchdog.json pins the deployment name.
foreach ($f in @("main.das","openai_server.das","llm_scheduler.das","ask.das","wav2txt.das","control.html","watchdog.json","tray.ico",".das_package")) {
    Copy-Item (Join-Path $PSScriptRoot $f) $Dest -Force
}
Copy-Item (Join-Path $Repo "utils\watchdog\watchdog.py") $Dest -Force
# ship the box tune sidecar (main.das's app sidecar) so first launch runs the tuned kernels
# instead of re-tuning (which would exit 3 restart-loop under the watchdog).
$tune = Join-Path $PSScriptRoot "main.tune.json"
if (Test-Path $tune) { Copy-Item $tune (Join-Path $Dest "main.tune.json") -Force }

# 5. config: ship a default only when none exists; never clobber a deployed one.
$cfg = Join-Path $Dest "dasllama-server.toml"
if (-not (Test-Path $cfg)) {
    Copy-Item (Join-Path $PSScriptRoot "dasllama-server.toml") $cfg -Force
    Write-Host "deploy-jit: shipped default dasllama-server.toml"
} else {
    Write-Host "deploy-jit: preserved existing dasllama-server.toml"
}

# 6. remove any dead baked exe from a prior daspkg release (it would refuse to start now).
foreach ($stale in @("dasllama-server.exe","dasllama-server.lib","dasllama-server.map","dasllama-server.tune.json")) {
    $p = Join-Path $Dest $stale
    if (Test-Path $p) { Remove-Item $p -Force }
}

Write-Host "deploy-jit: done. Launch from $Dest with:  python watchdog.py"
