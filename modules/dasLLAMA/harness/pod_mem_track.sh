#!/bin/sh
# pod_mem_track.sh <out> : every 5 s the resident memory of the largest dastest child on the box; <out> holds the peak
# seen so far as "<kB> <the child's first test file>". A fleet shard starts it beside its runner and stops it after.
out=$1
peak=0
while true; do
    line=$(ps -eo rss,args | grep "[d]astest/dastest.das" | grep -v "sh -c" | sort -rn | head -1)
    rss=$(echo "$line" | awk '{print $1 + 0}')
    if [ "${rss:-0}" -gt "$peak" ]; then
        peak=$rss
        echo "$peak $(echo "$line" | grep -o 'tests/[A-Za-z_0-9]*\.das' | head -1)" > "$out"
    fi
    sleep 5
done
