#!/usr/bin/env pwsh
# smoke_curl.ps1 — raw-HTTP diagnostic for daslang-live + dasImgui dispatch.
#
# Spawns daslang-live against examples/save_demo/main.das, then drives it
# entirely via Invoke-RestMethod. NO playwright module, NO dastest. If this
# script PASSes but the dastest suite FAILs, the bug is in the playwright
# wiring, not the underlying daslang-live + dasImgui plumbing. If THIS
# script fails, the wiring is broken at a deeper layer.
#
# Run: pwsh modules/dasImgui/tests/integration/smoke_curl.ps1
# Override exe:  $env:DASLANG_LIVE_EXE = "..." ; pwsh smoke_curl.ps1

$ErrorActionPreference = "Stop"

# ===== Paths =====
$scriptDir   = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot    = Resolve-Path (Join-Path $scriptDir "..\..\..\..")
$defaultExe  = Join-Path $repoRoot "bin\Release\daslang-live.exe"
$liveExe     = if ($env:DASLANG_LIVE_EXE) { $env:DASLANG_LIVE_EXE } else { $defaultExe }
$featureApp  = Resolve-Path (Join-Path $scriptDir "..\..\examples\save_demo\main.das")
$baseUrl     = "http://127.0.0.1:9090"

if (-not (Test-Path $liveExe)) {
    Write-Host "FAIL: daslang-live.exe not found at $liveExe" -ForegroundColor Red
    Write-Host "      override with `$env:DASLANG_LIVE_EXE"
    exit 1
}

Write-Host "[smoke_curl] exe:     $liveExe"
Write-Host "[smoke_curl] feature: $featureApp"

# ===== Spawn daslang-live =====
$proc = Start-Process -FilePath $liveExe -ArgumentList @("$featureApp") `
                      -PassThru -WindowStyle Hidden

function Cleanup {
    if ($proc -and -not $proc.HasExited) {
        try {
            Invoke-RestMethod -Method Post -Uri "$baseUrl/shutdown" -Body "" `
                              -ContentType "application/json" -TimeoutSec 2 | Out-Null
        } catch {}
        Start-Sleep -Milliseconds 500
        if (-not $proc.HasExited) {
            Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
        }
    }
}

try {
    # ===== Wait until /status returns 200 OK =====
    $readyDeadline = (Get-Date).AddSeconds(30)
    $ready = $false
    while ((Get-Date) -lt $readyDeadline) {
        try {
            $resp = Invoke-WebRequest -Uri "$baseUrl/status" -TimeoutSec 1 -UseBasicParsing
            if ($resp.StatusCode -eq 200) { $ready = $true; break }
        } catch {}
        Start-Sleep -Milliseconds 200
    }
    if (-not $ready) {
        Write-Host "FAIL: daslang-live did not become ready within 30s" -ForegroundColor Red
        Cleanup
        exit 1
    }
    Write-Host "[smoke_curl] ready"

    # ===== Snapshot — verify SAVE_BTN registered =====
    $snapBody = '{"name":"imgui_snapshot"}'
    $snap = Invoke-RestMethod -Method Post -Uri "$baseUrl/command" `
                              -Body $snapBody -ContentType "application/json"
    $saveBtnKind = $snap.globals.SAVE_BTN.kind
    if ($saveBtnKind -ne "button") {
        Write-Host "FAIL: SAVE_BTN.kind = '$saveBtnKind', expected 'button'" -ForegroundColor Red
        Cleanup
        exit 1
    }
    Write-Host "[smoke_curl] snapshot: SAVE_BTN.kind == 'button'"

    # ===== Click SAVE_BTN =====
    $clickBody = '{"name":"imgui_click","args":{"target":"SAVE_BTN"}}'
    Invoke-RestMethod -Method Post -Uri "$baseUrl/command" `
                      -Body $clickBody -ContentType "application/json" | Out-Null
    Write-Host "[smoke_curl] click posted"

    # ===== Poll until STATUS_TEXT.payload.value == "saved" =====
    $clickDeadline = (Get-Date).AddSeconds(5)
    $observed = ""
    while ((Get-Date) -lt $clickDeadline) {
        $snap2 = Invoke-RestMethod -Method Post -Uri "$baseUrl/command" `
                                   -Body $snapBody -ContentType "application/json"
        $observed = "$($snap2.globals.STATUS_TEXT.payload.value)"
        if ($observed -eq "saved") { break }
        Start-Sleep -Milliseconds 100
    }
    if ($observed -ne "saved") {
        Write-Host "FAIL: STATUS_TEXT.value = '$observed', expected 'saved'" -ForegroundColor Red
        Cleanup
        exit 1
    }
    Write-Host "[smoke_curl] STATUS_TEXT.value == 'saved'"

    Write-Host "PASS" -ForegroundColor Green
    Cleanup
    exit 0
} catch {
    Write-Host "FAIL: unhandled error: $_" -ForegroundColor Red
    Cleanup
    exit 1
}
