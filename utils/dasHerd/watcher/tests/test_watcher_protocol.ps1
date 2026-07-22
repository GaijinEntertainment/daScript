param(
    [Parameter(Mandatory = $true)][string]$DaslangPath,
    [Parameter(Mandatory = $true)][string]$Root
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 2
$script:stage = 'initializing'
$script:harnessLog = $null

function Set-TestStage([string]$Name) {
    $script:stage = $Name
    if ($script:harnessLog) {
        Add-Content -LiteralPath $script:harnessLog -Encoding UTF8 `
            -Value ("{0:o} {1}" -f [DateTimeOffset]::UtcNow, $Name)
    }
}

function Assert-Protocol([bool]$Condition, [string]$Message) {
    if (-not $Condition) { throw "ASSERT: $Message" }
}

function Invoke-Json([string]$Method, [string]$BaseUrl, [string]$Token,
                     [string]$Path, $Body = $null) {
    $separator = '?'
    if ($Path.Contains('?')) { $separator = '&' }
    $parameters = @{
        Method = $Method
        Uri = "$BaseUrl$Path${separator}token=$([uri]::EscapeDataString($Token))"
    }
    if ($null -ne $Body) {
        $parameters.ContentType = 'application/json; charset=utf-8'
        $parameters.Body = $Body | ConvertTo-Json -Depth 30 -Compress
    }
    return Invoke-RestMethod @parameters
}

function Send-WebSocketJson([System.Net.WebSockets.ClientWebSocket]$Socket, $Value) {
    $json = $Value | ConvertTo-Json -Depth 30 -Compress
    $bytes = [Text.Encoding]::UTF8.GetBytes($json)
    $segment = New-Object 'System.ArraySegment[byte]' -ArgumentList @(,$bytes)
    $send = $Socket.SendAsync($segment, [Net.WebSockets.WebSocketMessageType]::Text,
        $true, [Threading.CancellationToken]::None)
    if (-not $send.Wait(5000)) {
        $Socket.Abort()
        throw 'timed out sending websocket message'
    }
    [void]($send.GetAwaiter().GetResult())
}

function Receive-WebSocketJson([System.Net.WebSockets.ClientWebSocket]$Socket,
                               [int]$TimeoutMs = 5000) {
    $buffer = New-Object byte[] 65536
    $stream = New-Object IO.MemoryStream
    try {
        do {
            $segment = New-Object 'System.ArraySegment[byte]' -ArgumentList @(,$buffer)
            $receive = $Socket.ReceiveAsync($segment,
                [Threading.CancellationToken]::None)
            if (-not $receive.Wait($TimeoutMs)) {
                $Socket.Abort()
                throw "timed out receiving websocket message after ${TimeoutMs}ms"
            }
            $result = $receive.GetAwaiter().GetResult()
            if ($result.MessageType -eq [Net.WebSockets.WebSocketMessageType]::Close) {
                throw 'websocket closed while awaiting a message'
            }
            if ($result.Count -gt 0) { $stream.Write($buffer, 0, $result.Count) }
        } while (-not $result.EndOfMessage)
        Assert-Protocol ($result.MessageType -eq [Net.WebSockets.WebSocketMessageType]::Text) `
            'unexpected binary websocket frame'
        $text = [Text.Encoding]::UTF8.GetString($stream.ToArray())
        return $text | ConvertFrom-Json
    } finally {
        $stream.Dispose()
    }
}

function Wait-WebSocketType([System.Net.WebSockets.ClientWebSocket]$Socket,
                            [string]$Type, [int]$TimeoutMs = 10000) {
    $timer = [Diagnostics.Stopwatch]::StartNew()
    while ($timer.ElapsedMilliseconds -lt $TimeoutMs) {
        $remaining = [Math]::Max(100, $TimeoutMs - [int]$timer.ElapsedMilliseconds)
        $message = Receive-WebSocketJson $Socket $remaining
        if ($message.type -eq $Type) { return $message }
    }
    throw "timed out waiting for websocket message '$Type'"
}

function Connect-WebSocket([string]$BaseUrl, [string]$Token) {
    $socket = New-Object Net.WebSockets.ClientWebSocket
    $uri = [uri]($BaseUrl.Replace('http://', 'ws://') +
        "/ws?token=$([uri]::EscapeDataString($Token))")
    [void]($socket.ConnectAsync($uri,
        [Threading.CancellationToken]::None).GetAwaiter().GetResult())
    $connected = Wait-WebSocketType $socket 'connected'
    Assert-Protocol ($connected.client_id -gt 0) 'websocket client id was not assigned'
    return $socket
}

function Close-WebSocket([System.Net.WebSockets.ClientWebSocket]$Socket) {
    if ($null -eq $Socket) { return }
    try {
        if ($Socket.State -eq [Net.WebSockets.WebSocketState]::Open) {
            $close = $Socket.CloseOutputAsync(
                [Net.WebSockets.WebSocketCloseStatus]::NormalClosure,
                'test complete', [Threading.CancellationToken]::None)
            if (-not $close.Wait(2000)) {
                $Socket.Abort()
            }
        }
    } catch {
        $Socket.Abort()
    } finally {
        $Socket.Dispose()
    }
}

function Initialize-TestRepository([string]$Path, [string]$FileName, [string]$Contents) {
    New-Item -ItemType Directory -Force -Path $Path | Out-Null
    & git -C $Path init --quiet
    if ($LASTEXITCODE -ne 0) { throw "git init failed for $Path" }
    & git -C $Path config user.email 'dasherd-test@example.invalid'
    & git -C $Path config user.name 'dasHerd Test'
    & git -C $Path config core.autocrlf false
    [IO.File]::WriteAllText((Join-Path $Path $FileName), $Contents,
        ([Text.UTF8Encoding]::new($false)))
    & git -C $Path add -- $FileName
    & git -C $Path commit --quiet -m initial
    if ($LASTEXITCODE -ne 0) { throw "git commit failed for $Path" }
}

function Wait-Repository([string]$BaseUrl, [string]$Token, [string]$CheckoutPath,
                         [string]$RequiredWorktree = '') {
    $wanted = [IO.Path]::GetFullPath([string]$CheckoutPath)
    for ($attempt = 0; $attempt -lt 150; $attempt++) {
        $repositories = Invoke-Json GET $BaseUrl $Token '/api/v1/repositories'
        foreach ($repository in $repositories) {
            $observedPath = [string]$repository.record.checkout_path
            if ([string]::IsNullOrWhiteSpace($observedPath)) { continue }
            try {
                $observedFullPath = [IO.Path]::GetFullPath($observedPath)
            } catch {
                Add-Content -LiteralPath $script:harnessLog -Encoding UTF8 -Value `
                    "ignored malformed repository checkout_path=$($observedPath | ConvertTo-Json -Compress)"
                continue
            }
            if (-not [string]::Equals($observedFullPath,
                    $wanted, [StringComparison]::OrdinalIgnoreCase)) { continue }
            if ([bool]$repository.refreshing -or $repository.worktrees.Count -eq 0) {
                continue
            }
            if (-not $RequiredWorktree) { return $repository }
            foreach ($worktree in $repository.worktrees) {
                $observedWorktreePath = [string]$worktree.path
                if ([string]::IsNullOrWhiteSpace($observedWorktreePath)) { continue }
                if ([string]::Equals([IO.Path]::GetFullPath($observedWorktreePath),
                        [IO.Path]::GetFullPath($RequiredWorktree),
                        [StringComparison]::OrdinalIgnoreCase)) { return $repository }
            }
        }
        Start-Sleep -Milliseconds 100
    }
    throw "repository/worktree was not observed: $CheckoutPath :: $RequiredWorktree"
}

function Read-JsonLines([string]$Path) {
    return @(Get-Content -LiteralPath $Path -Encoding UTF8 |
        Where-Object { $_ } | ForEach-Object { $_ | ConvertFrom-Json })
}

function Read-SharedBytes([string]$Path) {
    $file = [IO.File]::Open($Path, [IO.FileMode]::Open, [IO.FileAccess]::Read,
        [IO.FileShare]::ReadWrite)
    $memory = New-Object IO.MemoryStream
    try {
        $file.CopyTo($memory)
        return $memory.ToArray()
    } finally {
        $memory.Dispose()
        $file.Dispose()
    }
}

function Assert-OrderedEvents($Events, [string[]]$RequiredNames, [string]$Context) {
    Assert-Protocol ($Events.Count -gt 0) "$Context has no events"
    for ($index = 0; $index -lt $Events.Count; $index++) {
        Assert-Protocol ($Events[$index].sequence -eq $index + 1) `
            "$Context event sequence is not contiguous"
        Assert-Protocol ($Events[$index].wall_time_seconds -gt 0) `
            "$Context event has no wall clock"
        Assert-Protocol ($null -ne $Events[$index].state) "$Context event has no state"
        Assert-Protocol ($null -ne $Events[$index].worktree_path) `
            "$Context event has no worktree identity"
        Assert-Protocol ($null -ne $Events[$index].controller_id) `
            "$Context event has no controller identity"
    }
    $names = @($Events | ForEach-Object { $_.event })
    foreach ($required in $RequiredNames) {
        Assert-Protocol ($names -contains $required) "$Context is missing event '$required'"
    }
}

$runId = "protocol-$PID-$([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds())"
$fixtureRoot = Join-Path $Root "tmp/dasHerd/$runId"
$logRoot = Join-Path $Root "logs/dasHerd/integration/$runId"
$repoA = Join-Path $fixtureRoot 'repo-a'
$repoB = Join-Path $fixtureRoot 'repo-b'
$agentWorktree = Join-Path $fixtureRoot 'repo-a-agent'
$fakeBin = Join-Path $fixtureRoot 'fake-bin'
$watcherSessions = Join-Path $logRoot 'sessions'
$watcherStdout = Join-Path $logRoot 'watcher.stdout.log'
$watcherStderr = Join-Path $logRoot 'watcher.stderr.log'
$script:harnessLog = Join-Path $logRoot 'harness.log'
$configPath = Join-Path $logRoot 'config.json'
$manifestPath = Join-Path $fixtureRoot 'bundle.json'
$token = "token-$runId"
$port = 9300 + ($PID % 300)
$baseUrl = "http://127.0.0.1:$port"
$watcher = $null
$ws1 = $null
$ws2 = $null
$ws3 = $null
$success = $false
$failure = $null
$oldPath = $env:PATH
$unicodeName = 'unicode-' + (-join ([char[]](0x041f, 0x0440, 0x0438,
    0x0432, 0x0435, 0x0442))) + '.txt'

try {
    New-Item -ItemType Directory -Force -Path $fixtureRoot, $logRoot, $fakeBin | Out-Null
    Set-TestStage 'create-fixtures'
    Initialize-TestRepository $repoA 'main.txt' "alpha`n"
    Initialize-TestRepository $repoB $unicodeName "beta`n"
    [IO.File]::WriteAllText((Join-Path $fakeBin 'codex.cmd'),
        "@echo off`r`necho fake-codex^|%DASHERD_SESSION_KIND%^|%DASHERD_SESSION_ID%^|%DASHERD_CONTEXT_PATH%`r`nping 127.0.0.1 -n 16 >nul`r`n",
        [Text.Encoding]::ASCII)

    Set-TestStage 'launch-watcher'
    $env:PATH = "$fakeBin;$oldPath"
    $watcherArgs = @('-jit', 'utils/dasHerd/watcher/main.das', '--',
        "--port=$port", "--token=$token", "--log-root=$watcherSessions",
        "--config=$configPath", '--exit-after-ms=60000')
    $watcher = Start-Process -FilePath $DaslangPath -ArgumentList $watcherArgs `
        -WorkingDirectory $Root -WindowStyle Hidden -RedirectStandardOutput $watcherStdout `
        -RedirectStandardError $watcherStderr -PassThru
    $env:PATH = $oldPath

    Set-TestStage 'wait-health'
    $healthy = $false
    for ($attempt = 0; $attempt -lt 200; $attempt++) {
        try {
            $health = Invoke-Json GET $baseUrl $token '/api/v1/health'
            if ($health.ok) { $healthy = $true; break }
        } catch {
            Start-Sleep -Milliseconds 100
        }
    }
    Assert-Protocol $healthy "watcher did not start; logs: $logRoot"

    Set-TestStage 'register-repositories'
    Invoke-Json POST $baseUrl $token '/api/v1/repositories' @{
        path = $repoA; display_name = 'Protocol A'; target_id = 'local'
    } | Out-Null
    Invoke-Json POST $baseUrl $token '/api/v1/repositories' @{
        path = $repoB; display_name = 'Protocol B'; target_id = 'local'
    } | Out-Null
    $observedA = Wait-Repository $baseUrl $token $repoA
    $observedB = Wait-Repository $baseUrl $token $repoB

    Set-TestStage 'launch-agent-worktree'
    $ws1 = Connect-WebSocket $baseUrl $token
    Assert-Protocol ($ws1 -is [Net.WebSockets.ClientWebSocket]) `
        "websocket helper returned $($ws1.GetType().FullName)"
    Set-TestStage 'request-agent-worktree'
    Send-WebSocketJson $ws1 @{
        op = 'launch_agent_worktree'
        repository_id = $observedA.record.id
        worktree_path = $repoA
        new_worktree_path = $agentWorktree
        branch = "codex/$runId"
        base_ref = 'HEAD'
        task_context = 'Diagnose the deterministic two-repository protocol fixture.'
    }
    $setupLaunch = Wait-WebSocketType $ws1 'launched' 15000
    Assert-Protocol ([bool]$setupLaunch.session_id) 'worktree task session was not launched'
    $agentLaunch = Wait-WebSocketType $ws1 'launched' 15000
    Assert-Protocol ([bool]$agentLaunch.session_id) 'agent session was not launched'
    Assert-Protocol ($agentLaunch.session_id -ne $setupLaunch.session_id) `
        'worktree task and agent reused one session identity'
    Set-TestStage 'wait-agent-worktree-observed'
    $observedA = Wait-Repository $baseUrl $token $repoA $agentWorktree

    Set-TestStage 'attach-first-controller'
    Send-WebSocketJson $ws1 @{
        op = 'attach'; session_id = $agentLaunch.session_id
        control = $true; raw = $false
    }
    $attached1 = Wait-WebSocketType $ws1 'attached'
    Assert-Protocol ([bool]$attached1.controls) 'first client did not receive control'

    Set-TestStage 'reject-second-controller'
    $ws2 = Connect-WebSocket $baseUrl $token
    Send-WebSocketJson $ws2 @{
        op = 'attach'; session_id = $agentLaunch.session_id
        control = $true; raw = $false
    }
    $contention = Wait-WebSocketType $ws2 'error'
    Assert-Protocol ($contention.error -match 'controller lease') `
        'second controller did not receive a lease error'
    $observing2 = Wait-WebSocketType $ws2 'attached'
    Assert-Protocol (-not [bool]$observing2.controls) `
        'second client unexpectedly received control'

    Set-TestStage 'detach-and-reattach'
    Send-WebSocketJson $ws1 @{ op = 'detach' }
    $detached1 = Wait-WebSocketType $ws1 'detached'
    Assert-Protocol ($detached1.type -eq 'detached') 'first client did not detach'
    Send-WebSocketJson $ws2 @{
        op = 'attach'; session_id = $agentLaunch.session_id
        control = $true; raw = $false
    }
    $attached2 = Wait-WebSocketType $ws2 'attached'
    Assert-Protocol ([bool]$attached2.controls) `
        'observing client could not claim control after detach'

    Set-TestStage 'disconnect-release'
    Close-WebSocket $ws2
    $ws2 = $null
    Start-Sleep -Milliseconds 300
    $ws3 = Connect-WebSocket $baseUrl $token
    Send-WebSocketJson $ws3 @{
        op = 'attach'; session_id = $agentLaunch.session_id
        control = $true; raw = $false
    }
    $attached3 = Wait-WebSocketType $ws3 'attached'
    Assert-Protocol ([bool]$attached3.controls) `
        'socket close did not release the controller lease'
    Send-WebSocketJson $ws3 @{ op = 'detach' }
    Wait-WebSocketType $ws3 'detached' | Out-Null

    Set-TestStage 'sync-two-repository-bundle'
    $manifest = [ordered]@{
        id = 'protocol-two-repository'
        title = 'Protocol two-repository review'
        kind = 'debug'
        status = 'ready'
        summary = 'First complete snapshot'
        participants = @(
            [ordered]@{
                repository_id = $observedA.record.id
                worktree_path = $agentWorktree
                role = 'implementation'; state = 'modified'
                files = @([ordered]@{ path = 'main.txt'; role = 'primary' })
            },
            [ordered]@{
                repository_id = $observedB.record.id
                worktree_path = $repoB
                role = 'dependency'; state = 'modified'
                files = @([ordered]@{
                    path = $unicodeName; role = 'risk'
                })
            }
        )
        focus = [ordered]@{
            targets = @(
                [ordered]@{
                    repository_id = $observedA.record.id
                    worktree_path = $agentWorktree; comparison = 'working'
                    role = 'primary'; file_path = 'main.txt'; whole_file = $true
                    ranges = @()
                },
                [ordered]@{
                    repository_id = $observedB.record.id
                    worktree_path = $repoB; comparison = 'working'
                    role = 'risk'; file_path = $unicodeName
                    whole_file = $false
                    ranges = @([ordered]@{
                        start_byte = 0; end_byte = 4; caption = 'cross-repository risk'
                    })
                }
            )
        }
    }
    [IO.File]::WriteAllText($manifestPath,
        ($manifest | ConvertTo-Json -Depth 30), ([Text.UTF8Encoding]::new($false)))
    $env:DASHERD_URL = $baseUrl
    $env:DASHERD_TOKEN = $token
    $env:DASHERD_SESSION_ID = $agentLaunch.session_id
    $cli = Join-Path $Root 'utils/dasHerd/dasherd.ps1'
    $syncOutput = @(& powershell.exe -NoProfile -File $cli bundle sync `
        --manifest $manifestPath 2>&1)
    Assert-Protocol ($LASTEXITCODE -eq 0) "bundle sync failed: $($syncOutput -join "`n")"
    $sync = ($syncOutput -join "`n") | ConvertFrom-Json
    Assert-Protocol ([bool]$sync.ok) 'bundle sync did not return success'
    $manifest.summary = 'Replacement complete snapshot'
    [IO.File]::WriteAllText($manifestPath,
        ($manifest | ConvertTo-Json -Depth 30), ([Text.UTF8Encoding]::new($false)))
    $syncAgain = @(& powershell.exe -NoProfile -File $cli bundle sync `
        --manifest $manifestPath 2>&1)
    Assert-Protocol ($LASTEXITCODE -eq 0) `
        "replacement bundle sync failed: $($syncAgain -join "`n")"
    Remove-Item Env:DASHERD_URL, Env:DASHERD_TOKEN, Env:DASHERD_SESSION_ID `
        -ErrorAction SilentlyContinue

    Set-TestStage 'assert-api-state'
    $bundles = Invoke-Json GET $baseUrl $token `
        "/api/v1/bundles?session_id=$([uri]::EscapeDataString($agentLaunch.session_id))"
    $attention = Invoke-Json GET $baseUrl $token `
        "/api/v1/mailbox?session_id=$([uri]::EscapeDataString($agentLaunch.session_id))&direction=outbox"
    Assert-Protocol (@($bundles).Count -eq 1) 'bundle replacement created a duplicate bundle'
    Assert-Protocol ($bundles[0].participants.Count -eq 2) `
        'bundle did not retain both repository participants'
    Assert-Protocol ($bundles[0].summary -eq 'Replacement complete snapshot') `
        'bundle replacement did not install the complete snapshot'
    Assert-Protocol (@($attention).Count -eq 1) 'bundle replacement duplicated Attention'
    Assert-Protocol ($attention[0].focus.targets.Count -eq 2) `
        'Attention did not retain both repository targets'

    Set-TestStage 'assert-durable-logs'
    $agentDir = Join-Path $watcherSessions $agentLaunch.session_id
    $setupDir = Join-Path $watcherSessions $setupLaunch.session_id
    $agentEvents = Read-JsonLines (Join-Path $agentDir 'events.jsonl')
    $setupEvents = Read-JsonLines (Join-Path $setupDir 'events.jsonl')
    Assert-OrderedEvents $setupEvents @('session_created', 'agent_worktree_queued',
        'agent_worktree_created', 'agent_launch_started') 'worktree setup session'
    Assert-OrderedEvents $agentEvents @('session_created', 'agent_bootstrap_prepared',
        'client_attached', 'controller_claimed', 'controller_claim_rejected',
        'controller_released', 'client_detached', 'bundle_synced',
        'bundle_attention_created', 'bundle_attention_updated') 'agent session'
    $rawOutput = [Text.Encoding]::UTF8.GetString(
        (Read-SharedBytes (Join-Path $agentDir 'output.raw')))
    Assert-Protocol ($rawOutput -match [regex]::Escape($agentLaunch.session_id)) `
        'fake Codex did not receive the watcher-assigned session id'
    Assert-Protocol ($rawOutput -match 'fake-codex\|debug\|') `
        'fake Codex did not receive the debug purpose'
    $metadata = [IO.File]::ReadAllText((Join-Path $agentDir 'session.json'))
    $context = [IO.File]::ReadAllText((Join-Path $agentDir 'context.md'))
    Assert-Protocol (-not $metadata.Contains($token)) 'routing token leaked into session metadata'
    Assert-Protocol ($context.Contains('Diagnose the deterministic two-repository protocol fixture.')) `
        'agent context did not retain the debug task'
    Assert-Protocol ((Read-JsonLines (Join-Path $agentDir 'bundles.jsonl')).Count -eq 2) `
        'bundle JSONL did not retain both declarative snapshots'

    Set-TestStage 'terminate-agent'
    Invoke-Json POST $baseUrl $token '/api/v1/terminate' @{
        session_id = $agentLaunch.session_id
    } | Out-Null
    $success = $true
    Write-Output "PASS watcher protocol integration: $runId"
} catch {
    $failure = "stage=$script:stage`n$($_.Exception.ToString())`n$($_.ScriptStackTrace)"
    if ($script:harnessLog) {
        Add-Content -LiteralPath $script:harnessLog -Encoding UTF8 -Value $failure
    }
    [Console]::Error.WriteLine(
        "$failure`nPreserved diagnostics: $logRoot`nPreserved fixtures: $fixtureRoot")
    if (Test-Path $watcherStderr) {
        [Console]::Error.WriteLine(
            (Get-Content -LiteralPath $watcherStderr -Raw -ErrorAction SilentlyContinue))
    }
} finally {
    $env:PATH = $oldPath
    Remove-Item Env:DASHERD_URL, Env:DASHERD_TOKEN, Env:DASHERD_SESSION_ID `
        -ErrorAction SilentlyContinue
    Close-WebSocket $ws1
    Close-WebSocket $ws2
    Close-WebSocket $ws3
    if ($null -ne $watcher -and -not $watcher.HasExited) {
        Stop-Process -Id $watcher.Id -Force
        [void]$watcher.WaitForExit(5000)
    }
    if ($success) {
        Remove-Item -LiteralPath $fixtureRoot -Recurse -Force
        Remove-Item -LiteralPath $logRoot -Recurse -Force
    }
}
if ($null -ne $failure) { exit 1 }
