#!/usr/bin/env bash
set -euo pipefail
repo_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.." && pwd)"
runtime="$repo_root/bin/daslang"
config="${1:-$repo_root/examples/games/latchpoint/server.local.toml}"
run_root="$repo_root/logs/latchpoint-server"
[[ -x "$runtime" ]] || { echo "Build this checkout's daslang first: $runtime" >&2; exit 1; }
[[ -f "$config" ]] || { echo "Create $config from server.example.toml with local model paths." >&2; exit 1; }
config="$(cd -- "$(dirname -- "$config")" && pwd)/$(basename -- "$config")"
mkdir -p "$run_root"
exec python3 "$repo_root/utils/watchdog/watchdog.py" --name latchpoint --daslang "$runtime" \
    --script "$repo_root/utils/dasllama-server/main.das" --cwd "$run_root" \
    --log "$run_root/watchdog.log" --pid-file "$run_root/watchdog.pid" \
    --health-url http://127.0.0.1:18082/v1/models --shutdown-url http://127.0.0.1:18082/shutdown \
    -- --config "$config"
