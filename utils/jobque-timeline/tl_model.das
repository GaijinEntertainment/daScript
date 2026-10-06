options gen2
options indenting = 4

module tl_model shared

// Event kinds — traceSave's tagName[] order (job_que.cpp), same ints trace_window.py uses
let public EV_PUBLISH = 0
let public EV_CHUNK = 1
let public EV_STAGE_WAIT = 2
let public EV_WAKE = 3
let public EV_FIFO = 4

// meta packing: kind(3b) | stage(6b) | nstages(6b) | cat(8b)
def public ev_meta(kind, stage, nstages, cat : int) : uint =>
    uint(kind & 7) | (uint(stage & 63) << 3u) | (uint(nstages & 63) << 9u) | (uint(cat & 255) << 15u)

def public ev_kind(meta : uint) : int => int(meta & 7u)
def public ev_stage(meta : uint) : int => int((meta >> 3u) & 63u)
def public ev_nstages(meta : uint) : int => int((meta >> 9u) & 63u)
def public ev_cat(meta : uint) : int => int((meta >> 15u) & 255u)

struct public Ev {
    t0 : double      // µs from file start (parsed as integer ns — exact)
    dur : float      // µs
    meta : uint
    arg : uint
    chain : uint
}

struct public Bucket {
    busy : float             // work fraction 0..1 (chunk+fifo+publish)
    wait : float             // stage_wait fraction
    dom : int                // dominant category by busy time; -1 = empty
    count : int              // events touching the bucket
}

struct public MipLevel {
    nb : int
    b : array<Bucket>
}

struct public Lane {
    name : string            // "worker N" / "caller N" from the thread_name metadata
    ev : array<Ev>           // sorted by t0 at load (ring order is completion order)
    max_dur : double         // widens visible-range binary search for overlapping events
    mips : array<MipLevel>   // [0] = coarsest (overview source), 4x finer while useful
}

struct public Category {
    tag : int                // the raw trace tag this category labels (0 = untagged/heuristic)
    name : string
    col : uint               // 0xRRGGBB
}

struct public MarkerKind {
    name : string            // e.g. "token", "layer"
    ts : array<double>       // sorted µs; unit i spans [ts[i], ts[i+1])
    args : array<int>
    lane : array<int8>
}

struct public TraceDoc {
    path : string
    title : string
    span : double            // µs
    lanes : array<Lane>
    cats : array<Category>   // index == ev_cat(meta); [0..3] are the heuristic base classes
    marker_kinds : array<MarkerKind>
    has_profile : bool       // jobqueProfile header present
    total_events : int64
    generation : int
}

// base classes for untagged chains (stage-count heuristic, same as the HTML viewer):
// 0 std/amber, 1 attn/blue, 2 ffn/green, 3 cls/purple — registered tags append after these
let public CAT_STD = 0
let public CAT_ATTN = 1
let public CAT_FFN = 2
let public CAT_CLS = 3

def public base_categories() : array<Category> {
    return <- [
        Category(tag = 0, name = "std kernel", col = 0xC39536u),
        Category(tag = 0, name = "attn chain", col = 0x5E96DEu),
        Category(tag = 0, name = "ffn chain", col = 0x43B08Au),
        Category(tag = 0, name = "classifier", col = 0xAC82E4u)
    ]
}

// legacy palette for tag-carrying traces saved without a jobqueProfile header (tags 1..11)
def public legacy_tag_category(tag : int) : Category {
    if (tag == 1) return Category(tag = tag, name = "attn chain", col = 0x5E96DEu)
    if (tag == 2) return Category(tag = tag, name = "attn std", col = 0x5E96DEu)
    if (tag == 3) return Category(tag = tag, name = "ffn chain", col = 0x43B08Au)
    if (tag == 4) return Category(tag = tag, name = "ffn per-op", col = 0x43B08Au)
    if (tag == 5) return Category(tag = tag, name = "classifier", col = 0xAC82E4u)
    if (tag == 6) return Category(tag = tag, name = "moe experts", col = 0xE08563u)
    if (tag == 7) return Category(tag = tag, name = "shexp", col = 0xC39536u)
    if (tag == 8) return Category(tag = tag, name = "ple", col = 0xC39536u)
    if (tag == 9) return Category(tag = tag, name = "router", col = 0x4FC3D0u)
    if (tag == 10) return Category(tag = tag, name = "park/reduce", col = 0xD678B4u)
    if (tag == 11) return Category(tag = tag, name = "deltanet", col = 0x8385E8u)
    return Category(tag = tag, name = "tag {tag}", col = 0xC39536u)
}

def public lane_lower_bound(lane : Lane; t : double) : int {
    var lo = 0
    var hi = length(lane.ev)
    while (lo < hi) {
        let mid = (lo + hi) / 2
        if (lane.ev[mid].t0 < t) {
            lo = mid + 1
        } else {
            hi = mid
        }
    }
    return lo
}

// heuristic class for an untagged chain — nstages>=3 attn, 2 ffn, 1 cls, else std
def public heuristic_cat(nstages : int) : int {
    if (nstages >= 3) return CAT_ATTN
    if (nstages == 2) return CAT_FFN
    if (nstages == 1) return CAT_CLS
    return CAT_STD
}
