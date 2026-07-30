#!/usr/bin/env pwsh
# Stage the docs-assets release MP4s into doc/source/_static/tutorials/.
# PowerShell twin of fetch.sh — see that file for the why and the re-record
# workflow. Requires gh on PATH (authenticated; the repo is public).

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot  = (Resolve-Path (Join-Path $scriptDir "..\..")).Path
$dest      = Join-Path $repoRoot "doc\source\_static\tutorials"

New-Item -ItemType Directory -Force $dest | Out-Null
gh release download docs-assets `
    --repo GaijinEntertainment/daScript `
    --pattern "*.mp4" `
    --dir $dest `
    --clobber
if ($LASTEXITCODE -ne 0) { exit 1 }
$count = (Get-ChildItem $dest -Filter *.mp4).Count
Write-Host "[docs_assets] staged $count MP4s into $dest"
