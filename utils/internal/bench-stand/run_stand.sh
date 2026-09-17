#!/bin/bash
# One pass of the stand on a ref - checkout, build, benchmark, report, publish. README.md.
set -euo pipefail
HOME_DIR=${BENCH_STAND_HOME:-/srv/bench-stand}; SRC=$HOME_DIR/src; SITE=$HOME_DIR/site; TOOL=utils/internal/bench-stand; BENCHCTL=utils/benchctl
REF=${1:-master}; shift || true
CMAKE_ARGS=(-G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_SQLITE_DISABLED=OFF -DDAS_PUGIXML_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF -DDAS_GLFW_DISABLED=ON -DDAS_HV_DISABLED=ON)
TARGETS=(daslang test_aot_subset test_llvm_aot dasModuleSQLITE dasModulePUGIXML dasModuleAudio dasModuleMinfft dasModuleTerminal dasModuleUnitTest dasModuleLLVM)

now() { date -u +%Y-%m-%dT%H:%M:%SZ; }
jstr() { printf '%s' "$1" | tr -d '\r' | LC_ALL=C awk 'BEGIN{ORS=""} {gsub(/\\/,"\\\\"); gsub(/"/,"\\\""); gsub(/\t/,"\\t"); if (NR>1) printf "\\n"; printf "%s", $0}' | tr '\000-\037' ' '; }
status() { printf '{"state":"%s","run_id":"%s","started":"%s","finished":"%s","sha":"%s","exit":%s}\n' "$1" "$RUN_ID" "$STARTED" "$(now)" "$SHA" "${2:-null}" > "$SITE/status.json"; }

mkdir -p "$HOME_DIR/runs" "$SITE" "$HOME_DIR/logs"
exec 9>"$HOME_DIR/lock"; flock -n 9 || { echo "another pass holds $HOME_DIR/lock"; exit 3; }
git -C "$SRC" fetch -q origin
SHA=$(git -C "$SRC" rev-parse --verify "origin/$REF^{commit}" 2>/dev/null || git -C "$SRC" rev-parse --verify "$REF^{commit}")
RUN_ID="$(date -u +%Y%m%dT%H%M%SZ)-${SHA:0:8}"; STARTED=$(now)
status running; trap 'status finished 1' ERR
git -C "$SRC" checkout -q --detach "$SHA"
git -C "$SRC" submodule update -q --init --recursive || true
git -C "$SRC" clean -fdxq --exclude=build --exclude=bin --exclude=lib --exclude=.jitted_scripts --exclude='*.shared_module'

LOG=$HOME_DIR/logs/build-$RUN_ID.log; BUILD=ok; t0=$(date +%s)
if [ "${BENCH_STAND_BUILD:-}" = skip ]; then
    : > "$LOG"; [ -x "$SRC/bin/daslang" ] || BUILD=failed
else
    launcher=(); command -v ccache >/dev/null && launcher=(-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache)
    ( cd "$SRC" && cmake --no-warn-unused-cli -B build "${CMAKE_ARGS[@]}" "${launcher[@]}" && cmake --build build --parallel "$(nproc)" --target "${TARGETS[@]}" ) > "$LOG" 2>&1 || BUILD=failed
fi
SECS=$(( $(date +%s) - t0 ))
META=$HOME_DIR/meta.json
printf '{"run_id":"%s","started":"%s","commit":{"sha":"%s","date":"%s","subject":"%s","author":"%s"},"machine":{"host":"%s","cores":%s},"build":{"status":"%s","seconds":%s,"log_tail":"%s"}}\n' \
    "$RUN_ID" "$STARTED" "$SHA" "$(TZ=UTC git -C "$SRC" log -1 --format=%cd --date=iso-strict-local | sed 's/+00:00$/Z/')" \
    "$(jstr "$(git -C "$SRC" log -1 --format=%s)")" "$(jstr "$(git -C "$SRC" log -1 --format=%an)")" "$(jstr "$(hostname)")" "$(nproc)" \
    "$BUILD" "$SECS" "$(jstr "$(tail -n 40 "$LOG" | cut -c1-400)")" > "$META"

[ -x "$SRC/bin/daslang" ] || { echo "no binary: the build failed and none survives from an earlier pass"; trap - ERR; status finished 1; exit 1; }
[ "$BUILD" = ok ] || set -- "$@" --failed "the build failed after $SECS s - $LOG on the box has the whole log"
rc=0; report_rc=0
( cd "$SRC" && bin/daslang "$BENCHCTL/main.das" -- run --root . --meta "$META" --out "$HOME_DIR/runs/$RUN_ID.json" "$@" ) || rc=$?
"$SRC/bin/daslang" "$SRC/$BENCHCTL/main.das" -- report --runs "$HOME_DIR/runs" --out-data "$SITE/data.json.tmp" --repo-url https://github.com/GaijinEntertainment/daScript || report_rc=$?
[ -s "$SITE/data.json.tmp" ] && mv "$SITE/data.json.tmp" "$SITE/data.json"
cp "$SRC/$TOOL"/site/{index.html,app.js,style.css} "$SITE/"; ln -sfn ../runs "$SITE/runs"
[ $rc -eq 0 ] && rc=$report_rc
trap - ERR; status finished "$rc"; exit "$rc"
