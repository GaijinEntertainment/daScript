#!/usr/bin/env bash
# Prints what the hosted runner gave this job: cores, memory, load, free disk. A job that runs
# several times slower than the night before is read against these lines first.
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
