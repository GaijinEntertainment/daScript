#!/usr/bin/env bash
# Bounded apt for GitHub-hosted ubuntu runners.
#
# The runner image points apt at azure.archive.ubuntu.com (azure.ports.ubuntu.com on arm).
# When that mirror degrades, apt burns its default retry budget on every index it asks
# for - dozens of them - and a twenty-second install step turns into an hours-long hang
# that never fails on its own. So: swap to the public archive, cap every fetch, retry the
# update a bounded number of times, and cap the install.
#
# Usage: sudo bash ci/apt_install.sh [apt-get install flags] package...
#   e.g. sudo bash ci/apt_install.sh --no-install-recommends libglfw3-dev libx11-dev
set -u
export DEBIAN_FRONTEND=noninteractive

for f in /etc/apt/sources.list /etc/apt/sources.list.d/ubuntu.sources; do
    if [ -f "$f" ]; then
        sed -i -e 's|http://azure\.archive\.ubuntu\.com|http://archive.ubuntu.com|g' \
               -e 's|http://azure\.ports\.ubuntu\.com|http://ports.ubuntu.com|g' "$f"
    fi
done

OPTS=(-o Acquire::Retries=3 -o Acquire::http::Timeout=30 -o Acquire::https::Timeout=30)

for attempt in 1 2 3; do
    if timeout 300 apt-get "${OPTS[@]}" update -y; then
        break
    fi
    if [ "$attempt" -eq 3 ]; then
        echo "apt_install: apt-get update failed three times" >&2
        exit 1
    fi
    echo "apt_install: apt-get update attempt $attempt failed, retrying in 10s" >&2
    sleep 10
done

timeout 900 apt-get "${OPTS[@]}" install -y "$@"
