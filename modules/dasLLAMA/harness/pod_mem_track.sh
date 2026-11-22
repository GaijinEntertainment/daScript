#!/bin/sh
# pod_mem_track.sh <out> [<models>] : every 5 s the resident memory of the largest dastest child on the box; <out> holds the
# peak seen so far as "<kB> <the child's first test file>". With <models>, the pod's folder of model copies, it also keeps
# the page cache off the pod's memory limit: while the cgroup's use stands past half its limit, the pages of every copy
# that no process maps are dropped - a cache at the limit stalls each load and CPU cell on reclaim. A fleet home starts it
# beside its runner and stops it after. pod_mem_track.sh drop <file>... : drops each file's pages from the page cache, the
# tracker's and the fleet copier's one spelling of the drop; it fails when one drop does.
if [ "$1" = drop ]; then
    shift
    rc=0
    for f in "$@"; do
        dd if="$f" iflag=nocache count=0 status=none || rc=1
    done
    exit $rc
fi
out=$1
models=$2
peak=0
limit=$(cat /sys/fs/cgroup/memory.max 2>/dev/null)
while true; do
    line=$(ps -eo rss,args | grep "[d]astest/dastest.das" | grep -v "sh -c" | sort -rn | head -1)
    rss=$(echo "$line" | awk '{print $1 + 0}')
    if [ "${rss:-0}" -gt "$peak" ]; then
        peak=$rss
        echo "$peak $(echo "$line" | grep -o 'tests/[A-Za-z_0-9]*\.das' | head -1)" > "$out"
    fi
    if [ -n "$models" ] && [ "${limit:-max}" != max ] && [ "$(cat /sys/fs/cgroup/memory.current)" -gt $((limit / 2)) ]; then
        find "$models" -type f -exec sh "$0" drop {} +
    fi
    sleep 5
done
