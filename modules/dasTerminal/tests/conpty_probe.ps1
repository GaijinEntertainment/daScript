$ErrorActionPreference = 'Stop'
[Console]::OutputEncoding = [Text.UTF8Encoding]::new($false)
[Console]::Write("PROBE_READY:$PID`r`n")
$line = [Console]::ReadLine()
[Console]::Write("PROBE_ECHO:$line`r`n")
exit 23
