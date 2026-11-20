#!/usr/bin/env bash
# Prints what the hosted runner gave this job: cores, memory, load, free disk. A job that runs
# several times slower than the night before is read against these lines first. Then the size of
# each directory a cache slot saves, which at the end of a job is what the slot is made of.
set -u
os=$(uname -s)
echo "runner: os=$os arch=$(uname -m) name=${RUNNER_NAME:-?} image=${ImageOS:-?} ${ImageVersion:-}"
case "$os" in
  Linux)
    echo "runner: cpus=$(nproc) model=$(grep -m1 'model name' /proc/cpuinfo | cut -d: -f2- | xargs)"
    awk '/MemTotal|MemAvailable|SwapTotal/ { printf "runner: %s %d MiB\n", $1, $2 / 1024 }' /proc/meminfo
    echo "runner: load=$(cut -d' ' -f1-3 /proc/loadavg)"
    ;;
  Darwin)
    echo "runner: cpus=$(sysctl -n hw.ncpu) model=$(sysctl -n machdep.cpu.brand_string)"
    echo "runner: MemTotal: $(( $(sysctl -n hw.memsize) / 1048576 )) MiB"
    page=$(sysctl -n hw.pagesize)
    free=$(vm_stat | awk '/Pages free|Pages inactive|Pages speculative/ { gsub("\\.", "", $NF); s += $NF } END { print s }')
    echo "runner: MemAvailable: $(( free * page / 1048576 )) MiB"
    echo "runner: load=$(sysctl -n vm.loadavg)"
    ;;
  *)
    echo "runner: cpus=${NUMBER_OF_PROCESSORS:-?} model=${PROCESSOR_IDENTIFIER:-?}"
    powershell -NoProfile -Command '$o = Get-CimInstance Win32_OperatingSystem; "runner: MemTotal: {0} MiB`nrunner: MemAvailable: {1} MiB" -f [int]($o.TotalVisibleMemorySize / 1024), [int]($o.FreePhysicalMemory / 1024)' || true
    ;;
esac
echo "runner: disk $(df -h . | awk 'NR == 2 { print "size=" $2 " free=" $4 }')"
for d in "${SCCACHE_DIR:-}" .cache/daslang/test_cache .cache/daslang/lint_cache; do
  if [ -n "$d" ] && [ -d "$d" ]; then
    echo "runner: cache $(du -sm "$d" | cut -f1) MiB $d"
  fi
done
if [ -d .cache/daslang ]; then
  jit_dir_count=$(find .cache/daslang -maxdepth 1 -type d -name '_anon_*' | wc -l | tr -d ' ')
  if [ "$jit_dir_count" != 0 ]; then
    jit_mib=$(find .cache/daslang -maxdepth 1 -type d -name '_anon_*' -exec du -scm {} + \
      | awk '$2 == "total" { s += $1 } END { print s + 0 }')
    echo "runner: cache $jit_mib MiB .cache/daslang/_anon_* ($jit_dir_count dirs)"
  fi
fi
