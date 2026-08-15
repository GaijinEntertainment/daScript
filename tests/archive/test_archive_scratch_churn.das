options gen2
options persistent_heap = true
options very_safe_context

// scope: the archive writer's eager-grow opt-in (MemSerializer.write -> scratch_ensure_capacity)
// eager expectation: a serialize leaves ~1x the payload live (the final buffer)
// deferred would leave: ~2x (the abandoned doubling ladder too) - the 1.5x bound splits them
// payload: element-wise records (struct fields, per-element strings) so the buffer walks the
//          ladder; a bulk workhorse array would memcpy in one write and prove nothing
// behavior-unchanged instrument: test_archive.das, deliberately untouched

require dastest/testing_boost public
require daslib/archive

struct Rec {
    id : int
    name : string
}

var g_recs : array<Rec>
var g_bytes : array<uint8>

[test]
def test_archive_scratch_churn(t : T?) {
    t |> run("round-trip is field-identical through the eager path") @(t : T?) {
        g_recs |> resize(2000)
        for (i in range(2000)) {
            g_recs[i].id = i
            g_recs[i].name = "record number {i} with some padding to stretch the stream"
        }
        g_bytes <- mem_archive_save(g_recs)   // nolint:PERF030 — the target global is fresh-empty
        var loaded : array<Rec>
        t |> success(mem_archive_load(g_bytes, loaded), "load must succeed")
        t |> equal(length(loaded), 2000)
        var mismatches = 0
        for (i in range(2000)) {
            if (loaded[i].id != g_recs[i].id || loaded[i].name != g_recs[i].name) {
                mismatches++
            }
        }
        t |> equal(mismatches, 0)
        delete loaded
    }
    t |> run("churn stays ~1x the payload under very_safe") @(t : T?) {
        // the payload and the previous arm's allocations are all outside this window
        let before = heap_bytes_allocated()
        var bytes <- mem_archive_save(g_recs)
        let grew = heap_bytes_allocated() - before
        let cap = uint64(long_capacity(bytes))
        // eager: the live survivor is the final buffer (the serializer shell nets to zero);
        // deferred would leave the doubling ladder too, ~2x - the 1.5x bound splits them
        t |> success(grew <= cap * 3ul / 2ul, "abandoned generations must not survive the save")
        t |> success(grew >= cap, "the final buffer itself is in the window")
        t |> success(long_capacity(bytes) >= long_length(bytes), "sanity: resize stayed within the ensured capacity")
        delete bytes
    }
}
