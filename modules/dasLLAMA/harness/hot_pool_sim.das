options gen2
options stack = 524288

// The hot expert pool's offline instrument: a routed pick trace (`DASLLAMA_MOE_TRACE`, the host expert steps of a serving run -
// a layer, its rows, k expert ids and weights a row, a mark at each prompt that starts at position 0) replayed through a pool
// policy, per layer, at a slot count; the hit share of the decode picks, the placements, and a token wall modeled from the measured
// costs. A policy that wins here earns the live run; the live run's per-layer profile (DASLLAMA_GPU_PROF) is its cross-check.
//
//   daslang -jit modules/dasLLAMA/harness/hot_pool_sim.das -- <trace.moet> [--slots 32] [--policy heat|lru|lfu] [--decay 256]
//       [--hyst 2.0] [--swaps 1] [--fills 2] [--window-swaps 8] [--device-us 22000] [--miss-us 72] [--place-us 840] [--reset-per-seq]
//       [--async [--place-host-us 570] [--place-link-us 172]] [--per-layer]

require daslib/clargs
require daslib/fio
require math
require strings

let WINDOW_MIN_ROWS = 32   //! the engine's RDEC_HOT_WINDOW_MIN: a record under it is a decode step or a verify's rows, at it and past it a window

[CommandLineArgs]
struct Args {
    @clarg_positional
    @clarg_doc = "The trace file DASLLAMA_MOE_TRACE wrote"
    trace : string

    @clarg_doc = "Pool slots a routed layer"
    slots : int = 32

    @clarg_doc = "The replacement policy: heat (the shipped one: decaying pick counts, a swap under hysteresis), lru (the least recently picked slot leaves, every miss placed within the budget), lfu (pick counts with no decay)"
    policy : string = "heat"

    @clarg_doc = "heat: the routed rows between halvings of the pick counts"
    decay : int = 256

    @clarg_doc = "heat / lfu: a candidate swaps in only past this multiple of the victim's count, plus one"
    hyst : float = 2.0

    @clarg_doc = "Experts a decode step may swap into a full pool, a layer"
    swaps : int = 1

    @clarg_doc = "Experts a decode step may fill into free slots, a layer"
    fills : int = 2

    @clarg_doc = "Experts a window's step may swap into a full pool, a layer"
    window_swaps : int = 8

    @clarg_doc = "The token's device wall, us (the zen2's 5060 Ti: 22000 a plain token)"
    device_us : int = 22000

    @clarg_doc = "A miss's host expert sum, us (72 on the zen2)"
    miss_us : int = 72

    @clarg_doc = "A placement's gather + upload, us (840 on the zen2), on the token's path"
    place_us : int = 840

    @clarg_doc = "The async placement model instead: a placement costs `place-host-us` on the token's path (the host's layout gather; 0 with a device-layout mirror of the experts in host memory) and `place-link-us` of link time that overlaps the device's wall - the token is the longer of the compute path and the link's queue"
    async : bool = false

    @clarg_doc = "async: a placement's host work on the token's path, us (570 the layout gather; 0 with a device-layout mirror)"
    place_host_us : int = 570

    @clarg_doc = "async: a placement's link time, us (2.25 MB over PCIe 4.0 x8 at 13 GB/s: 172)"
    place_link_us : int = 172

    @clarg_doc = "Empty the pool at every sequence mark (a fresh process a prompt); default: the pool persists as the serving process does"
    reset_per_seq : bool = false

    @clarg_doc = "Print every layer's hit share"
    per_layer : bool = false
}

struct Rec {
    layer : int
    rows : int
    ids : array<int>
    w : array<float>
}

struct Trace {
    layers : int
    ne : int
    k : int
    recs : array<Rec>
}

def read_trace(path : string) : Trace {
    var tr : Trace
    var bytes : array<uint8>
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("hot_pool_sim: cannot open {path}")
        }
        bytes |> resize(int64(fstat(f).size))
        fread(f, bytes)
    }
    let n = long_length(bytes)
    unsafe {
        let base = addr<uint8?>(bytes[0])
        let magic = *reinterpret<int?>(base)
        if (magic != 0x54454f4d) {
            panic("hot_pool_sim: {path} is not a MOET trace")
        }
        tr.layers = *reinterpret<int?>(base + 4l)
        tr.ne = *reinterpret<int?>(base + 8l)
        tr.k = *reinterpret<int?>(base + 12l)
        var off = 16l
        while (off + 8l <= n) {
            var r = Rec(layer = *reinterpret<int?>(base + off), rows = *reinterpret<int?>(base + off + 4l))
            off += 8l
            let cnt = int64(r.rows * tr.k)
            if (cnt > 0l) {
                break if (off + cnt * 8l > n)   // a flush the process never finished
                r.ids |> resize(cnt)
                r.w |> resize(cnt)
                memcpy(addr<void?>(r.ids[0]), reinterpret<void const?>(base + off), cnt * 4l)
                off += cnt * 4l
                memcpy(addr<void?>(r.w[0]), reinterpret<void const?>(base + off), cnt * 4l)
                off += cnt * 4l
            }
            tr.recs |> emplace(r)
        }
    }
    return <- tr
}

struct Layer {
    slot_of : array<int>     //! [ne] the expert's slot, -1 = host
    expert_of : array<int>   //! [slots] the slot's expert, -1 = free
    heat : array<float>      //! [ne] the policy's score: decaying counts (heat), counts (lfu), the last pick's tick (lru)
    rows : int
    nfree : int
    hits, picks, places, window_rows, window_hits : int64
}

var g_tick = 0
var g_cfg = Args()

def layer_init(var L : Layer; ne, slots : int) {
    L.slot_of |> resize(ne)
    L.expert_of |> resize(slots)
    L.heat |> resize(ne)
    for (i in range(ne)) {
        L.slot_of[i] = -1
        L.heat[i] = 0.0
    }
    for (i in range(slots)) {
        L.expert_of[i] = -1
    }
    L.nfree = slots
    L.rows = 0
}

def note(var L : Layer; ids : array<int>; n, rows : int) {
    g_tick++
    for (j in range(n)) {
        let e = ids[j]
        if (g_cfg.policy == "lru") {
            L.heat[e] = float(g_tick)
        } else {
            L.heat[e]++
        }
    }
    L.rows += rows
    if (g_cfg.policy == "heat" && L.rows >= g_cfg.decay) {
        L.rows = 0
        for (h in L.heat) {
            h *= 0.5
        }
    }
}

//! the slot expert `e` may take: a free one, else the coldest slot not among the `n` protected picks, when the policy admits the swap
def slot_for(L : Layer; e : int; ids : array<int>; n : int) : int {
    if (L.nfree > 0) {
        return find_index(L.expert_of, -1)
    }
    var victim = -1
    for (sl in range(length(L.expert_of))) {
        let ex = L.expert_of[sl]
        var picked = false
        for (j in range(n)) {
            picked = picked || ids[j] == ex
        }
        continue if (picked)
        if (victim < 0 || L.heat[ex] < L.heat[L.expert_of[victim]]) {
            victim = sl
        }
    }
    return -1 if (victim < 0)
    if (g_cfg.policy == "lru") {
        return victim
    }
    return L.heat[e] > g_cfg.hyst * L.heat[L.expert_of[victim]] + 1.0 ? victim : -1
}

def place(var L : Layer; e, slot : int) {
    let old = L.expert_of[slot]
    if (old >= 0) {
        L.slot_of[old] = -1
    } else {
        L.nfree--
    }
    L.expert_of[slot] = e
    L.slot_of[e] = slot
    L.places++
}

//! a decode step's picks: noted, placed under the step budgets, then counted as hits or misses
def decode_step(var L : Layer; ids : array<int>; k : int) {
    note(L, ids, k, 1)
    var budget = L.nfree > 0 ? g_cfg.fills : g_cfg.swaps
    for (j in range(k)) {
        break if (budget == 0)
        let e = ids[j]
        continue if (L.slot_of[e] >= 0)
        let slot = slot_for(L, e, ids, k)
        continue if (slot < 0)
        place(L, e, slot)
        budget--
    }
    for (j in range(k)) {
        L.hits += L.slot_of[ids[j]] >= 0 ? 1l : 0l
    }
    L.picks += int64(k)
}

def window(var L : Layer; ids : array<int>; n, rows : int) {
    note(L, ids, n, rows)
    var order <- [for (e in range(length(L.heat))); e; where L.heat[e] > 0.0 && L.slot_of[e] < 0]
    order |> sort() $(a, b) => L.heat[a] > L.heat[b]
    var swaps = g_cfg.window_swaps
    for (e in order) {
        break if (L.nfree == 0 && swaps == 0)
        let filling = L.nfree > 0
        let none : array<int>
        let slot = slot_for(L, e, none, 0)
        break if (slot < 0)
        place(L, e, slot)
        if (!filling) {
            swaps--
        }
    }
    for (j in range(n)) {
        L.window_hits += L.slot_of[ids[j]] >= 0 ? 1l : 0l
    }
    L.window_rows += int64(n)
}

[export]
def main : int {
    var cfg = Args()
    let rc = parse_args_with_help(cfg, "hot_pool_sim")
    return rc if (rc >= 0)
    g_cfg = cfg
    var tr <- read_trace(cfg.trace)
    var layers : array<Layer>
    layers |> resize(tr.layers)
    for (L in layers) {
        layer_init(L, tr.ne, cfg.slots)
    }
    var seqs = 0
    var tokens = 0l
    for (r in tr.recs) {
        if (r.layer < 0) {
            seqs++
            if (cfg.reset_per_seq) {
                for (L in layers) {
                    layer_init(L, tr.ne, cfg.slots)
                }
            }
            continue
        }
        if (r.rows < WINDOW_MIN_ROWS) {   // a decode step, or a verify's few rows: the step form a row (the window chain takes 32 rows and more)
            for (row in range(r.rows)) {
                let ids <- [for (j in range(tr.k)); r.ids[row * tr.k + j]]
                decode_step(layers[r.layer], ids, tr.k)
                tokens += r.layer == 0 ? 1l : 0l
            }
        } else {
            window(layers[r.layer], r.ids, r.rows * tr.k, r.rows)
        }
    }
    var hits = 0l
    var picks = 0l
    var places = 0l
    var whits = 0l
    var wrows = 0l
    for (L in layers) {
        hits += L.hits
        picks += L.picks
        places += L.places
        whits += L.window_hits
        wrows += L.window_rows
    }
    let misses = picks - hits
    let steps = max(tokens, 1l)
    var per_token_us = double(cfg.device_us) + double(misses) * double(cfg.miss_us) / double(steps) + double(places) * double(cfg.place_us) / double(steps)
    var link_us = 0.0lf
    if (cfg.async) {
        let path_us = double(cfg.device_us) + double(misses) * double(cfg.miss_us) / double(steps) + double(places) * double(cfg.place_host_us) / double(steps)
        link_us = double(places) * double(cfg.place_link_us) / double(steps)
        per_token_us = max(path_us, link_us)
    }
    print("hot_pool_sim {cfg.trace}: {tr.layers} routed layers, {tr.ne} experts, k {tr.k}, {seqs} sequences, {tokens} decode tokens, {length(tr.recs)} records\n")
    print("  policy {cfg.policy} slots {cfg.slots} decay {cfg.decay} hyst {cfg.hyst} swaps {cfg.swaps} fills {cfg.fills} window-swaps {cfg.window_swaps}\n")
    print("  decode picks on the device: {hits} of {picks} ({double(hits) * 100.0lf / double(max(picks, 1l)):.1f}%), {places} placements ({double(places) / double(steps):.2f} a token); window rows {whits} of {wrows} ({double(whits) * 100.0lf / double(max(wrows, 1l)):.1f}%)\n")
    if (cfg.async) {
        print("  modeled token (async placements): {per_token_us / 1000.0lf:.1f} ms = max(device {double(cfg.device_us) / 1000.0lf:.1f} + misses {double(misses) * double(cfg.miss_us) / double(steps) / 1000.0lf:.1f} + placements' host {double(places) * double(cfg.place_host_us) / double(steps) / 1000.0lf:.1f}, link {link_us / 1000.0lf:.1f}) -> {1.0e6lf / per_token_us:.1f} tok/s\n")
    } else {
        print("  modeled token: {per_token_us / 1000.0lf:.1f} ms = device {double(cfg.device_us) / 1000.0lf:.1f} + misses {double(misses) * double(cfg.miss_us) / double(steps) / 1000.0lf:.1f} + placements {double(places) * double(cfg.place_us) / double(steps) / 1000.0lf:.1f} -> {1.0e6lf / per_token_us:.1f} tok/s\n")
    }
    if (cfg.per_layer) {
        let line = build_string() $(var w) {
            for (l in range(length(layers))) {
                let pct = layers[l].picks > 0l ? int(double(layers[l].hits) * 100.0lf / double(layers[l].picks)) : 0
                w |> write("{l}:{pct}% ")
            }
        }
        print("  per layer: {line}\n")
    }
    return 0
}
