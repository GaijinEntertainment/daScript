#!/usr/bin/env pwsh
# Stage the docs-assets release MP4s into doc/source/_static/tutorials/.
# PowerShell twin of fetch.sh — see that file for the why and the re-record
# workflow. Requires gh on PATH (authenticated; the repo is public).

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot  = (Resolve-Path (Join-Path $scriptDir "..\..\..")).Path
$dest      = Join-Path $repoRoot "doc\source\_static\tutorials"

New-Item -ItemType Directory -Force $dest | Out-Null
$delay = 5
for ($attempt = 1; ; $attempt++) {
    gh release download docs-assets `
        --repo GaijinEntertainment/daScript `
        --pattern "*.mp4" `
        --dir $dest `
        --clobber
    if ($LASTEXITCODE -eq 0) { break }
    if ($attempt -ge 5) { exit 1 }
    Write-Host "[docs_assets] attempt $attempt failed, retrying in ${delay}s"
    Start-Sleep -Seconds $delay
    $delay *= 2
}
$count = (Get-ChildItem $dest -Filter *.mp4).Count
Write-Host "[docs_assets] staged $count MP4s into $dest"
