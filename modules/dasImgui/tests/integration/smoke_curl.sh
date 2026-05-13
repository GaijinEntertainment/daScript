#!/usr/bin/env bash
# smoke_curl.sh — raw-HTTP diagnostic for daslang-live + dasImgui dispatch.
#
# Spawns daslang-live against examples/save_demo/main.das, then drives it
# entirely via curl + jq. NO playwright module, NO dastest. If this script
# PASSes but the dastest suite FAILs, the bug is in the playwright wiring,
# not the underlying daslang-live + dasImgui plumbing. If THIS script fails,
# the wiring is broken at a deeper layer.
#
# Requires: curl, jq.
# Run: bash modules/dasImgui/tests/integration/smoke_curl.sh
# Override exe: DASLANG_LIVE_EXE=/path/to/daslang-live bash smoke_curl.sh

set -u

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../../../.." && pwd)"
default_exe="$repo_root/bin/Release/daslang-live"
[ -e "${default_exe}.exe" ] && default_exe="${default_exe}.exe"
live_exe="${DASLANG_LIVE_EXE:-$default_exe}"
feature_app="$(cd "$script_dir/../.." && pwd)/examples/save_demo/main.das"
base_url="http://127.0.0.1:9090"

if [ ! -e "$live_exe" ]; then
    echo "FAIL: daslang-live not found at $live_exe" >&2
    echo "      override with DASLANG_LIVE_EXE=..." >&2
    exit 1
fi

echo "[smoke_curl] exe:     $live_exe"
echo "[smoke_curl] feature: $feature_app"

# Required tools.
for tool in curl jq; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "FAIL: '$tool' is required but not on PATH" >&2
        exit 1
    fi
done

# ===== Spawn daslang-live =====
"$live_exe" "$feature_app" >/dev/null 2>&1 &
live_pid=$!

cleanup() {
    if kill -0 "$live_pid" 2>/dev/null; then
        curl -s -X POST -H "Content-Type: application/json" -d '' \
             --max-time 2 "$base_url/shutdown" >/dev/null 2>&1 || true
        sleep 0.5
        kill -0 "$live_pid" 2>/dev/null && kill -9 "$live_pid" 2>/dev/null || true
    fi
}
trap cleanup EXIT

# ===== Wait until /status returns 200 OK =====
ready=0
deadline=$(($(date +%s) + 30))
while [ "$(date +%s)" -lt "$deadline" ]; do
    code="$(curl -s -o /dev/null -w '%{http_code}' --max-time 1 "$base_url/status" || true)"
    if [ "$code" = "200" ]; then
        ready=1
        break
    fi
    sleep 0.2
done
if [ "$ready" != "1" ]; then
    echo "FAIL: daslang-live did not become ready within 30s" >&2
    exit 1
fi
echo "[smoke_curl] ready"

# ===== Snapshot — verify SAVE_BTN registered =====
snap="$(curl -s -X POST -H "Content-Type: application/json" \
        -d '{"name":"imgui_snapshot"}' "$base_url/command")"
save_btn_kind="$(printf '%s' "$snap" | jq -r '.globals.SAVE_BTN.kind // ""')"
if [ "$save_btn_kind" != "button" ]; then
    echo "FAIL: SAVE_BTN.kind = '$save_btn_kind', expected 'button'" >&2
    exit 1
fi
echo "[smoke_curl] snapshot: SAVE_BTN.kind == 'button'"

# ===== Click SAVE_BTN =====
curl -s -X POST -H "Content-Type: application/json" \
     -d '{"name":"imgui_click","args":{"target":"SAVE_BTN"}}' \
     "$base_url/command" >/dev/null
echo "[smoke_curl] click posted"

# ===== Poll until STATUS_TEXT.payload.value == "saved" =====
observed=""
deadline=$(($(date +%s) + 5))
while [ "$(date +%s)" -lt "$deadline" ]; do
    snap="$(curl -s -X POST -H "Content-Type: application/json" \
            -d '{"name":"imgui_snapshot"}' "$base_url/command")"
    observed="$(printf '%s' "$snap" | jq -r '.globals.STATUS_TEXT.payload.value // ""')"
    if [ "$observed" = "saved" ]; then
        break
    fi
    sleep 0.1
done
if [ "$observed" != "saved" ]; then
    echo "FAIL: STATUS_TEXT.value = '$observed', expected 'saved'" >&2
    exit 1
fi
echo "[smoke_curl] STATUS_TEXT.value == 'saved'"

echo "PASS"
exit 0
