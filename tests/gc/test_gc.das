options gen2
options persistent_heap
options gc
// tests here call heap_collect with a collectable argument or a live
// `T?`, and AOT refuses a function whose frame the collector must
// walk (error 50503)
options no_aot

require dastest/testing_boost public

// ---- Reserved capacity (heap_total_allocated) drops after a dead
// burst is collected. First in the file on purpose: live data left by
// the other tests keeps decks pinned.

struct Pebble {
    value : int
    weight : float
}

var g_pebbles : array<Pebble?>

def private make_pebbles(n : int) {
    g_pebbles |> resize(n)
    for (p, i in g_pebbles, count()) {
        p = new Pebble(value = i, weight = float(i))
    }
}

[test(description = "a collect after a dead burst releases the empty decks: reserved capacity drops, stays above live bytes, and allocation continues")]
def test_collection_releases_empty_decks(t : T?) {
    let reserved_idle = heap_total_allocated()
    make_pebbles(200000)
    let reserved_peak = heap_total_allocated()
    t |> success(reserved_peak > reserved_idle, "the burst grows the reserved capacity")
    g_pebbles |> resize(0)
    unsafe(heap_collect(true, false))
    let reserved_after = heap_total_allocated()
    t |> success(reserved_after < reserved_peak, "a collection releases the burst's empty decks")
    t |> success(reserved_after >= heap_bytes_allocated(),
                 "reserved capacity stays at or above live bytes")
    t |> success(reserved_after > 0ul, "reserved capacity stays non-zero")
    make_pebbles(1000)
    t |> equal(g_pebbles[999].value, 999, "allocation continues on the kept decks")
    g_pebbles |> resize(0)
}

// ---- honest-frames marking: caller locals survive a collect no
// matter where in the chain it fires - inlined callee, same frame,
// block/lambda/function pointer indirection, argument evaluation, or
// an inlined callee's own for loop.

def churn_strings(tag : string) : int {
    var junk <- [for (i in range(200)); "junk/{tag}/segment_{i}/{i * 3}_tail"]
    var total = 0
    for (s in junk) {
        total += length(s)
    }
    delete junk
    return total
}

// tiny and inline-eligible on purpose: the collect call site gets
// spliced into the caller
def collect_every(var since : int&; period : int) {
    since++
    if (since >= period) {
        since = 0
        unsafe(heap_collect(true, true))
    }
}

[never_inline]
def apply_block_noinline(b : block) {
    var pad = 1
    invoke(b)
    pad++
}

def collect_now_regular() {
    var pad = 1
    unsafe(heap_collect(true, true))
    pad++
}

def make_files(var files : array<string>) {
    files |> reserve(60)
    for (d in range(12)) {
        for (i in range(5)) {
            files |> push("root/dir_{d}/file_{i}.das")
        }
    }
}

def verify_files(t : T?; files : array<string>; tag : string) {
    var idx = 0
    for (f in files) {
        if (f != "root/dir_{idx / 5}/file_{idx % 5}.das") {
            t |> failure("{tag}: corrupt entry at index {idx}")
            return
        }
        idx++
    }
    t |> success(idx == 60, "{tag}: walked all 60 entries")
}

// arg 0's initializer collects (validate on) while arg 1's temp slot
// is still raw stack garbage - its position interval must keep that
// slot invisible to the walk
def churn_and_collect_str(seed : int) : string {
    var junk <- [for (i in range(400)); "garbage_{seed}_{i}_padding_padding"]
    delete junk
    let keep = "kept_{seed}"
    unsafe(heap_collect(true, true))
    return keep
}

def two_args(a, b : string) : int {
    let pad = length(a) + length(b)
    return pad
}

// a collect while a LATER argument evaluates - the earlier temp holds
// the only reference. the two arms below are probabilistic
// instruments: a regression frees the temp's string, and the assert
// fires only when the freed block gets reused (pre-fix: 1 in 20
// iterations)
def collect_and_tail(seed : int) : string {
    var junk <- [for (i in range(400)); "garbage2_{seed}_{i}_padding_padding"]
    delete junk
    unsafe(heap_collect(true, true))
    return "tail_{seed}"
}

def pick_first(a, _b : string) : string {
    let r = a
    return r
}

// inline-eligible callee whose own `for` hosts the collect - pins the
// iterator variable's position interval through the splice
def scan_with_collect(names : array<string>; var since : int&) : int {
    var total = 0
    for (nm in names) {
        total += length(nm)
        collect_every(since, 3)
    }
    return total
}

[test(description = "caller locals survive a validating collect fired from an inlined callee, the same frame, a block, a lambda, a function pointer, a later argument or an inlined for loop")]
def test_collect_frame_marking(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("collect in an inlined callee keeps caller locals") @(t : T?) {
        var files : array<string>
        make_files(files)
        var since = 0
        var churned = 0
        for (f in files) {
            churned += churn_strings(f)
            collect_every(since, 5)
        }
        verify_files(t, files, "inlined callee")
        t |> success(churned > 0)
    }
    t |> run("same-frame collect keeps locals") @(t : T?) {
        var files : array<string>
        make_files(files)
        var since = 0
        for (f in files) {
            churn_strings(f)
            since++
            if (since >= 5) {
                since = 0
                unsafe(heap_collect(true, true))
            }
        }
        verify_files(t, files, "same frame")
    }
    t |> run("collect through a real block invoke keeps caller locals") @(t : T?) {
        var files : array<string>
        make_files(files)
        var since = 0
        for (f in files) {
            churn_strings(f)
            since++
            if (since >= 5) {
                since = 0
                apply_block_noinline() {
                    unsafe(heap_collect(true, true))
                }
            }
        }
        verify_files(t, files, "block invoke")
    }
    t |> run("collect through a lambda keeps caller locals") @(t : T?) {
        var files : array<string>
        make_files(files)
        let lam <- @() {
            var pad = 1
            unsafe(heap_collect(true, true))
            pad++
        }
        var since = 0
        for (f in files) {
            churn_strings(f)
            since++
            if (since >= 5) {
                since = 0
                invoke(lam)
            }
        }
        verify_files(t, files, "lambda")
    }
    t |> run("collect through a function pointer keeps caller locals") @(t : T?) {
        var files : array<string>
        make_files(files)
        let fp = @@collect_now_regular
        var since = 0
        for (f in files) {
            churn_strings(f)
            since++
            if (since >= 5) {
                since = 0
                invoke(fp)
            }
        }
        verify_files(t, files, "function pointer")
    }
    t |> run("collect during argument evaluation of an inlined call") @(t : T?) {
        for (i in range(20)) {
            let r = two_args(churn_and_collect_str(i), "tail_{i}")
            t |> equal(r, length("kept_{i}") + length("tail_{i}"))
        }
    }
    t |> run("earlier argument temp survives a collect in a later argument") @(t : T?) {
        for (i in range(20)) {
            let kept = pick_first("kept_{i}", collect_and_tail(i))
            t |> equal(kept, "kept_{i}")
        }
    }
    t |> run("collect inside an inlined callee's for loop") @(t : T?) {
        var names : array<string>
        make_files(names)
        var since = 0
        var expected = 0
        for (nm in names) {
            expected += length(nm)
        }
        let total = scan_with_collect(names, since)
        t |> equal(total, expected)
        verify_files(t, names, "inlined for")
    }
}

// ---- A borrowed array view (a mapped file plane) whose byte length
// is a whole multiple of 4 GiB: the mark walk sizes the range in 64
// bits. Sized in 32, the range read as empty and the validating
// collect indexed the heap's chunk table by it.

var g_backing : uint64[16]   // global memory, outside the heap, as a mapping is
var g_plane : array<uint64>

[test(description = "a borrowed array view spanning a whole multiple of 4 GiB survives a validating collect with its length intact")]
def test_gc_borrowed_range_of_4gib(t : T?) {
    // 2^29 uint64 = 4 GiB of range over 16 real elements; the walk reads
    // no element of a uint64 array
    let n = 1l << 29l
    unsafe {
        _builtin_make_temp_array_i64(g_plane, addr<void?>(g_backing[0]), n)
        heap_collect(true, true)
    }
    t |> equal(long_length(g_plane), n, "the view survives a validating collect")
    unsafe(_builtin_forget_temp_array(g_plane))
}

// ---- Coverage for runtime heap GC walk paths not exercised by
// tests/decs/test_gc.das: garbage reclamation, cyclic references
// (un/reachable), table keys+values, big (bigStuff) allocations, and
// repeated-collect stability. All collects use validate mode so a
// dangling or double-freed pointer fails loudly.

struct CNode {
    next : CNode?
}

struct Holder {
    name : string
    vals : array<int>
    tags : table<string; int>
}

// globals are unconditional GC roots
var g_holder : Holder?
var g_cycle : CNode?
var g_big : array<int>
// storing each allocation through a global makes it escape (heap, not
// stack-allocated by escape analysis) so this suite keeps exercising
// the heap GC; cleared to null so it all becomes garbage
var g_garbage_sink : Holder?

def collect() {
    unsafe(heap_collect(true, true))
}

def make_garbage(n : int) {
    for (i in range(n)) {
        g_garbage_sink = new Holder(name = "garbage_{i}", vals <- [for (x in range(8)); x])
    }
    g_garbage_sink = null
}

// reclaim garbage; reachable data survives intact
[test(description = "a collect reclaims unreachable garbage while data reachable from a global survives intact")]
def test_gc_frees_garbage(t : T?) {
    g_holder = new Holder()
    g_holder.name = "live"
    g_holder.vals <- [for (x in range(16)); x * 2]
    collect()
    let live = heap_bytes_allocated()
    make_garbage(2000)
    let withGarbage = heap_bytes_allocated()
    t |> success(withGarbage > live, "garbage should grow the heap")
    collect()
    t |> success(heap_bytes_allocated() < withGarbage, "collect must reclaim garbage")
    t |> equal(g_holder.name, "live")
    t |> equal(g_holder.vals[8], 16)
}

// an unreachable cycle must be reclaimed (a refcount collector would
// leak it)
def make_unreachable_cycle() {
    var a = new CNode()
    var b = new CNode()
    a.next = b
    b.next = a              // a<->b; both go out of scope here -> unreachable cycle
}

[test(description = "an unreachable two-node cycle is reclaimed by a collect")]
def test_gc_unreachable_cycle_freed(t : T?) {
    g_cycle = null
    collect()
    let before = heap_bytes_allocated()
    make_unreachable_cycle()
    collect()
    t |> success(heap_bytes_allocated() <= before, "unreachable cycle must be reclaimed")
}

// a reachable cycle must survive and must not hang the marker
[test(description = "a cycle reachable from a global survives a collect with its links intact and does not hang the marker")]
def test_gc_reachable_cycle_survives(t : T?) {
    var a = new CNode()
    var b = new CNode()
    a.next = b
    b.next = a
    g_cycle = a
    make_garbage(500)
    collect()
    t |> success(g_cycle != null, "cycle head survived")
    t |> success(g_cycle.next != null && g_cycle.next.next == g_cycle, "cycle preserved through GC")
    g_cycle = null
}

// table with heap keys (strings) and a struct holding it
[test(description = "a table with string keys held by a heap struct keeps all its entries through a collect")]
def test_gc_table_keys_values(t : T?) {
    g_holder = new Holder()
    g_holder.name = "tbl"
    for (i in range(64)) {
        g_holder.tags |> insert("key_{i}", i * i)
    }
    make_garbage(1000)
    collect()
    t |> equal(length(g_holder.tags), 64)
    t |> equal(g_holder.tags?["key_7"] ?? -1, 49)
}

// large array routes through bigStuff (>256B); must survive a GC
// alongside churn
[test(description = "a large array allocated through the big-allocation path survives a collect alongside garbage churn")]
def test_gc_big_allocation(t : T?) {
    delete g_big
    g_big <- [for (x in range(2000)); x]
    make_garbage(1000)
    collect()
    t |> equal(length(g_big), 2000)
    t |> equal(g_big[1234], 1234)
}

// repeated collects on a stable live set must not change the heap (no
// leak, no premature free)
[test(description = "repeated collects over a stable live set leave the heap byte count unchanged and the data intact")]
def test_gc_repeated_stable(t : T?) {
    g_holder = new Holder()
    g_holder.name = "stable"
    g_holder.vals <- [for (x in range(64)); x]
    for (i in range(64)) {
        g_holder.tags |> insert("k_{i}", i)
    }
    collect()
    let b1 = heap_bytes_allocated()
    for (_i in range(5)) {
        collect()
    }
    t |> equal(b1, heap_bytes_allocated())
    t |> equal(g_holder.name, "stable")
    t |> equal(length(g_holder.vals), 64)
}

// ---- Regression for the bounded-recursion mark walk: a deeply
// nested heap graph must be collected without overflowing the C++
// stack. Before the guard, this SIGSEGV'd in the recursive
// GcMarkAnyHeap::walk; now it collects and the live chain survives.

struct Node {
    name : string
    next : Node?
}

var g_head : Node?

def build_chain(depth : int) {
    var head : Node? = null
    for (_i in range(depth)) {
        head = new Node(name = "n", next = head)
    }
    g_head = head
}

def chain_len(head : Node?) : int {
    var n = 0
    var p = head
    while (p != null) {
        n ++
        p = p.next
    }
    return n
}

[test(description = "a 200000-node linked chain is marked without overflowing the C++ stack and the whole chain survives")]
def test_gc_deep_chain_no_overflow(t : T?) {
    let depth = 200000
    build_chain(depth)
    unsafe(heap_collect(true, true))    // validate=true: no overflow, no dangling pointers
    t |> equal(chain_len(g_head), depth)    // whole chain survived GC (reachable, not swept)
}

// ---- runtime API: allocation budgets, operation counters and
// collection statistics

struct ApiGarbage {
    values : array<int>
}

var g_api_garbage : ApiGarbage?

def private make_api_garbage() {
    for (i in range(128)) {
        g_api_garbage = new ApiGarbage(values <- [for (x in range(64)); x + i])
    }
    g_api_garbage = null
}

[test(description = "the GC runtime API rejects a zero budget, keeps the old one, and its operation, collection and reclaim counters observe a sweep")]
def gc_runtime_api_reports_and_rejects_invalid_budgets(t : T?) {
    let original_budget = gc_allocation_budget()

    set_gc_allocation_budget(1048576ul, 2097152ul)
    t |> equal(gc_allocation_budget(), urange64(1048576ul, 2097152ul),
        "budget getter preserves heap/string order")

    for (bad, i in fixed_array(urange64(0ul, 1ul), urange64(1ul, 0ul)), count()) {
        var rejected = false
        try {
            set_gc_allocation_budget(bad.x, bad.y)
        } recover {
            rejected = true
        }
        t |> success(rejected, i == 0
            ? "zero heap budget must be rejected"
            : "zero string budget must be rejected")
        t |> equal(gc_allocation_budget(), urange64(1048576ul, 2097152ul),
            "a rejected budget leaves both values unchanged")
    }

    let heap_ops_before = heap_operation_counts(false)
    let string_ops_before = heap_operation_counts(true)
    var scratch : array<int>
    scratch |> resize(64)
    scratch |> resize(512)
    delete scratch
    let heap_ops_after = heap_operation_counts(false)
    t |> success(heap_ops_after.x > heap_ops_before.x,
                 "explicit array release increments free calls")
    t |> success(heap_ops_after.y > heap_ops_before.y, "array growth increments reallocations")
    t |> equal(heap_operation_counts(true), string_ops_before,
        "value-heap operations do not enter string-heap counters")

    let collections_before = gc_collection_stats()
    let reclaimed_before = gc_reclaimed_stats()
    let tick_before = gc_last_collection_tick()
    make_api_garbage()
    unsafe(heap_collect(true, false))

    let collections_after = gc_collection_stats()
    let pause_after = gc_pause_stats()
    let reclaimed_after = gc_reclaimed_stats()
    let tick_after = gc_last_collection_tick()
    t |> equal(collections_after.x, collections_before.x + 1ul,
        "collection count observes the completed sweep")
    t |> success(collections_after.y >= collections_before.y,
        "cumulative collection time is monotonic")
    t |> success(pause_after.y >= pause_after.x,
        "peak collection pause is at least the last pause")
    t |> success(reclaimed_after.x > reclaimed_before.x,
        "reclaimed heap bytes observe unreachable payloads")
    t |> success(reclaimed_after.y >= reclaimed_before.y,
        "reclaimed string bytes are monotonic")
    t |> success(tick_after > 0l && tick_after >= tick_before,
        "last collection tick observes the completed sweep")
    t |> success(!unsafe(heap_collect_if_needed()),
        "fresh baselines and large budgets decline an immediate repeat")
    set_gc_allocation_budget(original_budget.x, original_budget.y)
}
