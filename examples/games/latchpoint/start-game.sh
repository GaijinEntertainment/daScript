#!/usr/bin/env bash
set -euo pipefail
repo_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.." && pwd)"
runtime="$repo_root/bin/daslang-live"
run_root="$repo_root/logs/latchpoint-scene"
[[ -x "$runtime" ]] || { echo "Build this checkout's daslang-live first: $runtime" >&2; exit 1; }
mkdir -p "$run_root"
if curl --silent --fail --max-time 2 http://127.0.0.1:19091/status >/dev/null; then
    echo "Latchpoint is already running on port 19091."
else
    if [[ "$(uname -s)" == Darwin ]]; then
        app="$repo_root/bin/Latchpoint.app"
        mkdir -p "$app/Contents/MacOS"
        cp "$runtime" "$app/Contents/MacOS/Latchpoint"
        cp "$repo_root/examples/games/latchpoint/macos/Info.plist" "$app/Contents/Info.plist"
        codesign --force --sign - "$app"
        open -n "$app" --stdout "$run_root/game-live.stdout.log" --stderr "$run_root/game-live.stderr.log" \
            --args -dasroot "$repo_root" -no-module-cache "$repo_root/examples/games/latchpoint/main.das" --live-port 19091
        echo "Latchpoint launched as a Mac app. Allow microphone access when prompted."
        exit 0
    fi
    cd "$run_root"
    nohup "$runtime" -no-module-cache "$repo_root/examples/games/latchpoint/main.das" --live-port 19091 \
        >game-live.stdout.log 2>game-live.stderr.log </dev/null &
    echo "$!" >game-live.pid
    echo "Latchpoint launched. Inference server: 18082; live development: 19091."
fi
