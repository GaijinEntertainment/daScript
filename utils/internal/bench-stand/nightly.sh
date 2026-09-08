#!/bin/bash
# bench-stand nightly driver - the box side of the daslang benchmark stand. Runs as the
# `bench` user on the web box; the GitHub workflow (.github/workflows/nightly_bench.yml)
# reaches it over ssh with a forced command (`gate`). Verbs:
#
#   start <ref> [fail-on-regression]
#                      fetch, resolve <ref>, launch `run` detached; prints the run id
#   run <ref>          the whole night, synchronous: checkout, build, benchmark, report, publish
#   follow [run_id]    stream the run's log until it finishes; exits with the run's exit code
#   status             print status.json
#   gate               ssh forced command: dispatches start/follow/status from SSH_ORIGINAL_COMMAND
#
# Layout under BENCH_STAND_HOME (default /srv/bench-stand):
#   src/        clone of the repository - the tree each night checks out, builds and benchmarks
#   runs/       one JSON record per night (utils/internal/bench-stand/README.md - data model)
#   site/       what Caddy serves at /bench/: viewer, data.json, status.json, summary.md, runs -> ../runs
#   logs/       nightly-<run_id>.log
#   last_good/  bin/daslang + daslib + the tool, from the last night whose build passed; renders
#               the report when tonight's build fails, so a broken master still publishes a red night
#   state/      lock and current run id
#
# Environment knobs (all optional; documented in the README):
#   BENCH_STAND_HOME       layout root (default /srv/bench-stand)
#   BENCH_STAND_REPO_URL   commit-link base (default https://github.com/GaijinEntertainment/daScript)
#   BENCH_STAND_SITE_URL   what the summary links to (default https://daslang.io/bench/)
#   BENCH_STAND_JOBS       build parallelism (default nproc)
#   BENCH_STAND_CMAKE_ARGS extra configure arguments
#   BENCH_STAND_BUILD      `skip` reuses the existing build - local dry runs only
#   BENCH_STAND_AOT        `skip` leaves the AOT binary unbuilt; the aot lane then reports itself
#                          unavailable and the other lanes still measure
#   BENCH_STAND_FILTER     forwarded as `run --filter` - local dry runs only
#   BENCH_STAND_LANES      forwarded as `run --lanes` (comma-separated)
#   BENCH_STAND_REPEAT     forwarded as `run --repeat`
#   BENCH_STAND_FAIL_ON_REGRESSION  non-empty: `report --fail-on-regression` (exit 2 on a regression);
#                          `start`'s second word is the same request from a caller with no environment
#
# One variable this script reads that nothing here sets: SSH_ORIGINAL_COMMAND, which the `gate`
# verb parses. An inbound ssh connection whose authorized_keys line forces `nightly.sh gate`
# arrives with the caller's words there and nowhere else.
set -euo pipefail

HOME_DIR=${BENCH_STAND_HOME:-/srv/bench-stand}
SRC=$HOME_DIR/src
RUNS=$HOME_DIR/runs
SITE=$HOME_DIR/site
LOGS=$HOME_DIR/logs
LAST_GOOD=$HOME_DIR/last_good
STATE=$HOME_DIR/state
TOOL=utils/internal/bench-stand
REPO_URL=${BENCH_STAND_REPO_URL:-https://github.com/GaijinEntertainment/daScript}
SITE_URL=${BENCH_STAND_SITE_URL:-https://daslang.io/bench/}
# Portable, because every verb of this script runs wherever a developer sits: nproc is
# GNU-only, and on a box without it the bare $(nproc) failed the whole script with 127
# before it ever reached a verb.
cpu_count() {
    if command -v nproc >/dev/null 2>&1; then nproc
    elif command -v sysctl >/dev/null 2>&1 && sysctl -n hw.ncpu >/dev/null 2>&1; then sysctl -n hw.ncpu
    else echo 4
    fi
}
JOBS=${BENCH_STAND_JOBS:-$(cpu_count)}
# The same module set the sql/xml/audio/terminal benchmarks require; dasLLVM for the jit lane.
# Release: RelWithDebInfo arms the C++ allocation tracker whose exit-time leak report turns
# every clean run into exit 1.
CMAKE_ARGS=(-G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_SQLITE_DISABLED=OFF -DDAS_PUGIXML_DISABLED=OFF
    -DDAS_LLVM_DISABLED=OFF -DDAS_GLFW_DISABLED=ON -DDAS_HV_DISABLED=ON)
BUILD_TARGETS=(daslang dasModuleSQLITE dasModulePUGIXML dasModuleAudio dasModuleMinfft dasModuleTerminal dasModuleUnitTest dasModuleLLVM)
# The AOT lane measures native code, so its binary is built, not flagged: test_aot carries the
# benchmark bodies' stubs (the `benchmarks` row of DAS_AOT_SUITES). It is EXCLUDE_FROM_ALL and
# ~1080 TUs, so it is its own target and its own opt-out - a stand running only interp and jit
# should not pay for it.
AOT_TARGET=test_aot

now_iso() { date -u +%Y-%m-%dT%H:%M:%SZ; }
log() { echo "[$(now_iso)] $*"; }

# JSON string escaper for meta.json. Multi-byte UTF-8 passes through untouched - it is legal
# inside a JSON string, and stripping it would mangle a commit subject or an author name written
# in any non-ASCII script.
json_str() {
    printf '%s' "$1" | tr -d '\r' \
        | LC_ALL=C awk 'BEGIN{ORS=""} {gsub(/\\/,"\\\\"); gsub(/"/,"\\\""); gsub(/\t/,"\\t"); if (NR>1) printf "\\n"; printf "%s", $0}' \
        | tr '\000-\010\013\014\016-\037' ' '
}

write_status() {
    local state=$1 run_id=$2 started=$3 sha=$4 exit_code=${5:-null} tmp=$SITE/status.json.tmp
    printf '{"state":"%s","run_id":"%s","started":"%s","finished":"%s","sha":"%s","exit":%s}\n' \
        "$state" "$run_id" "$started" "$(now_iso)" "$sha" "$exit_code" > "$tmp"
    mv "$tmp" "$SITE/status.json"
}

machine_json() {
    local cpu cores mem kernel compiler load
    cpu=$(lscpu 2>/dev/null | sed -n 's/^Model name:[[:space:]]*//p' | head -1)
    [ -n "$cpu" ] || cpu=$(sysctl -n machdep.cpu.brand_string 2>/dev/null || true)
    cores=$(cpu_count)
    mem=$(free -g 2>/dev/null | awk '/^Mem:/{print $2}')
    [ -n "$mem" ] || mem=$(( $(sysctl -n hw.memsize 2>/dev/null || echo 0) / 1073741824 ))
    kernel=$(uname -r)
    compiler=$( (c++ --version 2>/dev/null || cc --version 2>/dev/null) | head -1)
    load=$(cut -d' ' -f1-3 /proc/loadavg 2>/dev/null || echo "")
    printf '{"host":"%s","cpu":"%s","kernel":"%s","compiler":"%s","cores":%s,"mem_gb":%s,"load_start":"%s"}' \
        "$(json_str "$(hostname)")" "$(json_str "$cpu")" "$(json_str "$kernel")" "$(json_str "$compiler")" "${cores:-0}" "${mem:-0}" "$(json_str "$load")"
}

commit_json() {
    local sha date subject author
    sha=$(git -C "$SRC" rev-parse HEAD)
    date=$(TZ=UTC git -C "$SRC" log -1 --format=%cd --date=iso-strict-local | sed 's/+00:00$/Z/')
    subject=$(git -C "$SRC" log -1 --format=%s)
    author=$(git -C "$SRC" log -1 --format=%an)
    printf '{"sha":"%s","date":"%s","subject":"%s","author":"%s"}' "$sha" "$date" "$(json_str "$subject")" "$(json_str "$author")"
}

resolve_sha() {
    git -C "$SRC" fetch --quiet origin "+refs/heads/*:refs/remotes/origin/*" 2>/dev/null || git -C "$SRC" fetch --quiet origin
    git -C "$SRC" rev-parse --verify --quiet "$1^{commit}" \
        || git -C "$SRC" rev-parse --verify --quiet "origin/$1^{commit}" \
        || { echo "cannot resolve '$1' to a commit" >&2; return 1; }
}

publish_viewer() {
    local from=$1
    cp "$from/site/index.html" "$from/site/app.js" "$from/site/style.css" "$SITE/"
    ln -sfn ../runs "$SITE/runs"
}

report_with() {
    local root=$1 bin=$2 rc=0
    local extra=()
    [ -n "${BENCH_STAND_FAIL_ON_REGRESSION:-}" ] && extra+=(--fail-on-regression)
    "$bin" "$root/$TOOL/main.das" -- report --suite "$root/$TOOL/suite.json" --runs "$RUNS" \
        --out-data "$SITE/data.json.tmp" --out-summary "$SITE/summary.md.tmp" \
        --repo-url "$REPO_URL" --site-url "$SITE_URL" "${extra[@]}" || rc=$?
    if [ -s "$SITE/data.json.tmp" ] && [ -s "$SITE/summary.md.tmp" ]; then
        mv "$SITE/data.json.tmp" "$SITE/data.json"
        mv "$SITE/summary.md.tmp" "$SITE/summary.md"
        REPORT_RENDERED=1
    fi
    rm -f "$SITE/data.json.tmp" "$SITE/summary.md.tmp"
    return $rc
}

run_night() {
    local run_id=$1 sha=$2 started rc=0 build_rc=0 build_start build_end
    started=$(now_iso)
    mkdir -p "$RUNS" "$SITE" "$LOGS" "$STATE" "$LAST_GOOD"
    exec 9>"$STATE/lock"
    if ! flock -n 9; then
        log "another run holds $STATE/lock - refusing to start"
        return 3
    fi
    echo "$run_id" > "$STATE/current"
    write_status running "$run_id" "$started" "$sha"
    # a run that dies on an unexpected error still closes status.json, so `follow` returns
    trap 'write_status finished "$run_id" "$started" "$sha" 1; log "run $run_id: aborted"' ERR
    log "run $run_id: checkout $sha"
    git -C "$SRC" checkout --quiet --detach "$sha"
    git -C "$SRC" submodule update --quiet --init --recursive || log "submodule update failed - continuing with what is checked out"
    # build/, lib/ (the LLVM download + shared modules) and the module cache survive so the rebuild stays incremental
    git -C "$SRC" clean -fdxq --exclude=build --exclude=bin --exclude=lib --exclude=.jitted_scripts

    local meta=$STATE/meta.json build_log=$LOGS/build-$run_id.log
    build_start=$(date +%s)
    if [ "${BENCH_STAND_BUILD:-}" = "skip" ]; then
        log "BENCH_STAND_BUILD=skip - reusing $SRC/bin/daslang"
        [ -x "$SRC/bin/daslang" ] || build_rc=1
        : > "$build_log"
    else
        log "build: cmake ${CMAKE_ARGS[*]} ${BENCH_STAND_CMAKE_ARGS:-} -> ${BUILD_TARGETS[*]} (-j $JOBS)"
        local launcher=()
        command -v ccache >/dev/null && launcher=(-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache)
        local targets=("${BUILD_TARGETS[@]}")
        if [ "${BENCH_STAND_AOT:-}" = skip ]; then
            log "BENCH_STAND_AOT=skip - the aot lane will report itself unavailable"
        else
            targets+=("$AOT_TARGET")
        fi
        # shellcheck disable=SC2086 - BENCH_STAND_CMAKE_ARGS must word-split into separate arguments
        if ! ( cd "$SRC" && cmake --no-warn-unused-cli -B build "${CMAKE_ARGS[@]}" "${launcher[@]}" ${BENCH_STAND_CMAKE_ARGS:-} \
                && cmake --build build --parallel "$JOBS" --target "${targets[@]}" ) > "$build_log" 2>&1; then
            build_rc=1
        fi
    fi
    build_end=$(date +%s)
    local build_status=ok
    [ $build_rc -eq 0 ] || build_status=failed
    log "build: $build_status in $((build_end - build_start)) s (log $build_log)"
    printf '{"run_id":"%s","started":"%s","commit":%s,"machine":%s,"build":{"status":"%s","seconds":%s,"log_tail":"%s"}}\n' \
        "$run_id" "$started" "$(commit_json)" "$(machine_json)" "$build_status" "$((build_end - build_start))" \
        "$(json_str "$(tail -n 40 "$build_log" | cut -c1-400)")" > "$meta"

    local record=$RUNS/$run_id.json
    if [ $build_rc -eq 0 ]; then
        local run_args=()
        [ -n "${BENCH_STAND_FILTER:-}" ] && run_args+=(--filter "$BENCH_STAND_FILTER")
        [ -n "${BENCH_STAND_LANES:-}" ] && run_args+=(--lanes "$BENCH_STAND_LANES")
        [ -n "${BENCH_STAND_REPEAT:-}" ] && run_args+=(--repeat "$BENCH_STAND_REPEAT")
        log "benchmarks: start"
        local tool_rc=0
        ( cd "$SRC" && bin/daslang "$TOOL/main.das" -- run --root . --meta "$meta" --out "$record" "${run_args[@]}" ) || tool_rc=$?
        log "benchmarks: exit $tool_rc"
        rc=$tool_rc
        if [ ! -s "$record" ]; then
            log "the tool wrote no run record - writing a failed one"
            rc=1
            record_failed "$record" "$meta" "the bench-stand tool exited $tool_rc without writing a record; see logs/nightly-$run_id.log on the box"
        fi
    else
        rc=1
        record_failed "$record" "$meta" "the build failed after $((build_end - build_start)) s - logs/build-$run_id.log on the box has the whole log"
    fi

    local report_root=$SRC report_bin=$SRC/bin/daslang report_rc=0
    if [ $build_rc -ne 0 ] || [ ! -x "$report_bin" ]; then
        if [ -x "$LAST_GOOD/bin/daslang" ]; then
            report_root=$LAST_GOOD; report_bin=$LAST_GOOD/bin/daslang
            log "report: rendering with last_good's binary"
        else
            log "report: no binary at all (first night, build failed) - status.json is the only publication"
            report_bin=""
        fi
    fi
    if [ -n "$report_bin" ]; then
        REPORT_RENDERED=0
        report_with "$report_root" "$report_bin" || report_rc=$?
        if [ "$REPORT_RENDERED" = 0 ] && [ "$report_root" != "$LAST_GOOD" ] && [ -x "$LAST_GOOD/bin/daslang" ]; then
            # tonight's tree built but its copy of this tool cannot render: the night is still
            # published, by the last good tool, so the site shows the red night rather than yesterday's
            log "report: tonight's tool failed to render (exit $report_rc) - retrying with last_good"
            report_root=$LAST_GOOD; report_bin=$LAST_GOOD/bin/daslang; report_rc=0
            report_with "$report_root" "$report_bin" || report_rc=$?
        fi
        publish_viewer "$report_root/$TOOL"
        log "report: exit $report_rc (rendered=$REPORT_RENDERED)"
    fi
    # last_good advances only when tonight's own tree rendered the report: a tree whose build
    # passed but whose copy of this tool is broken must not become the fallback
    if [ $build_rc -eq 0 ] && [ "${REPORT_RENDERED:-0}" = 1 ] && [ "$report_root" = "$SRC" ]; then
        mkdir -p "$LAST_GOOD/bin"
        cp "$SRC/bin/daslang" "$LAST_GOOD/bin/daslang"
        rsync -a --delete "$SRC/daslib/" "$LAST_GOOD/daslib/"
        mkdir -p "$LAST_GOOD/$TOOL"
        rsync -a --delete "$SRC/$TOOL/" "$LAST_GOOD/$TOOL/"
    fi
    local final=$rc
    [ $final -eq 0 ] && final=$report_rc
    trap - ERR
    write_status finished "$run_id" "$started" "$sha" "$final"
    log "run $run_id: finished, exit $final"
    return $final
}

record_failed() {
    # The record of a night that never benchmarked, written by the tool so the run-record schema
    # has exactly one writer. `reason` must be non-empty - an empty one would read to the tool as
    # "no --failed given" and start a real run. Neither binary available (a first night whose
    # build failed) leaves status.json as the night's only publication.
    local record=$1 meta=$2 reason=$3 root out
    for root in "$SRC" "$LAST_GOOD"; do
        [ -x "$root/bin/daslang" ] || continue
        out=$("$root/bin/daslang" "$root/$TOOL/main.das" -- run --suite "$root/$TOOL/suite.json" \
            --meta "$meta" --out "$record" --failed "$reason" 2>&1) || true
        if [ -s "$record" ]; then
            log "failed record written by $root/bin/daslang"
            return 0
        fi
        # the one path whose failure is otherwise invisible: it runs when everything else already
        # went wrong, so its own output is the only thing left to read
        log "record_failed: $root/bin/daslang wrote nothing - $(printf '%s' "$out" | tail -n 3 | tr '\n' ' ')"
    done
    log "no binary could write the failed record - status.json is the night's only publication"
    return 1
}

verb_start() {
    # start <ref> [fail-on-regression]. The second word is how the caller asks for a regression
    # to fail the night: an ssh forced command carries no environment, so BENCH_STAND_* cannot
    # reach here from the workflow - the verb's own argument is the only channel.
    local ref=${1:-master} regression=${2:-} sha run_id
    mkdir -p "$LOGS" "$STATE" "$SITE"
    case "$regression" in
        ""|fail-on-regression) ;;
        *) echo "start: second argument is 'fail-on-regression' or nothing, got '$regression'" >&2; return 2 ;;
    esac
    sha=$(resolve_sha "$ref")
    run_id="$(date -u +%Y%m%dT%H%M%SZ)-${sha:0:8}"
    if [ -e "$STATE/lock" ] && ! flock -n "$STATE/lock" true; then
        echo "busy: $(cat "$STATE/current" 2>/dev/null) is still running" >&2
        return 3
    fi
    echo "$run_id" > "$STATE/current"
    local fail_flag=${BENCH_STAND_FAIL_ON_REGRESSION:-}
    [ "$regression" = fail-on-regression ] && fail_flag=1
    BENCH_STAND_FAIL_ON_REGRESSION=$fail_flag \
        setsid nohup bash "$0" run-id "$run_id" "$sha" > "$LOGS/nightly-$run_id.log" 2>&1 < /dev/null &
    echo "$run_id"
}

verb_follow() {
    local run_id=${1:-$(cat "$STATE/current" 2>/dev/null || true)}
    [ -n "$run_id" ] || { echo "no current run" >&2; return 1; }
    local logf=$LOGS/nightly-$run_id.log
    local waited=0
    while [ ! -f "$logf" ] && [ $waited -lt 60 ]; do sleep 1; waited=$((waited + 1)); done
    if [ ! -f "$logf" ]; then
        # a run driven by `run` (no detached log) still answers through status.json
        if grep -q "\"run_id\":\"$run_id\"" "$SITE/status.json" 2>/dev/null && grep -q '"state":"finished"' "$SITE/status.json"; then
            cat "$SITE/status.json"
            return "$(sed -n 's/.*"exit":\([0-9]*\).*/\1/p' "$SITE/status.json")"
        fi
        echo "no log for $run_id" >&2; return 1
    fi
    local pos=0
    while :; do
        local size
        size=$(stat -c %s "$logf")
        if [ "$size" -gt "$pos" ]; then
            tail -c +"$((pos + 1))" "$logf" | head -c "$((size - pos))"
            pos=$size
        fi
        if grep -q "\"run_id\":\"$run_id\"" "$SITE/status.json" 2>/dev/null && grep -q '"state":"finished"' "$SITE/status.json"; then
            local exit_code
            exit_code=$(sed -n 's/.*"exit":\([0-9]*\).*/\1/p' "$SITE/status.json")
            echo "--- run $run_id finished with exit ${exit_code:-?} ---"
            return "${exit_code:-1}"
        fi
        sleep 20
    done
}

verb_gate() {
    # SSH forced command: only these verbs, at most two arguments, only path characters in each.
    local cmd=${SSH_ORIGINAL_COMMAND:-status}
    # shellcheck disable=SC2086 - the forced command's one string is split into words on purpose
    set -- $cmd
    if [ $# -gt 3 ]; then
        echo "refused: too many arguments" >&2; exit 2
    fi
    local verb=${1:-status} arg=${2:-} arg2=${3:-}
    case "$arg$arg2" in
        *[!A-Za-z0-9._/-]*) echo "refused: argument '$arg$arg2' has characters outside A-Za-z0-9._/-" >&2; exit 2 ;;
    esac
    case "$verb" in
        start) verb_start "$arg" "$arg2" ;;
        follow) verb_follow "$arg" ;;
        status) cat "$SITE/status.json" 2>/dev/null || echo '{"state":"never-run"}' ;;
        summary) cat "$SITE/summary.md" 2>/dev/null || echo "no summary yet" ;;
        *) echo "refused: unknown verb '$verb'" >&2; exit 2 ;;
    esac
}

case "${1:-}" in
    start) verb_start "${2:-master}" "${3:-}" ;;
    run) sha=$(resolve_sha "${2:-master}"); run_night "$(date -u +%Y%m%dT%H%M%SZ)-${sha:0:8}" "$sha" ;;
    run-id) run_night "$2" "$3" ;;
    follow) verb_follow "${2:-}" ;;
    status) cat "$SITE/status.json" 2>/dev/null || echo '{"state":"never-run"}' ;;
    summary) cat "$SITE/summary.md" 2>/dev/null || echo "no summary yet" ;;
    gate) verb_gate ;;
    *) echo "usage: nightly.sh start <ref> [fail-on-regression] | run <ref> | follow [run_id] | status | summary | gate" >&2; exit 2 ;;
esac
