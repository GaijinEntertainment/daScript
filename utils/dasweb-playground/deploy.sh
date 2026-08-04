#!/bin/sh
# dasweb-playground box-side installer. Run as root on the web box:
#
#   deploy.sh <short-sha> /tmp/dasweb-playground-<short-sha>.tar.gz
#
# The bundle comes from the builder box (zen4) — from the arc worktree there:
#
#   cd ~/daScript-dasweb && git pull --ff-only origin <branch>
#   bin/daslang utils/daspkg/main.das -- release --root utils/dasweb-playground --out ~/dasweb_release
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

cd "$APP/releases"
tar xzf "$TARBALL"
rm -rf "$SHA"
mv dasweb-playground "$SHA"
if [ -f "$APP/current/dasweb-playground.toml" ]; then
    cp "$APP/current/dasweb-playground.toml" "$SHA/dasweb-playground.toml"
fi
chown -R dasweb:dasweb "$SHA"
ln -sfn "$APP/releases/$SHA" "$APP/current"
systemctl restart dasweb-playground
sleep 5
systemctl is-active dasweb-playground
curl -sf http://127.0.0.1:8101/healthz
echo
echo "deployed $SHA"
