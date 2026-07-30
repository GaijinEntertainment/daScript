#!/usr/bin/env pwsh
# rerecord_all.ps1 — sequentially re-record every APNG under
# doc/source/_static/tutorials/.
#
# All `tests/integration/record_*.das` drivers spawn their own daslang-live on
# port 9090, so the sweep is strictly serial. Full pass takes ~20 minutes for
# 37+ drivers; cross-cutting visual changes (narrate placement, theme, font
# scale) typically need it. Per-driver runtime = its with_recording_app
# max_seconds + ~3s host boot/drain.
#
# After re-recording, .apng files land in doc/source/_static/tutorials/
# (gitignored -- the intermediate artifact, not the deliverable). Convert
# each to .mp4 via ffmpeg before committing. PowerShell equivalent of
# the bash loop in tests/integration/README.md:
#
#   Set-Location doc/source/_static/tutorials
#   Get-ChildItem *.apng | ForEach-Object {
#       $base = $_.BaseName
#       ffmpeg -y -loglevel error -i $_.Name -c:v libx264 -crf 23 `
#              -pix_fmt yuv420p -movflags +faststart "$base.mp4"
#   }
#
# Then `git add doc/source/_static/tutorials/*.mp4` and push.
#
# Run:
#   pwsh modules/dasImgui/tests/integration/rerecord_all.ps1 -DaslangExe D:/Work/daScript/bin/Release/daslang.exe
#
# Or set once:
#   $env:DASLANG_EXE = "D:/Work/daScript/bin/Release/daslang.exe"
#   pwsh modules/dasImgui/tests/integration/rerecord_all.ps1
#
# Flags:
#   -DaslangExe <path>  daslang.exe; defaults to $env:DASLANG_EXE.
#   -From <name>        Resume at driver whose name (without `record_` prefix /
#                       `.das` suffix) is <name>; skip everything earlier.
#   -Skip <a,b,c>       Comma-separated names to skip.
#   -DryRun             Print what would run; spawn nothing.
#   -StopOnFail         Abort the sweep on the first non-zero exit.
#
# Failures are collected and reported at the end; the sweep continues by
# default (one bad driver shouldn't waste the other 20 minutes).

param(
    [string]$DaslangExe = $env:DASLANG_EXE,
    [string]$From = "",
    [string]$Skip = "",
    [switch]$DryRun,
    [switch]$StopOnFail
)

if (-not $DaslangExe) {
    Write-Host "FAIL: daslang.exe path not set." -ForegroundColor Red
    Write-Host "      Pass -DaslangExe <path> or set `$env:DASLANG_EXE."
    exit 2
}
if (-not (Test-Path $DaslangExe)) {
    Write-Host "FAIL: daslang.exe not found at '$DaslangExe'." -ForegroundColor Red
    exit 2
}

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot  = (Resolve-Path (Join-Path $scriptDir "..\..")).Path

Write-Host "[rerecord_all] daslang:      $DaslangExe"
Write-Host "[rerecord_all] project_root: $repoRoot"

# ===== Discover drivers =====
$drivers = Get-ChildItem -Path $scriptDir -Filter "record_*.das" | Sort-Object Name
Write-Host "[rerecord_all] $($drivers.Count) drivers found"

# ===== Build skip set =====
$skipSet = @{}
if ($Skip) {
    foreach ($s in $Skip.Split(",")) {
        $key = $s.Trim()
        if ($key) { $skipSet[$key] = $true }
    }
}

# ===== Sweep =====
$skipUntilFound = $From -ne ""
$results = @()
$skipped = 0
$started = Get-Date

foreach ($d in $drivers) {
    $name = $d.BaseName -replace "^record_", ""

    if ($skipUntilFound) {
        if ($name -eq $From) {
            $skipUntilFound = $false
        } else {
            Write-Host "  SKIP (before -From): $name" -ForegroundColor DarkGray
            $skipped++
            continue
        }
    }
    if ($skipSet.ContainsKey($name)) {
        Write-Host "  SKIP (in -Skip):     $name" -ForegroundColor DarkGray
        $skipped++
        continue
    }

    Write-Host ""
    Write-Host "==== $name ====" -ForegroundColor Cyan

    if ($DryRun) {
        Write-Host "  [dry-run] would run: $($d.FullName)"
        continue
    }

    $t0 = Get-Date
    & $DaslangExe -project_root "$repoRoot" "$($d.FullName)"
    $code = $LASTEXITCODE
    $elapsed = (Get-Date) - $t0

    $results += [PSCustomObject]@{
        Name     = $name
        ExitCode = $code
        Seconds  = [int]$elapsed.TotalSeconds
    }

    if ($code -eq 0) {
        Write-Host "  OK ($([int]$elapsed.TotalSeconds)s)" -ForegroundColor Green
    } else {
        Write-Host "  FAIL exit=$code ($([int]$elapsed.TotalSeconds)s)" -ForegroundColor Red
        if ($StopOnFail) {
            Write-Host "  -StopOnFail set; aborting sweep." -ForegroundColor Red
            break
        }
    }
}

$totalElapsed = (Get-Date) - $started

# ===== Summary =====
Write-Host ""
Write-Host "==== Summary ====" -ForegroundColor Cyan
$ok   = @($results | Where-Object { $_.ExitCode -eq 0 }).Count
$fail = @($results | Where-Object { $_.ExitCode -ne 0 }).Count
Write-Host "  OK:      $ok"
Write-Host "  FAIL:    $fail"
Write-Host "  Skipped: $skipped"
$mins = [int]$totalElapsed.TotalMinutes
$secs = [int]($totalElapsed.TotalSeconds % 60)
Write-Host "  Total:   ${mins}m ${secs}s"

if ($fail -gt 0) {
    Write-Host ""
    Write-Host "  Failed drivers:" -ForegroundColor Red
    foreach ($r in $results | Where-Object { $_.ExitCode -ne 0 }) {
        Write-Host "    $($r.Name) (exit=$($r.ExitCode), $($r.Seconds)s)"
    }
    exit 1
}
exit 0
