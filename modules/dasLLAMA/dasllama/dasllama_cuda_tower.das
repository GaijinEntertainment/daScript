options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA tower driver: the vision ViT chains - gemma3v, gemma4v, the gemma4uv embedder, qwen3v, qwen25v - served whole
//! on the CUDA home off the image planes, each seat running its family's shared body (`dasllama_tower_chains`) on the
//! CUDA rail (`CutRail`: no descriptor sets, no hazard rail, the stream ordering the launches). The q8 lane reads each family's
//! CUDA-flavored image's device block (`ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block`); the exact lanes read the
//! file's planes as stored, the bf16 ones through the f32 tile's bf16 arm. Registers the vision hook seats and the vision
//! flavors under the CUDA home (`register_gpu_home`), so the Vulkan tower's vision seats stand down under the CUDA ask. The seat
//! plumbing every family's seat on this home shares is `dasllama_cuda_tower_seat`, re-exported.
module dasllama_cuda_tower shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, register_gpu_home, gpu_want_arms_tier, TILE_READ_SLACK, GpuDeclineCounter
require dasllama/dasllama_gpu_math   // cm2_tiles_at, batch_sched_map, f32_gemm_wgs
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_rail
require dasllama/dasllama_cuda_tower_seat public
require dasllama/dasllama_cuda_tower_audio   // [init]-only: a program holding the CUDA tower holds its audio installer
require dasllama/dasllama_tower_chains
require dasllama/dasllama_gemma3v
require dasllama/dasllama_gemma4v
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_qwen3v
require dasllama/dasllama_qwen25v

var private g_cut_g3v = CutResident(family = "gemma3v")
var private g_cut_g4v = CutResident(family = "gemma4v")
var private g_cut_uv = CutResident(family = "gemma4uv")
var private g_cut_q3v = CutResident(family = "qwen3v")
var private g_cut_q25v = CutResident(family = "qwen25v")
var private @scratch @exact_size g_cut_rope_tabs : array<float>    //! the packed rope tables staging (gemma4v)
var private @scratch @exact_size g_cut_win_sort : array<uint>      //! the window-sort row map staging (qwen25v)
var private @scratch @exact_size g_cut_win_table : array<uint>      //! the window table staging (qwen25v)

def private cut_vis_gate(r : CutResident; q8 : bool; dev : TowerDev) : bool {
    return cut_decline(r, TowerDecline.knob) if (!cuda_tower_on())
    return !q8 || dev == TowerDev.cuda || cut_decline(r, TowerDecline.flavor)
}

//! the vision residents released: the seat plumbing's sweep (`cut_on_release`)
[cold_path]
def private cut_vis_release {
    cut_release(g_cut_g3v)
    cut_release(g_cut_g4v)
    cut_release(g_cut_uv)
    cut_release(g_cut_q3v)
    cut_release(g_cut_q25v)
}

//! the vision planes at the canvas and the column plane at the scratch's rows; false counted `memory`
def private cut_vis_bufs(var r : CutResident; w, h, pd : int64) : bool {
    cut_grow(r, r.planes, w * h * 3l * 4l)
    cut_grow(r, r.col, r.cap_rows * pd * 4l)
    return !r.oom || cut_decline(r, TowerDecline.memory)
}

def private cut_planes_up(r : CutResident; planes : array<float>; w, h : int64) {
    cut_htod(r.planes.dev, unsafe(addr<void?>(planes[0])), w * h * 3l * 4l)
}

def private cut_vis_tail_ensure(vis : TowerVisTail) : bool
    => (ensure_tower_pool_2d_cls() && ensure_tower_rms_cls() && cut_f32_ensure(vis.proj_bf16) && (!vis.affine || ensure_tower_affine_rows_cls())
        && (!vis.clamp.active || ensure_tower_clamp_cls()))

let private CUT_G3V_PIECE = "cu.g3v"

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_gemma3v_bake(t : Gemma3vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- gemma3v_all_regions(t)
    tower_bake_q8(b, CUT_G3V_PIECE, t.qblob, t.qscales, regions)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_gemma3v_chain(t : Gemma3vTower; var s : Gemma3vState; planes : array<float>; w, h, npos : int64) : bool {
    var r & = g_cut_g3v
    return false if (!cut_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let pd = gemma3v_patch_pad(t)
    let n_side = t.image_px / t.patch
    let nout = (n_side / t.merge) * (n_side / t.merge)
    if (hs > 128l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || w != t.image_px || h != t.image_px
            || n_side % t.merge != 0l || npos != n_side * n_side) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    let vis = TowerVisTail(nout = nout, gw = n_side, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = t.soft_norm_w, proj_w = t.proj_w, eps = t.eps)
    if (!cut_ln_ensure(f32w, hsp) || !ensure_tower_patch_im2col_cls() || !cut_vis_tail_ensure(vis)) {
        return cut_decline(r, TowerDecline.device)
    }
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, d) || (!f32w && !cut_upload_q8(r, t, CUT_G3V_PIECE))) {
            return cut_decline(r, TowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, dp, max(d, t.proj_dim)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    tower_gemma3v_offs(t, g_cut_ln_offs)
    var rl & = g_cut_rl
    rail_begin(rl, f32w ? RailLane.f32 : RailLane.q8)
    if (!f32w) {
        if (!cut_site_pick(rl, r, RailSite.d, d, npos, d, false) || !cut_site_pick(rl, r, RailSite.ff, ff, npos, d, false)
                || !cut_site_pick(rl, r, RailSite.fc2, d, npos, ff, true)) {
            return cut_decline(r, TowerDecline.device)
        }
        let nrec = t.n_layer * LN_BLOCK_GEMMS
        cut_sched_begin(nrec, t.n_layer * (4l * rail_site(rl, RailSite.d).groups + rail_site(rl, RailSite.ff).groups + rail_site(rl, RailSite.fc2).groups))
        tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), nrec) $(rid : int64) : TowerSchedRec {
            return tower_ln_rec(rl, g_cut_ln_offs, d, ff, npos, rid)
        }
        return false if (!cut_sched_up(r))
        if (!key_exists(r.picks_said, npos)) {
            cut_pick_say(r, npos, rail_site(rl, RailSite.d), rail_site(rl, RailSite.ff), rail_site(rl, RailSite.fc2))
        }
    }
    cut_planes_up(r, planes, w, h)
    cut_rail_seats(rl, r)
    cut_chain(rl, "gemma3v", (w << 32l) | npos) {
        tower_gemma3v_chain(rl, t, g_cut_ln_offs, vis, w, pd, hsp, npos)
    }
    cut_count_stem()
    cut_count_tail()
    ensure_length(s.tail, nout * t.proj_dim)
    s.tail_ready = cut_served(r, r.tail_proj.dev, s.tail, nout * t.proj_dim * 4l, npos, t.n_layer)
    return s.tail_ready
}

let private CUT_G4V_PIECE = "cu.g4v"

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_gemma4v_bake(t : Gemma4vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- gemma4v_all_regions(t)
    tower_bake_q8(b, CUT_G4V_PIECE, t.qblob, t.qscales, regions)
    return true
}

def private cut_g4v_ensure : bool
    => (ensure_fa_cm2_h64_bidir_cls() && ensure_tower_rms_cls() && ensure_tower_clamp_cls() && ensure_tower_rope2d_cls() && ensure_tower_post_add_cls()
        && ensure_tower_geglu_cls() && ensure_tower_clamp_cvt_cls() && ensure_tower_patch_im2col_cls() && ensure_tower_pos_add_2d_cls()
        && ensure_tower_clamp_copy_cls() && cut_f32_ensure(false) && cut_f32_ensure(true))

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_gemma4v_chain(t : Gemma4vTower; var s : Gemma4vState; planes : array<float>; w, h, npos : int64) : bool {
    var r & = g_cut_g4v
    return false if (!cut_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.n_ff
    let hs = d / t.n_head
    let pd = t.patch_dim
    let n_cols = w / t.patch
    let n_rows = h / t.patch
    let nout = (n_cols / t.merge) * (n_rows / t.merge)
    let vis = TowerVisTail(nout = nout, gw = n_cols, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = -1l, proj_w = t.proj_w, proj_bf16 = t.proj_bf16, eps = t.eps,
        affine = true, scale = sqrt(float(d)), std = t.has_std, std_bias = t.std_bias, std_scale = t.std_scale, clamp = gemma4v_clamp_at(t, t.proj_clamp))
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || npos != n_cols * n_rows
            || n_cols % t.merge != 0l || n_rows % t.merge != 0l) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_g4v_ensure() || !cut_vis_tail_ensure(vis)) {
        return cut_decline(r, TowerDecline.device)
    }
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, max(hs, d)) || (!f32w && !cut_upload_q8(r, t, CUT_G4V_PIECE))) {
            return cut_decline(r, TowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, 0l, max(d, t.proj_dim)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    let c = r.cap_rows
    cut_grow(r, r.q2, c * d * 4l)
    cut_grow(r, r.k2, c * d * 4l)
    cut_grow(r, r.v2, c * d * 4l)
    cut_grow(r, r.k16, c * d * 2l)
    cut_grow(r, r.v16, c * d * 2l)
    cut_grow(r, r.tabs, c * hs * 4l)
    if (f32w) {
        cut_grow(r, r.xc, c * d * 4l)
    }
    return cut_decline(r, TowerDecline.memory) if (r.oom)
    var rl & = g_cut_rl
    rail_begin(rl, f32w ? RailLane.f32 : RailLane.q8)
    if (!f32w) {
        if (!cut_site_pick(rl, r, RailSite.d, d, npos, d, false) || !cut_site_pick(rl, r, RailSite.ff, ff, npos, d, false)) {
            return cut_decline(r, TowerDecline.device)
        }
        let nrec = t.n_layer * GEMMA4V_GEMMS
        cut_sched_begin(nrec, t.n_layer * (5l * rail_site(rl, RailSite.d).groups + 2l * rail_site(rl, RailSite.ff).groups))   // the down site rides the d-wide column
        tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), nrec) $(rid : int64) : TowerSchedRec {
            return tower_gemma4v_rec(rl, t, npos, rid)
        }
        return false if (!cut_sched_up(r))
    }
    let pairs = hs / 4l
    g_cut_rope_tabs |> reserve(npos * 4l * pairs)
    g_cut_rope_tabs |> resize(npos * 4l * pairs)
    gemma4v_pack_rope_tabs(s, npos, pairs, unsafe(addr(g_cut_rope_tabs[0])))
    cut_htod(r.tabs.dev, unsafe(addr<void?>(g_cut_rope_tabs[0])), npos * 4l * pairs * 4l)
    cut_planes_up(r, planes, w, h)
    cut_rail_seats(rl, r)
    cut_chain(rl, "gemma4v", (w << 32l) | npos) {
        tower_gemma4v_chain(rl, t, vis, w, n_cols, npos)
    }
    cut_count_stem()
    cut_count_tail()
    ensure_length(s.tail, nout * t.proj_dim)
    s.tail_ready = cut_served(r, r.tail_proj.dev, s.tail, nout * t.proj_dim * 4l, npos, t.n_layer)
    return s.tail_ready
}

def private cut_uv_ensure(patch_bf16, proj_bf16 : bool) : bool
    => (ensure_tower_patch_im2col_cls() && ensure_tower_ln_cls() && ensure_tower_pos_add_2d_cls() && ensure_tower_rms_cls()
        && cut_f32_ensure(patch_bf16) && cut_f32_ensure(proj_bf16))

[hot_path, unused_argument(s), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; planes : array<float>; w, h, npos, n_cols : int64; var out : array<float>) : bool {
    var r & = g_cut_uv
    return cut_decline(r, TowerDecline.knob) if (!cuda_tower_on())
    let d = t.n_embd
    let pd = t.patch_dim
    let proj = t.proj_dim
    if (d % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || npos != n_cols * (h / t.patch) || n_cols != w / t.patch) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_uv_ensure(t.patch_bf16, t.proj_bf16)) {
        return cut_decline(r, TowerDecline.device)
    }
    let key = cut_key(t, 0l)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, d)) {
            return cut_decline(r, TowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, d, 0l, proj) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    cut_planes_up(r, planes, w, h)
    var rl & = g_cut_rl
    rail_begin(rl, RailLane.q8)
    cut_rail_seats(rl, r)
    cut_chain(rl, "gemma4uv", (w << 32l) | npos) {
        tower_gemma4uv_chain(rl, t, w, n_cols, npos)
    }
    cut_count_stem()
    return cut_served(r, r.tail_proj.dev, out, npos * proj * 4l, npos, 0l)
}

let private CUT_Q3V_PIECE = "cu.q3v"

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_qwen3v_bake(t : Qwen3vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- qwen3v_all_regions(t)
    tower_bake_q8(b, CUT_Q3V_PIECE, t.qblob, t.qscales, regions)
    return true
}

def private cut_q3v_ensure(hsp : int64) : bool
    => (ensure_tower_ln_cls() && ensure_tower_ln16_cls() && ensure_tower_bias_act_cls() && ensure_tower_bias_act_cvt16_cls() && ensure_tower_rope_tab_cls()
        && ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln16_cls() && ensure_tower_patch_im2col_cls() && ensure_tower_stem_asm_cls()
        && ensure_tower_act_cls() && ensure_tower_clamp_cvt_cls() && cut_attn_ensure(hsp) && cut_f32_ensure(false))

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_qwen3v_chain(t : Qwen3vTower; var s : Qwen3vState; planes : array<float>; w, h, npos : int64) : bool {
    var r & = g_cut_q3v
    return false if (!cut_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let nds = t.n_deepstack
    let pd = t.patch_dim
    let d4 = 4l * d
    let nout = npos / 4l
    let pw = w / t.patch
    let proj = t.proj_dim
    if (hs > 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || npos % 4l != 0l
            || npos != pw * (h / t.patch) || pw % 2l != 0l || d > ff) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_q3v_ensure(hsp)) {
        return cut_decline(r, TowerDecline.device)
    }
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, d) || (!f32w && !cut_upload_q8(r, t, CUT_Q3V_PIECE))) {
            return cut_decline(r, TowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, dp, max(d4, (nds + 1l) * proj)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    let c = r.cap_rows
    let half = hs / 2l
    cut_grow(r, r.qkv, 3l * c * d * 4l)
    cut_grow(r, r.cos, c * half * 4l)
    cut_grow(r, r.sin, c * half * 4l)
    let pos_rs = qwen3v_pos_resized(t, s)
    if (pos_rs) {
        cut_grow(r, r.pos_resized, c * d * 4l)
    }
    return cut_decline(r, TowerDecline.memory) if (r.oom)
    var rl & = g_cut_rl
    rail_begin(rl, f32w ? RailLane.f32 : RailLane.q8)
    if (!f32w) {
        if (!cut_site_pick(rl, r, RailSite.qkv, 3l * d, npos, d, false) || !cut_site_pick(rl, r, RailSite.d, d, npos, d, false)
                || !cut_site_pick(rl, r, RailSite.ff, ff, npos, d, false)) {
            return cut_decline(r, TowerDecline.device)
        }
        let nrec = t.n_layer * QWEN3V_GEMMS
        cut_sched_begin(nrec, t.n_layer * (rail_site(rl, RailSite.qkv).groups + 2l * rail_site(rl, RailSite.d).groups + rail_site(rl, RailSite.ff).groups))   // the down site rides the d-wide column
        tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), nrec) $(rid : int64) : TowerSchedRec {
            return tower_qwen3v_rec(rl, t, npos, rid)
        }
        return false if (!cut_sched_up(r))
    }
    let btab = npos * half * 4l
    let bslice = nout * proj * 4l
    unsafe {
        cut_planes_up(r, planes, w, h)
        if (pos_rs) {
            cut_htod(r.pos_resized.dev, addr<void?>(s.pos_rs[0]), npos * d * 4l)
        }
        cut_htod(r.cos.dev, addr<void?>(s.cos_t[0]), btab)
        cut_htod(r.sin.dev, addr<void?>(s.sin_t[0]), btab)
    }
    cut_rail_seats(rl, r)
    cut_chain(rl, "qwen3v", (w << 32l) | (npos << 1l) | (pos_rs ? 1l : 0l)) {
        tower_qwen3v_chain(rl, t, w, hsp, npos, pos_rs)
    }
    cut_count_stem()
    cut_count_tail()
    if (nds > 0l) {
        cut_dtoh(unsafe(addr<void?>(s.ds_stash[0])), r.tail_proj.dev + uint64(bslice), nds * bslice)
    }
    return cut_served(r, r.tail_proj.dev, s.ds_slice, bslice, npos, t.n_layer)
}

def private cut_q25v_ensure : bool
    => (ensure_f16_gemm_cm2_w_cls() && ensure_fa_cm2_h128_bidir_cls() && ensure_tower_clamp_cvt_cls() && ensure_tower_rms_cls() && ensure_tower_bias_act_cls()
        && ensure_tower_rope_tab_cls() && ensure_tower_hpad_cls() && ensure_tower_hpad16_cls() && ensure_tower_hunpad_cls() && ensure_tower_win_attn_cls()
        && ensure_tower_post_add_rms_cls() && ensure_tower_bias_gate16_cls() && ensure_tower_patch_im2col_cls() && cut_f32_ensure(false)
        && ensure_tts_row_gather_cls() && ensure_tower_act_cls())

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_qwen25v_chain(t : Qwen25vTower; var s : Qwen25vState; planes : array<float>; w, h, npos : int64) : bool {
    var r & = g_cut_q25v
    return cut_decline(r, TowerDecline.knob) if (!cuda_tower_on())
    return cut_decline(r, TowerDecline.quant_mode) if (!(t.wblob.n > 0l && !t.blk_bf16))
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let pd = q25v_patch_pad(t)
    let d4 = 4l * d
    let nout = npos / 4l
    let nwin = long_length(s.win_lo) - 1l
    let has_win = t.n_wa_pattern > 1l && t.n_layer > 0l
    let wmax = has_win ? qwen25v_window_widest(s) : 0l
    if (hs > 128l || hsp != 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || nwin <= 0l
            || npos % 4l != 0l || npos != (w / t.patch) * (h / t.patch) || (has_win && (wmax > 64l || hs != 80l))) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_q25v_ensure()) {
        return cut_decline(r, TowerDecline.device)
    }
    let w_off = t.layer_w
    let w_n = t.n_layer * t.layer_w_stride
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        r.key = key
        r.oom = false
        cut_up(r, r.norms, unsafe(reinterpret<void?>(t.blob.p)), t.blob.n * 4l)
        cut_up(r, r.wb, unsafe(reinterpret<void?>(plane_at(t.wblob, w_off))), w_n * 2l)
        r.ones_off = t.blob.n
        if (r.oom) {
            cut_release(r)
            return cut_decline(r, TowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, dp, max(d4, t.proj_dim)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    let c = r.cap_rows
    let half = hs / 2l
    cut_grow(r, r.cos, c * half * 4l)
    cut_grow(r, r.sin, c * half * 4l)
    cut_grow(r, r.win_sort, c * 4l)
    cut_grow(r, r.win_table, (nwin + 1l) * 4l)
    return cut_decline(r, TowerDecline.memory) if (r.oom)
    g_cut_win_table |> reserve(nwin + 1l)
    g_cut_win_table |> resize(nwin + 1l)
    qwen25v_window_table(s, unsafe(addr(g_cut_win_table[0])))
    g_cut_win_sort |> reserve(npos)
    g_cut_win_sort |> resize(npos)
    for (i in range64(npos)) {
        g_cut_win_sort[i] = uint(s.src_of[i])
    }
    let btab = npos * half * 4l
    unsafe {
        cut_planes_up(r, planes, w, h)
        cut_htod(r.win_sort.dev, addr<void?>(g_cut_win_sort[0]), npos * 4l)
        cut_htod(r.cos.dev, addr<void?>(s.cos_t[0]), btab)
        cut_htod(r.sin.dev, addr<void?>(s.sin_t[0]), btab)
        cut_htod(r.win_table.dev, addr<void?>(g_cut_win_table[0]), (nwin + 1l) * 4l)
    }
    var rl & = g_cut_rl
    rail_begin(rl, RailLane.q8)
    cut_rail_seats(rl, r)
    cut_chain(rl, "qwen25v", (w << 32l) | npos) {
        tower_qwen25v_chain(rl, t, w, hsp, npos, nwin)
    }
    cut_count_stem()
    cut_count_tail()
    ensure_length(s.mout, nout * t.proj_dim)
    return cut_served(r, r.tail_proj.dev, s.mout, nout * t.proj_dim * 4l, npos, t.n_layer)
}

//! the head width is the model's, unknown before the load: the pick builds the attention classes at both padded widths the chains serve
def private cuda_gemma3v_serves_model(q8 : bool) : bool
    => (cut_q8_pick_arms(q8) && cut_ln_ensure(false, 64l) && cut_ln_ensure(false, 128l) && ensure_splitk_reduce_cls()
        && ensure_tower_patch_im2col_cls() && cut_vis_tail_ensure(TowerVisTail()))

def private cuda_gemma4v_serves_model(q8 : bool) : bool
    => cut_q8_pick_arms(q8) && cut_g4v_ensure() && cut_vis_tail_ensure(TowerVisTail(proj_bf16 = true, affine = true, clamp = Clamp(active = true)))

def private cuda_qwen3v_serves_model(q8 : bool) : bool => cut_q8_pick_arms(q8) && cut_q3v_ensure(64l) && cut_q3v_ensure(128l)

//! the CUDA home's vision seats - the chain hooks and the q8 flavors - at [init] under the CUDA ask, or when `set_gpu_home("cuda")` moves the tier here
[arch(at = "../ARCHITECTURE_MEDIA.md#tower-weight-lane")]
def cuda_tower_install_vision {
    register_gemma3v_gpu(@@cuda_gemma3v_chain, @@cuda_tower_on, false)
    register_gemma3v_flavor(TowerDev.cuda, @@cuda_gemma3v_serves_model, @@cuda_gemma3v_bake)
    register_gemma4v_gpu(@@cuda_gemma4v_chain, @@cuda_tower_on, false)
    register_gemma4v_flavor(TowerDev.cuda, @@cuda_gemma4v_serves_model, @@cuda_gemma4v_bake)
    register_gemma4uv_gpu(@@cuda_gemma4uv_encode)
    register_qwen3v_gpu(@@cuda_qwen3v_chain, @@cuda_tower_on, false)
    register_qwen3v_flavor(TowerDev.cuda, @@cuda_qwen3v_serves_model, @@cuda_qwen3v_bake)
    register_qwen25v_gpu(@@cuda_qwen25v_chain)
}

[init]
def dasllama_cuda_tower_register {
    register_gpu_home("cuda", @@cuda_tower_install_vision)
    cut_on_release(@@cut_vis_release)
    return if (!gpu_want_cuda())
    cuda_tower_install_vision()
}
