#!/bin/sh
# dasllama-ladder box-side deploy tool for dasweb-1. This is the ONE privileged
# surface: installed root-owned at /usr/local/sbin/dasllama-deploy.sh and reached
# through a sudoers drop-in that grants NOPASSWD for exactly this script (not a
# root shell). Verbs:
#
#   provision                  one-time: service user, dirs, systemd unit, restic snapshot line
#   caddy                      bring the dasllama.io vhost in line with caddy.snippet, validate, reload
#   install <sha> <tarball>    per-release: unpack, carry operator files, flip current, restart, health
#   open-submit | close-submit flip the public-submission gate (loopback admin call)
#   sidecars                   list stored sidecars + the gate state (loopback admin listing)
#   promote <sha> | demote <sha>  flip a sidecar's verified flag
#   plant <sidecar.json>       import an official sidecar (stored verified; re-plant promotes)
#   status                     systemd active-state + /api health + the submit-gate state
#
# The curl verbs need no root — they talk to the loopback admin surface — but they live here
# so the operator has ONE tool; the browser twin is /admin/ over `ssh -L 8201:127.0.0.1:8201`.
#
# The bundle comes from the builder box (zen4), from the arc worktree there:
#   cd ~/daScript-dasweb && git pull --ff-only origin <branch>
#   bin/daslang utils/daspkg/main.das -- release --root utils/internal/dasllama-ladder --out ~/dasllama_release
#   SHA=$(git rev-parse --short HEAD)
#   cd ~/dasllama_release && tar czf dasllama-ladder-$SHA.tar.gz dasllama-ladder
#   # scp dasllama-ladder-$SHA.tar.gz to dasweb-1:/tmp, then on the box:
#   sudo dasllama-deploy.sh install $SHA /tmp/dasllama-ladder-$SHA.tar.gz
set -eu

APP=/srv/apps/dasllama-ladder
DATA=/srv/dasllama-ladder
SVCUSER=dasllama
PORT=8201
UNIT=/etc/systemd/system/dasllama-ladder.service
CADDYFILE=/etc/caddy/Caddyfile
RESTIC_ENV=/etc/restic/env

verb="${1:?usage: dasllama-deploy.sh provision|caddy|install <sha> <tarball>|open-submit|close-submit|sidecars|promote <sha>|demote <sha>|plant <file>|status}"

provision() {
    # Dedicated service user isolates the public-upload process from the playground's dasweb.
    id -u "$SVCUSER" >/dev/null 2>&1 || useradd --system --home-dir "$DATA" --shell /usr/sbin/nologin "$SVCUSER"
    install -d -o "$SVCUSER" -g "$SVCUSER" "$DATA"   # db + logs, outside the release tree so a flip preserves them
    install -d -o root -g root "$APP" "$APP/releases"
    cat > "$UNIT" <<EOF
[Unit]
Description=dasllama-ladder (dasllama.io exchange service, watchdog-supervised)
After=network.target

[Service]
Type=simple
User=$SVCUSER
Group=$SVCUSER
WorkingDirectory=$APP/current
ExecStart=$APP/current/watchdog
Restart=on-failure
RestartSec=5

# Sandbox: contain a hypothetical parser/runtime RCE to a capability-less process
# that can only write its own db + release logs; host FS and other /srv services stay
# read-only. Does NOT replace input-validation review. ReadWritePaths covers BOTH the
# data dir and the release tree (the service writes libhv + watchdog logs under current/).
NoNewPrivileges=true
ProtectSystem=strict
ReadWritePaths=$DATA $APP/releases
ProtectHome=true
PrivateTmp=true
ProtectControlGroups=true
ProtectKernelTunables=true
ProtectKernelModules=true
ProtectKernelLogs=true
ProtectClock=true
ProtectHostname=true
RestrictSUIDSGID=true
RestrictRealtime=true
RestrictNamespaces=true
LockPersonality=true
RestrictAddressFamilies=AF_INET AF_INET6 AF_UNIX
CapabilityBoundingSet=
AmbientCapabilities=
SystemCallArchitectures=native
# TasksMax fits because the service does no parallel work (the daslang jobque is never
# created). If it adopts parallel_for/channels/create_job_que the pool spawns cores-1
# threads on spawn - raise this or set DAS_JOBQUE_THREADS then.
TasksMax=64

[Install]
WantedBy=multi-user.target
EOF
    systemctl daemon-reload
    systemctl enable dasllama-ladder >/dev/null 2>&1 || true
    systemctl try-restart dasllama-ladder >/dev/null 2>&1 || true   # re-provision applies the unit to a running service; no-op on first provision
    # /srv is already in restic BACKUP_PATHS; add the db to SQLITE_DBS so it is snapshotted
    # with sqlite3 .backup (a plain copy of a live DB can capture a torn write).
    if [ -f "$RESTIC_ENV" ] && ! grep -q "$DATA/ladder.db" "$RESTIC_ENV"; then
        cp "$RESTIC_ENV" "$RESTIC_ENV.bak-$(date +%Y%m%d-%H%M%S)"
        sed -i "s#^SQLITE_DBS=\"\(.*\)\"#SQLITE_DBS=\"\1 $DATA/ladder.db\"#" "$RESTIC_ENV"
    fi
    echo "provisioned: user=$SVCUSER data=$DATA unit=$UNIT; restic snapshot line added"
}

REGION_OPEN="# >>> caddy.snippet (dasllama-deploy.sh caddy rewrites to <<<)"
REGION_CLOSE="# <<< caddy.snippet"

snippet_directive_keys() {
    awk '/^[ \t]*#/ || !NF { next }
         !depth { print $1 " " $2 }
         { depth += gsub(/\{/, "{") - gsub(/\}/, "}") }' "$snippet"
}

snippet_region() {
    printf '\t%s\n' "$REGION_OPEN"
    awk '/^[ \t]*#/ || !NF { next } { print "\t" $0 }' "$snippet"
    printf '\t%s\n' "$REGION_CLOSE"
}

vhost_region() {
    # the marked region of the dasllama.io vhost in file $1; nothing when the vhost has no opening marker
    awk -v opener="$REGION_OPEN" -v closer="$REGION_CLOSE" '
        /^dasllama\.io \{/ { invhost = 1; next }
        invhost && /^\}/ { exit }
        invhost && index($0, opener) { inregion = 1 }
        inregion { print }
        inregion && index($0, closer) { exit }
    ' "$1"
}

caddy_apply() {
    # Make the dasllama.io vhost carry caddy.snippet (from the installed release) as written: the
    # snippet's directives open the vhost in a marked region this verb owns, and nothing else in
    # the vhost repeats one - a top-level copy outside the region, or a second region, is dropped.
    # A directive the snippet changed, dropped or renamed therefore follows it. Idempotent,
    # validated before reload, with a timestamped Caddyfile backup.
    snippet="$APP/current/caddy.snippet"
    [ -f "$snippet" ] || { echo "no $snippet - install a release first"; exit 1; }
    want=$(mktemp); have=$(mktemp); keys=$(mktemp); next=$(mktemp)
    snippet_region > "$want"
    snippet_directive_keys > "$keys"
    unpaired=$(awk -v opener="$REGION_OPEN" -v closer="$REGION_CLOSE" '
        /^dasllama\.io \{/ { invhost = 1; next }
        invhost && /^\}/ { exit }
        invhost && index($0, opener) { opened++ }
        invhost && index($0, closer) { if (opened) opened--; else stray++ }
        END { print opened + stray }' "$CADDYFILE")
    if [ "$unpaired" != "0" ]; then
        rm -f "$want" "$have" "$keys" "$next"
        echo "caddy: the caddy.snippet markers of the dasllama.io vhost do not pair up - fix them by hand"; exit 1
    fi
    awk -v want="$want" -v keys="$keys" -v opener="$REGION_OPEN" -v closer="$REGION_CLOSE" '
        function braces() { return $0 ~ /^[ \t]*#/ ? 0 : gsub(/\{/, "{") - gsub(/\}/, "}") }
        function emit(   line) { while ((getline line < want) > 0) print line; close(want) }
        BEGIN { while ((getline key < keys) > 0) owned[key] = 1; close(keys) }
        /^dasllama\.io \{/ && !invhost { invhost = 1; depth = 0; print; emit(); next }
        invhost && /^\}/ { invhost = 0; inregion = 0; skipping = 0; print; next }
        inregion { if (index($0, closer)) inregion = 0; next }
        invhost && index($0, opener) { inregion = 1; next }
        skipping { skip += braces(); if (skip <= 0) skipping = 0; next }
        invhost && depth == 0 && (($1 " " $2) in owned) { skip = braces(); skipping = skip > 0; next }
        invhost { depth += braces() }
        { print }
    ' "$CADDYFILE" > "$next"
    vhost_region "$next" > "$have"
    if ! cmp -s "$want" "$have"; then
        rm -f "$want" "$have" "$keys" "$next"
        echo "caddy: found no dasllama.io vhost to write caddy.snippet into - nothing changed"; exit 1
    fi
    if cmp -s "$next" "$CADDYFILE"; then
        rm -f "$want" "$have" "$keys" "$next"
        echo "caddy: the dasllama.io vhost carries caddy.snippet as written - nothing to do"
        return 0
    fi
    backup="$CADDYFILE.bak-$(date +%Y%m%d-%H%M%S)"
    [ ! -e "$backup" ] || backup="$backup-$$"
    cp "$CADDYFILE" "$backup"
    cat "$next" > "$CADDYFILE"
    rm -f "$want" "$have" "$keys" "$next"
    if ! caddy validate --config "$CADDYFILE" --adapter caddyfile >/dev/null 2>&1; then
        echo "caddy validate FAILED - restoring $backup"; cp "$backup" "$CADDYFILE"; exit 1
    fi
    systemctl reload caddy
    echo "caddy: caddy.snippet written into the dasllama.io vhost + reloaded (backup $backup)"
}

install_release() {
    SHA="${1:?install <sha> <tarball>}"; TARBALL="${2:?install <sha> <tarball>}"
    # $SHA names a release dir under $APP/releases and drives rm -rf/mv/ln as root. Pin it to a
    # git short-sha shape so a typo like "." or "../x" can't escape the releases tree.
    case "$SHA" in ''|*[!0-9a-fA-F]*) echo "bad sha '$SHA' (hex only)"; exit 2;; esac
    [ -f "$TARBALL" ] || { echo "no tarball at '$TARBALL'"; exit 2; }
    # `tar xzf` runs as root below; reject an archive with absolute or `..` entries so a crafted
    # or corrupt tarball can't escape the releases tree and overwrite arbitrary files.
    if tar tzf "$TARBALL" | grep -qE '(^/|(^|/)\.\.(/|$))'; then
        echo "unsafe tarball: contains absolute or '..' path entries"; exit 2
    fi
    CARRY=$(mktemp -d); trap 'rm -rf "$CARRY"' EXIT
    had_toml=0
    for f in dasllama-ladder.toml watchdog.json; do
        [ -f "$APP/current/$f" ] && cp "$APP/current/$f" "$CARRY/$f"
    done
    [ -f "$CARRY/dasllama-ladder.toml" ] && had_toml=1
    cd "$APP/releases"
    rm -rf dasllama-ladder
    tar xzf "$TARBALL"
    rm -rf "$SHA"; mv dasllama-ladder "$SHA"
    for f in dasllama-ladder.toml watchdog.json; do
        [ -f "$CARRY/$f" ] && cp "$CARRY/$f" "$SHA/$f"
    done
    # First deploy has no operator toml to carry: pin the db at the stable data dir (the release
    # tree is wiped on every flip) and make it authoritative so the path can't drift. submit_open
    # stays absent = closed. Later deploys carry the box's edited toml forward untouched.
    if [ "$had_toml" -eq 0 ]; then
        sed -i "s#^db = .*#db = \"$DATA/ladder.db\"#" "$SHA/dasllama-ladder.toml"
        grep -q '^authoritative' "$SHA/dasllama-ladder.toml" || printf '\nauthoritative = true\n' >> "$SHA/dasllama-ladder.toml"
    fi
    chown -R "$SVCUSER":"$SVCUSER" "$SHA"
    ln -sfn "$APP/releases/$SHA" "$APP/current"
    systemctl restart dasllama-ladder
    sleep 5
    systemctl is-active dasllama-ladder
    curl -sf "http://127.0.0.1:$PORT/healthz" && echo && echo "deployed $SHA"
}

submit_toggle() {
    curl -sf -X POST "http://127.0.0.1:$PORT/admin/submit" -d "{\"open\":$1}" && echo
}

set_verified() {
    # $1 = sha, $2 = true|false. Match the server's is_valid_sha exactly (64 LOWERCASE hex) so a
    # bad paste fails HERE with a clear message, not as a silent curl -sf 400 from the server.
    case "$1" in *[!0-9a-f]*|"") echo "bad sha '$1' (expected 64 lowercase hex)"; exit 2;; esac
    [ ${#1} -eq 64 ] || { echo "bad sha '$1' (expected 64 chars, got ${#1})"; exit 2; }
    curl -sf -X POST "http://127.0.0.1:$PORT/admin/sidecar/verify" -d "{\"sha\":\"$1\",\"verified\":$2}" && echo
}

case "$verb" in
    provision) provision ;;
    caddy) caddy_apply ;;
    install) shift; install_release "$@" ;;
    open-submit) submit_toggle true ;;
    close-submit) submit_toggle false ;;
    sidecars)
        curl -sf "http://127.0.0.1:$PORT/admin/sidecars" && echo
        ;;
    promote) shift; set_verified "${1:?promote <sha>}" true ;;
    demote) shift; set_verified "${1:?demote <sha>}" false ;;
    plant)
        shift; F="${1:?plant <sidecar.json>}"
        [ -f "$F" ] || { echo "no file at '$F'"; exit 2; }
        curl -sf -X POST "http://127.0.0.1:$PORT/admin/import-sidecar" --data-binary @"$F" && echo
        ;;
    status)
        systemctl is-active dasllama-ladder || true
        curl -sf "http://127.0.0.1:$PORT/api/versions" >/dev/null 2>&1 && echo "api: ok" || echo "api: DOWN"
        # advisory: parses the pretty-printed admin JSON; reads "unknown" if the format drifts
        gate=$(curl -sf "http://127.0.0.1:$PORT/admin/sidecars" 2>/dev/null | grep -o '"submit_open" : [a-z]*' | grep -o '[a-z]*$' || true)
        case "$gate" in
            true) echo "submit: open" ;;
            false) echo "submit: closed" ;;
            *) echo "submit: unknown" ;;
        esac
        ;;
    *) echo "unknown verb: $verb"; exit 2 ;;
esac
