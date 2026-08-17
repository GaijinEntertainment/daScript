#!/bin/sh
# dasweb-playground box-side installer. Run as root on the web box:
#
#   deploy.sh <short-sha> /tmp/dasweb-playground-<short-sha>.tar.gz
#
# The bundle comes from the builder box (zen4) — from the arc worktree there:
#
#   cd ~/daScript-dasweb && git pull --ff-only origin <branch>
#   bin/daslang utils/daspkg/main.das -- release --root utils/internal/dasweb-playground --out ~/dasweb_release
#   SHA=$(git rev-parse --short HEAD)
#   cd ~/dasweb_release && tar czf dasweb-playground-$SHA.tar.gz dasweb-playground
#   # scp the tarball to the web box's /tmp
#
# Install: unpack into /srv/apps/dasweb-playground/releases/<sha>, carry the
# deployed (user-owned) toml forward, flip the `current` symlink atomically,
# restart the systemd-wrapped watchdog, verify health. Rollback = point
# `current` at the previous release dir and restart.
set -eu

SHA="${1:?usage: deploy.sh <short-sha> <tarball>}"
TARBALL="${2:?usage: deploy.sh <short-sha> <tarball>}"
APP=/srv/apps/dasweb-playground

# The service's own data dir (db + logs) lives outside the release tree so it
# survives every flip; on a fresh box nothing has created it yet.
install -d -o dasweb -g dasweb /srv/dasweb-playground

# Snapshot the operator-owned files BEFORE touching the release tree: when this
# same sha is redeployed, `current` points into the directory the rm below
# removes, and the carry-forward would find nothing left to copy.
CARRY=$(mktemp -d)
# set -e means any failure below (a bad tarball, a full disk) exits before the
# explicit cleanup, so the trap is what keeps /tmp from accumulating one
# directory per failed deploy.
trap 'rm -rf "$CARRY"' EXIT
for f in dasweb-playground.toml watchdog.json; do
    if [ -f "$APP/current/$f" ]; then
        cp "$APP/current/$f" "$CARRY/$f"
    fi
done

cd "$APP/releases"
rm -rf dasweb-playground
tar xzf "$TARBALL"
rm -rf "$SHA"
mv dasweb-playground "$SHA"
for f in dasweb-playground.toml watchdog.json; do
    if [ -f "$CARRY/$f" ]; then
        cp "$CARRY/$f" "$SHA/$f"
    fi
done
chown -R dasweb:dasweb "$SHA"
ln -sfn "$APP/releases/$SHA" "$APP/current"
systemctl restart dasweb-playground
sleep 5
systemctl is-active dasweb-playground
curl -sf http://127.0.0.1:8101/healthz
echo
echo "deployed $SHA"
