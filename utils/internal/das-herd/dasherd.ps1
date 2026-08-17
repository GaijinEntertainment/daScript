param([Parameter(ValueFromRemainingArguments = $true)][string[]]$Arguments)

$ErrorActionPreference = 'Stop'

function Take-Option([System.Collections.Generic.List[string]]$Items, [string]$Name, [string]$Default = '') {
    $index = $Items.IndexOf($Name)
    if ($index -lt 0) { return $Default }
    if ($index + 1 -ge $Items.Count) { throw "$Name requires a value" }
    $value = $Items[$index + 1]
    $Items.RemoveAt($index + 1)
    $Items.RemoveAt($index)
    return $value
}

function Invoke-DasHerd([string]$Method, [string]$Path, $Body = $null) {
    $separator = '?'
    if ($Path.Contains('?')) { $separator = '&' }
    $uri = "$script:BaseUrl$Path${separator}token=$([uri]::EscapeDataString($script:Token))"
    $parameters = @{ Method = $Method; Uri = $uri }
    if ($null -ne $Body) {
        $parameters.ContentType = 'application/json; charset=utf-8'
        $parameters.Body = $Body | ConvertTo-Json -Depth 20 -Compress
    }
    Invoke-RestMethod @parameters
}

function Write-Json($Value) {
    ConvertTo-Json -InputObject $Value -Depth 20
}

function Write-JsonList($Value) {
    $list = @($Value)
    ConvertTo-Json -InputObject $list -Depth 20
}

$items = [System.Collections.Generic.List[string]]::new()
foreach ($item in $Arguments) { $items.Add($item) }
$script:BaseUrl = (Take-Option $items '--url' $env:DASHERD_URL).TrimEnd('/')
if (-not $script:BaseUrl) { $script:BaseUrl = 'http://127.0.0.1:9191' }
$script:Token = Take-Option $items '--token' $env:DASHERD_TOKEN
$sessionId = Take-Option $items '--session' $env:DASHERD_SESSION_ID
$sender = Take-Option $items '--sender' ''
if (-not $sender -and $sessionId) { $sender = "agent_session:$sessionId" }

if ($items.Count -eq 0) { throw 'command required: whoami | inbox | outbox | bundle | repository' }
$command = $items[0]
$items.RemoveAt(0)

if ($command -eq 'whoami') {
    $identity = [ordered]@{
        sender = $sender
        session_id = $sessionId
        url = $script:BaseUrl
        configured = [bool]($script:Token -and $sessionId)
    }
    if ($identity.configured) {
        $sessions = @(Invoke-DasHerd GET '/api/v1/sessions')
        $session = $sessions | Where-Object { $_.id -eq $sessionId } | Select-Object -First 1
        if ($session) { $identity.session = $session }
    }
    Write-Json $identity
    exit 0
}
if (-not $script:Token) { throw 'watcher token is required (--token or DASHERD_TOKEN)' }
if (-not $sessionId) { throw 'session is required (--session or DASHERD_SESSION_ID)' }
if ($items.Count -eq 0) { throw "$command subcommand required" }
$subcommand = $items[0]
$items.RemoveAt(0)

if ($command -eq 'inbox') {
    if ($subcommand -eq 'list') {
        Write-JsonList (Invoke-DasHerd GET "/api/v1/mailbox?session_id=$([uri]::EscapeDataString($sessionId))&direction=inbox")
        exit 0
    }
    if ($subcommand -eq 'get') {
        if ($items.Count -lt 1) { throw 'inbox get requires a message id' }
        $messageId = $items[0]
        Write-JsonList (Invoke-DasHerd GET "/api/v1/mailbox?session_id=$([uri]::EscapeDataString($sessionId))&direction=inbox&message_id=$([uri]::EscapeDataString($messageId))")
        exit 0
    }
    if ($subcommand -eq 'ack' -or $subcommand -eq 'complete') {
        if ($items.Count -lt 1) { throw "inbox $subcommand requires a message id" }
        $status = 'completed'
        if ($subcommand -eq 'ack') { $status = 'acknowledged' }
        Write-Json (Invoke-DasHerd POST '/api/v1/mailbox/state' ([ordered]@{
            session_id = $sessionId; message_id = $items[0]; status = $status
        }))
        exit 0
    }
    throw "unknown inbox subcommand: $subcommand"
}

if ($command -eq 'bundle') {
    if ($subcommand -eq 'list') {
        Write-JsonList (Invoke-DasHerd GET "/api/v1/bundles?session_id=$([uri]::EscapeDataString($sessionId))")
        exit 0
    }
    if ($subcommand -eq 'sync') {
        $manifestPath = Take-Option $items '--manifest' ''
        if (-not $manifestPath) { throw 'bundle sync requires --manifest <path>' }
        $bundle = Get-Content -LiteralPath $manifestPath -Raw -Encoding UTF8 | ConvertFrom-Json
        Write-Json (Invoke-DasHerd POST '/api/v1/bundles/sync' ([ordered]@{
            session_id = $sessionId
            bundle = $bundle
        }))
        exit 0
    }
    throw "unknown bundle subcommand: $subcommand"
}

if ($command -eq 'repository') {
    if ($subcommand -eq 'list') {
        Write-JsonList (Invoke-DasHerd GET '/api/v1/repositories')
        exit 0
    }
    if ($subcommand -eq 'add') {
        $path = Take-Option $items '--path' ''
        $displayName = Take-Option $items '--name' ''
        if (-not $path) { throw 'repository add requires --path <checkout-or-worktree>' }
        Write-Json (Invoke-DasHerd POST '/api/v1/repositories' ([ordered]@{
            path = [System.IO.Path]::GetFullPath($path)
            display_name = $displayName
            target_id = 'local'
            herd_session_id = "$env:DASHERD_HERD_SESSION_ID"
        }))
        exit 0
    }
    throw "unknown repository subcommand: $subcommand"
}

if ($command -eq 'outbox') {
    if ($subcommand -ne 'send' -and $subcommand -ne 'reply') {
        throw "unknown outbox subcommand: $subcommand"
    }
    $replyTo = ''
    if ($subcommand -eq 'reply') {
        if ($items.Count -lt 1) { throw 'outbox reply requires an inbox message id' }
        $replyTo = $items[0]
        $items.RemoveAt(0)
    }
    $subject = Take-Option $items '--subject' ''
    $focusPath = Take-Option $items '--focus-json' ''
    $bundleId = Take-Option $items '--bundle' ''
    if (-not $subject) { throw 'outbox send requires --subject' }
    $focus = [ordered]@{ repository_id = ''; worktree_path = ''; comparison = 'working'; revision = ''; summary = ''; targets = @() }
    if ($focusPath) {
        $focus = Get-Content -LiteralPath $focusPath -Raw -Encoding UTF8 | ConvertFrom-Json
    }
    Write-Json (Invoke-DasHerd POST '/api/v1/mailbox' ([ordered]@{
        session_id = $sessionId
        direction = 'outbox'
        sender = $sender
        kind = 'focus_set'
        subject = $subject
        reply_to = $replyTo
        bundle_id = $bundleId
        notify = $false
        focus = $focus
    }))
    exit 0
}

throw "unknown command: $command"
