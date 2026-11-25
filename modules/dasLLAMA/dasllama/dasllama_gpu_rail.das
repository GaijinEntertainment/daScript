options gen2
options indenting = 4
options _dasllama_internal = true

//! The tower rail's home-free vocabulary: the device planes a tower chain names by role, a plane's seat on a rail
//! (`RailBuf`), the GEMM sites and the planes each site binds (`RailSite`, `RailSiteBind`), the weight lane a chain rides
//! (`RailLane`) and a rail's begin, the profile roles a chain marks, the reasons a tower seat declines (`TowerDecline`), and the
//! encoder rows' device handoffs (`TowerRowsHandoff`, `TowerEncOut`). A chain body in `dasllama_tower_chains` dispatches a stamp as
//! `enc_<stamp>_cls(rail, roles, pc, wgs)` - the overload each home's dispatch lens generates beside its raw form - and
//! the rail turns the role list into the home's binding tuple (`vkd_rail_dispatch` / `cud_rail_launch`).
module dasllama_gpu_rail shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/strings_boost   // split: the pricing rail's fragments
require daslib/enum_trait
require dasllama/dasllama_kqformat
require dasllama/dasllama_env   // g_env_gpu: the profile ledger's knob

//! the device planes a tower chain names by role, grouped by what the plane holds: the block rows and their feeds, the attention
//! panels, the FFN rows, the stems' and tails' rows, the weight planes and the schedule; the fronts' stages; the rel-position
//! tables; the kq lane's member planes; the decoders' caches and state; the heads' rows
enum TowerBuf {
    x           //! the residual rows [cap x d]
    xb          //! the normed rows the GEMMs read, the post-norm's landing
    xc          //! the exact lane's clamped GEMM input (gemma4v)
    xh          //! the f16 feed [cap + slack x max(d, ff)]
    xq          //! the Q8_0 feed's quants and scales (the mul_mm lane, the q8 GEMV)
    xs
    q
    k
    v
    q2          //! the head-normed q / k / v (gemma4v)
    k2
    v2
    k16         //! the halfword k / v panels the flash tile reads
    v16
    qp          //! the padded attention panels
    kp16
    vp16
    attp
    att
    yo
    hg          //! the FFN's gate rows, a single-GEMM FFN's hidden rows
    hu
    yd
    tabs        //! the packed rope tables (gemma4v)
    qkv         //! the fused [q | k | v] rows (qwen3v, the whisper decoder)
    cos         //! the per-token rope tables (qwen3v, qwen25v)
    sin
    sk          //! the split-k partial planes
    planes      //! a stem's input: the canvas's pixel planes, a front's samples or mel rows, the VAD's context and chunks
    col         //! a stem's or a front stage's im2col rows
    win_sort    //! qwen25v's window sort: a stem's row map
    pos_resized //! qwen3v's position rows resized to the encode's grid
    win_table   //! the windows' row starts (qwen25v)
    tail_out    //! the tail rows at the tail's widest width, the projected rows
    tail_proj
    norms       //! the f32 blob as stored, the ones row behind it
    wb          //! the halfword plane as stored (bf16 or f16 rows)
    wq          //! the q8 lane's quants and halfword scales
    ws
    meta        //! the encode's schedule: the GEMM records' maps and the GEMV records' region pairs, each site's past its offset
    spec        //! a front's spectrum or framed-window rows
    frames      //! a front's windowed frames cut on the device off the samples in `planes` (gigaam)
    mel         //! a front's log-mel rows
    stage1      //! a front's conv stages' rows, in stage order
    stage2
    stage3
    stage4
    stage5
    feat       //! a front's shuffled feature rows, its projection's input
    feat16      //! a front GEMM's own f16 feed
    rows_out    //! an encoder's output rows: the stem's rows the blocks read, a front's rows, the rows a decoder reads
    fw16        //! a front's halfword conv weight slab
    fnorm       //! a front's f32 slab: the conv biases and the position rows
    twiddle     //! a front's DFT twiddles
    filters     //! a front's mel filterbank
    rel_tab     //! the rel-position table's f32 rows, its Q8_0 feed, its f16 feed, its projected rows
    rel_tabq
    rel_tabs
    rel_tabh
    rel_proj
    rel_qh      //! the rel-plane attention's (q + bias) and projected-table head panels and its score plane (canary)
    rel_ph
    rel_r
    taps        //! the depthwise taps as the family's own halfwords (gigaam)
    kq_q0       //! the kq lane's member planes, quants then scales, a pair a K-quant member of the carrier
    kq_s0
    kq_q1
    kq_s1
    kq_q2
    kq_s2
    kq_q3
    kq_s3
    tail_hid    //! a projector tail's hidden rows and gate rows
    tail_gate
    vad_gates   //! the VAD's input gates, its [h | c] state, its probabilities
    vad_state
    vad_probs
    dec_in      //! a decoder's embedded input rows
    kc          //! the whisper decoder's self K / V halfword caches and its cross K / V rows
    vc
    kxr
    vxr
    cq          //! the decoder's cross-attention query rows
    pms         //! the chunked attention's partial max and sum, and its partial rows
    py
    enc_h       //! the encoder rows' f16 feed and Q8_0 feed (a decoder's cross site), and their projected rows
    enc_q
    enc_s
    enc_proj
    pred_w16    //! the transducer's prediction-net and joint halfword weights, its f32 rows, the encoder projection's f32 weights
    joint_w16
    net_rows
    enc_proj_w
    dec_state   //! the transducer's state words, its LSTM h and c, the prediction net's input and output rows
    lstm_h
    lstm_c
    pred_in
    pred_out
    head_w      //! a head's weights and bias (the CTC head)
    head_b
    logits      //! a classifier's logit rows (the whisper decoder's, the joint's, the CTC head's)
    picks       //! the records a head lands for the host (the transducer's tokens, the CTC picks)
}

let TOWER_BUFS = typeinfo enum_length(type<TowerBuf>)   //! the rail's plane table width

//! a plane's seat on a rail: its device handle, the byte range a set binds, its Vulkan hazard bit (0 on a weight plane and on
//! every CUDA plane) and the byte its binding starts at in the buffer (0 but where a primitive narrows the seat to one record's
//! range - a GEMV record's region pairs in the schedule)
struct RailBuf {
    dev : uint64
    bytes : int64
    bit : uint
    off : int64
}

//! the seat of the plane at `role` on a rail of either home
def rail_buf(rl : auto(R); role : TowerBuf) : RailBuf => rl.bufs[int(role)]

//! One conv dispatch as a home's own conv tile takes it (`rail_tts_conv`): the layer's geometry, the rows of its call, where
//! its weight rows and bias row sit in the slab (elements), and - a stored leaf - its format and its place in the home's planes
struct TtsConvSite {
    cin : int64
    k : int64
    stride : int64
    pad_l : int64      //! the left pad of this call (a stream's first window carries its own)
    dilation : int64
    transposed : bool
    half : bool        //! the chain takes f16-staged operands at this site
    tin : int64
    tout : int64
    cout : int64
    cout_p : int64     //! the outputs on the tile's lattice
    kdim_p : int64     //! a weight row's stride
    woff : int64       //! the weight rows' element in the slab (a leaf has none)
    boff : int64       //! the bias row's element in the slab
    stored : bool      //! the weights are a stored leaf of the home's planes ...
    native : bool      //! ... a quant leaf in the home's own layout: only the home's own tile reads it
    halves : bool      //! ... half floats ...
    fmt : KqFmt        //! ... or this quant format ...
    qoff : int64       //! ... at this place of its quant plane, its scale rows at `soff` and a split format's d tail at `doff` of the scale plane: the home's own units
    soff : int64
    doff : int64
}

//! One dense GEMM as a home's own tile takes it (`rail_tts_mm`): y [rows][nout] at element `yoff` = x [rows][nin] at
//! element `xoff` against the layer's weights - the slab's rows, or a stored leaf in the home's planes - its bias row added under `biased`
struct TtsMmSite {
    nin : int64
    nout : int64
    rows : int64
    xoff : int64
    yoff : int64
    biased : bool
    cout_p : int64     //! the outputs on the tile's lattice: the weight rows the slab or the leaf holds
    kdim_p : int64     //! a weight row's stride
    woff : int64       //! the weight rows' element in the slab (a leaf has none)
    boff : int64       //! the bias row's element in the slab
    stored : bool
    native : bool      //! a quant leaf in the home's own layout: only the home's own tile reads it
    halves : bool
    fmt : KqFmt
    qoff : int64
    soff : int64
    doff : int64
}

let TTS_PK_GEMV_DOT = 0          //! the Pocket frame's row GEMV by its stamp's kind: the plain dot ...
let TTS_PK_GEMV_LN_SILU = 1
let TTS_PK_GEMV_GATE = 2
let TTS_PK_GEMV_ADD_SILU = 3
let TTS_PK_GEMV_TAIL = 4
let TTS_PK_GEMV_LN_GELU = 5      //! the frame loop's ffn up: the normed input, the GELU out
let TTS_PK_GEMV_ADD_SCALE = 6    //! ... and its residual joins: y += d times the slab's scale row at aoff
let TTS_PK_GEMV_ADD = 7          //! ... or y += d alone
let TTS_PK_GEMV_LN_QKV = 8       //! ... and its qkv row: normed, the q span roped into y, the k and v spans into the caches' row
let TTS_PK_ST_HALVES = -1        //! `TtsPkStSite.fmt` of a leaf held as half floats

//! One stored row GEMV (`TtsPkGemvStoredT`): its stamp's kind (a `TTS_PK_GEMV_*`), the leaf's format - a `KqFmt` slot, or
//! `TTS_PK_ST_HALVES` - and the planes a kind's stamp seats: the slab's rows, x and y, then the kind's own, then the leaf's plane pair
struct TtsPkStSite {
    kind : int
    fmt : int
    rows : RailBuf
    x : RailBuf
    y : RailBuf
    mod : RailBuf    //! the modulation rows: the normed-modulated input, the gate, the tail
    x0 : RailBuf     //! the tail's noise row ...
    lat : RailBuf    //! ... and latent row
    tabs : RailBuf   //! the qkv row's rope tables ...
    kc : RailBuf     //! ... and the caches' rows
    vc : RailBuf
    wq : RailBuf
    ws : RailBuf
}

//! the site hooks as a home with no tile of its own answers them, none taken; a home's own overloads outrank these
[unused_argument(rl, rows, wq, ws, site, x, y)]
def rail_tts_conv(var rl : auto(R); rows, wq, ws : RailBuf; site : TtsConvSite; x, y : RailBuf) : bool => false

[unused_argument(rl, rows, wq, ws, site, x, y)]
def rail_tts_mm(var rl : auto(R); rows, wq, ws : RailBuf; site : TtsMmSite; x, y : RailBuf) : bool => false

//! ... a stored row GEMV over a leaf in a home's own layout (`args` the kernel's argument record)
[unused_argument(rl, s, args, nout)]
def rail_tts_pk_own(var rl : auto(R); s : TtsPkStSite; args; nout : int64) : bool => false

[unused_argument(rl, x, stats, t, c)]
def rail_tts_colstats(var rl : auto(R); x, stats : RailBuf; t, c : int64) : bool => false

[unused_argument(rl, g, wq, y, qoff, t, hidden, backward)]
def rail_tts_lstm_q8(var rl : auto(R); g, wq, y : RailBuf; qoff, t, hidden : int64; backward : bool) : bool => false

//! ... and the classes its site hooks dispatch: none
[unused_argument(rl)]
def rail_tts_sites_ensure(rl : auto(R)) : bool => true

//! The device hooks of a home with no class-build wall, whole-buffer binds and no readback buffer
[unused_argument(rl)]
def rail_tts_build_us(rl : auto(R)) : int => 0

[unused_argument(rl, bytes)]
def rail_tts_bind_ok(rl : auto(R); bytes : int64) : bool => true

[unused_argument(rl)]
def rail_tts_bind_cap(rl : auto(R)) : int64 => LONG_MAX

[unused_argument(rl, bytes)]
def rail_tts_take_ready(rl : auto(R); bytes : int64) : bool => true

[unused_argument(rl)]
def rail_tts_take_bytes(rl : auto(R)) : int64 => LONG_MAX

[unused_argument(rl)]
def rail_tts_take_drop(rl : auto(R)) {}

//! a small plane the host writes in place and the device reads at once: a chain's schedule records, written as its dispatches are recorded
struct RailMapped {
    host : void?
    dev : uint64
    mem : uint64    //! the home's own handle behind `dev`, where it keeps one
    bytes : int64
}

//! `bufs` seated at the rail's first roles, in order, and those roles: the planes of one dispatch, for a chain whose planes are the call's own rows
def rail_seats(var rl : auto(R); bufs) {
    var roles : TowerBuf[typeinfo dim(bufs)]
    for (i in range(typeinfo dim(bufs))) {
        rl.bufs[i] = bufs[i]
        roles[i] = unsafe(reinterpret<TowerBuf>(i))
    }
    return roles
}

//! the plane at `role` seated on a rail of either home: its handle, the bytes a binding covers, its hazard bit (0 on CUDA)
def rail_seat(var rl : auto(R); role : TowerBuf; dev : uint64; bytes : int64; bit : uint) {
    rl.bufs[int(role)] = RailBuf(dev = dev, bytes = bytes, bit = bit)
}

//! why a discrete home's tower seat leaves an encode on the CPU chain; the decline counters key on the value names
enum TowerDecline {
    knob         //! the home's tower knob is off
    quant_mode   //! the tower's planes are not the lane the driver serves
    kq_lane      //! a kq lane member whose K-quant tile class did not compile on the device - every superblock format has one
    flavor       //! the model is not the home's flavored image: each backend serves its own (`#tower-dev-block`)
    shape        //! a width, head size, row count, window or buffer range the stamps do not serve
    device       //! the home has no device, or a class failed to build
    memory       //! a device or pinned allocation failed
}

//! a tower decline in words: `home` the home's name in its image flavor, `knob`, `device` and `memory` the home's own spelling of those reasons
[cold_path]
def tower_decline_words(why : TowerDecline; home, knob, device, memory : string) : string {
    if (why == TowerDecline.knob) {
        return knob
    } elif (why == TowerDecline.quant_mode) {
        return "the tower's planes are not the lane the driver serves"
    } elif (why == TowerDecline.kq_lane) {
        return "a K-quant member of the carrier's own block GEMM planes has no tile class compiled on this device's f16 feed"
    } elif (why == TowerDecline.flavor) {
        return "the model is not the {home}-flavored image - each backend serves its own"
    } elif (why == TowerDecline.shape) {
        return "a width, head size, row count, window or buffer range the stamps do not serve"
    } elif (why == TowerDecline.memory) {
        return memory
    }
    return device
}

//! the weight lane a tower chain rides: the q8 tiles over the sites' schedule records, the exact f32 tile over the norms plane
//! (no feed), or the f16 GEMM over the halfword plane `wb` off each site's f16 feed
enum RailLane {
    q8
    f32
    f16
}

//! the lane of a family whose second lane is the f16 one: `RailLane.f16` under `f16`, else the q8 lane
def rail_lane_of(f16 : bool) : RailLane => f16 ? RailLane.f16 : RailLane.q8

//! a GEMM site on the rail - its tile pick (each home's `sites`) and the planes it binds (`binds`): the block sites (the d-wide, the
//! ff-wide, the fused [q | k | v]-wide and the fc2 site, whose pick may split k over the `sk` planes), the conv module's 2d-wide
//! pointwise GEMM, the rel table's projection, a projector tail's two GEMMs, the stem's second conv, a front's GEMM stages, a
//! decoder's cross K / V site over the encoder rows, and the q8 GEMV records
enum RailSite {
    d
    ff
    qkv
    fc2
    d2
    rel
    tail1
    tail2
    conv2
    front1
    front2
    front3
    cross
    gemv
}

let RAIL_SITES = typeinfo enum_length(type<RailSite>)   //! the rail's site table width

//! the planes a GEMM site binds by role - the weight quants and scales, the schedule and the byte its records' maps count from, the
//! f16 feed (the cm2 tiles) and the Q8_0 feed (the mul_mm tiles, the GEMV) - and the weight format the site's tile decodes
struct RailSiteBind {
    wq : TowerBuf
    ws : TowerBuf
    meta : TowerBuf
    xh : TowerBuf
    xq : TowerBuf
    xs : TowerBuf
    meta_off : int64
    fmt : KqFmt
}

//! the block sites' bind: the q8 lane's planes, the schedule from its start, the block feeds, the q8 format
def rail_block_bind() : RailSiteBind
    => RailSiteBind(wq = TowerBuf.wq, ws = TowerBuf.ws, meta = TowerBuf.meta, xh = TowerBuf.xh, xq = TowerBuf.xq, xs = TowerBuf.xs, meta_off = 0l, fmt = KqFmt.q8)

//! the planes GEMM site `site` binds on a rail of either home
def rail_bind(rl : auto(R); site : RailSite) : RailSiteBind => rl.binds[int(site)]

//! GEMM site `site`'s pick on a rail of either home: a `VtTile` on Vulkan, a `CutPick` on CUDA
def rail_site(rl : auto(R); site : RailSite) => rl.sites[int(site)]

//! the planes the tile at `site` splits k over on a rail of either home: the site pick's split, 1 where it runs whole
def rail_site_nsplit(rl : auto(R); site : RailSite) : int64 => max(rl.sites[int(site)].nsplit, 1l)

//! every site bound to the block sites' planes (`rail_block_bind`): a seat's start before it binds its own sites
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-sites")]
def rail_binds_reset(var rl : auto(R)) {
    for (i in range(RAIL_SITES)) {
        rl.binds[i] = rail_block_bind()
    }
}

//! a rail of either home at an encode's start on `lane`: no norms base, no ones row, every site's pick cleared and on the block
//! sites' planes (`rail_binds_reset`); the home sets its own fields after
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-sites")]
def rail_begin(var rl : auto(R); lane : RailLane) {
    rl.lane = lane
    rl.norms_base = 0l
    rl.ones_off = 0l
    for (i in range(RAIL_SITES)) {
        rl.sites[i] = default<typedecl(rl.sites[i])>
    }
    rail_binds_reset(rl)
}

//! GEMM `site` fed off planes of its own: the f16 feed `xh` the cm2 tiles read, the Q8_0 feed `xq` / `xs` the mul_mm arm reads
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-sites")]
def rail_bind_feeds(var rl : auto(R); site : RailSite; xh, xq, xs : TowerBuf) {
    rl.binds[int(site)].xh = xh
    rl.binds[int(site)].xq = xq
    rl.binds[int(site)].xs = xs
}

//! the f16 GEMM sites `sites` fed off the f16 plane `xh`: a front's and a projector's GEMMs over their own feed
def rail_bind_f16(var rl : auto(R); sites : auto(S); xh : TowerBuf) {
    for (site in sites) {
        rl.binds[int(site)].xh = xh
    }
}

//! the rel table's projection fed off the table's own feeds
def rail_bind_rel(var rl : auto(R)) {
    rail_bind_feeds(rl, RailSite.rel, TowerBuf.rel_tabh, TowerBuf.rel_tabq, TowerBuf.rel_tabs)
}

[cold_path]
def private rail_kq_meta_off_refused(site : RailSite; off : int64) {
    panic("dasLLAMA tower rail: GEMM site {site} bound past the schedule's start (meta_off {off}) - a GEMM tile reads its record at the index from the plane's first word, so only the GEMV site binds past it")
}

//! the schedule word GEMM record `rid` at `site` maps from (a GEMM site binds from byte 0, `#rail-sites`)
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-sites")]
def rail_kq_map(rl : auto(R); site : RailSite; rid : int64) : uint {
    let off = rl.binds[int(site)].meta_off
    if (off != 0l) {
        rail_kq_meta_off_refused(site, off)
    }
    return rl.maps[rid]
}

let RAIL_GEMV_META_BYTES = 64l   //! a q8 GEMV record's region pairs: eight regions
let RAIL_META_ALIGN = 256l       //! a GEMV record's start in the schedule: the largest storage-offset alignment a Vulkan device may ask

//! the profile ledger's roles (`rail_prof_on`): the role of every GPU stamp a chain marks (stamp 0
//! is the baseline)
enum RailProfRole {
    ln
    rq
    gemm_q
    gemm_k
    gemm_v
    bias
    hpad
    fa
    hunpad
    gemm_o
    post_add
    gemm_fc1
    gemm_fc2
    sk_reduce   //! the fc2 GEMM's split-k planes summed
    copy_out
    rms
    attn        //! the rel-pos attention kernels (gemma4a, canary)
    silu
    glu
    dwconv
    gemm_rel    //! the rel-table projection
    gemm_pw     //! the conv module's pointwise GEMMs
    upload
    kv_store    //! the whisper decoder's K/V stores
    logits      //! the whisper decoder's tied-embedding logits GEMV
    im2col      //! the fronts' column builders (the 2D and the stem's k3)
    spectrum    //! the fronts' DFT
    mel         //! the fronts' log filterbank and canary's normalization
    shuffle     //! the fronts' feature-row shuffles
    pool        //! the chat projector tails' pair pool
    tts_gemm    //! the TTS seats' f32 tile GEMMs (the linears, the convs' tiles, the LSTM input gates)
    tts_im2col  //! the TTS convs' column builder
    tts_lstm    //! the LSTM recurrence walks
    tts_ln      //! the TTS layernorm stamps, the residual seams included
    tts_act     //! the leaky ReLU and tanh GELU maps
    tts_adain   //! the column stats and the AdaIN folds
    tts_attn    //! the PL-BERT attention rows
    tts_source  //! the harmonic source's stamps: the noise draw, the increments, the cumsum, the sines
    tts_stft    //! the STFT and the inverse STFT
    tts_rows    //! the row movers: gathers, concats, sums, pools, reflects, axpys
    tts_pk      //! the Pocket family's row movers, rope, scales and ELUs
    tts_pk_dot  //! the Pocket row GEMV's bare dot
    tts_pk_qkv  //! ... the frame loop's normed qkv stamp with its rope-and-store
    tts_pk_attn //! ... the cached attention row
    tts_pk_res  //! ... the residual joins
    tts_pk_ffn  //! ... the normed ffn up with its GELU
    tts_pk_head //! ... the flow head's modulated stamps: the LN-SiLU, the gate, the add-SiLU, the tail
}

let RAIL_PROF_ROLES = 64   //! the ledger's role width: past `RailProfRole`'s count

//! a shot's profile ledger: the device microseconds and the stamps a role took, a stamp the span
//! since the one before it
struct RailProfSums {
    us : double[RAIL_PROF_ROLES]
    n : int[RAIL_PROF_ROLES]
    tot : double
    stamps : int
}

var private g_rail_prof_pin = false
var private g_rail_prof_last : RailProfSums

//! the profile ledger is on: `DASLLAMA_GPU_PROF`, or a program's pin
def rail_prof_on : bool => g_env_gpu.gpu_prof || g_rail_prof_pin

//! Test seat: the ledger on (`on`) whatever the knob says; answers the pin it replaced
def set_rail_prof_pin(on : bool) : bool {
    let was = g_rail_prof_pin
    g_rail_prof_pin = on
    return was
}

[cold_path]
def rail_prof_add(var s : RailProfSums; role : RailProfRole; us : double) {
    s.us[int(role)] += us
    s.n[int(role)]++
    s.tot += us
    s.stamps++
}

[cold_path]
def rail_prof_lines(s : RailProfSums; tag, chain : string; nl, npos : int64;
                    wall_us : int) : array<string> {
    //! the ledger's report: the chain's line under the home's `tag`, then a line a role that took a
    //! stamp, in role order
    let head = "{tag} tower {chain} gpu: {nl} blocks x {npos} rows, {s.stamps} stamps"
    var lines <- ["{head}, {int(s.tot)} us on the device, {wall_us} us host wall"]
    for (r in type<RailProfRole>) {
        let i = int(r)
        continue if (s.n[i] == 0)
        let pct = int(s.us[i] * 100.0lf / max(s.tot, 1.0lf))
        lines |> push("  {r}: {int(s.us[i])} us over {s.n[i]} stamps ({pct}%)")
    }
    return <- lines
}

//! the report logged, the ledger kept as the last one (`rail_prof_last`)
[cold_path]
def rail_prof_report(s : RailProfSums; tag, chain : string; nl, npos : int64;
                     wall_us : int) {
    g_rail_prof_last = s
    var lines <- rail_prof_lines(s, tag, chain, nl, npos, wall_us)
    for (l in lines) {
        to_log(LOG_INFO, "{l}\n")
    }
    delete lines
}

//! the ledger the last report logged, on any home
def rail_prof_last : RailProfSums => g_rail_prof_last

//! the last report's ledger dropped: a home's model discharge runs it, so a swap reads no other model's ledger
def rail_prof_forget {
    g_rail_prof_last = RailProfSums()
}

//! the ledger's reach on a home's speech shots, said once a process at the home's first profiled shot
[cold_path]
def rail_prof_say(var said : bool&; tag, how : string) {
    return if (said)
    said = true
    to_log(LOG_INFO, "dasLLAMA {tag} tier: the profile ledger is on (DASLLAMA_GPU_PROF, or a test's pin) - a speech seat's shot {how}\n")
}

//! The audio families whose GPU seats move between the discrete homes as one (`ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs`)
enum TowerAudioFamily {
    whisper      //! the whisper class - the conv stem, the pre-LN blocks, the chat projector tails, the device mel - and whisper's decoder
    qwen3a       //! qwen3a's front and projector; its core rides the whisper class's blocks
    gemma4a
    canary
    parakeet
    gigaam       //! gigaam's encoder and its CTC head
    transducer   //! the transducer decode parakeet and gigaam share, and the FastConformer rows' handoff
    vad
}

//! a family's q8 regions' device-block pieces under the home's prefix (`vk`, `cu`)
def tower_family_piece(home, fam : string) : string => "{home}.{fam}"

//! a family's front convs' f16 slab piece under the home's prefix
def tower_front16_piece(home, fam : string) : string => "{home}.{fam}.front16"

//! the rows a served FastConformer chain leaves on the device for the decoders, under a generation the session records (`bytes`
//! 0: none - every FastConformer seat entry clears it)
struct TowerRowsHandoff {
    served : uint64   //! the served encodes' generation counter, bumped by each
    gen : uint64
    bytes : int64
    dev : uint64
}

//! the rows of a served chain left at `dev` (`bytes` long) under the next generation
def rows_handoff_land(var h : TowerRowsHandoff; dev : uint64; bytes : int64) {
    h.served++
    h.gen = h.served
    h.bytes = bytes
    h.dev = dev
}

//! the generation of the rows the last chain left on the device, 0 where none
def rows_handoff_gen(h : TowerRowsHandoff) : uint64 => h.bytes > 0l ? h.gen : 0ul

//! the device plane of generation `gen`'s rows (`bytes` long) where no seat has run since, else 0
def rows_handoff_dev(h : TowerRowsHandoff; gen : uint64; bytes : int64) : uint64
    => (gen != 0ul && h.bytes == bytes && h.gen == gen) ? h.dev : 0ul

//! a served post-norm chain's `xb` rows left on the device under the host array they were read back into (`bytes` 0: none -
//! every audio hook entry clears it, so it names the encode in flight alone)
struct TowerEncOut {
    host : uint64
    bytes : int64
    dev : uint64
}

//! the device plane of the rows a caller is about to upload from `rows` (`bytes` long) where the handoff holds them and the
//! resident that landed them is still up (`live`), else 0
def enc_out_dev(o : TowerEncOut; rows : float const?; bytes : int64; live : bool) : uint64
    => (o.bytes == bytes && o.host == intptr(rows) && live) ? o.dev : 0ul

var private g_rail_cm2_tile_said = false

//! whether the knob's reach on the tower GEMMs has been said this process
def rail_cm2_tile_said : bool => g_rail_cm2_tile_said

//! DASLLAMA_CM2_TILE's reach on the tower GEMMs (`forced` the column it names), said once a process
[cold_path]
def rail_cm2_tile_say(forced : int64) {
    g_rail_cm2_tile_said = true
    to_log(LOG_INFO, "dasLLAMA tower rail: DASLLAMA_CM2_TILE={forced} reaches the tower GEMMs on the discrete homes - the q8 tiles take that column; the f16 tile takes s at 32 and m at 128, and keeps its own pick at 256 (unset the knob for every pick's own column)\n")
}

//! the pricing rails' list: the comma-separated kernel-name fragments of `frags`
def kernel_skip_parse(frags : string) : array<string> => <- [for (f in split(frags, ",")); f; where !empty(f)]

//! a kernel `name` holds one of the pricing rail's `frags`
def kernel_skip_hit(frags : array<string>; name : string) : bool {
    for (f in frags) {
        return true if (find(name, f) >= 0)
    }
    return false
}
