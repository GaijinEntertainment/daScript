# utils/rerecord.ps1 - clean full re-record of THIS repo's tutorial videos.
#
# Default (no args) re-records the ENTIRE repo in four phases:
#   Clean    wipe apng/mp4/music/ffmpeg.txt + voiceover/ under doc/source/_static/tutorials
#   Prepare  per driver: prepare_recording.das -> TTS each say() line via Kokoro (:8880)
#   Record   per driver: run it -> .apng + voiceover sidecar
#   Convert  per .apng:  convert_recording.das -> mux video + music bed + voiceovers -> .mp4
#
# Run one repo at a time. Kokoro TTS must be up at :8880 for the Prepare phase.
# Filters: -Only NAME (single driver, NAME = stem without record_/.das), -From NAME (resume),
#          -Skip "a,b", -Skip<Phase>, -DryRun, -StopOnFail.

param(
    [string]$DaslangExe  = $(if ($env:DASLANG) { $env:DASLANG } else { "D:/Work/daScript/bin/Release/daslang.exe" }),
    [string]$DasRoot     = "D:/Work/daScript",   # convert: music render + sf2 live under the daslang source tree
    [string]$DasImguiRoot= "",                   # holds prepare/convert + the imgui module; default below
    [string]$Voice       = "bf_emma",
    [switch]$SkipClean,
    [switch]$SkipPrepare,
    [switch]$SkipRecord,
    [switch]$SkipConvert,
    [string]$From = "",
    [string]$Skip = "",
    [string]$Only = "",
    [switch]$DryRun,
    [switch]$StopOnFail
)

$ErrorActionPreference = "Continue"
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path

# ===== per-repo config (dasImgui owns the recording infra + the imgui module) =====
if (-not $DasImguiRoot) { $DasImguiRoot = $RepoRoot }
$ModuleArgs = @("-project_root", $RepoRoot)
$AssetRoot  = ""                                  # dasImgui default asset root
# ==================================================================================

$tutDir        = Join-Path $RepoRoot "doc/source/_static/tutorials"
$voDir         = Join-Path $tutDir "voiceover"
$prepareScript = Join-Path $DasImguiRoot "utils/prepare_recording.das"
$convertScript = Join-Path $DasImguiRoot "utils/convert_recording.das"
$scanDir       = Join-Path $RepoRoot "tests/integration"

if (-not (Test-Path $DaslangExe))    { Write-Host "FAIL: daslang not found: $DaslangExe" -ForegroundColor Red; exit 2 }
if (-not (Test-Path $prepareScript)) { Write-Host "FAIL: missing $prepareScript (DasImguiRoot=$DasImguiRoot)" -ForegroundColor Red; exit 2 }
if (-not (Test-Path $scanDir))       { Write-Host "FAIL: no $scanDir" -ForegroundColor Red; exit 2 }

$drivers = Get-ChildItem -Path $scanDir -Filter "record_*.das" | Sort-Object Name
Write-Host "[rerecord] repo:    $RepoRoot"
Write-Host "[rerecord] daslang: $DaslangExe"
Write-Host "[rerecord] drivers: $($drivers.Count)"
Write-Host "[rerecord] modules: $($ModuleArgs -join ' ')"
Write-Host "[rerecord] assets:  $(if ($AssetRoot) { $AssetRoot } else { '(dasImgui default)' })"

$skipSet = @{}
if ($Skip) { foreach ($s in $Skip.Split(",")) { $k = $s.Trim(); if ($k) { $skipSet[$k] = $true } } }
$skipUntil = $From -ne ""

function Sel($name) {
    if ($Only -ne "" -and $name -ne $Only) { return $false }
    if ($script:skipUntil) { if ($name -eq $From) { $script:skipUntil = $false } else { return $false } }
    if ($skipSet.ContainsKey($name)) { return $false }
    return $true
}

# ---- Clean ----
if (-not $SkipClean) {
    Write-Host "`n==== CLEAN ====" -ForegroundColor Cyan
    if ($DryRun) {
        Write-Host "  [dry-run] would remove *.apng *.mp4 *_music.wav *.mp4.ffmpeg.txt and voiceover/ under $tutDir"
    } else {
        Get-ChildItem -Path (Join-Path $tutDir '*') -Include *.apng,*.mp4,*_music.wav,*.mp4.ffmpeg.txt -File -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
        if (Test-Path $voDir) { Remove-Item -Recurse -Force $voDir -ErrorAction SilentlyContinue }
        Write-Host "  cleaned $tutDir"
    }
}

$fail = @()

# ---- Prepare ----
if (-not $SkipPrepare) {
    Write-Host "`n==== PREPARE (TTS) ====" -ForegroundColor Cyan
    foreach ($d in $drivers) {
        $name = $d.BaseName -replace "^record_", ""
        if (-not (Sel $name)) { continue }
        $pargs = @($ModuleArgs) + @($prepareScript, "--", "--driver", $d.FullName, "--voice", $Voice)
        if ($AssetRoot) { $pargs += @("--asset-root", $AssetRoot) }
        if ($DryRun) { Write-Host "  [dry-run] $DaslangExe $($pargs -join ' ')"; continue }
        Write-Host "  prepare $name" -ForegroundColor DarkCyan
        & $DaslangExe @pargs
        if ($LASTEXITCODE -ne 0) { $fail += "prepare:$name"; Write-Host "    FAIL prepare $name" -ForegroundColor Red; if ($StopOnFail) { break } }
    }
}

# ---- Record ----
if (-not $SkipRecord -and $fail.Count -eq 0) {
    Write-Host "`n==== RECORD ====" -ForegroundColor Cyan
    foreach ($d in $drivers) {
        $name = $d.BaseName -replace "^record_", ""
        if (-not (Sel $name)) { continue }
        $rargs = @($ModuleArgs) + @($d.FullName)
        if ($DryRun) { Write-Host "  [dry-run] $DaslangExe $($rargs -join ' ')"; continue }
        Write-Host "  record $name" -ForegroundColor DarkCyan
        $t0 = Get-Date
        & $DaslangExe @rargs
        $sec = [int]((Get-Date) - $t0).TotalSeconds
        if ($LASTEXITCODE -ne 0) { $fail += "record:$name"; Write-Host "    FAIL record $name (${sec}s)" -ForegroundColor Red; if ($StopOnFail) { break } }
        else { Write-Host "    ok (${sec}s)" -ForegroundColor Green }
    }
}

# ---- Convert ----
if (-not $SkipConvert) {
    Write-Host "`n==== CONVERT ====" -ForegroundColor Cyan
    $apngs = Get-ChildItem -Path $tutDir -Filter "*.apng" -File -ErrorAction SilentlyContinue | Sort-Object Name
    Write-Host "  $($apngs.Count) apng(s) to convert"
    foreach ($a in $apngs) {
        if (-not (Sel $a.BaseName)) { continue }
        if ($DryRun) { Write-Host "  [dry-run] convert $($a.Name)"; continue }
        Write-Host "  convert $($a.BaseName)" -ForegroundColor DarkCyan
        & $DaslangExe "-load_module" $DasImguiRoot $convertScript "--" "--apng" $a.FullName "--das_root" $DasRoot
        if ($LASTEXITCODE -ne 0) { $fail += "convert:$($a.BaseName)"; Write-Host "    FAIL convert $($a.BaseName)" -ForegroundColor Red; if ($StopOnFail) { break } }
    }
}

# ---- Summary ----
Write-Host "`n==== SUMMARY ($RepoRoot) ====" -ForegroundColor Cyan
$mp4s = @(Get-ChildItem -Path $tutDir -Filter "*.mp4" -File -ErrorAction SilentlyContinue)
Write-Host "  mp4s produced: $($mp4s.Count)"
if ($fail.Count -gt 0) {
    Write-Host "  FAILURES ($($fail.Count)):" -ForegroundColor Red
    foreach ($f in $fail) { Write-Host "    $f" -ForegroundColor Red }
    exit 1
}
Write-Host "  all phases clean" -ForegroundColor Green
exit 0
