#!/usr/bin/env pwsh
# CI smoke test: full daspkg install -> release -> launch cycle for the sequence
# game on Windows. Exercises the release_include_dll manifest API + daspkg's
# native-dep shipping + the LoadLibraryEx-based loader resolution end-to-end.
#
# Args: $args[0] (optional) - path to a daslang install root (must contain
#       bin/daslang.exe and utils/daspkg/main.das). Defaults to env DAS_ROOT.
#
# Exit: 0 on success; non-zero on any step failure.

$ErrorActionPreference = "Stop"

$dasRoot = if ($args.Count -ge 1) { $args[0] } elseif ($env:DAS_ROOT) { $env:DAS_ROOT } else { "" }
if (-not $dasRoot) {
    Write-Error "usage: ci_smoke_test.ps1 <daslang-install-root>  (or set DAS_ROOT)"
    exit 2
}
$dasRoot = (Resolve-Path $dasRoot).Path
# Honor $env:BIN if set (extended_checks sets it to bin/Release for Visual
# Studio multi-config generators); default to <dasRoot>/bin for installed layouts.
$binDir = if ($env:BIN) { (Resolve-Path $env:BIN).Path } else { Join-Path $dasRoot "bin" }
$daslang = Join-Path $binDir "daslang.exe"
$daspkg  = Join-Path $dasRoot "utils\daspkg\main.das"
foreach ($p in @($daslang, $daspkg)) {
    if (-not (Test-Path $p)) { Write-Error "missing: $p"; exit 2 }
}

# Source dir of THIS script's project (the in-tree sequence example).
$srcDir = Split-Path -Parent $PSCommandPath
$workDir = Join-Path $env:TEMP "daspkg_smoke_sequence_$([guid]::NewGuid().ToString('N').Substring(0,8))"
Write-Host "smoke: src=$srcDir"
Write-Host "smoke: work=$workDir"

# Copy the project to a scratch dir so we don't dirty the source tree on rerun.
# Filter out previous bundle/install artifacts so we always start clean.
New-Item -ItemType Directory -Path $workDir | Out-Null
Get-ChildItem -Path $srcDir -Force | Where-Object {
    $_.Name -notin @("modules", "sequence", "daspkg.lock", ".daspkg_cache", ".daspkg_tmp", ".daspkg.log")
} | ForEach-Object {
    Copy-Item -LiteralPath $_.FullName -Destination (Join-Path $workDir $_.Name) -Recurse -Force
}

try {
    Push-Location $workDir
    Write-Host "smoke: daspkg install"
    & $daslang $daspkg -- install
    if ($LASTEXITCODE -ne 0) { Write-Error "daspkg install failed (rc=$LASTEXITCODE)"; exit 1 }

    Write-Host "smoke: daspkg release"
    & $daslang $daspkg -- release
    if ($LASTEXITCODE -ne 0) { Write-Error "daspkg release failed (rc=$LASTEXITCODE)"; exit 1 }

    # Verify expected bundle artifacts. Fail the test if anything is missing -
    # these are the load-bearing files for a runnable bundle.
    $bundle = Join-Path $workDir "sequence"
    $required = @(
        (Join-Path $bundle "sequence.exe"),
        (Join-Path $bundle "libDaScriptDyn.dll"),
        (Join-Path $bundle "libDaScriptDyn_runtime.dll"),
        (Join-Path $bundle "modules\dasGlfw\dasModuleGlfw.shared_module"),
        # The whole point: native dep colocated with its shared_module.
        (Join-Path $bundle "modules\dasGlfw\glfw3.dll"),
        # Regression check: dep release_include assets still ship.
        (Join-Path $bundle "modules\das-cards\cards\svg-cards.svg")
    )
    foreach ($f in $required) {
        if (-not (Test-Path $f)) {
            Write-Error "missing bundle artifact: $f"
            exit 1
        }
        Write-Host "smoke: ok  $f"
    }

    # Launch the bundled exe with --smoke under a 60s outer timeout. --smoke
    # exits cleanly after module load + arg parse, before glfwCreateWindow.
    # windows-latest-fat runners are headless (no interactive WindowServer
    # session), so a full GLFW init crashes the same way macOS arm64-xlarge
    # does. --smoke still exercises the complete loader chain (every `require`
    # resolves its .shared_module and dlopens its native deps at module-load
    # time, before main() runs), which is the actual change under test.
    # Failure modes: missing DLL -> non-zero exit immediately; hang -> timeout.
    # Use System.Diagnostics.Process directly because Start-Process -PassThru +
    # -RedirectStandard* drops ExitCode (known PowerShell quirk).
    Write-Host "smoke: launching bundled sequence.exe --smoke"
    $exe = Join-Path $bundle "sequence.exe"
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $exe
    $psi.Arguments = "--smoke"
    $psi.WorkingDirectory = $bundle
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.UseShellExecute = $false
    $proc = [System.Diagnostics.Process]::Start($psi)
    # Async-drain streams to prevent buffer-fill deadlocks.
    $outTask = $proc.StandardOutput.ReadToEndAsync()
    $errTask = $proc.StandardError.ReadToEndAsync()
    $exited = $proc.WaitForExit(60000)
    if (-not $exited) {
        Write-Host "smoke: TIMEOUT (60s) - sequence did not exit; killing"
        $proc.Kill()
        $proc.WaitForExit()
        $outTask.Wait()
        $errTask.Wait()
        Write-Host "--- stdout ---"; Write-Host $outTask.Result
        Write-Host "--- stderr ---"; Write-Host $errTask.Result
        Write-Error "smoke: outer timeout fired"
        exit 1
    }
    # Block until WaitForExit confirms streams are flushed (idiomatic .NET pattern).
    $proc.WaitForExit()
    $outTask.Wait()
    $errTask.Wait()
    Write-Host "--- stdout ---"; Write-Host $outTask.Result
    Write-Host "--- stderr ---"; Write-Host $errTask.Result
    $rc = $proc.ExitCode
    if ($rc -ne 0) {
        Write-Error "smoke: sequence exited with rc=$rc"
        exit 1
    }
    Write-Host "smoke: PASS (rc=0)"
} finally {
    Pop-Location
    # Leave the work dir on disk for postmortem; CI runners get cleaned anyway.
    Write-Host "smoke: artifacts in $workDir"
}
