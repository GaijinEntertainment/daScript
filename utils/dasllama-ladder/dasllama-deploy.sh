#!/bin/sh
# dasllama-ladder box-side deploy tool for dasweb-1. This is the ONE privileged
# surface: installed root-owned at /usr/local/sbin/dasllama-deploy.sh and reached
# through a sudoers drop-in that grants NOPASSWD for exactly this script (not a
# root shell). Verbs:
#
#   provision                  one-time: service user, dirs, systemd unit, restic snapshot line
#   caddy                      splice caddy.snippet into the dasllama.io vhost, validate, reload
#   install <sha> <tarball>    per-release: unpack, carry operator files, flip current, restart, health
#   open-submit | close-submit flip the public-submission gate (loopback admin call)
#   status                     systemd active-state + /api health (the gate state is not
#                              remotely queryable; it is in the service's startup banner/log)
#
# The bundle comes from the builder box (zen4), from the arc worktree there:
#   cd ~/daScript-dasweb && git pull --ff-only origin <branch>
#   bin/daslang utils/daspkg/main.das -- release --root utils/dasllama-ladder --out ~/dasllama_release
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

verb="${1:?usage: dasllama-deploy.sh provision|caddy|install <sha> <tarball>|open-submit|close-submit|status}"

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
ExecStart=/usr/bin/python3 watchdog.py
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

caddy_apply() {
    # Splice caddy.snippet (from the installed release) into the dasllama.io vhost, ahead of
    # root/file_server. Idempotent, validated before reload, with a timestamped Caddyfile backup.
    snippet="$APP/current/caddy.snippet"
    [ -f "$snippet" ] || { echo "no $snippet - install a release first"; exit 1; }
    if grep -q "reverse_proxy 127.0.0.1:$PORT" "$CADDYFILE"; then
        echo "caddy: /api already spliced - nothing to do"; return 0
    fi
    ts=$(date +%Y%m%d-%H%M%S)
    cp "$CADDYFILE" "$CADDYFILE.bak-$ts"
    awk -v snip="$snippet" '
        /^dasllama\.io \{/ && !done {
            print
            while ((getline line < snip) > 0) if (line !~ /^[ \t]*#/ && line !~ /^[ \t]*$/) print "\t" line
            close(snip); done=1; next
        }
        { print }
    ' "$CADDYFILE.bak-$ts" > "$CADDYFILE"
    # The awk match is exact (`dasllama.io {`); if the vhost is ever reformatted (shared address,
    # renamed) it matches nothing, validate still passes on the unchanged file, and we would
    # wrongly report success. Confirm the proxy line actually landed before reloading.
    if ! grep -q "reverse_proxy 127.0.0.1:$PORT" "$CADDYFILE"; then
        echo "caddy: splice inserted nothing (dasllama.io vhost not matched) - restoring $CADDYFILE.bak-$ts"; cp "$CADDYFILE.bak-$ts" "$CADDYFILE"; exit 1
    fi
    if ! caddy validate --config "$CADDYFILE" --adapter caddyfile >/dev/null 2>&1; then
        echo "caddy validate FAILED - restoring $CADDYFILE.bak-$ts"; cp "$CADDYFILE.bak-$ts" "$CADDYFILE"; exit 1
    fi
    systemctl reload caddy
    echo "caddy: /api spliced + reloaded (backup $CADDYFILE.bak-$ts)"
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

case "$verb" in
    provision) provision ;;
    caddy) caddy_apply ;;
    install) shift; install_release "$@" ;;
    open-submit) submit_toggle true ;;
    close-submit) submit_toggle false ;;
    status)
        systemctl is-active dasllama-ladder || true
        curl -sf "http://127.0.0.1:$PORT/api/versions" >/dev/null 2>&1 && echo "api: ok" || echo "api: DOWN"
        ;;
    *) echo "unknown verb: $verb"; exit 2 ;;
esac
