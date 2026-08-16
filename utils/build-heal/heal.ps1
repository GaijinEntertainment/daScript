# Runs the cmake build and heals corrupt-obj link failures. An interrupted compile leaves a
# truncated .obj with a fresh timestamp, which incremental builds trust forever: the linker
# reports LNK1103 "debugging information corrupt" (or LNK1136), or — when the truncation hits
# a code section — links a binary that crashes at startup. On a corrupt-obj signature this
# deletes the named .obj files so they recompile, and retries the build.
#
# Usage: powershell utils/build-heal/heal.ps1 [-Target daslang[,tests-cpp-small]]
#        [-Config Release] [-Jobs 32] [-BuildDir build] [-MaxHeals 2]
param(
    [string[]] $Target = @(),
    [string] $Config = "Release",
    [int] $Jobs = 32,
    [int] $MaxHeals = 2,
    [string] $BuildDir = "build"
)

$ErrorActionPreference = "Continue"

for ($round = 0; $round -le $MaxHeals; $round++) {
    $buildArgs = @("--build", $BuildDir, "--config", $Config, "-j", $Jobs)
    if ($Target.Count -gt 0) {
        $buildArgs += "--target"
        $buildArgs += $Target
    }
    Write-Output "build round $round : cmake $($buildArgs -join ' ')"
    $out = & cmake @buildArgs 2>&1 | ForEach-Object { $_.ToString() }
    $rc = $LASTEXITCODE
    $errLines = $out | Where-Object { $_ -match "fatal error|error C[0-9]|LNK[0-9]" }
    $errLines | Select-Object -First 20 | ForEach-Object { Write-Output $_ }
    if ($rc -eq 0 -and -not ($errLines | Where-Object { $_ -match "LNK1103|LNK1136" })) {
        Write-Output "build OK (round $round)"
        exit 0
    }

    # "dasHV.obj : fatal error LNK1103: debugging information corrupt; recompile module [...]"
    $corrupt = @()
    foreach ($line in ($out | Where-Object { $_ -match "LNK1103|LNK1136" })) {
        if ($line -match "([\w.+-]+\.obj)") { $corrupt += $Matches[1] }
    }
    $corrupt = $corrupt | Sort-Object -Unique
    if ($corrupt.Count -eq 0) {
        Write-Output "build failed (rc=$rc) with no corrupt-obj signature - not healable here"
        exit $rc
    }
    if ($round -eq $MaxHeals) { break }
    foreach ($name in $corrupt) {
        $found = @(Get-ChildItem -Path $BuildDir -Recurse -Filter $name -ErrorAction SilentlyContinue)
        if ($found.Count -eq 0) {
            Write-Output "corrupt obj $name not found under $BuildDir - clean its owning target manually"
            continue
        }
        foreach ($f in $found) {
            Write-Output "healing: deleting $($f.FullName)"
            Remove-Item -Force $f.FullName
        }
    }
}

Write-Output "still failing after $MaxHeals heal rounds"
exit 1
