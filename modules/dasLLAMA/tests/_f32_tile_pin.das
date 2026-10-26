// The fp32 batch tile's permutation grid as callable clones, and the pin that serves the fp32 batch
// GEMM through the tile's declared fallback instead of the stamp this box's sidecar crowned - a
// parity bar measured at one summation order then holds on every box, whatever winner it minted.

options gen2
options _dasllama_internal = true

module _f32_tile_pin shared public

require dasllama/dasllama_lint public
require math   // mad: the cloned tile bodies call it
require dastest/testing_boost
require daslib/rtti   // this_context: the panic's text re-raised once the pin is put back
require dasllama/dasllama_math
require dasllama/dasllama_par   // maybe_parallel_for
require dasllama/dasllama_tune   // [dasllama_grid], [dasllama_fallbacks]

[dasllama_grid(src = "f32_batch_tile_template")]
struct private GridF32BatchTile {}

[dasllama_fallbacks]
struct private TileFallbacks {}

typedef F32TileFn = function<(var yp : float?; wp : float const?; xp : float const?; n : int64; d : int64) : void>

//! the permutation `f32_batch_tile` stamps on a box with no sidecar entry, as this box resolves its chain
def f32_tile_reference_perm() : string => dasllama_tuned_fallbacks()?["f32_batch_tile"] ?? ""

//! the grid's clone named `perm`, null when the grid has no such row
def f32_tile_variant(perm : string) : F32TileFn {
    for (v in f32_batch_tile_template_variants()) {
        return v._1 if (v._0 == perm)
    }
    return default<F32TileFn>
}

//! every 4x4 tile of rows [0, d - d % 4) by tokens [t0, t1) through `tile`; t0 and t1 sit on the tile lattice
def f32_tile_lattice(tile : F32TileFn; var yp : float?; wp, xp : float const?; n, d, t0, t1 : int64) {
    let d4 = d - d % 4l
    unsafe {
        for (tk in range64(t0 / 4l, t1 / 4l)) {
            for (i in range64(d4 / 4l)) {
                invoke(tile, yp + tk * 4l * d + i * 4l, wp + i * 4l * n, xp + tk * 4l * n, n, d)
            }
        }
    }
}

var private g_pinned_tile : F32TileFn
var private g_pinned_calls = 0

//! the fp32 batch GEMMs the pin has served in this process
def f32_tile_pin_calls() : int => g_pinned_calls

// The production walk's outputs at the pinned stamp: an output's bits depend only on its own row and
// token - every tile output on the 4x4 lattice from row 0 and token 0, every other one `dot` - so the
// lattice plus `batch_dot_rows` over the rest lands the walk's bits whatever chunking either uses.
def private pinned_f32_batch(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) : bool {
    let tile = g_pinned_tile
    g_pinned_calls++
    let d4 = d - d % 4l
    let t4 = ntok - ntok % 4l
    var myp = yp
    maybe_parallel_for(0, int(t4 / 4l), lanes_for_work(n * d * ntok, 0)) $(tb, te) {
        f32_tile_lattice(tile, myp, wp, xp, n, d, int64(tb) * 4l, int64(te) * 4l)
        batch_dot_rows(myp, wp, xp, n, d, d4, d, int64(tb) * 4l, int64(te) * 4l)
    }
    batch_dot_rows(yp, wp, xp, n, d, 0l, d, t4, ntok)
    return true
}

//! `blk` with every fp32 batch GEMM (`matmul_batch`, every form) served through the tile's reference
//! permutation at every dot length and token count, through the engine's f32 tile pin - its own seat, ahead
//! of the float-batch override an accelerate lane arms - the pin found put back however `blk` leaves, a panic
//! included. Only the f32 tile is pinned: the bf16 tile, the `[tune]` GEMMs and every other kernel keep their stamps.
def with_reference_f32_tile(t : T?; blk : block<() : void>) {
    let perm = f32_tile_reference_perm()
    let tile = f32_tile_variant(perm)
    if (tile == null) {
        t |> failure("the reference f32 tile pin: perm '{perm}' has no grid clone")
        return
    }
    to_log(LOG_INFO, "the fp32 batch GEMM pinned to f32_batch_tile's reference permutation {perm}\n")
    let tile_was = g_pinned_tile
    g_pinned_tile = tile
    let pin_was = set_f32_tile_pin(@@pinned_f32_batch)
    var raised = false
    var why = ""
    try {
        invoke(blk)
    } recover {
        raised = true
        why = this_context().last_exception
    }
    set_f32_tile_pin(pin_was)
    g_pinned_tile = tile_was
    if (raised) {
        panic(why)
    }
}
