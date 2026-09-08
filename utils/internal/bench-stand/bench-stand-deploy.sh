#!/bin/sh
# bench-stand box-side provisioning for dasweb-1. The ONE privileged surface of the stand:
# installed root-owned at /usr/local/sbin/bench-stand-deploy.sh and run with sudo. Verbs:
#
#   provision <authorized_key_file>   one-time: packages, the `bench` user, the layout, the clone,
#                                     the forced-command ssh key the GitHub workflow uses
#   caddy                             splice caddy.snippet into the daslang.io vhost, validate, reload
#   status                            user, layout, last status.json, Caddy route presence
#
# Everything the stand does afterwards runs unprivileged as `bench` through nightly.sh, whose
# gate verbs (start / follow / status / summary) are all the key can reach - never a shell. The
# workflow's key (DASWEB_BENCH_KEY, an environment secret on `github-pages`) is generated on
# the operator's machine: `ssh-keygen -t ed25519 -f bench_key -N '' -C bench-stand`; the
# public half is what `provision` installs, the private half goes into the secret.
set -eu

HOME_DIR=/srv/bench-stand
SVCUSER=bench
REPO=https://github.com/GaijinEntertainment/daScript.git
CADDYFILE=/etc/caddy/Caddyfile
TOOL=utils/internal/bench-stand

verb="${1:?usage: bench-stand-deploy.sh provision <authorized_key_file> | caddy | status}"

provision() {
    keyfile="${1:?provision needs the public key file the workflow will authenticate with}"
    [ -f "$keyfile" ] || { echo "no such key file: $keyfile"; exit 1; }
    # Only what the night needs: the build itself, ccache to keep the nightly rebuild
    # incremental, rsync for the last_good copy, flock (util-linux) for the run lock. No GLFW or
    # X11 development packages - `nightly.sh` configures with -DDAS_GLFW_DISABLED=ON.
    apt-get install -y --no-install-recommends git cmake ninja-build g++ ccache rsync curl util-linux >/dev/null
    id -u "$SVCUSER" >/dev/null 2>&1 || useradd --system --create-home --home-dir "$HOME_DIR" --shell /bin/bash "$SVCUSER"
    install -d -o "$SVCUSER" -g "$SVCUSER" "$HOME_DIR" "$HOME_DIR/runs" "$HOME_DIR/site" "$HOME_DIR/logs" "$HOME_DIR/state" "$HOME_DIR/last_good"
    if [ ! -d "$HOME_DIR/src/.git" ]; then
        su -s /bin/sh "$SVCUSER" -c "git clone --quiet '$REPO' '$HOME_DIR/src'"
    fi
    # Forced command: the key can only start/follow/status the stand, never open a shell.
    install -d -m 700 -o "$SVCUSER" -g "$SVCUSER" "$HOME_DIR/.ssh"
    key=$(head -n 1 "$keyfile")
    line="command=\"$HOME_DIR/src/$TOOL/nightly.sh gate\",no-port-forwarding,no-agent-forwarding,no-X11-forwarding,no-pty $key"
    auth="$HOME_DIR/.ssh/authorized_keys"
    touch "$auth"
    grep -qF "$key" "$auth" || echo "$line" >> "$auth"
    chown "$SVCUSER:$SVCUSER" "$auth"
    chmod 600 "$auth"
    echo "provisioned: user=$SVCUSER home=$HOME_DIR clone=$HOME_DIR/src key installed with forced command"
    echo "next: sudo bench-stand-deploy.sh caddy; then a workflow_dispatch of nightly_bench.yml"
}

vhost_has() {
    # true when the daslang.io block of the shared Caddyfile carries $1 as a literal - the other
    # vhosts may spell the same directive, so a whole-file grep is not the question
    awk '/^daslang\.io \{/ { b = 1 } b { print } b && /^\}/ { b = 0 }' "$CADDYFILE" | grep -qF "$1"
}

# Every route line the snippet declares, each the literal the vhost must carry. Derived from the
# snippet rather than written twice, so a route added there is checked without editing this script.
snippet_routes() {
    sed -n 's/ *{$//; s/^\(redir\|handle_path\|handle\|header\|respond\|reverse_proxy\|root\) /\1 /p' "$1"
}

# Prints "all", "none" or "partial": how much of the snippet the vhost already carries. A partial
# splice is never repaired automatically - re-splicing would duplicate the routes already there.
routes_state() {
    local snippet=$1 route have=0 miss=0
    while IFS= read -r route; do
        [ -n "$route" ] || continue
        if vhost_has "$route"; then have=$((have + 1)); else miss=$((miss + 1)); fi
    done <<ROUTES
$(snippet_routes "$snippet")
ROUTES
    if [ "$have" -gt 0 ] && [ "$miss" -eq 0 ]; then echo all
    elif [ "$have" -eq 0 ]; then echo none
    else echo partial; fi
}

caddy_apply() {
    snippet="$HOME_DIR/src/$TOOL/caddy.snippet"
    [ -f "$snippet" ] || { echo "no $snippet - provision first"; exit 1; }
    case "$(routes_state "$snippet")" in
        all) echo "caddy: every /bench route is already spliced - nothing to do"; return 0 ;;
        partial)
            echo "caddy: the daslang.io vhost carries SOME of the snippet's routes but not all - fix it by hand"
            echo "the snippet declares:"; snippet_routes "$snippet" | sed 's/^/  /'
            exit 1 ;;
    esac
    ts=$(date +%Y%m%d-%H%M%S)
    cp "$CADDYFILE" "$CADDYFILE.bak-$ts"
    # Insert the snippet's directive lines (comments dropped) right after the vhost's opening
    # brace, so they sit ahead of the vhost's root/file_server.
    awk -v snip="$snippet" '
        /^daslang\.io \{/ && !done {
            print
            while ((getline line < snip) > 0) {
                if (line ~ /^#/ || line ~ /^[[:space:]]*$/) continue
                print "\t" line
            }
            close(snip); done = 1; next
        }
        { print }
    ' "$CADDYFILE.bak-$ts" > "$CADDYFILE"
    if [ "$(routes_state "$snippet")" != all ]; then
        echo "caddy: splice did not land every route (daslang.io vhost not matched?) - restoring"; cp "$CADDYFILE.bak-$ts" "$CADDYFILE"; exit 1
    fi
    if ! caddy validate --config "$CADDYFILE" --adapter caddyfile >/dev/null 2>&1; then
        echo "caddy validate FAILED - restoring $CADDYFILE.bak-$ts"; cp "$CADDYFILE.bak-$ts" "$CADDYFILE"; exit 1
    fi
    systemctl reload caddy
    echo "caddy: /bench/ spliced + reloaded (backup $CADDYFILE.bak-$ts)"
}

status() {
    id "$SVCUSER" 2>/dev/null || echo "user $SVCUSER: missing"
    for d in src runs site logs state last_good; do
        [ -d "$HOME_DIR/$d" ] && echo "$HOME_DIR/$d: ok" || echo "$HOME_DIR/$d: missing"
    done
    [ -f "$HOME_DIR/site/status.json" ] && cat "$HOME_DIR/site/status.json" || echo "status.json: never run"
    echo "caddy: routes $(routes_state "$HOME_DIR/src/$TOOL/caddy.snippet" 2>/dev/null || echo "unknown - no snippet")"
    ls "$HOME_DIR/runs" 2>/dev/null | wc -l | sed 's/^/runs: /'
}

case "$verb" in
    provision) provision "${2:-}" ;;
    caddy) caddy_apply ;;
    status) status ;;
    *) echo "unknown verb: $verb"; exit 2 ;;
esac
