options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gemm_gen shared private

require dasllama/dasllama_lint public
require llvm/daslib/llvm_boost
require llvm/daslib/llvm_jit_code
require llvm/daslib/llvm_jit_common
require daslib/ast_boost
require dasllama/dasllama_gemm_schema
require llvm/daslib/llvm_tune

// The perm-parameterized Q8 GEMM tile generator (gemm_generator_plan.md part 3, promoted from
// the M2 hand perm): emits the mr-row x 4-token register tile as LLVM IR — the loop nest,
// interleaved loads, accumulators, scale folds and stores are generic IR (proven to lower
// hand-shaped on arm64, M1 experiment); only the dot primitive is per-ISA. Layout is
// repack=grp<mr> (f32 scales, strides derived from q8q8_repack_type so kernel and repack
// cannot desync), acc=i32reg, scale=block_fold, load=bcast_x. Knobs read from the [llvm_code]
// annotation args (stamped by the [tune] macro; defaults = the M2 perm, kstep=2 nrsplit=4):
//   kstep   blocks per main-loop iteration (1|2|4); the remainder is a chain of conditional
//           single blocks, so kstep=2 reproduces the M2 CFG exactly
//   nrsplit tokens per accumulator slice (4|2); the 4-token call contract is fixed — nrsplit=2
//           runs the k-loop twice over half the tokens (halves live accumulators)
//   mr      rows per interleave group; must be a multiple of the dot width's rows-per-vector
//           (4 on NEON q-regs, 8 on ymm, 16 on zmm) inside the vreg budget; the layout
//           companion (q8q8_layout below) reports the same mr to the runtime repack
//   gkstep  the GEMV companion's blocks per K-iteration (1|2|4) — fresh int accumulators per
//           block give the single-token loop the independent dot chains a serial laneq
//           chain lacks; the tile ignores it (its K knob is kstep)
//   dot     the per-ISA dot primitive: "sdot" (NEON indexed sdot — the arm64 leg),
//           "maddubs" (x64 VPMADDUBSW+VPMADDWD sign-trick pair), "vpdpbusd" (x64 VNNI,
//           sign trick, one dot op), "vpdpbssd" (x64 AVX-VNNI-INT8, native s8·s8),
//           "smmla" (NEON i8mm 2×2 MMA, slice J — implies the kg8 plane; tile only, the
//           gemv/mx4 companions ride the sdot lattice), "amx_int8" (x64 TMUL TDPBSSD
//           macro-tile over the plain grp16/kg4 plane, slice I — tile only; nrsplit ≙
//           tiles per macro side, kstep pinned 1; the gemv/mx4 companions ride the
//           sign-trick busd512 lattice)
//   width   vector width in bits: 128 (NEON, implied by sdot), 256 (ymm), 512 (zmm)
//   bias    128 = bias128 (vpdpbusd only): the repack bakes w^0x80 into the grp<mr> plane,
//           dots run plain vpdpbusd(acc, w_biased, x) — no abs/sign ops — and the exactness
//           correction −128·Σx(block) becomes the accumulator init (slice H)
// Illegal perms decline (return false) and the reference body compiles instead — the decline
// path is the fallback tier; every companion declines in LOCKSTEP (the one shared
// perm_declines), so repacks and kernels fall back to the grp4 reference family together.
// Runs in the llvm-jit context; wired via llvm_user_modules.das. The stubs it generates for
// are dasllama/dasllama_math_gen.das::q8q8_tile_gen, ::q8q8_layout_gen, ::q8q8_wbias_gen,
// ::q8q8_kgroup_gen, ::q8q8_tokstep_gen, ::q8q8_gemv_gen, ::mx4q8_gemv_gen, ::mx4q8_tile_gen,
// ::q8q8_family_live (the witness), the wscale_f16 twins ::q8q8_tile_s16_gen /
// ::q8q8_gemv_s16_gen (same perm, same fold order — only the scale-fold load widens f16), and
// the K-quant rows cores ::k4q8_gemv_gen / ::k5q8_gemv_gen / ::k6q8_gemv_gen (unsigned-quant
// superblock GEMVs over the grp<mr> kq planes — kq stage 4).

// dot-primitive kinds (TilePerm.dotPrim, parsed once into TileEmit.dotKind)
let DOT_SDOT = 0
let DOT_MADDUBS = 1
let DOT_VPDPBUSD = 2
let DOT_VPDPBSSD = 3
let DOT_SMMLA = 4
let DOT_AMX = 5

def private dot_kind(prim : string) : int {
    if (prim == "maddubs") return DOT_MADDUBS
    if (prim == "vpdpbusd") return DOT_VPDPBUSD
    if (prim == "vpdpbssd") return DOT_VPDPBSSD
    if (prim == "smmla") return DOT_SMMLA
    if (prim == "amx_int8") return DOT_AMX
    return DOT_SDOT
}

// Under an amx stamp the VECTOR companions (gemv/mx4) ride the busd lattice over the same
// grp16/kg4 plane: plain sign-trick for unbiased rows, the BIASED busd lattice for bias=128
// rows. Everything downstream then treats the perm exactly like the matching vpdpbusd row.
def private companion_perm(p : TilePerm) : TilePerm {
    var cp = p
    if (cp.dotPrim == "amx_int8") { cp.dotPrim = "vpdpbusd" }
    return cp
}

// The perm's plane kgroup (k-bytes per row per interleave group): smmla reads row-pair × 8-k
// MMA operands off a kg8 plane; every vector-dot perm reads the dword-group kg4 plane. Feeds
// q8q8_repack_type so kernel strides and the runtime repack derive from the one schema.
def private kgroup_of(p : TilePerm) : int => p.dotPrim == "smmla" ? 8 : 4

// the perm vector both generators read off the [llvm_code] annotation args
struct private TilePerm {
    kstep : int = 2
    nrsplit : int = 4
    mr : int = GEMM_REFERENCE_MR()
    gkstep : int = 1
    dotPrim : string = "sdot"
    width : int = 128
    bias : int = 0        // 128 = bias128: grp<mr> bytes baked as w^0x80, plain vpdpbusd dots
    pipe : int = 0        // 1 = software-pipelined amx fold (slice K I4): double-buffered C
                          // scratch, block i−1's scale fold interleaved with block i's TMUL ops
    latch : int = 0       // 1 = raw immediate-tmm amx ops (slice K I1): no per-call
                          // ldtilecfg/tilerelease — the cfg companion owns per-chunk config
    grid : string = ""    // "vbmi": the grid formats' gemv decodes through the VBMI symbol lattice
                          // (the row form; the tile and the planes are the plain row's)
    pf : int = 0          // software prefetch distance in bytes along the gemv's weight stream (0 = none):
                          // one llvm.prefetch per weight line, pf bytes ahead - memory-level parallelism at
                          // many lanes, where the hardware prefetcher alone leaves DRAM bandwidth unused
}

def private parse_perm(gc : LlvmCodeCtx) : TilePerm {
    var p = TilePerm()
    let kv = find_arg(gc.ann.arguments, "kstep")
    if (kv is tInt) { p.kstep = kv as tInt }
    let nv = find_arg(gc.ann.arguments, "nrsplit")
    if (nv is tInt) { p.nrsplit = nv as tInt }
    let mv = find_arg(gc.ann.arguments, "mr")
    if (mv is tInt) { p.mr = mv as tInt }
    let gv = find_arg(gc.ann.arguments, "gkstep")
    if (gv is tInt) { p.gkstep = gv as tInt }
    let dv = find_arg(gc.ann.arguments, "dot")
    if (dv is tString) { p.dotPrim = dv as tString }
    let wv = find_arg(gc.ann.arguments, "width")
    if (wv is tInt) { p.width = wv as tInt }
    let bv = find_arg(gc.ann.arguments, "bias")
    if (bv is tInt) { p.bias = bv as tInt }
    let pv = find_arg(gc.ann.arguments, "pipe")
    if (pv is tInt) { p.pipe = pv as tInt }
    let lv = find_arg(gc.ann.arguments, "latch")
    if (lv is tInt) { p.latch = lv as tInt }
    let grv = find_arg(gc.ann.arguments, "grid")
    if (grv is tString) { p.grid = grv as tString }
    let pfv = find_arg(gc.ann.arguments, "pf")
    if (pfv is tInt) { p.pf = pfv as tInt }
    return p
}

// The ONE decline predicate all four generators share — the tile and its companions must
// fall back together on every rail, or repack and kernel desync (the two-function-stamp
// contract). Rails: knob validity, schema, per-ISA leg, geometry, vreg budget, dot availability.
def private perm_declines(var gc : LlvmCodeCtx; p : TilePerm) : bool { // nolint:STYLE037,STYLE038 — the flat decline ladder IS the perm contract; splitting hides which leg refused
    if (p.dotPrim == "amx_int8") {
        // the TMUL leg: 2×2 TDPBSSD macro over the grp16/kg4 plane; kstep pinned 1 (Q8's per-block scale boundary); Linux-only (arch_prctl enable)
        if ((p.kstep != 1)
            || (p.nrsplit != 1 && p.nrsplit != 2)
            || (p.gkstep != 1 && p.gkstep != 2 && p.gkstep != 4)
            || (p.mr != 16) || (p.width != 512)
            || (p.bias != 0 && p.bias != 128)
            || (p.pipe != 0 && p.pipe != 1)
            || (p.latch != 0 && p.latch != 1)
            || (p.grid != "") || (p.pf != 0)
            || (!g_target_is_x64) || (!g_target_x64_amx)
            || (!g_target_x64_avx512vnni)     // the companions' busd512 lattice + zmm fold
            || (!g_target_os_linux)) return true
        let rt = q8q8_repack_type(p.mr, p.bias, kgroup_of(p))
        if (rt.interleave != p.mr || !rt.f32_scales) return true
        // tile + companion machinery availability (belt — shared, so the family stays lockstep)
        let mod = LLVMGetGlobalParent(gc.impl)
        var noTypes : array<LLVMOpaqueType?>
        for (nm in fixed_array("llvm.x86.tileloadd64.internal", "llvm.x86.tdpbssd.internal",
                               "llvm.x86.tilestored64.internal", "llvm.x86.tilezero.internal",
                               "llvm.x86.avx512.vpdpbusd.512", "llvm.x86.avx512.pshuf.b.512")) {
            let id = LLVMLookupIntrinsicID(nm)
            if (LLVMGetIntrinsicDeclaration(mod, id, noTypes) == null) return true
        }
        if (p.latch != 0) {
            // the raw immediate-tmm leg (slice K I1) + the cfg companion's LDTILECFG
            for (nm in fixed_array("llvm.x86.tileloadd64", "llvm.x86.tdpbssd", "llvm.x86.tdpbsud",
                                   "llvm.x86.tilestored64", "llvm.x86.tilezero", "llvm.x86.ldtilecfg")) {
                let id = LLVMLookupIntrinsicID(nm)
                if (LLVMGetIntrinsicDeclaration(mod, id, noTypes) == null) return true
            }
        }
        if (p.bias != 0 && LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.tdpbsud.internal"), noTypes) == null) {
            return true       // the biased leg's signed×unsigned TMUL dot
        }
        var v64i8 = LLVMVectorType(gc.jit.types.t_int8, 64u)
        let absId = LLVMLookupIntrinsicID("llvm.abs")
        var absTypes <- [v64i8]
        return LLVMGetIntrinsicDeclaration(mod, absId, absTypes) == null
    }
    // bias128 is a vpdpbusd-only knob; pipe/latch are TMUL-only — vector dots have no tile spill or config
    if ((p.kstep != 1 && p.kstep != 2 && p.kstep != 4)
        || (p.nrsplit != 2 && p.nrsplit != 4)
        || (p.gkstep != 1 && p.gkstep != 2 && p.gkstep != 4)
        || (p.mr < 4) || (p.pipe != 0) || (p.latch != 0)
        || (p.grid != "" && p.grid != "vbmi")
        || (p.pf != 0 && p.pf != 1024 && p.pf != 2048 && p.pf != 4096)
        || (p.bias != 0 && (p.bias != 128 || p.dotPrim != "vpdpbusd"))) return true
    let rt = q8q8_repack_type(p.mr, p.bias, kgroup_of(p))
    if (rt.interleave != p.mr || !rt.f32_scales) return true
    if (p.dotPrim == "smmla") {
        // the i8mm MMA leg: NEON widths, row-quad geometry, same q-reg budget as sdot, and the i8mm tier flag (M1 is dotprod-only)
        if ((!g_target_is_aarch64) || (!g_target_arm64_i8mm) || (p.width != 128) || (p.grid != "") || (p.pf != 0)
            || (p.mr % 4 != 0)
            || (2 * p.mr + p.nrsplit * p.mr / 4 + 2 * p.nrsplit + 2 > 32)) return true
        let mod = LLVMGetGlobalParent(gc.impl)
        var v16i8 = LLVMVectorType(gc.jit.types.t_int8, 16u)
        let mmId = LLVMLookupIntrinsicID("llvm.aarch64.neon.smmla")
        var mmTypes <- [gc.jit.types->LLVMInt4Type(), v16i8]
        if (LLVMGetIntrinsicDeclaration(mod, mmId, mmTypes) == null) return true
        // the gemv/mx4 companions ride the sdot lattice over the kg8 plane — both rails
        let id = LLVMLookupIntrinsicID("llvm.aarch64.neon.sdot")
        var declTypes <- [gc.jit.types->LLVMInt4Type(), v16i8]
        if (LLVMGetIntrinsicDeclaration(mod, id, declTypes) == null) return true
        let tblId = LLVMLookupIntrinsicID("llvm.aarch64.neon.tbl1")
        var tblTypes <- [v16i8]
        return LLVMGetIntrinsicDeclaration(mod, tblId, tblTypes) == null
    }
    if (p.dotPrim == "sdot") {
        // the NEON leg (M2 shape): 128-bit q-regs, 4-row sdot lanes; mr>=8 budgets the FUSED-acc shape (kq shares one lo+hi acc there — half the mr4 formula's int-acc bank)
        if ((!g_target_is_aarch64) || (p.width != 128) || (p.grid != "") || (p.pf != 0)
            || (p.mr % 4 != 0)
            || (p.mr >= 8 ? (p.mr + p.nrsplit * p.mr / 4 + 2 * p.nrsplit + 2 > 32)
                          : (2 * p.mr + p.nrsplit * p.mr / 4 + 2 * p.nrsplit + 2 > 32))) return true
        let mod = LLVMGetGlobalParent(gc.impl)
        var v16i8 = LLVMVectorType(gc.jit.types.t_int8, 16u)
        let id = LLVMLookupIntrinsicID("llvm.aarch64.neon.sdot")
        var declTypes <- [gc.jit.types->LLVMInt4Type(), v16i8]
        if (LLVMGetIntrinsicDeclaration(mod, id, declTypes) == null) return true
        // the mx4 companion's dequant primitive — shared so mx4 gemv and layout never desync on mr
        let tblId = LLVMLookupIntrinsicID("llvm.aarch64.neon.tbl1")
        var tblTypes <- [v16i8]
        return LLVMGetIntrinsicDeclaration(mod, tblId, tblTypes) == null
    }
    // the x64 legs — the mx4 companion's pshufb dequant is implied by the tiers below, so no separate rail
    if ((p.dotPrim != "maddubs" && p.dotPrim != "vpdpbusd" && p.dotPrim != "vpdpbssd")
        || (!g_target_is_x64)
        || (p.width != 256 && p.width != 512)
        || (p.grid == "vbmi" && (p.width != 512 || p.dotPrim == "vpdpbssd" || !g_target_x64_avx512vbmi))   // the lattice is zmm VBMI
        || (p.dotPrim == "vpdpbssd" && p.width != 256)) return true    // 512-bit bssd is AVX10 — unproven
    if (p.width == 256) {
        if ((p.dotPrim == "maddubs" && !g_target_x64_avx2)
            || (p.dotPrim == "vpdpbusd" && !g_target_x64_vnni256)
            || (p.dotPrim == "vpdpbssd" && !g_target_x64_vnniint8)) return true
    } else {
        if ((p.dotPrim == "maddubs" && !g_target_x64_avx512bw)
            || (p.dotPrim == "vpdpbusd" && !g_target_x64_avx512vnni)) return true
    }
    let rv = p.width / 32                         // rows per vector = i32 acc lanes
    if (p.mr % rv != 0) return true
    let vq = p.mr / rv
    if ((vq > 2)                                  // the weight bank is 8 dword-groups x vq
        || (p.nrsplit * vq > 8)) return true      // accumulator bank (emit_slice phi arrays)
    // vreg budget: float + int acc tiles + x splat/sign scratch + weight/|weight| in flight; EVEX targets have 32 ymm too
    return 2 * p.nrsplit * vq + 6 > (p.width == 512 || g_target_x64_avx512bw ? 32 : 16)
}

// emission context threaded through the helpers: builder/types + the per-function value handles
struct private TileEmit {
    builder : LLVMOpaqueBuilder?
    types : PrimitiveTypes?
    v16i8 : LLVMOpaqueType?              // one 16-byte activation chunk (fixed at any width)
    v4i32 : LLVMOpaqueType?              // the chunk's dword view (lane-splat source)
    vwi8 : LLVMOpaqueType?               // weight vector at the perm's width (v16i8/v32i8/v64i8)
    vni32 : LLVMOpaqueType?              // int accumulator (v4i32/v8i32/v16i32)
    vnf32 : LLVMOpaqueType?              // float accumulator (v4f32/v8f32/v16f32)
    width : int                          // vector bits (128 NEON / 256 ymm / 512 zmm)
    rv : int                             // rows per weight vector = i32 acc lanes
    dotKind : int                        // DOT_*
    sdot_decl : LLVMOpaqueValue?         // NEON
    sdot_ty : LLVMOpaqueType?
    abs_decl : LLVMOpaqueValue?          // x64 sign trick: llvm.abs (VPABSB)
    abs_ty : LLVMOpaqueType?
    psign_decl : LLVMOpaqueValue?        // x64 width-256 sign apply (VPSIGNB)
    gridPanel : LLVMOpaqueValue?        // iq3s/iq3xxs gemv: the per-superblock alloca panel (null = tile mode, the caller unpacked)
    gridGlobal : LLVMOpaqueValue?       // iq3s gemv: the [512 x i32] grid global (iq3xxs: its [256 x i32] halved grid)
    sign_code_pm1_g : LLVMOpaqueValue?   // the row-group grid decode: ksigns code -> eight +-1 bytes, [128 x i64], parity bit folded in
    gridRows : bool                      // set while the row-group grid decode emits: the column dword read (a panel-form win) reverts to byte loads
    vbmi : bool                          // grid = "vbmi": the grid gemv decodes through the VBMI symbol lattice (row form)
    pf : int                             // software prefetch distance along the weight stream (bytes; 0 = none)
    prefetch_decl : LLVMOpaqueValue?     // llvm.prefetch.p0
    prefetch_ty : LLVMOpaqueType?
    vb_plane : LLVMOpaqueValue? [2]      // the grid's compact code planes, [256 x i8] each: the entry's low / high symbols
    vb_alpha : LLVMOpaqueValue?          // the symbol alphabet as a vpshufb table (16 bytes per lane, [64 x i8])
    vb_ksigns : LLVMOpaqueValue?         // ksigns_iq2xs as [128 x i8] - one VPERMI2B turns 7-bit codes into sign bytes, parity in
    permi2b_decl : LLVMOpaqueValue?      // llvm.x86.avx512.vpermi2var.qi.512
    permi2b_ty : LLVMOpaqueType?
    mshift_decl : LLVMOpaqueValue?       // llvm.x86.avx512.pmultishift.qb.512
    mshift_ty : LLVMOpaqueType?
    shufb_decl : LLVMOpaqueValue?        // llvm.x86.avx512.pshuf.b.512 (the lattice's alphabet lookup)
    shufb_ty : LLVMOpaqueType?
    gfn : LLVMOpaqueValue?               // the function under emit (block creation for emitted inner loops)
    gctx : LLVMOpaqueContext?
    psign_ty : LLVMOpaqueType?
    madd_decl : LLVMOpaqueValue?         // x64 maddubs pair (VPMADDUBSW)
    madd_ty : LLVMOpaqueType?
    maddwd_decl : LLVMOpaqueValue?       // (VPMADDWD)
    maddwd_ty : LLVMOpaqueType?
    dp_decl : LLVMOpaqueValue?           // x64 one-op dot (VPDPBUSD / VPDPBSSD)
    dp_ty : LLVMOpaqueType?
    interleave : int                     // rows per group (q8q8_repack_type(mr).interleave)
    rq : int                             // weight VECTORS per group (mr / rv; NEON: row quads)
    kgroup : int                         // k-bytes per row per group (4; 8 = the smmla plane)
    bias : int                           // 128 = bias128 dots (weight plane is w^0x80); 0 = sign trick
    smmla_decl : LLVMOpaqueValue?        // NEON i8mm 2x2 MMA (the smmla tile lattice)
    smmla_ty : LLVMOpaqueType?
    reduce_decl : LLVMOpaqueValue?       // llvm.vector.reduce.add at vni32 (the gemv inline bsum)
    reduce_ty : LLVMOpaqueType?
    wg : LLVMOpaqueValue?                // interleaved mr-row weight group base (mx4: nibble plane)
    sg : LLVMOpaqueValue?                // group scales (float x interleave per block; mx4: E8M0 bytes)
    x : LLVMOpaqueValue? [4]             // per-token activation base pointers
    xs : LLVMOpaqueValue? [4]            // per-token activation scale base pointers
    xbs : LLVMOpaqueValue? [4]           // per-token bsum bases (−128·Σx i32 per block) — the tile's
                                         // bias128 correction plane; null = compute inline (gemv)
    mx4 : bool                           // block emitter: mx4 LUT-dequant instead of Q8 loads
    q51 : bool                           // block emitter: q51 nibble+qh unpack (32-weight blocks,
                                         // per-block f16 d/m scale pairs + xbs min-term)
    kq : int                             // the gemm_schema kq id (0 = not kq): the
                                         // block unit is a 256-weight superblock over the grp<mr>
                                         // kq planes; dots run unsigned-q (kq_dot_lane)
    kqBytes : bool                       // kq weights are a BYTE-EXPANDED panel (the tile's
                                         // per-token-block scratch — see unpack_kq_panel_grp):
                                         // wlo/whi load verbatim, no 5/6-bit unpack. k5/k6
                                         // tiles only; gemv and k4 read the packed planes
    t_amx : LLVMOpaqueType?              // x86_amx — virtual tile values (the TMUL tile, slice I)
    tload_decl : LLVMOpaqueValue?        // llvm.x86.tileloadd64.internal (latch: raw tileloadd64)
    tload_ty : LLVMOpaqueType?
    tdp_decl : LLVMOpaqueValue?          // llvm.x86.tdpbssd.internal (latch: raw tdpbssd)
    tdp_ty : LLVMOpaqueType?
    tstore_decl : LLVMOpaqueValue?       // llvm.x86.tilestored64.internal (latch: raw tilestored64)
    tstore_ty : LLVMOpaqueType?
    tzero_decl : LLVMOpaqueValue?        // llvm.x86.tilezero.internal (latch: raw tilezero)
    tzero_ty : LLVMOpaqueType?
    rawTiles : bool                      // latch=1: immediate-tmm ops, void results, no backend
                                         // ldtilecfg/tilerelease (the cfg companion owns config)
    tbl_decl : LLVMOpaqueValue?          // llvm.aarch64.neon.tbl1 (NEON mx4)
    tbl_ty : LLVMOpaqueType?
    pshuf_decl : LLVMOpaqueValue?        // llvm.x86.avx2.pshuf.b / avx512.pshuf.b.512 (x64 mx4)
    pshuf_ty : LLVMOpaqueType?
    lut : LLVMOpaqueValue?               // the doubled-e2m1 LUT: v16i8 on NEON; broadcast per
                                         // 16-byte lane to vwi8 on x64 (pshufb is per-lane)
    sgF16 : bool                         // the wscale_f16 rail: the group-scale plane is raw
                                         // binary16 halfwords — scale loads widen via fpext
    vni16 : LLVMOpaqueType?              // <rv x i16> — the halfword scale-load type (sgF16)
    vnf16 : LLVMOpaqueType?              // <rv x half> — its bitcast view, the fpext source
}

// one indexed sdot: acc += w · (dword-group `lane` of x, splatted). The bitcast-to-v4i32 lane
// shuffle is what the AArch64 backend folds into the `sdot v,v,v[lane]` encoding (M1 experiment:
// the generic partial.reduce form leaves a dup per sdot instead).
def private sdot_lane(var te : TileEmit; var acc, wv, xv : LLVMOpaqueValue?; lane : int) : LLVMOpaqueValue? {
    var xi32 = LLVMBuildBitCast(te.builder, xv, te.v4i32, "")
    var xspl = LLVMBuildShuffleVector(te.builder, te.types, xi32, xi32, [lane, lane, lane, lane], "xlane")
    var xb = LLVMBuildBitCast(te.builder, xspl, te.v16i8, "")
    var args <- [acc, wv, xb]
    return LLVMBuildCall2(te.builder, te.sdot_ty, te.sdot_decl, args, "sdot")
}

// all-ones v(width/16)i16 — the VPMADDWD pair-sum operand of the maddubs chain
def private ones_i16(te : TileEmit) : LLVMOpaqueValue? {
    var elems <- [for (_i in range(te.width / 16)); LLVMConstInt(te.types.t_int16, 1ul, 0)]
    return LLVMConstVector(array_data_ptr(elems), uint(te.width / 16))
}

// One lane-dot: acc += w · (dword-group `lane` of the 16-byte x chunk, splatted). NEON: the
// indexed sdot above. x64: a lane-splat shuffle then the per-ISA chain — vpdpbssd native
// s8·s8; vpdpbusd/maddubs run the sign-trick chain (x in [-127,127], w may be -128).
def private dot_lane(var te : TileEmit; var acc, wv, uwv, xv : LLVMOpaqueValue?; lane : int) : LLVMOpaqueValue? {
    if (te.dotKind == DOT_SDOT || te.dotKind == DOT_SMMLA) {
        // SMMLA reaches here only off the MMA lattice's fallbacks — those ride the sdot lattice instead
        return sdot_lane(te, acc, wv, xv, lane)
    }
    let b = te.builder
    var xi32 = LLVMBuildBitCast(b, xv, te.v4i32, "")
    let mask <- [for (_i in range(te.rv)); lane]
    var xspl = LLVMBuildShuffleVector(b, te.types, xi32, xi32, mask, "xlane")
    var xb = LLVMBuildBitCast(b, xspl, te.vwi8, "")
    if (te.dotKind == DOT_VPDPBSSD) {
        var dargs <- [acc, wv, xb]
        return LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "dot")
    }
    if (te.bias != 0) {
        // bias128: plane bytes ARE the unsigned side (w+128); the −128·Σx correction is the accumulator's init value
        var dargs <- [acc, wv, xb]
        return LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "dot")
    }
    var sx : LLVMOpaqueValue?
    if (te.width == 256) {
        var pargs <- [xb, wv]
        sx = LLVMBuildCall2(b, te.psign_ty, te.psign_decl, pargs, "signx")
    } else {
        var wneg = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, wv, LLVMConstNull(te.vwi8), "wneg")
        var xneg = LLVMBuildSub(b, LLVMConstNull(te.vwi8), xb, "negx")
        sx = LLVMBuildSelect(b, wneg, xneg, xb, "signx")
    }
    if (te.dotKind == DOT_VPDPBUSD) {
        var dargs <- [acc, uwv, sx]
        return LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "dot")
    }
    var margs <- [uwv, sx]
    var pairs = LLVMBuildCall2(b, te.madd_ty, te.madd_decl, margs, "pairs")
    var wargs <- [pairs, ones_i16(te)]
    var quads = LLVMBuildCall2(b, te.maddwd_ty, te.maddwd_decl, wargs, "quads")
    return LLVMBuildAdd(b, acc, quads, "dot")
}

// |w| for the u8 side of the x64 sign trick (generic llvm.abs -> VPABSB; is_int_min_poison =
// false keeps -128 defined), hoisted once per weight vector. Null for dot kinds without it
// (bias128 included — the plane bytes are already the unsigned side).
def private abs_w(var te : TileEmit; var wv : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    if (te.bias != 0 || (te.dotKind != DOT_MADDUBS && te.dotKind != DOT_VPDPBUSD)) return null
    var args <- [wv, LLVMConstInt(te.types.t_int1, 0ul, 0)]
    return LLVMBuildCall2(te.builder, te.abs_ty, te.abs_decl, args, name)
}

def private load_vec(var te : TileEmit; ty : LLVMOpaqueType?; var base, off : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    var p = LLVMBuildGEP2(te.builder, te.types.t_int8, base, off, name)
    return LLVMBuildLoad2Aligned(te.builder, ty, p, 1u, name)
}

def private load_v16i8(var te : TileEmit; var base, off : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    return load_vec(te, te.v16i8, base, off, name)
}

def private splat_f32(var te : TileEmit; var v : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    var one = LLVMBuildInsertElement(te.builder, LLVMGetUndef(te.vnf32), v, te.types->ConstI32(0ul), "")
    let mask <- [for (_i in range(te.rv)); 0]
    return LLVMBuildShuffleVector(te.builder, te.types, one, one, mask, name)
}

def private splat_i32(var te : TileEmit; var v : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    var one = LLVMBuildInsertElement(te.builder, LLVMGetUndef(te.vni32), v, te.types->ConstI32(0ul), "")
    let mask <- [for (_i in range(te.rv)); 0]
    return LLVMBuildShuffleVector(te.builder, te.types, one, one, mask, name)
}

// The bias128 accumulator init: −128·Σx(block) for one token — the exactness correction
// (init + Σ(w+128)·x = Σw·x). Tile: one load off the bsums plane. GEMV (no plane): concat
// the block's two chunks, one vpdpbusd(0, splat(128u), x), reduce, negate, splat.
def private bias_acc_init(var te : TileEmit; var bi, xlo, xhi : LLVMOpaqueValue?; tk : int) : LLVMOpaqueValue? {
    let b = te.builder
    var s : LLVMOpaqueValue?
    if (te.xbs[tk] != null) {
        var bp = LLVMBuildGEP2(b, te.types.t_int32, te.xbs[tk], bi, "bp{tk}")
        s = LLVMBuildLoad2Aligned(b, te.types.t_int32, bp, 4u, "bsum{tk}")
    } else {
        let cat <- [for (i in range(32)); i]
        var x32 = LLVMBuildShuffleVector(b, te.types, xlo, xhi, cat, "xcat")
        var xw = x32
        if (te.width == 512) {
            let pad <- [for (i in range(64)); i]     // lanes 32..63 read the zero vector
            xw = LLVMBuildShuffleVector(b, te.types, x32, LLVMConstNull(LLVMVectorType(te.types.t_int8, 32u)), pad, "xpad")
        }
        var dargs <- [LLVMConstNull(te.vni32), splat_i8w(te, 128), xw]
        var pdot = LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "bdot")
        var rargs <- [pdot]
        var r = LLVMBuildCall2(b, te.reduce_ty, te.reduce_decl, rargs, "bsum")
        s = LLVMBuildSub(b, te.types->ConstI32(0ul), r, "nbsum")
    }
    var one = LLVMBuildInsertElement(b, LLVMGetUndef(te.vni32), s, te.types->ConstI32(0ul), "")
    let mask <- [for (_i in range(te.rv)); 0]
    return LLVMBuildShuffleVector(b, te.types, one, one, mask, "binit{tk}")
}

//! llvm.prefetch(read, high locality, data) of the weight plane at byte offset `off`
def private prefetch_wg(var te : TileEmit; var off : LLVMOpaqueValue?) {
    let b = te.builder
    var p = LLVMBuildGEP2(b, te.types.t_int8, te.wg, off, "")
    var args <- [p, te.types->ConstI32(0ul), te.types->ConstI32(3ul), te.types->ConstI32(1ul)]
    LLVMBuildCall2(b, te.prefetch_ty, te.prefetch_decl, args, "")
}

// One 32-k block for tokens [tokBase, tokBase+tokCount): 8*rq shared weight vectors x tokens
// lane-dots at full width, then the per-block scale fold — the exact op order of
// dot_q8q8_laneq4x4_template, so fast-math contraction sees the same input across emitters.
def private emit_block(var te : TileEmit; var bi : LLVMOpaqueValue?; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) {
    let b = te.builder
    let rq = te.rq
    var wb = LLVMBuildMul(b, bi, te.types->ConstI64(uint64(te.interleave * 0x20)), "wb")
    var xb = LLVMBuildMul(b, bi, te.types->ConstI64(0x20ul), "xb")
    var wv : LLVMOpaqueValue? [16]
    var uw : LLVMOpaqueValue? [16]
    if (te.kgroup == 8) {
        // kg8 plane under the sdot lattice: two row-pair loads + uzp1/uzp2.4s reassemble the kg4-shaped weight vectors
        for (g in range(4)) {
            for (qd in range(rq)) {
                let boff = g * 8 * te.interleave + (2 * qd) * 16
                var off0 = boff == 0 ? wb : LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(boff)), "")
                var p0 = load_v16i8(te, te.wg, off0, "wp{g * rq + qd}a")
                var off1 = LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(boff + 16)), "")
                var p1 = load_v16i8(te, te.wg, off1, "wp{g * rq + qd}b")
                var p0i = LLVMBuildBitCast(b, p0, te.v4i32, "")
                var p1i = LLVMBuildBitCast(b, p1, te.v4i32, "")
                var lo = LLVMBuildShuffleVector(b, te.types, p0i, p1i, [0, 2, 4, 6], "wlo{g * rq + qd}")
                var hi = LLVMBuildShuffleVector(b, te.types, p0i, p1i, [1, 3, 5, 7], "whi{g * rq + qd}")
                wv[(2 * g) * rq + qd] = LLVMBuildBitCast(b, lo, te.vwi8, "wv{(2 * g) * rq + qd}")
                wv[(2 * g + 1) * rq + qd] = LLVMBuildBitCast(b, hi, te.vwi8, "wv{(2 * g + 1) * rq + qd}")
                uw[(2 * g) * rq + qd] = abs_w(te, wv[(2 * g) * rq + qd], "uw{(2 * g) * rq + qd}")
                uw[(2 * g + 1) * rq + qd] = abs_w(te, wv[(2 * g + 1) * rq + qd], "uw{(2 * g + 1) * rq + qd}")
            }
        }
    } else {
        for (kg in range(8)) {
            for (qd in range(rq)) {
                let boff = kg * 4 * te.interleave + qd * (te.width / 8)
                var off = boff == 0 ? wb : LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(boff)), "")
                wv[kg * rq + qd] = load_vec(te, te.vwi8, te.wg, off, "wv{kg * rq + qd}")
                uw[kg * rq + qd] = abs_w(te, wv[kg * rq + qd], "uw{kg * rq + qd}")
                if (te.pf != 0 && tokCount == 1) {   // the gemv's weight stream: the same line, pf bytes ahead
                    prefetch_wg(te, LLVMBuildAdd(b, off, te.types->ConstI64(uint64(te.pf)), ""))
                }
            }
        }
    }
    var xv0 : LLVMOpaqueValue? [4]
    var xv1 : LLVMOpaqueValue? [4]
    var xb1 = LLVMBuildAdd(b, xb, te.types->ConstI64(16ul), "")
    for (i in range(tokCount)) {
        xv0[i] = load_v16i8(te, te.x[tokBase + i], xb, "x{tokBase + i}lo")
        xv1[i] = load_v16i8(te, te.x[tokBase + i], xb1, "x{tokBase + i}hi")
    }
    var a : LLVMOpaqueValue? [8]
    if (te.bias == 0) {
        for (i in range(tokCount * rq)) {
            a[i] = LLVMConstNull(te.vni32)
        }
    } else {
        for (i in range(tokCount)) {    // one init per token, shared across the qd accumulators
            var init = bias_acc_init(te, bi, xv0[i], xv1[i], tokBase + i)
            for (qd in range(rq)) {
                a[i * rq + qd] = init
            }
        }
    }
    for (l in range(4)) {           // chunk 0 (k 0..15): lane l of each token vs shared weight vectors
        for (i in range(tokCount)) {
            for (qd in range(rq)) {
                a[i * rq + qd] = dot_lane(te, a[i * rq + qd], wv[l * rq + qd], uw[l * rq + qd], xv0[i], l)
            }
        }
    }
    for (l in range(4)) {           // chunk 1 (k 16..31)
        for (i in range(tokCount)) {
            for (qd in range(rq)) {
                a[i * rq + qd] = dot_lane(te, a[i * rq + qd], wv[(4 + l) * rq + qd], uw[(4 + l) * rq + qd], xv1[i], l)
            }
        }
    }
    emit_scale_fold(te, bi, a, f, tokBase, tokCount)
}

// the fold's a·b+c as @llvm.fmuladd — the SAME intrinsic daslang's mad() lowers to
// (intrinsic_mad in llvm_jit_intrin): generated IR never passes das-level lowering, so the
// generator must license the fusion itself. Falls back to fmul+fadd when unresolvable.
def private fold_fma(var te : TileEmit; var a, b, c : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let bld = te.builder
    let id = LLVMLookupIntrinsicID("llvm.fmuladd")
    if (id != 0u) {
        var mod = LLVMGetGlobalParent(LLVMGetBasicBlockParent(LLVMGetInsertBlock(bld)))
        var tys <- [te.vnf32]
        var decl = LLVMGetIntrinsicDeclaration(mod, id, tys)
        if (decl != null) {
            var fty = LLVMFunctionType(te.vnf32, [te.vnf32, te.vnf32, te.vnf32])
            var args <- [a, b, c]
            return LLVMBuildCall2(bld, fty, decl, args, name)
        }
    }
    return LLVMBuildFAdd(bld, LLVMBuildFMul(bld, a, b, ""), c, name)
}

// One vnf32 group-scale vector at element index `idx` off `base` — the f32-plane load, or
// (sgF16) the raw-binary16 plane widened via bitcast-to-half + fpext. Every weight-scale
// load in the family goes through here, so the two planes differ ONLY in this load.
def private load_scale_vec(var te : TileEmit; var base, idx : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    if (!te.sgF16) {
        var sgp = LLVMBuildGEP2(b, te.types.t_float, base, idx, name)
        return LLVMBuildLoad2Aligned(b, te.vnf32, sgp, 4u, name)
    }
    var sgp = LLVMBuildGEP2(b, te.types.t_int16, base, idx, name)
    var hv = LLVMBuildLoad2Aligned(b, te.vni16, sgp, 2u, name)
    return LLVMBuildFPExt(b, LLVMBuildBitCast(b, hv, te.vnf16, ""), te.vnf32, name)
}

// The per-block Q8 scale fold shared by every lattice: f[acc] += float(a[acc]) * s[qd] *
// splat(q[i]) — the exact op order of dot_q8q8_laneq4x4_template, so every emitter's
// fast-math contraction sees the same input. `a` is token-major (token * rq + row-vector).
def private emit_scale_fold(var te : TileEmit; var bi : LLVMOpaqueValue?; a : LLVMOpaqueValue? [8]; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) {
    let b = te.builder
    let rq = te.rq
    var s4 : LLVMOpaqueValue? [4]
    var sgIdx0 = LLVMBuildMul(b, bi, te.types->ConstI64(uint64(te.interleave)), "")
    for (qd in range(rq)) {
        var idx = qd == 0 ? sgIdx0 : LLVMBuildAdd(b, sgIdx0, te.types->ConstI64(uint64(qd * te.rv)), "")
        s4[qd] = load_scale_vec(te, te.sg, idx, qd == 0 ? "s4" : "s4{qd}")
    }
    for (i in range(tokCount)) {
        let tk = tokBase + i
        var qp = LLVMBuildGEP2(b, te.types.t_float, te.xs[tk], bi, "qp{tk}")
        var q = LLVMBuildLoad2Aligned(b, te.types.t_float, qp, 4u, "q{tk}")
        for (qd in range(rq)) {
            var af = LLVMBuildSIToFP(b, a[i * rq + qd], te.vnf32, "af{tk}")
            var sq = LLVMBuildFMul(b, s4[qd], splat_f32(te, q, "qs{tk}"), "")
            f[i * rq + qd] = fold_fma(te, af, sq, f[i * rq + qd], "f{tk}")
        }
    }
}

// One 32-k block, SMMLA form (the kg8 plane): B operands are row-pair × 8-k loads off the
// plane; A operands pair tokens via zip1/zip2.2d shuffles. The MMA lattice accumulates 2×2
// patches, then transposes to token-major and hands off to the SAME per-block scale fold.
def private emit_block_smmla(var te : TileEmit; var bi : LLVMOpaqueValue?; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) {
    let b = te.builder
    let rq = te.rq                    // row quads — the transpose/fold granularity
    let nrp = te.interleave / 2       // row pairs — the MMA granularity
    let ntp = tokCount / 2            // token pairs
    var wb = LLVMBuildMul(b, bi, te.types->ConstI64(uint64(te.interleave * 0x20)), "wb")
    var xb = LLVMBuildMul(b, bi, te.types->ConstI64(0x20ul), "xb")
    var wv : LLVMOpaqueValue? [16]    // [g * nrp + rp]; 4 * nrp <= 16 under the budget rail
    for (g in range(4)) {
        for (rp in range(nrp)) {
            let boff = g * 8 * te.interleave + rp * 16
            var off = boff == 0 ? wb : LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(boff)), "")
            wv[g * nrp + rp] = load_v16i8(te, te.wg, off, "wv{g * nrp + rp}")
        }
    }
    var xv0 : LLVMOpaqueValue? [4]
    var xv1 : LLVMOpaqueValue? [4]
    var xb1 = LLVMBuildAdd(b, xb, te.types->ConstI64(16ul), "")
    for (i in range(tokCount)) {
        xv0[i] = load_v16i8(te, te.x[tokBase + i], xb, "x{tokBase + i}lo")
        xv1[i] = load_v16i8(te, te.x[tokBase + i], xb1, "x{tokBase + i}hi")
    }
    // A[tp][g] = tokens (2tp, 2tp+1) k-bytes (8g..8g+7): zip1/zip2.2d byte-shuffle shapes
    let mlo <- [for (i in range(16)); i < 8 ? i : 8 + i]        // [0..7, 16..23]
    let mhi <- [for (i in range(16)); i < 8 ? 8 + i : 16 + i]   // [8..15, 24..31]
    var av : LLVMOpaqueValue? [8]     // [tp * 4 + g]; ntp * 4 <= 8
    for (tp in range(ntp)) {
        let i0 = 2 * tp
        av[tp * 4 + 0] = LLVMBuildShuffleVector(b, te.types, xv0[i0], xv0[i0 + 1], mlo, "a{tp}k0")
        av[tp * 4 + 1] = LLVMBuildShuffleVector(b, te.types, xv0[i0], xv0[i0 + 1], mhi, "a{tp}k1")
        av[tp * 4 + 2] = LLVMBuildShuffleVector(b, te.types, xv1[i0], xv1[i0 + 1], mlo, "a{tp}k2")
        av[tp * 4 + 3] = LLVMBuildShuffleVector(b, te.types, xv1[i0], xv1[i0 + 1], mhi, "a{tp}k3")
    }
    var m : LLVMOpaqueValue? [8]      // [tp * nrp + rp]; ntp * nrp <= 8 under the budget rail
    for (i in range(ntp * nrp)) {
        m[i] = LLVMConstNull(te.vni32)
    }
    for (g in range(4)) {
        for (tp in range(ntp)) {
            for (rp in range(nrp)) {
                var margs <- [m[tp * nrp + rp], av[tp * 4 + g], wv[g * nrp + rp]]
                m[tp * nrp + rp] = LLVMBuildCall2(b, te.smmla_ty, te.smmla_decl, margs, "mm")
            }
        }
    }
    // transpose the 2×2 patches to token-major row quads (uzp1.2d / uzp2.2d shapes)
    var a : LLVMOpaqueValue? [8]
    for (tp in range(ntp)) {
        for (q in range(rq)) {
            var mA = m[tp * nrp + 2 * q]
            var mB = m[tp * nrp + 2 * q + 1]
            a[(2 * tp) * rq + q] = LLVMBuildShuffleVector(b, te.types, mA, mB, [0, 1, 4, 5], "at{tokBase + 2 * tp}q{q}")
            a[(2 * tp + 1) * rq + q] = LLVMBuildShuffleVector(b, te.types, mA, mB, [2, 3, 6, 7], "at{tokBase + 2 * tp + 1}q{q}")
        }
    }
    emit_scale_fold(te, bi, a, f, tokBase, tokCount)
}

// constant i8 splat at the weight-vector width
def private splat_i8w(te : TileEmit; v : int) : LLVMOpaqueValue? {
    var elems <- [for (_i in range(te.width / 8)); LLVMConstInt(te.types.t_int8, uint64(v & 255), 0)]
    return LLVMConstVector(array_data_ptr(elems), uint(te.width / 8))
}

// constant i32 splat at the accumulator width
def private splat_i32n(te : TileEmit; v : int) : LLVMOpaqueValue? {
    var elems <- [for (_i in range(te.rv)); LLVMConstInt(te.types.t_int32, uint64(v), 0)]
    return LLVMConstVector(array_data_ptr(elems), uint(te.rv))
}

// the LUT dequant of one nibble-index vector: NEON tbl1 (v16i8 LUT), x64 pshufb (the LUT
// broadcast per 16-byte lane — indices are 0..15 with bit 7 clear, so pshufb's zeroing path
// never fires and its per-lane semantics match the broadcast exactly)
def private lut_lookup(var te : TileEmit; var idx : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    if (te.dotKind == DOT_SDOT) {
        var targs <- [te.lut, idx]
        return LLVMBuildCall2(te.builder, te.tbl_ty, te.tbl_decl, targs, name)
    }
    var pargs <- [te.lut, idx]
    return LLVMBuildCall2(te.builder, te.pshuf_ty, te.pshuf_decl, pargs, name)
}

// One 32-k block, MXFP4 form: per (row vector, dword-group j), ONE nibble load feeds LUT-lo
// and LUT-hi — 4*rq loads + 8*rq lookups replace the Q8 block's 8*rq weight loads; lane-dot
// lattice and fold are the Q8 emitter's. Scale computed in-register from E8M0 bytes (e8m0_half's bit trick).
def private emit_block_mx4(var te : TileEmit; var bi : LLVMOpaqueValue?; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) {
    let b = te.builder
    let rq = te.rq
    var wb = LLVMBuildMul(b, bi, te.types->ConstI64(uint64(te.interleave * 16)), "wb")
    var xb = LLVMBuildMul(b, bi, te.types->ConstI64(0x20ul), "xb")
    var wlo : LLVMOpaqueValue? [8]
    var whi : LLVMOpaqueValue? [8]
    var ulo : LLVMOpaqueValue? [8]
    var uhi : LLVMOpaqueValue? [8]
    for (j in range(4)) {
        for (qd in range(rq)) {
            let boff = j * 4 * te.interleave + qd * (te.width / 8)
            var off = boff == 0 ? wb : LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(boff)), "")
            var nv = load_vec(te, te.vwi8, te.wg, off, "nv{j * rq + qd}")
            var loIdx = LLVMBuildAnd(b, nv, splat_i8w(te, 15), "nib.lo")
            var hiIdx = LLVMBuildLShr(b, nv, splat_i8w(te, 4), "nib.hi")
            wlo[j * rq + qd] = lut_lookup(te, loIdx, "wlo{j * rq + qd}")
            whi[j * rq + qd] = lut_lookup(te, hiIdx, "whi{j * rq + qd}")
            ulo[j * rq + qd] = abs_w(te, wlo[j * rq + qd], "ulo{j * rq + qd}")
            uhi[j * rq + qd] = abs_w(te, whi[j * rq + qd], "uhi{j * rq + qd}")
        }
    }
    var xv0 : LLVMOpaqueValue? [4]
    var xv1 : LLVMOpaqueValue? [4]
    var xb1 = LLVMBuildAdd(b, xb, te.types->ConstI64(16ul), "")
    for (i in range(tokCount)) {
        xv0[i] = load_v16i8(te, te.x[tokBase + i], xb, "x{tokBase + i}lo")
        xv1[i] = load_v16i8(te, te.x[tokBase + i], xb1, "x{tokBase + i}hi")
    }
    var a : LLVMOpaqueValue? [8]
    for (i in range(tokCount * rq)) {
        a[i] = LLVMConstNull(te.vni32)
    }
    for (l in range(4)) {           // chunk 0 (k 0..15): low-nibble weights
        for (i in range(tokCount)) {
            for (qd in range(rq)) {
                a[i * rq + qd] = dot_lane(te, a[i * rq + qd], wlo[l * rq + qd], ulo[l * rq + qd], xv0[i], l)
            }
        }
    }
    for (l in range(4)) {           // chunk 1 (k 16..31): high-nibble weights
        for (i in range(tokCount)) {
            for (qd in range(rq)) {
                a[i * rq + qd] = dot_lane(te, a[i * rq + qd], whi[l * rq + qd], uhi[l * rq + qd], xv1[i], l)
            }
        }
    }
    var vri8 = LLVMVectorType(te.types.t_int8, uint(te.rv))
    var s4 : LLVMOpaqueValue? [4]
    var sgIdx0 = LLVMBuildMul(b, bi, te.types->ConstI64(uint64(te.interleave)), "")
    for (qd in range(rq)) {
        var idx = qd == 0 ? sgIdx0 : LLVMBuildAdd(b, sgIdx0, te.types->ConstI64(uint64(qd * te.rv)), "")
        var egp = LLVMBuildGEP2(b, te.types.t_int8, te.sg, idx, qd == 0 ? "egp" : "egp{qd}")
        var ev = LLVMBuildLoad2Aligned(b, vri8, egp, 1u, qd == 0 ? "e4" : "e4{qd}")
        var e32 = LLVMBuildZExt(b, ev, te.vni32, "")
        var lt2 = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntULT, e32, splat_i32n(te, 2), "")
        var loBits = LLVMBuildShl(b, splat_i32n(te, int(0x00200000)), e32, "")
        var hiBits = LLVMBuildShl(b, LLVMBuildSub(b, e32, splat_i32n(te, 1), ""), splat_i32n(te, 23), "")
        var bits = LLVMBuildSelect(b, lt2, loBits, hiBits, "")
        // name hoisted: a ternary string fed straight to a raw extern's string param SIGSEGVs the compiler
        let s4name = qd == 0 ? "s4" : "s4{qd}"
        s4[qd] = LLVMBuildBitCast(b, bits, te.vnf32, s4name)
    }
    for (i in range(tokCount)) {
        let tk = tokBase + i
        var qp = LLVMBuildGEP2(b, te.types.t_float, te.xs[tk], bi, "qp{tk}")
        var q = LLVMBuildLoad2Aligned(b, te.types.t_float, qp, 4u, "q{tk}")
        for (qd in range(rq)) {
            var af = LLVMBuildSIToFP(b, a[i * rq + qd], te.vnf32, "af{tk}")
            var sq = LLVMBuildFMul(b, s4[qd], splat_f32(te, q, "qs{tk}"), "")
            f[i * rq + qd] = fold_fma(te, af, sq, f[i * rq + qd], "f{tk}")
        }
    }
}

// The qh bit-plane of one width-sized row-group qh vector as 0x10-or-0 bytes for k-group j
// (half 0 = bits 4j..4j+3 per row, half 1 = the +16 chunk): constant-shuffle byte pick +
// and/cmpeq/sext. Width-generic: rv rows per vector, 4 qh bytes per row.
def private qh_bits(var te : TileEmit; var qhv : LLVMOpaqueValue?; j, half : int; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    let wbytes = te.width / 8
    let byteIdx = half * 2 + j / 2
    let shufMask <- [for (lane in range(wbytes)); (lane / 4) * 4 + byteIdx]
    var picked = LLVMBuildShuffleVector(b, te.types, qhv, qhv, shufMask, "{name}p")
    var bitElems <- [for (lane in range(wbytes)); LLVMConstInt(te.types.t_int8, uint64(1 << ((j % 2) * 4 + lane % 4)), 0)]
    var bmask = LLVMConstVector(array_data_ptr(bitElems), uint(wbytes))
    var got = LLVMBuildAnd(b, picked, bmask, "")
    var isSet = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntEQ, got, bmask, "")
    var sx = LLVMBuildSExt(b, isSet, te.vwi8, "")
    return LLVMBuildAnd(b, sx, splat_i8w(te, 16), name)
}

// One 32-k block, q51 form: nibble load ORs qh bits in register, unsigned-lattice dots, fold
// (d·isum + m·asum)·xs in q51_grp_row_dot's op order; width-generic (activations stay 16B)
def private emit_block_q51(var te : TileEmit; var bi : LLVMOpaqueValue?; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) {
    let b = te.builder
    let rq = te.rq
    let wbytes = te.width / 8
    var wb = LLVMBuildMul(b, bi, te.types->ConstI64(uint64(te.interleave * 20)), "wb")
    var xb = LLVMBuildMul(b, bi, te.types->ConstI64(0x20ul), "xb")
    var qh : LLVMOpaqueValue? [2]
    for (qd in range(rq)) {
        var hoff = LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(16 * te.interleave + qd * wbytes)), "")
        qh[qd] = load_vec(te, te.vwi8, te.wg, hoff, "qh{qd}")
    }
    var wlo : LLVMOpaqueValue? [8]
    var whi : LLVMOpaqueValue? [8]
    for (j in range(4)) {
        for (qd in range(rq)) {
            let boff = j * 4 * te.interleave + qd * wbytes
            var off = boff == 0 ? wb : LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(boff)), "")
            var nv = load_vec(te, te.vwi8, te.wg, off, "nv{j * rq + qd}")
            var lo = LLVMBuildAnd(b, nv, splat_i8w(te, 15), "")
            var hi = LLVMBuildLShr(b, nv, splat_i8w(te, 4), "")
            wlo[j * rq + qd] = LLVMBuildOr(b, lo, qh_bits(te, qh[qd], j, 0, "hl{j * rq + qd}"), "wlo{j * rq + qd}")
            whi[j * rq + qd] = LLVMBuildOr(b, hi, qh_bits(te, qh[qd], j, 1, "hh{j * rq + qd}"), "whi{j * rq + qd}")
        }
    }
    var xv0 : LLVMOpaqueValue? [4]
    var xv1 : LLVMOpaqueValue? [4]
    var xb1 = LLVMBuildAdd(b, xb, te.types->ConstI64(16ul), "")
    for (i in range(tokCount)) {
        xv0[i] = load_v16i8(te, te.x[tokBase + i], xb, "x{tokBase + i}lo")
        xv1[i] = load_v16i8(te, te.x[tokBase + i], xb1, "x{tokBase + i}hi")
    }
    var a : LLVMOpaqueValue? [8]
    for (i in range(tokCount * rq)) {
        a[i] = LLVMConstNull(te.vni32)
    }
    for (l in range(4)) {           // chunk 0 (k 0..15): low-nibble weights
        for (i in range(tokCount)) {
            for (qd in range(rq)) {
                a[i * rq + qd] = kq_dot_lane(te, a[i * rq + qd], wlo[l * rq + qd], xv0[i], l)
            }
        }
    }
    for (l in range(4)) {           // chunk 1 (k 16..31): high-nibble weights
        for (i in range(tokCount)) {
            for (qd in range(rq)) {
                a[i * rq + qd] = kq_dot_lane(te, a[i * rq + qd], whi[l * rq + qd], xv1[i], l)
            }
        }
    }
    // fold: f += (d4·isum4 + m4·asum)·xs — d/m interleaved f16 pairs, 2·rv-lane i16 load
    var vdm16 = LLVMVectorType(te.types.t_int16, uint(te.rv * 2))
    var d4 : LLVMOpaqueValue? [2]
    var m4 : LLVMOpaqueValue? [2]
    var sIdx0 = LLVMBuildMul(b, bi, te.types->ConstI64(uint64(4 * te.interleave)), "")
    for (qd in range(rq)) {
        var sIdx = qd == 0 ? sIdx0 : LLVMBuildAdd(b, sIdx0, te.types->ConstI64(uint64(qd * te.rv * 4)), "")
        var sp = LLVMBuildGEP2(b, te.types.t_int8, te.sg, sIdx, "sp{qd}")
        var dm = LLVMBuildLoad2Aligned(b, vdm16, sp, 1u, "dm{qd}")
        var dh = LLVMBuildShuffleVector(b, te.types, dm, dm, [for (lane in range(te.rv)); lane * 2], "")
        var mh = LLVMBuildShuffleVector(b, te.types, dm, dm, [for (lane in range(te.rv)); lane * 2 + 1], "")
        d4[qd] = LLVMBuildFPExt(b, LLVMBuildBitCast(b, dh, te.vnf16, ""), te.vnf32, "d4{qd}")
        m4[qd] = LLVMBuildFPExt(b, LLVMBuildBitCast(b, mh, te.vnf16, ""), te.vnf32, "m4{qd}")
    }
    for (i in range(tokCount)) {
        let tk = tokBase + i
        var bsPtr = LLVMBuildGEP2(b, te.types.t_int32, te.xbs[tk], bi, "bsp{tk}")
        var bsv = LLVMBuildLoad2Aligned(b, te.types.t_int32, bsPtr, 4u, "bs{tk}")
        var asum = splat_f32(te, LLVMBuildSIToFP(b, bsv, te.types.t_float, ""), "as{tk}")
        var qp = LLVMBuildGEP2(b, te.types.t_float, te.xs[tk], bi, "qp{tk}")
        var q = LLVMBuildLoad2Aligned(b, te.types.t_float, qp, 4u, "q{tk}")
        var xsv = splat_f32(te, q, "qs{tk}")
        for (qd in range(rq)) {
            var isumf = LLVMBuildSIToFP(b, a[i * rq + qd], te.vnf32, "af{tk}")
            var mm = LLVMBuildFMul(b, m4[qd], asum, "")
            var s = fold_fma(te, d4[qd], isumf, mm, "s{tk}")
            f[i * rq + qd] = fold_fma(te, s, xsv, f[i * rq + qd], "f{tk}")
        }
    }
}

// One K-quant lane-dot: acc += q · (dword-group `lane` of x, splatted). Quants are UNSIGNED
// (0..15/31/63) — they ARE the u8 side of vpdpbusd/maddubs, and fit s8 non-negatively for
// bssd/sdot, so no |w|/sign-apply/bias plane on any leg. maddubs pair-sums stay exact (2·63·127 < 32767).
def private kq_dot_lane(var te : TileEmit; var acc, qv, xv : LLVMOpaqueValue?; lane : int) : LLVMOpaqueValue? {
    if (te.dotKind == DOT_SDOT || te.dotKind == DOT_SMMLA) {
        return sdot_lane(te, acc, qv, xv, lane)
    }
    let b = te.builder
    var xi32 = LLVMBuildBitCast(b, xv, te.v4i32, "")
    let mask <- [for (_i in range(te.rv)); lane]
    var xspl = LLVMBuildShuffleVector(b, te.types, xi32, xi32, mask, "xlane")
    var xb = LLVMBuildBitCast(b, xspl, te.vwi8, "")
    if (te.dotKind == DOT_MADDUBS) {
        var margs <- [qv, xb]
        var pairs = LLVMBuildCall2(b, te.madd_ty, te.madd_decl, margs, "pairs")
        var wargs <- [pairs, ones_i16(te)]
        var quads = LLVMBuildCall2(b, te.maddwd_ty, te.maddwd_decl, wargs, "quads")
        return LLVMBuildAdd(b, acc, quads, "dot")
    }
    var dargs <- [acc, qv, xb]
    return LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "dot")
}

// Activation dword BROADCAST FROM MEMORY at BYTE offset (x64 legs): the register-splat form
// costs a shuffle uop competing with pmaddubsw on FP1 (Zen2); vpbroadcastd m32 folds the
// splat into the load port instead — bit-identical results, only the port mix changes.
def private bcast_dword(var te : TileEmit; var xp, off : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    var p = LLVMBuildGEP2(b, te.types.t_int8, xp, off, name)
    var s = LLVMBuildLoad2Aligned(b, te.types.t_int32, p, 1u, name)
    var one = LLVMBuildInsertElement(b, LLVMGetUndef(te.vni32), s, te.types->ConstI32(0ul), "")
    let mask <- [for (_i in range(te.rv)); 0]
    var xspl = LLVMBuildShuffleVector(b, te.types, one, one, mask, name)
    return LLVMBuildBitCast(b, xspl, te.vwi8, "")
}

// one K-quant lane-dot over a memory-broadcast activation dword (the non-maddubs x64 legs —
// vpdpbusd/bssd are already one dot uop per 32B; maddubs rides the i16 lattice below instead)
def private kq_dot_mem(var te : TileEmit; var acc, qv, xb : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    if (te.dotKind == DOT_MADDUBS) {
        var margs <- [qv, xb]
        var pairs = LLVMBuildCall2(b, te.madd_ty, te.madd_decl, margs, "pairs")
        var wargs <- [pairs, ones_i16(te)]
        var quads = LLVMBuildCall2(b, te.maddwd_ty, te.maddwd_decl, wargs, "quads")
        return LLVMBuildAdd(b, acc, quads, "dot")
    }
    var dargs <- [acc, qv, xb]
    return LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "dot")
}

// One pmaddubsw into an i16 pair-sum chain (null head = first madd). UNSIGNED-quant pair-sums
// are bounded (k4 ±3810, k5 ±7874, k6 ±16002), so several 32B dots accumulate EXACTLY in i16
// before ONE pmaddwd widens — cuts the maddubs lattice from 3 uops/32B to ~2.1 (k4).
def private madd16_acc(var te : TileEmit; var chain, qv, xb : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    var margs <- [qv, xb]
    var pairs = LLVMBuildCall2(b, te.madd_ty, te.madd_decl, margs, "pairs")
    if (chain == null) return pairs
    return LLVMBuildAdd(b, chain, pairs, "p16")
}

// widen an i16 chain into the i32 accumulator (pmaddwd against ones) — the chain flush
def private madd16_flush(var te : TileEmit; var acc, chain : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    var wargs <- [chain, ones_i16(te)]
    var quads = LLVMBuildCall2(b, te.maddwd_ty, te.maddwd_decl, wargs, "quads")
    return LLVMBuildAdd(b, acc, quads, "a32")
}

//! the rows' sub-scales (k6/k3 int8, k4/k5 unsigned 6-bit) as an i16 pair splat (lanes 2r, 2r + 1 = row r): pmaddwd against a chain widens AND scales in one op
def private scale16_dup(var te : TileEmit; var off : LLVMOpaqueValue?; signed : bool; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    var vri8 = LLVMVectorType(te.types.t_int8, uint(te.rv))
    var p = LLVMBuildGEP2(b, te.types.t_int8, te.sg, off, "")
    var s8 = LLVMBuildLoad2Aligned(b, vri8, p, 1u, "")
    var s16 = signed ? LLVMBuildSExt(b, s8, te.vni16, "") : LLVMBuildZExt(b, s8, te.vni16, "")
    let dup <- [for (i in range(2 * te.rv)); i / 2]
    return LLVMBuildShuffleVector(b, te.types, s16, s16, dup, name)
}

//! the chain flush with the sub-scale folded in - exact in i32 (|chain| <= 32004, |scale| <= 128, 16 flushes per superblock)
def private madd16_flush_scaled(var te : TileEmit; var acc, chain, s16 : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    var wargs <- [chain, s16]
    var quads = LLVMBuildCall2(b, te.maddwd_ty, te.maddwd_decl, wargs, "squads")
    return LLVMBuildAdd(b, acc, quads, "a32s")
}

// one <rv x i8> row-byte load at BYTE offset, each byte splatted across its row's 4 lanes to
// vwi8 (constant shufflevector — the backend picks tbl/pshufb itself; no intrinsic needed)
def private load_row_bytes_x4(var te : TileEmit; var base, off : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    var vri8 = LLVMVectorType(te.types.t_int8, uint(te.rv))
    var p = LLVMBuildGEP2(te.builder, te.types.t_int8, base, off, name)
    var v = LLVMBuildLoad2Aligned(te.builder, vri8, p, 1u, name)
    let mask <- [for (i in range(te.width / 8)); i / 4]
    return LLVMBuildShuffleVector(te.builder, te.types, v, v, mask, name)
}

// one <rv x i16> binary16 load at BYTE offset off, widened to vnf32 (the kq scale planes are
// byte-addressed; fcvtl / vcvtph2ps lowering, same as load_scale_vec's sgF16 arm)
def private load_f16_vec_at(var te : TileEmit; var base, off : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    var p = LLVMBuildGEP2(b, te.types.t_int8, base, off, name)
    var hv = LLVMBuildLoad2Aligned(b, te.vni16, p, 2u, name)
    return LLVMBuildFPExt(b, LLVMBuildBitCast(b, hv, te.vnf16, ""), te.vnf32, name)
}

// pairwise i32 fold of two bsum quads into one v4 of per-32 sums (even+odd shuffle + add —
// the backend lowers the pair to a single addp)
def private pairwise_add_i32(var te : TileEmit; var av, bv : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    let evens <- [for (i in range(te.rv)); 2 * i]
    let odds <- [for (i in range(te.rv)); 2 * i + 1]
    var e = LLVMBuildShuffleVector(b, te.types, av, bv, evens, "")
    var o = LLVMBuildShuffleVector(b, te.types, av, bv, odds, "")
    return LLVMBuildAdd(b, e, o, name)
}

// splat one lane of a v4i32 across the accumulator width (dup .4s[lane] after lowering)
def private splat_lane_i32(var te : TileEmit; var v : LLVMOpaqueValue?; lane : int; name : string) : LLVMOpaqueValue? {
    let mask <- [for (_i in range(te.rv)); lane]
    return LLVMBuildShuffleVector(te.builder, te.types, v, v, mask, name)
}

// or-in of a masked bit test as the nibble's 0x10: w |= (bytes & mask) != 0 ? 0x10 : 0
// (pcmpeqb+pand shape after lowering — the k5 high-bit deposit). NEON shifts bytes per lane, so the sdot
// lattice moves bit t (lo) / 4 + t (hi) straight onto bit 4: one ushl / ushr and one and
def private or_bit_x10(te : TileEmit; var w, bytes, maskv : LLVMOpaqueValue?; hi : bool; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    if (te.dotKind == DOT_SDOT) {
        let w8 = te.width / 8
        var shifts <- [for (i in range(w8)); LLVMConstInt(te.types.t_int8, uint64(hi ? i % 4 : 4 - i % 4), 0)]
        var shv = LLVMConstVector(array_data_ptr(shifts), uint(w8))
        var moved = hi ? LLVMBuildLShr(b, bytes, shv, "") : LLVMBuildShl(b, bytes, shv, "")
        return LLVMBuildOr(b, w, LLVMBuildAnd(b, moved, splat_i8w(te, 16), ""), name)
    }
    var hit = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntNE, LLVMBuildAnd(b, bytes, maskv, ""), LLVMConstNull(te.vwi8), "")
    var sel = LLVMBuildSelect(b, hit, splat_i8w(te, 16), LLVMConstNull(te.vwi8), name)
    return LLVMBuildOr(b, w, sel, name)
}

// One 256-weight SUPERBLOCK, K-quant grp<mr> form (te.kq set): weight vectors unpacked
// ONCE per (sub-block, dword-group) and dotted against every token — weight-stationary,
// bit-exact vs per-token GEMVs (tokCount=1). kq v2 folds integer sub-scales/bsums against Q8_K-form activations.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kq-subblock-planes")]
def private emit_block_kqv2(var te : TileEmit; var sbi : LLVMOpaqueValue?; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) { // nolint:STYLE037,STYLE038 — one register-coupled superblock loop nest; the k-quant decode arms share live values
    let b = te.builder
    let rq = te.rq
    let mr = te.interleave
    let w8 = te.width / 8
    let k6 = te.kq == 6
    let k3 = te.kq == 3
    let k2 = te.kq == 2
    let k63 = k6 || k3          // k3 rides k6's scale row and fold; only the quant compose and the offset differ
    let per16 = k63 || k2       // per-16 scale granularity: split lo/hi accumulators, unsummed bs0/bs1
    let wholeSub = te.kq == 4 || te.kq == 40 || k3 || k2   // the i16 chains run the whole sub-block before one widen (k5: 4 madds, k6: 2)
    let koff = k3 ? 4 : 32      // the signed-lattice offset the bsum fold removes
    let q40 = te.kq == 40
    let qsb = (te.kq == 4 || q40) ? 128 : (k3 ? 96 : (k2 ? 64 : (te.kqBytes ? 256 : (te.kq == 5 ? 160 : 192))))
    var wb = LLVMBuildMul(b, sbi, te.types->ConstI64(uint64(mr * qsb)), "wb")
    var sb = LLVMBuildMul(b, sbi, te.types->ConstI64(uint64(mr * (k63 ? 18 : (q40 ? 16 : 20)))), "sb")
    var xb = LLVMBuildMul(b, sbi, te.types->ConstI64(0x100ul), "xb")
    var gb = LLVMBuildShl(b, sbi, te.types->ConstI64(3ul), "gb")
    var maskLoElems <- [for (i in range(w8)); LLVMConstInt(te.types.t_int8, uint64(1 << (i % 4)), 0)]
    var maskHiElems <- [for (i in range(w8)); LLVMConstInt(te.types.t_int8, uint64(16 << (i % 4)), 0)]
    var maskLo = LLVMConstVector(array_data_ptr(maskLoElems), uint(w8))
    var maskHi = LLVMConstVector(array_data_ptr(maskHiElems), uint(w8))
    // x64 dot kinds take the activation dword straight from memory (vpbroadcastd, see kq_dot_mem); NEON keeps chunk loads
    let memBcast = te.dotKind != DOT_SDOT && te.dotKind != DOT_SMMLA
    let madd16 = memBcast && te.dotKind == DOT_MADDUBS
    let fuseScale = madd16 && (k63 || te.kq == 5)   // the sub-scale rides the chain flush (pmaddwd): no i32 multiply, no a0/a1; k4 stays unfused - one flush per sub-block loses more than the fold saves
    var vri8 = LLVMVectorType(te.types.t_int8, uint(te.rv))
    // weight-side superblock scale rows: k4/k5 f16 d/dmin; k6/k3 f16 d only; q40 has none (per-32 d loads live below)
    var dv : LLVMOpaqueValue? [2]
    var mv : LLVMOpaqueValue? [2]
    if (!q40) {
        for (qd in range(rq)) {
            if (k63 || k2) {
                dv[qd] = load_f16_vec_at(te, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(16 * mr + 2 * (qd * te.rv))), ""), "d{qd}")
                if (k2) {
                    mv[qd] = load_f16_vec_at(te, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(18 * mr + 2 * (qd * te.rv))), ""), "dm{qd}")
                }
            } else {
                dv[qd] = load_f16_vec_at(te, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(2 * (qd * te.rv))), ""), "d{qd}")
                mv[qd] = load_f16_vec_at(te, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(2 * mr + 2 * (qd * te.rv))), ""), "dm{qd}")
            }
        }
    }
    var iacc : LLVMOpaqueValue? [8]
    var bacc : LLVMOpaqueValue? [8]
    var facc : LLVMOpaqueValue? [8]   // q40: float superblock accumulators (per-block d fold)
    for (i in range(tokCount * rq)) {
        iacc[i] = LLVMConstNull(te.vni32)
        bacc[i] = LLVMConstNull(te.vni32)
        facc[i] = LLVMConstNull(te.vnf32)
    }
    // vector bsums: sdot leg only (x64's vpbroadcastd makes scalars free; k6 needs bs0/bs1 unsummed)
    let vecBsums = !memBcast && !per16
    // mr>=8 fuses lo+hi into ONE acc (i32-exact — the fold adds them anyway); mr4 keeps split accs for ILP
    let fuseAcc = !memBcast && mr >= 8 && !per16
    var a32v8 : LLVMOpaqueValue? [8]   // [tok * 2 + blk / 4], lane blk % 4
    if (vecBsums) {
        var xb16 = LLVMBuildShl(b, sbi, te.types->ConstI64(4ul), "xb16")
        for (i in range(tokCount)) {
            let tk = tokBase + i
            var q : LLVMOpaqueValue? [4]
            for (q4 in range(4)) {
                var bp = LLVMBuildGEP2(b, te.types.t_int32, te.xbs[tk], LLVMBuildAdd(b, xb16, te.types->ConstI64(uint64(q4 * 4)), ""), "bsp{tk}_{q4}")
                q[q4] = LLVMBuildLoad2Aligned(b, te.vni32, bp, 4u, "bsv{tk}_{q4}")
            }
            a32v8[i * 2] = pairwise_add_i32(te, q[0], q[1], "a32lo{tk}")
            a32v8[i * 2 + 1] = pairwise_add_i32(te, q[2], q[3], "a32hi{tk}")
        }
    }
    for (blk in range(8)) {
        var xoff0 = LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32)), "")
        var xoff1 = LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32 + 16)), "")
        var xv0 : LLVMOpaqueValue? [4]
        var xv1 : LLVMOpaqueValue? [4]
        if (!memBcast) {
            for (i in range(tokCount)) {
                xv0[i] = load_v16i8(te, te.x[tokBase + i], xoff0, "x{tokBase + i}_{blk}lo")
                xv1[i] = load_v16i8(te, te.x[tokBase + i], xoff1, "x{tokBase + i}_{blk}hi")
            }
        }
        var a0 : LLVMOpaqueValue? [8]     // [token * rq + qd]; tokCount*rq <= 8 (nrsplit*vq rail)
        var a1 : LLVMOpaqueValue? [8]
        for (i in range(tokCount * rq)) {
            a0[i] = LLVMConstNull(te.vni32)
            a1[i] = LLVMConstNull(te.vni32)
        }
        // the maddubs i16 lattice (see madd16_acc): k4/k5 combine lo+hi in one chain; k6 keeps them apart (per-16 scales)
        var p16lo : LLVMOpaqueValue? [8]
        var p16hi : LLVMOpaqueValue? [8]
        for (i in range(tokCount * rq)) {
            p16lo[i] = null
            p16hi[i] = null
        }
        var s16lo : LLVMOpaqueValue? [2]   // fuseScale: this sub-block's two sub-scales as i16 pair splats
        var s16hi : LLVMOpaqueValue? [2]
        if (fuseScale && k63) {
            for (qd in range(rq)) {
                s16lo[qd] = scale16_dup(te, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(2 * blk * mr + qd * te.rv)), ""), true, "s16{blk}_{qd}a")
                s16hi[qd] = scale16_dup(te, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64((2 * blk + 1) * mr + qd * te.rv)), ""), true, "s16{blk}_{qd}b")
            }
        } elif (fuseScale) {   // k5: one 6-bit scale per sub-block, one chain
            for (qd in range(rq)) {
                s16lo[qd] = scale16_dup(te, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(4 * mr + blk * mr + qd * te.rv)), ""), false, "s16{blk}_{qd}")
            }
        }
        for (j in range(4)) {
            for (qd in range(rq)) {
                // weight vectors unpacked ONCE, dotted against every token below (kqBytes panel loads verbatim instead)
                var noff = LLVMBuildAdd(b, wb, te.types->ConstI64(uint64((blk * 16 + j * 4) * mr + qd * w8)), "")
                var wlo : LLVMOpaqueValue?
                var whi : LLVMOpaqueValue?
                if (k3) {
                    var q3l = load_vec(te, te.vwi8, te.wg, LLVMBuildAdd(b, wb, te.types->ConstI64(uint64((blk * 2) * 4 * mr + qd * w8)), ""), "q3l{blk}_{qd}")
                    var q3h = load_vec(te, te.vwi8, te.wg, LLVMBuildAdd(b, wb, te.types->ConstI64(uint64((blk * 2 + 1) * 4 * mr + qd * w8)), ""), "q3h{blk}_{qd}")
                    var h3 = load_vec(te, te.vwi8, te.wg, LLVMBuildAdd(b, wb, te.types->ConstI64(uint64((64 + blk * 4) * mr + qd * w8)), ""), "h3{blk}_{qd}")
                    var lo2 = LLVMBuildAnd(b, LLVMBuildLShr(b, q3l, splat_i8w(te, 2 * j), ""), splat_i8w(te, 3), "")
                    var hi2 = LLVMBuildAnd(b, LLVMBuildLShr(b, q3h, splat_i8w(te, 2 * j), ""), splat_i8w(te, 3), "")
                    var lob = LLVMBuildShl(b, LLVMBuildAnd(b, LLVMBuildLShr(b, h3, splat_i8w(te, j), ""), splat_i8w(te, 1), ""), splat_i8w(te, 2), "")
                    var hib = LLVMBuildShl(b, LLVMBuildAnd(b, LLVMBuildLShr(b, h3, splat_i8w(te, 4 + j), ""), splat_i8w(te, 1), ""), splat_i8w(te, 2), "")
                    wlo = LLVMBuildOr(b, lo2, lob, "wlo3{blk}_{j * rq + qd}")
                    whi = LLVMBuildOr(b, hi2, hib, "whi3{blk}_{j * rq + qd}")
                } elif (k2) {
                    // k2: qs column (blk/4)*8 + j (lo 16) / + 4 (hi 16), 2-bit lane at shift 2*(blk%4) - k3's walk minus the hmask
                    let c0 = (blk / 4) * 8 + j
                    let sh = (blk % 4) * 2
                    var q2l = load_vec(te, te.vwi8, te.wg, LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(c0 * 4 * mr + qd * w8)), ""), "q2l{blk}_{j * rq + qd}")
                    var q2h = load_vec(te, te.vwi8, te.wg, LLVMBuildAdd(b, wb, te.types->ConstI64(uint64((c0 + 4) * 4 * mr + qd * w8)), ""), "q2h{blk}_{j * rq + qd}")
                    wlo = LLVMBuildAnd(b, LLVMBuildLShr(b, q2l, splat_i8w(te, sh), ""), splat_i8w(te, 3), "wlo2{blk}_{j * rq + qd}")
                    whi = LLVMBuildAnd(b, LLVMBuildLShr(b, q2h, splat_i8w(te, sh), ""), splat_i8w(te, 3), "whi2{blk}_{j * rq + qd}")
                } elif (te.kqBytes) {
                    var hoff2 = LLVMBuildAdd(b, noff, te.types->ConstI64(uint64(128 * mr)), "")
                    wlo = load_vec(te, te.vwi8, te.wg, noff, "wlo{blk}_{j * rq + qd}")
                    whi = load_vec(te, te.vwi8, te.wg, hoff2, "whi{blk}_{j * rq + qd}")
                } else {
                    var nv = load_vec(te, te.vwi8, te.wg, noff, "nv{blk}_{j * rq + qd}")
                    wlo = LLVMBuildAnd(b, nv, splat_i8w(te, 15), "wlo{blk}_{j * rq + qd}")
                    whi = LLVMBuildLShr(b, nv, splat_i8w(te, 4), "whi{blk}_{j * rq + qd}")
                }
                if (te.kq == 5 && !te.kqBytes) {
                    var hoff = LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(128 * mr + (blk * 4 + j) * mr + qd * te.rv)), "")
                    var hb = load_row_bytes_x4(te, te.wg, hoff, "qh{blk}_{j * rq + qd}")
                    wlo = or_bit_x10(te, wlo, hb, maskLo, false, "wlo5{blk}_{j * rq + qd}")
                    whi = or_bit_x10(te, whi, hb, maskHi, true, "whi5{blk}_{j * rq + qd}")
                } elif (k6 && !te.kqBytes) {
                    let hbase = 128 * mr + blk * 2 * 4 * mr
                    var hloOff = LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(hbase + qd * w8)), "")
                    var hhiOff = LLVMBuildAdd(b, wb, te.types->ConstI64(uint64(hbase + 4 * mr + qd * w8)), "")
                    var qhlo = load_vec(te, te.vwi8, te.wg, hloOff, "qhl{blk}_{qd}")
                    var qhhi = load_vec(te, te.vwi8, te.wg, hhiOff, "qhh{blk}_{qd}")
                    var dlo = LLVMBuildAnd(b, LLVMBuildLShr(b, qhlo, splat_i8w(te, 2 * j), ""), splat_i8w(te, 3), "")
                    var dhi = LLVMBuildAnd(b, LLVMBuildLShr(b, qhhi, splat_i8w(te, 2 * j), ""), splat_i8w(te, 3), "")
                    wlo = LLVMBuildOr(b, wlo, LLVMBuildShl(b, dlo, splat_i8w(te, 4), ""), "wlo6{blk}_{j * rq + qd}")
                    whi = LLVMBuildOr(b, whi, LLVMBuildShl(b, dhi, splat_i8w(te, 4), ""), "whi6{blk}_{j * rq + qd}")
                }
                if (memBcast) {
                    var doff0 = LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32 + j * 4)), "")
                    var doff1 = LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32 + 16 + j * 4)), "")
                    for (i in range(tokCount)) {
                        var xlo = bcast_dword(te, te.x[tokBase + i], doff0, "xd{tokBase + i}_{blk}_{j}a")
                        var xhi = bcast_dword(te, te.x[tokBase + i], doff1, "xd{tokBase + i}_{blk}_{j}b")
                        if (madd16) {
                            let k = i * rq + qd
                            if (per16) {
                                p16lo[k] = madd16_acc(te, p16lo[k], wlo, xlo)
                                p16hi[k] = madd16_acc(te, p16hi[k], whi, xhi)
                            } else {
                                p16lo[k] = madd16_acc(te, madd16_acc(te, p16lo[k], wlo, xlo), whi, xhi)
                            }
                        } else {
                            a0[i * rq + qd] = kq_dot_mem(te, a0[i * rq + qd], wlo, xlo)
                            a1[i * rq + qd] = kq_dot_mem(te, a1[i * rq + qd], whi, xhi)
                        }
                    }
                } else {
                    for (i in range(tokCount)) {
                        if (fuseAcc) {
                            a0[i * rq + qd] = kq_dot_lane(te, kq_dot_lane(te, a0[i * rq + qd], wlo, xv0[i], j), whi, xv1[i], j)
                        } else {
                            a0[i * rq + qd] = kq_dot_lane(te, a0[i * rq + qd], wlo, xv0[i], j)
                            a1[i * rq + qd] = kq_dot_lane(te, a1[i * rq + qd], whi, xv1[i], j)
                        }
                    }
                }
            }
            // i16 chain flushes at the format's overflow bound: k5 after 4 madds, k6 after 2 (2 x 16002); wholeSub formats run the sub-block
            if (madd16 && (j == 1 || j == 3) && !wholeSub) {
                for (k in range(tokCount * rq)) {
                    if (fuseScale) {
                        iacc[k] = madd16_flush_scaled(te, iacc[k], p16lo[k], s16lo[k % rq])
                        if (per16) {
                            iacc[k] = madd16_flush_scaled(te, iacc[k], p16hi[k], s16hi[k % rq])
                            p16hi[k] = null
                        }
                    } else {
                        a0[k] = madd16_flush(te, a0[k], p16lo[k])
                        if (per16) {
                            a1[k] = madd16_flush(te, a1[k], p16hi[k])
                            p16hi[k] = null
                        }
                    }
                    p16lo[k] = null
                }
            }
        }
        if (madd16 && wholeSub) {
            for (k in range(tokCount * rq)) {
                if (fuseScale) {
                    iacc[k] = madd16_flush_scaled(te, iacc[k], p16lo[k], s16lo[k % rq])
                    if (per16) {
                        iacc[k] = madd16_flush_scaled(te, iacc[k], p16hi[k], s16hi[k % rq])
                        p16hi[k] = null
                    }
                } else {
                    a0[k] = madd16_flush(te, a0[k], p16lo[k])
                    if (per16) {
                        a1[k] = madd16_flush(te, a1[k], p16hi[k])
                        p16hi[k] = null
                    }
                }
                p16lo[k] = null
            }
        }
        // the integer fold — no float ops per block, except q40 (its per-32 f16 d admits no cross-block integer fold)
        var scv : LLVMOpaqueValue? [2]
        var mnv : LLVMOpaqueValue? [2]
        var mnlo : LLVMOpaqueValue? [2]   // k2: the two groups' min nibbles (scv/mnv carry their sc nibbles)
        var mnhi : LLVMOpaqueValue? [2]
        var dvb : LLVMOpaqueValue? [2]
        for (qd in range(rq)) {
            if (q40) {
                dvb[qd] = load_f16_vec_at(te, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(blk * 2 * mr + 2 * (qd * te.rv))), ""), "d{blk}_{qd}")
            } elif (k63) {
                var sc0p = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(2 * blk * mr + qd * te.rv)), ""), "scp{blk}_{qd}a")
                var sc1p = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64((2 * blk + 1) * mr + qd * te.rv)), ""), "scp{blk}_{qd}b")
                scv[qd] = LLVMBuildSExt(b, LLVMBuildLoad2Aligned(b, vri8, sc0p, 1u, ""), te.vni32, "sc{blk}_{qd}a")
                mnv[qd] = LLVMBuildSExt(b, LLVMBuildLoad2Aligned(b, vri8, sc1p, 1u, ""), te.vni32, "sc{blk}_{qd}b")
            } elif (k2) {
                // per-16 nibble pairs: byte 2blk = the lo group's sc|mn<<4, byte 2blk+1 = the hi group's
                var b0p = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(2 * blk * mr + qd * te.rv)), ""), "k2p{blk}_{qd}a")
                var b1p = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64((2 * blk + 1) * mr + qd * te.rv)), ""), "k2p{blk}_{qd}b")
                var z0 = LLVMBuildZExt(b, LLVMBuildLoad2Aligned(b, vri8, b0p, 1u, ""), te.vni32, "k2z{blk}_{qd}a")
                var z1 = LLVMBuildZExt(b, LLVMBuildLoad2Aligned(b, vri8, b1p, 1u, ""), te.vni32, "k2z{blk}_{qd}b")
                scv[qd] = LLVMBuildAnd(b, z0, splat_i32n(te, 15), "sc2{blk}_{qd}a")
                mnv[qd] = LLVMBuildAnd(b, z1, splat_i32n(te, 15), "sc2{blk}_{qd}b")
                mnlo[qd] = LLVMBuildLShr(b, z0, splat_i32n(te, 4), "mn2{blk}_{qd}a")
                mnhi[qd] = LLVMBuildLShr(b, z1, splat_i32n(te, 4), "mn2{blk}_{qd}b")
            } else {
                var scp = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(4 * mr + blk * mr + qd * te.rv)), ""), "scp{blk}_{qd}")
                scv[qd] = LLVMBuildZExt(b, LLVMBuildLoad2Aligned(b, vri8, scp, 1u, ""), te.vni32, "sc{blk}_{qd}")
                if (!vecBsums) {   // mins feed the inline bacc fold; the vector-bsum leg folds them in the epilogue instead
                    var mnp = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(12 * mr + blk * mr + qd * te.rv)), ""), "mnp{blk}_{qd}")
                    mnv[qd] = LLVMBuildZExt(b, LLVMBuildLoad2Aligned(b, vri8, mnp, 1u, ""), te.vni32, "mn{blk}_{qd}")
                }
            }
        }
        var bIdx = LLVMBuildAdd(b, gb, te.types->ConstI64(uint64(blk)), "b{blk}")
        var bi2 = LLVMBuildShl(b, bIdx, te.types->ConstI64(1ul), "")
        var bi2b = LLVMBuildAdd(b, bi2, te.types->ConstI64(1ul), "")
        for (i in range(tokCount)) {
            let tk = tokBase + i
            var bs0 : LLVMOpaqueValue?
            var bs1 : LLVMOpaqueValue?
            var a32v : LLVMOpaqueValue?
            if (vecBsums) {
                a32v = splat_lane_i32(te, a32v8[i * 2 + blk / 4], blk % 4, "a32v{tk}_{blk}")
            } else {
                var bp0 = LLVMBuildGEP2(b, te.types.t_int32, te.xbs[tk], bi2, "bp{tk}_{blk}a")
                bs0 = LLVMBuildLoad2Aligned(b, te.types.t_int32, bp0, 4u, "bs{tk}_{blk}a")
                var bp1 = LLVMBuildGEP2(b, te.types.t_int32, te.xbs[tk], bi2b, "bp{tk}_{blk}b")
                bs1 = LLVMBuildLoad2Aligned(b, te.types.t_int32, bp1, 4u, "bs{tk}_{blk}b")
                if (!per16) {
                    a32v = splat_i32(te, LLVMBuildAdd(b, bs0, bs1, "a32{tk}_{blk}"), "a32v{tk}_{blk}")
                }
            }
            if (q40) {
                for (qd in range(rq)) {
                    var acc = fuseAcc ? a0[i * rq + qd] : LLVMBuildAdd(b, a0[i * rq + qd], a1[i * rq + qd], "acc{tk}_{blk}_{qd}")
                    var isub = LLVMBuildSub(b, acc, LLVMBuildMul(b, a32v, splat_i32n(te, 8), ""), "is{tk}_{blk}_{qd}")
                    facc[i * rq + qd] = fold_fma(te, LLVMBuildSIToFP(b, isub, te.vnf32, ""), dvb[qd], facc[i * rq + qd], "fa{tk}_{blk}_{qd}")
                }
            } elif (k63) {
                var bs0v = splat_i32(te, bs0, "bs0v{tk}_{blk}")
                var bs1v = splat_i32(te, bs1, "bs1v{tk}_{blk}")
                for (qd in range(rq)) {
                    if (!fuseScale) {   // the fused flushes already scaled the dots into iacc
                        var ia = LLVMBuildAdd(b, LLVMBuildMul(b, scv[qd], a0[i * rq + qd], ""), LLVMBuildMul(b, mnv[qd], a1[i * rq + qd], ""), "ia6{tk}_{blk}_{qd}")
                        iacc[i * rq + qd] = LLVMBuildAdd(b, iacc[i * rq + qd], ia, "ia{tk}_{blk}_{qd}")
                    }
                    var ba = LLVMBuildAdd(b, LLVMBuildMul(b, scv[qd], bs0v, ""), LLVMBuildMul(b, mnv[qd], bs1v, ""), "ba6{tk}_{blk}_{qd}")
                    bacc[i * rq + qd] = LLVMBuildAdd(b, bacc[i * rq + qd], ba, "ba{tk}_{blk}_{qd}")
                }
            } elif (k2) {
                // ia = sc_lo*dot_lo + sc_hi*dot_hi; ba = mn_lo*bs0 + mn_hi*bs1 - the k4/k5 epilogue folds bacc on dmin
                var bs0v = splat_i32(te, bs0, "bs0v{tk}_{blk}")
                var bs1v = splat_i32(te, bs1, "bs1v{tk}_{blk}")
                for (qd in range(rq)) {
                    var ia = LLVMBuildAdd(b, LLVMBuildMul(b, scv[qd], a0[i * rq + qd], ""), LLVMBuildMul(b, mnv[qd], a1[i * rq + qd], ""), "ia2{tk}_{blk}_{qd}")
                    var ba = LLVMBuildAdd(b, LLVMBuildMul(b, mnlo[qd], bs0v, ""), LLVMBuildMul(b, mnhi[qd], bs1v, ""), "ba2{tk}_{blk}_{qd}")
                    iacc[i * rq + qd] = LLVMBuildAdd(b, iacc[i * rq + qd], ia, "ia{tk}_{blk}_{qd}k2")
                    bacc[i * rq + qd] = LLVMBuildAdd(b, bacc[i * rq + qd], ba, "ba{tk}_{blk}_{qd}k2")
                }
            } else {
                for (qd in range(rq)) {
                    if (!fuseScale) {   // the fused flushes already scaled the dots into iacc
                        var acc = fuseAcc ? a0[i * rq + qd] : LLVMBuildAdd(b, a0[i * rq + qd], a1[i * rq + qd], "acc{tk}_{blk}_{qd}")
                        iacc[i * rq + qd] = LLVMBuildAdd(b, iacc[i * rq + qd], LLVMBuildMul(b, scv[qd], acc, ""), "ia{tk}_{blk}_{qd}")
                    }
                    if (!vecBsums) {
                        bacc[i * rq + qd] = LLVMBuildAdd(b, bacc[i * rq + qd], LLVMBuildMul(b, mnv[qd], a32v, ""), "ba{tk}_{blk}_{qd}")
                    }
                }
            }
        }
    }
    // epilogue min-bias fold (k4/k5 vector-bsum leg): bit-exact i32, keeps bacc out of the block loop's register set
    if (vecBsums && !q40) {
        for (blk in range(8)) {
            for (qd in range(rq)) {
                var mnp = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(12 * mr + blk * mr + qd * te.rv)), ""), "emnp{blk}_{qd}")
                var mnq = LLVMBuildZExt(b, LLVMBuildLoad2Aligned(b, vri8, mnp, 1u, ""), te.vni32, "emn{blk}_{qd}")
                for (i in range(tokCount)) {
                    var a32v = splat_lane_i32(te, a32v8[i * 2 + blk / 4], blk % 4, "ea32{tokBase + i}_{blk}")
                    bacc[i * rq + qd] = LLVMBuildAdd(b, bacc[i * rq + qd], LLVMBuildMul(b, mnq, a32v, ""), "eba{tokBase + i}_{blk}_{qd}")
                }
            }
        }
    }
    // superblock fold: k4/k5 iacc·d8 − bacc·dmin·d8; k6/k3 (iacc−off·bacc)·d·d8 exact int32 (off 32 / 4); q40 facc·d8
    for (i in range(tokCount)) {
        let tk = tokBase + i
        var qp = LLVMBuildGEP2(b, te.types.t_float, te.xs[tk], sbi, "qp{tk}")
        var d8 = LLVMBuildLoad2Aligned(b, te.types.t_float, qp, 4u, "d8{tk}")
        var d8v = splat_f32(te, d8, "d8v{tk}")
        for (qd in range(rq)) {
            if (q40) {
                f[i * rq + qd] = fold_fma(te, facc[i * rq + qd], d8v, f[i * rq + qd], "f{tk}_{qd}s")
                continue
            }
            var ds = LLVMBuildFMul(b, dv[qd], d8v, "ds{tk}_{qd}")
            if (k63) {
                var isub = LLVMBuildSub(b, iacc[i * rq + qd], LLVMBuildMul(b, bacc[i * rq + qd], splat_i32n(te, koff), ""), "is{tk}_{qd}")
                f[i * rq + qd] = fold_fma(te, LLVMBuildSIToFP(b, isub, te.vnf32, ""), ds, f[i * rq + qd], "f{tk}_{qd}s")
            } else {
                var ms = LLVMBuildFNeg(b, LLVMBuildFMul(b, mv[qd], d8v, ""), "ms{tk}_{qd}")
                f[i * rq + qd] = fold_fma(te, LLVMBuildSIToFP(b, iacc[i * rq + qd], te.vnf32, ""), ds, f[i * rq + qd], "f{tk}_{qd}s")
                f[i * rq + qd] = fold_fma(te, LLVMBuildSIToFP(b, bacc[i * rq + qd], te.vnf32, ""), ms, f[i * rq + qd], "f{tk}_{qd}m")
            }
        }
    }
}

// The iq3s gemv gather global: the grid as a [512 x i32] private constant.
def private iq3s_emit_globals(var te : TileEmit; mod : LLVMOpaqueModule?) {
    var grid = LLVMGetNamedGlobal(mod, "dasllama.iq3s.grid")
    if (grid == null) {
        let g = iq3s_grid()
        var elems : array<LLVMOpaqueValue?>
        elems |> reserve(512)
        for (i in range(512)) {
            elems |> push(LLVMConstInt(te.types.t_int32, uint64(g[i]), 0))
        }
        grid = LLVMAddGlobal(mod, LLVMArrayType(te.types.t_int32, 512u), "dasllama.iq3s.grid")
        LLVMSetInitializer(grid, LLVMConstArray(te.types.t_int32, array_data_ptr(elems), 512u))
        LLVMSetGlobalConstant(grid, 1)
        LLVMSetLinkage(grid, LLVMLinkage.LLVMPrivateLinkage)
        LLVMSetAlignment(grid, 64u)
    }
    te.gridGlobal = grid
}

// The five grid formats' per-dword decode - the raw grid word of the row whose packed grp region
// starts at byte `base`, for block `blk`, sign group `l`, half `h`; loads shared across (blk, l, h) fold in LLVM's CSE.
def private wg_byte(var te : TileEmit; var base : LLVMOpaqueValue?; off : int) : LLVMOpaqueValue? {
    var p = LLVMBuildGEP2(te.builder, te.types.t_int8, te.wg, LLVMBuildAdd(te.builder, base, te.types->ConstI64(uint64(off)), ""), "")
    return LLVMBuildZExt(te.builder, LLVMBuildLoad2Aligned(te.builder, te.types.t_int8, p, 1u, ""), te.types.t_int32, "")
}

def private wg_i32(var te : TileEmit; var base : LLVMOpaqueValue?; off : int) : LLVMOpaqueValue? {
    var p = LLVMBuildGEP2(te.builder, te.types.t_int8, te.wg, LLVMBuildAdd(te.builder, base, te.types->ConstI64(uint64(off)), ""), "")
    return LLVMBuildLoad2Aligned(te.builder, te.types.t_int32, p, 4u, "")
}

//! a packed-plane byte position to its grp<mr> column offset (4-byte columns, mr rows each)
def private wg_col(mr, pos : int) : int => (pos / 4) * mr * 4 + pos % 4

//! the byte at packed position `pos`, read as one dword load of its column with the byte split off in
//! registers - the four positions of a column share the load through CSE
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#grid-decode-forms")]
def private wg_col_byte(var te : TileEmit; var base : LLVMOpaqueValue?; pos : int) : LLVMOpaqueValue? {
    if (te.gridRows) return wg_byte(te, base, wg_col(te.interleave, pos))
    let b = te.builder
    var col = wg_i32(te, base, (pos / 4) * te.interleave * 4)
    let sh = 8 * (pos % 4)
    var v = sh == 0 ? col : LLVMBuildLShr(b, col, LLVMConstInt(te.types.t_int32, uint64(sh), 0), "")
    return sh == 24 ? v : LLVMBuildAnd(b, v, LLVMConstInt(te.types.t_int32, 255ul, 0), "")
}

//! grid[gidx] - a raw magnitude word; the block emitter applies the signs per vector
def private grid_word(var te : TileEmit; var gidx : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    var gp = LLVMBuildGEP2(te.builder, te.types.t_int32, te.gridGlobal, gidx, "")
    return LLVMBuildLoad2Aligned(te.builder, te.types.t_int32, gp, 4u, "")
}

//! the u64 grid's word pair at even index `idx2`: one 8-byte load, half `h` split off in registers
//! (the h = 0 and h = 1 decodes share the load through CSE)
def private grid_pair_half(var te : TileEmit; var idx2 : LLVMOpaqueValue?; h : int) : LLVMOpaqueValue? {
    let b = te.builder
    var g64 = grid_pair_i64(te, idx2)
    return LLVMBuildTrunc(b, h == 0 ? g64 : LLVMBuildLShr(b, g64, LLVMConstInt(te.types.t_int64, 32ul, 0), ""), te.types.t_int32, "")
}

//! the u64 grid's word pair at even index `idx2` as one i64 - a row's 8 weights in one load
def private grid_pair_i64(var te : TileEmit; var idx2 : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    var gp = LLVMBuildGEP2(b, te.types.t_int64, te.gridGlobal, LLVMBuildLShr(b, idx2, LLVMConstInt(te.types.t_int32, 1ul, 0), ""), "")
    return LLVMBuildLoad2Aligned(b, te.types.t_int64, gp, 4u, "")
}

//! iq3s: 8-bit qs byte + the block's qh bit = the 9-bit grid index
def private decode_iq3s_dword(var te : TileEmit; var base : LLVMOpaqueValue?; blk, l, h : int) : LLVMOpaqueValue? {
    let b = te.builder
    let mr = te.interleave
    var qh = wg_byte(te, base, wg_col(mr, 64 + blk))
    var qv = wg_byte(te, base, wg_col(mr, blk * 8 + 2 * l + h))
    var hb = LLVMBuildAnd(b, LLVMBuildShl(b, qh, LLVMConstInt(te.types.t_int32, uint64(8 - h - 2 * l), 0), ""), LLVMConstInt(te.types.t_int32, 256ul, 0), "")
    return grid_word(te, LLVMBuildOr(b, qv, hb, ""))
}

//! iq3xxs: an 8-bit index into the halved grid, read through its column dword
def private decode_iq3xxs_dword(var te : TileEmit; var base : LLVMOpaqueValue?; blk, l, h : int) : LLVMOpaqueValue? {
    return grid_word(te, wg_col_byte(te, base, blk * 8 + 2 * l + h))
}

//! iq2xs: row `base`'s u16 word for (blk, l) - low 9 bits the grid index, the top 7 the ksigns code,
//! read as its two column bytes
def private iq2xs_word16(var te : TileEmit; var base : LLVMOpaqueValue?; blk, l : int) : LLVMOpaqueValue? {
    let b = te.builder
    let lp = (blk * 4 + l) * 2
    var qlo = wg_col_byte(te, base, lp)
    var qhi = wg_col_byte(te, base, lp + 1)
    return LLVMBuildOr(b, qlo, LLVMBuildShl(b, qhi, LLVMConstInt(te.types.t_int32, 8ul, 0), ""), "")
}

//! the iq2 formats' doubled index into the u64 grid's word pairs for row `base`, (blk, l): iq2s the qs byte
//! under the qh 0x300 window, iq2xs the u16 word's low 9 bits, iq2xxs block b's grid byte (column 2b)
def private iq2_idx2(var te : TileEmit; var base : LLVMOpaqueValue?; blk, l : int) : LLVMOpaqueValue? {
    let b = te.builder
    var idx : LLVMOpaqueValue?
    if (te.kq == 23) {
        let mr = te.interleave
        var qh = wg_byte(te, base, wg_col(mr, 64 + blk))
        var hb = LLVMBuildAnd(b, LLVMBuildShl(b, qh, LLVMConstInt(te.types.t_int32, uint64(8 - 2 * l), 0), ""), LLVMConstInt(te.types.t_int32, 0x300ul, 0), "")
        idx = LLVMBuildOr(b, wg_byte(te, base, wg_col(mr, blk * 4 + l)), hb, "")
    } elif (te.kq == 24) {
        idx = LLVMBuildAnd(b, iq2xs_word16(te, base, blk, l), LLVMConstInt(te.types.t_int32, 511ul, 0), "")
    } else {
        idx = wg_byte(te, base, 2 * blk * te.interleave * 4 + l)
    }
    return LLVMBuildShl(b, idx, LLVMConstInt(te.types.t_int32, 1ul, 0), "")
}

//! row `base`'s 7-bit ksigns code for (blk, l) - iq2xs the u16 word's top bits, iq2xxs/iq3xxs the aux32's 7-bit fields
def private kq_row_sign_code(var te : TileEmit; var base : LLVMOpaqueValue?; blk, l : int) : LLVMOpaqueValue? {
    let b = te.builder
    if (te.kq == 24) return LLVMBuildLShr(b, iq2xs_word16(te, base, blk, l), LLVMConstInt(te.types.t_int32, 9ul, 0), "")
    var aux = wg_i32(te, base, (te.kq == 34 ? 16 + blk : 2 * blk + 1) * te.interleave * 4)
    var sh = l == 0 ? aux : LLVMBuildLShr(b, aux, LLVMConstInt(te.types.t_int32, uint64(7 * l), 0), "")
    return LLVMBuildAnd(b, sh, LLVMConstInt(te.types.t_int32, 127ul, 0), "")
}

//! the eight +-1 bytes of a ksigns code: one table row (bit k of the expanded byte set = lane k negative)
def private sign_pm1_row(var te : TileEmit; var code : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    var gp = LLVMBuildGEP2(b, te.types.t_int64, te.sign_code_pm1_g, code, "")
    return LLVMBuildBitCast(b, LLVMBuildLoad2Aligned(b, te.types.t_int64, gp, 8u, ""), LLVMVectorType(te.types.t_int8, 8u), "")
}

//! rows r0.. of a row group's +-1 sign bytes off the plane's sign column (iq3s/iq2s): byte r*4 + l
//! broadcast over the row's 8 lanes, bit k selects lane k, the mask | 1
def private sign_col_pm1(var te : TileEmit; var col : LLVMOpaqueValue?; r0, l : int; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    let w8 = te.width / 8
    let idx <- [for (i in range(w8)); (r0 + i / 8) * 4 + l]
    var bytes = LLVMBuildShuffleVector(b, te.types, col, col, idx, "sgp")
    var bits <- [for (i in range(w8)); LLVMConstInt(te.types.t_int8, uint64(1 << (i % 8)), 0)]
    var bitsv = LLVMConstVector(array_data_ptr(bits), uint(w8))
    var m = LLVMBuildSExt(b, LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntEQ, LLVMBuildAnd(b, bytes, bitsv, ""), bitsv, ""), te.vwi8, "sgpm")
    return LLVMBuildOr(b, m, splat_i8w(te, 1), name)
}

[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#grid-decode-forms")]
def private grid_rows_path(te : TileEmit) : bool {
    if (te.vbmi) return true
    if (te.dotKind == DOT_SDOT) return te.rv == 4
    if (te.dotKind != DOT_MADDUBS && te.dotKind != DOT_VPDPBUSD && te.dotKind != DOT_VPDPBSSD) return false
    let cls = tune_cpu_class()
    if (cls == "x86-vnni512") return te.kq == 25
    if (cls == "x86-amx") return te.kq == 25 || te.kq == 34
    return false
}

//! eight +-1 bytes for sign byte `sb`, lane k negative when bit k is set
def private pm1_of(sb : int) : uint64 {
    var v = 0ul
    for (k in range(8)) {
        v |= uint64(((sb >> k) & 1) != 0 ? 0xFF : 0x01) << uint64(8 * k)
    }
    return v
}

//! the row-pair decode's sign table: the 128 ksigns codes with the code's parity as the 8th bit
//! (ksigns[i] == i | parity(i) << 7), each as its eight +-1 bytes
def private sign_pm1_emit_globals(var te : TileEmit; mod : LLVMOpaqueModule?) {
    if (!grid_rows_path(te)) return     // only the row-group grid decode reads it
    te.sign_code_pm1_g = LLVMGetNamedGlobal(mod, "dasllama.sign.code_pm1")
    if (te.sign_code_pm1_g != null) return
    var codes : array<LLVMOpaqueValue?>
    codes |> reserve(128)
    for (c in range(128)) {
        var par = c ^ (c >> 4)
        par ^= par >> 2
        par ^= par >> 1
        codes |> push(LLVMConstInt(te.types.t_int64, pm1_of(c | ((par & 1) << 7)), 0))
    }
    te.sign_code_pm1_g = LLVMAddGlobal(mod, LLVMArrayType(te.types.t_int64, 128u), "dasllama.sign.code_pm1")
    LLVMSetInitializer(te.sign_code_pm1_g, LLVMConstArray(te.types.t_int64, array_data_ptr(codes), 128u))
    LLVMSetGlobalConstant(te.sign_code_pm1_g, 1)
    LLVMSetLinkage(te.sign_code_pm1_g, LLVMLinkage.LLVMPrivateLinkage)
    LLVMSetAlignment(te.sign_code_pm1_g, 64u)
}

//! block `blk`'s sign bytes for rows qd*rv.., byte (r, l) at r*4 + l - the plane's own column for
//! iq3s/iq2s, synthesized from the 7-bit ksigns codes for the other three (the 8th bit is the
//! code's parity, ksigns[i] == i | parity(i) << 7); one masked negate per weight vector applies them
def private kq_sign_bytes(var te : TileEmit; var wbP : LLVMOpaqueValue?; blk, qd : int; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    let mr = te.interleave
    let w8 = te.width / 8
    let colBytes = uint64(mr * 4)
    if (te.kq == 33 || te.kq == 23) {   // the plane carries the sign bytes
        let col = (te.kq == 33 ? 18 : 8) + blk
        return load_vec(te, te.vwi8, te.wg, LLVMBuildAdd(b, wbP, te.types->ConstI64(uint64(col) * colBytes + uint64(qd * w8)), ""), name)
    }
    var codesCol : LLVMOpaqueValue? [2]   // iq2xs: columns 2blk (l = 0, 1) and 2blk+1 (l = 2, 3) of u16 words; else one aux32 column
    if (te.kq == 24) {
        for (c in range(2)) {
            codesCol[c] = load_vec(te, te.vni32, te.wg, LLVMBuildAdd(b, wbP, te.types->ConstI64(uint64(2 * blk + c) * colBytes + uint64(qd * w8)), ""), "sgw{blk}_{c}")
        }
    } else {
        let col = te.kq == 34 ? 16 + blk : 2 * blk + 1
        codesCol[0] = load_vec(te, te.vni32, te.wg, LLVMBuildAdd(b, wbP, te.types->ConstI64(uint64(col) * colBytes + uint64(qd * w8)), ""), "sga{blk}")
    }
    var bytes = LLVMConstNull(te.vni32)
    for (l in range(4)) {
        var code : LLVMOpaqueValue?
        if (te.kq == 24) {
            var word = l % 2 == 0 ? LLVMBuildAnd(b, codesCol[l / 2], splat_i32n(te, 65535), "") : LLVMBuildLShr(b, codesCol[l / 2], splat_i32n(te, 16), "")
            code = LLVMBuildLShr(b, word, splat_i32n(te, 9), "")
        } else {
            code = LLVMBuildAnd(b, l == 0 ? codesCol[0] : LLVMBuildLShr(b, codesCol[0], splat_i32n(te, 7 * l), ""), splat_i32n(te, 127), "")
        }
        var par = LLVMBuildXor(b, code, LLVMBuildLShr(b, code, splat_i32n(te, 4), ""), "")
        par = LLVMBuildXor(b, par, LLVMBuildLShr(b, par, splat_i32n(te, 2), ""), "")
        par = LLVMBuildXor(b, par, LLVMBuildLShr(b, par, splat_i32n(te, 1), ""), "")
        var sb = LLVMBuildOr(b, code, LLVMBuildShl(b, LLVMBuildAnd(b, par, splat_i32n(te, 1), ""), splat_i32n(te, 7), ""), "")
        bytes = LLVMBuildOr(b, bytes, l == 0 ? sb : LLVMBuildShl(b, sb, splat_i32n(te, 8 * l), ""), "")
    }
    return LLVMBuildBitCast(b, bytes, te.vwi8, name)
}

//! one masked negate per weight vector: byte (r, k) of the mask is 0xFF when bit (h*4 + k) of row r's
//! sign byte for group `l` is set - a constant shuffle broadcasts each row's byte to its four lanes
def private apply_sign_col(var te : TileEmit; var w, col : LLVMOpaqueValue?; l, h : int; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    let w8 = te.width / 8
    let idx <- [for (i in range(w8)); (i / 4) * 4 + l]
    var bytes = LLVMBuildShuffleVector(b, te.types, col, col, idx, "sgb")
    var bits : array<LLVMOpaqueValue?>
    bits |> reserve(w8)
    for (i in range(w8)) {
        bits |> push(LLVMConstInt(te.types.t_int8, uint64(1 << (h * 4 + i % 4)), 0))
    }
    var bitsv = LLVMConstVector(array_data_ptr(bits), uint(w8))
    var m = LLVMBuildSExt(b, LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntEQ, LLVMBuildAnd(b, bytes, bitsv, ""), bitsv, ""), te.vwi8, "sgm")
    return LLVMBuildSub(b, LLVMBuildXor(b, w, m, ""), m, name)
}

def private decode_kq_dword(var te : TileEmit; var base : LLVMOpaqueValue?; blk, l, h : int) : LLVMOpaqueValue? {
    if (te.kq == 33) return decode_iq3s_dword(te, base, blk, l, h)
    if (te.kq == 34) return decode_iq3xxs_dword(te, base, blk, l, h)
    return grid_pair_half(te, iq2_idx2(te, base, blk, l), h)
}

//! bytes per row per superblock of a panel format's packed grp region
def private kq_gather_stride(kq : int) : int => kq == 33 ? 104 : (kq == 34 ? 96 : (kq == 23 ? 72 : 64))

//! byte offset of the (blk, l, h) dword group in the panel: two 32-group halves, mr dwords per group
def private panel_group_off(mr, blk, l, h : int) : int {
    let e = l * 8 + h * 4
    return (e < 16 ? 0 : 128 * mr) + (blk * 4 + (e % 16) / 4) * mr * 4
}

// gemv, gather="panel": superblock `sbi` off the PACKED grp planes into the alloca panel - one
// emitted loop over the mr rows (64 decoded dwords per iteration), a single straight-line loop
// block, so the phi's back edge is the block itself.
def private emit_kq_gather(var te : TileEmit; var sbi : LLVMOpaqueValue?) {
    let b = te.builder
    let mr = te.interleave
    var wbP = LLVMBuildMul(b, sbi, te.types->ConstI64(uint64(mr) * uint64(kq_gather_stride(te.kq))), "kq.wb")
    var pre = LLVMGetInsertBlock(b)
    var head = LLVMAppendBasicBlockInContext(te.gctx, te.gfn, "kq.gath")
    var cont = LLVMAppendBasicBlockInContext(te.gctx, te.gfn, "kq.cont")
    LLVMBuildBr(b, head)
    LLVMPositionBuilderAtEnd(b, head)
    var rPhi = LLVMBuildPhi(b, te.types.t_int64, "kq.r")
    var preVals <- [te.types->ConstI64(0ul)]
    var preBlocks <- [pre]
    LLVMAddIncoming(rPhi, preVals, preBlocks)
    var rT4 = LLVMBuildMul(b, rPhi, te.types->ConstI64(4ul), "kq.r4")
    var base = LLVMBuildAdd(b, wbP, rT4, "kq.base")
    for (blk in range(8)) {
        for (l in range(4)) {
            for (h in range(2)) {
                var w = decode_kq_dword(te, base, blk, l, h)
                var dp = LLVMBuildGEP2(b, te.types.t_int8, te.gridPanel, LLVMBuildAdd(b, rT4, te.types->ConstI64(uint64(panel_group_off(mr, blk, l, h))), ""), "")
                LLVMBuildStore(b, w, dp)
            }
        }
    }
    var rNext = LLVMBuildAdd(b, rPhi, te.types->ConstI64(1ul), "kq.rn")
    var more = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, rNext, te.types->ConstI64(uint64(mr)), "kq.more")
    LLVMBuildCondBr(b, more, head, cont)
    var loopVals <- [rNext]
    var loopBlocks <- [head]
    LLVMAddIncoming(rPhi, loopVals, loopBlocks)
    LLVMPositionBuilderAtEnd(b, cont)
}

// The iq3xxs gemv gather global: the HALVED grid as a [256 x i32] private constant.
def private iq3xxs_emit_globals(var te : TileEmit; mod : LLVMOpaqueModule?) {
    var grid = LLVMGetNamedGlobal(mod, "dasllama.iq3xxs.grid")
    if (grid == null) {
        let g = iq3xxs_grid()
        var elems : array<LLVMOpaqueValue?>
        elems |> reserve(256)
        for (i in range(256)) {
            elems |> push(LLVMConstInt(te.types.t_int32, uint64(g[i]), 0))
        }
        grid = LLVMAddGlobal(mod, LLVMArrayType(te.types.t_int32, 256u), "dasllama.iq3xxs.grid")
        LLVMSetInitializer(grid, LLVMConstArray(te.types.t_int32, array_data_ptr(elems), 256u))
        LLVMSetGlobalConstant(grid, 1)
        LLVMSetLinkage(grid, LLVMLinkage.LLVMPrivateLinkage)
        LLVMSetAlignment(grid, 64u)
    }
    te.gridGlobal = grid
}


// The iq2s gemv gather global: the u64 grid as a [2048 x i32] private constant (low/high word pairs).
def private iq2s_emit_globals(var te : TileEmit; mod : LLVMOpaqueModule?) {
    var grid = LLVMGetNamedGlobal(mod, "dasllama.iq2s.grid")
    if (grid == null) {
        let g = iq2s_grid2()
        var elems : array<LLVMOpaqueValue?>
        elems |> reserve(2048)
        for (i in range(2048)) {
            elems |> push(LLVMConstInt(te.types.t_int32, uint64(g[i]), 0))
        }
        grid = LLVMAddGlobal(mod, LLVMArrayType(te.types.t_int32, 2048u), "dasllama.iq2s.grid")
        LLVMSetInitializer(grid, LLVMConstArray(te.types.t_int32, array_data_ptr(elems), 2048u))
        LLVMSetGlobalConstant(grid, 1)
        LLVMSetLinkage(grid, LLVMLinkage.LLVMPrivateLinkage)
        LLVMSetAlignment(grid, 64u)
    }
    te.gridGlobal = grid
}


// The iq2xs gemv gather global: the u64 grid as a [1024 x i32] private constant (low/high word pairs).
def private iq2xs_emit_globals(var te : TileEmit; mod : LLVMOpaqueModule?) {
    var grid = LLVMGetNamedGlobal(mod, "dasllama.iq2xs.grid")
    if (grid == null) {
        let g = iq2xs_grid2()
        var elems : array<LLVMOpaqueValue?>
        elems |> reserve(1024)
        for (i in range(1024)) {
            elems |> push(LLVMConstInt(te.types.t_int32, uint64(g[i]), 0))
        }
        grid = LLVMAddGlobal(mod, LLVMArrayType(te.types.t_int32, 1024u), "dasllama.iq2xs.grid")
        LLVMSetInitializer(grid, LLVMConstArray(te.types.t_int32, array_data_ptr(elems), 1024u))
        LLVMSetGlobalConstant(grid, 1)
        LLVMSetLinkage(grid, LLVMLinkage.LLVMPrivateLinkage)
        LLVMSetAlignment(grid, 64u)
    }
    te.gridGlobal = grid
}

// The iq2xxs gemv gather global: the u64 grid as a [512 x i32] private constant (low/high word pairs).
def private iq2xxs_emit_globals(var te : TileEmit; mod : LLVMOpaqueModule?) {
    var grid = LLVMGetNamedGlobal(mod, "dasllama.iq2xxs.grid")
    if (grid == null) {
        let g = iq2xxs_grid2()
        var elems : array<LLVMOpaqueValue?>
        elems |> reserve(512)
        for (i in range(512)) {
            elems |> push(LLVMConstInt(te.types.t_int32, uint64(g[i]), 0))
        }
        grid = LLVMAddGlobal(mod, LLVMArrayType(te.types.t_int32, 512u), "dasllama.iq2xxs.grid")
        LLVMSetInitializer(grid, LLVMConstArray(te.types.t_int32, array_data_ptr(elems), 512u))
        LLVMSetGlobalConstant(grid, 1)
        LLVMSetLinkage(grid, LLVMLinkage.LLVMPrivateLinkage)
        LLVMSetAlignment(grid, 64u)
    }
    te.gridGlobal = grid
}



//! the 8 activation bytes at byte offset `off` of token 0, replicated over every 64-bit lane of the
//! weight width (ld1r after lowering): the dot operand for a [row 8w | row 8w ...] weight vector
def private bcast_qword_x(var te : TileEmit; var off : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    let n = te.width / 64
    var p = LLVMBuildGEP2(b, te.types.t_int8, te.x[0], off, "")
    var q = LLVMBuildLoad2Aligned(b, te.types.t_int64, p, 1u, "")
    var vn = LLVMVectorType(te.types.t_int64, uint(n))
    var v = LLVMBuildInsertElement(b, LLVMGetUndef(vn), q, LLVMConstInt(te.types.t_int32, 0ul, 0), "")
    let zeros <- [for (_i in range(n)); 0]
    var spl = LLVMBuildShuffleVector(b, te.types, v, v, zeros, "")
    return LLVMBuildBitCast(b, spl, te.vwi8, name)
}

//! `rows` <8 x i8> vectors side by side as one weight-width vector (pairwise shuffles - ins after lowering)
def private concat_rows(var te : TileEmit; var rows : array<LLVMOpaqueValue?>; lanes : int = 8) : LLVMOpaqueValue? {
    if (length(rows) == 1) return rows[0]
    let b = te.builder
    let idx <- [for (i in range(2 * lanes)); i]
    var nxt <- [for (i in range(length(rows) / 2)); LLVMBuildShuffleVector(b, te.types, rows[2 * i], rows[2 * i + 1], idx, "")]
    return concat_rows(te, nxt, 2 * lanes)
}

//! one row group's dot with its +-1 sign bytes: the signed-weight lattices (sdot, vpdpbssd) multiply them
//! into the magnitudes; the unsigned-weight ones (maddubs, vpdpbusd) put them on the activation copy
def private grid_rows_dot(var te : TileEmit; var acc, w, sg, xv : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    if (te.dotKind == DOT_SDOT || te.dotKind == DOT_VPDPBSSD) {
        var args <- [acc, LLVMBuildMul(b, w, sg, "ws"), xv]
        return LLVMBuildCall2(b, te.dotKind == DOT_SDOT ? te.sdot_ty : te.dp_ty, te.dotKind == DOT_SDOT ? te.sdot_decl : te.dp_decl, args, "dot")
    }
    var xs : LLVMOpaqueValue?
    if (te.width == 256) {
        var pargs <- [xv, sg]
        xs = LLVMBuildCall2(b, te.psign_ty, te.psign_decl, pargs, "signx")
    } else {
        var neg = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, sg, LLVMConstNull(te.vwi8), "sneg")
        xs = LLVMBuildSelect(b, neg, LLVMBuildSub(b, LLVMConstNull(te.vwi8), xv, ""), xv, "signx")
    }
    if (te.dotKind == DOT_VPDPBUSD) {
        var dargs <- [acc, w, xs]
        return LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "dot")
    }
    var margs <- [w, xs]
    var pairs = LLVMBuildCall2(b, te.madd_ty, te.madd_decl, margs, "pairs")
    var wargs <- [pairs, ones_i16(te)]
    return LLVMBuildAdd(b, acc, LLVMBuildCall2(b, te.maddwd_ty, te.maddwd_decl, wargs, "quads"), "dot")
}

//! row `base`'s 8 raw grid weights for (blk, l) as <8 x i8>: the iq2 formats' u64 grid entry in one
//! load, iq3s/iq3xxs the two dword decodes side by side
def private row_weights8(var te : TileEmit; var base : LLVMOpaqueValue?; blk, l : int) : LLVMOpaqueValue? {
    let b = te.builder
    var v8i8 = LLVMVectorType(te.types.t_int8, 8u)
    if (te.kq != 33 && te.kq != 34) return LLVMBuildBitCast(b, grid_pair_i64(te, iq2_idx2(te, base, blk, l)), v8i8, "")
    var w0 = decode_kq_dword(te, base, blk, l, 0)
    var w1 = decode_kq_dword(te, base, blk, l, 1)
    var v2 = LLVMVectorType(te.types.t_int32, 2u)
    var v = LLVMBuildInsertElement(b, LLVMGetUndef(v2), w0, LLVMConstInt(te.types.t_int32, 0ul, 0), "")
    v = LLVMBuildInsertElement(b, v, w1, LLVMConstInt(te.types.t_int32, 1ul, 0), "")
    return LLVMBuildBitCast(b, v, v8i8, "")
}

//! the lattice's constant tables, emitted once per module: the grid's two compact code planes (entry e's
//! low / high half - four 2-bit symbols for the iq2 alphabet, two 3-bit symbols for iq3), the alphabet as a
//! per-lane vpshufb table, and ksigns_iq2xs whole (128 bytes: exactly the two registers one VPERMI2B indexes)
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#vbmi-lattice")]
def private vbmi_emit_globals(var te : TileEmit; mod : LLVMOpaqueModule?) {
    let tag = vbmi_tag(te.kq)
    te.vb_plane[0] = LLVMGetNamedGlobal(mod, "dasllama.vbmi.{tag}.p0")
    te.vb_plane[1] = LLVMGetNamedGlobal(mod, "dasllama.vbmi.{tag}.p1")
    te.vb_alpha = LLVMGetNamedGlobal(mod, "dasllama.vbmi.{tag}.alpha")
    te.vb_ksigns = LLVMGetNamedGlobal(mod, "dasllama.vbmi.ksigns")
    if (te.vb_plane[0] != null && te.vb_plane[1] != null && te.vb_alpha != null && te.vb_ksigns != null) return
    let alpha <- vbmi_alphabet(te.kq)
    let entries = vbmi_entries(te.kq)
    var p0 : array<LLVMOpaqueValue?>
    var p1 : array<LLVMOpaqueValue?>
    p0 |> reserve(entries)
    p1 |> reserve(entries)
    if (te.kq == 34) {
        vbmi_planes_iq3xxs(te, p0, p1, alpha)
    } elif (te.kq == 33) {
        vbmi_planes_iq3s(te, p0, p1, alpha)
    } elif (te.kq == 25) {
        vbmi_planes_iq2xxs(te, p0, p1, alpha)
    } elif (te.kq == 24) {
        vbmi_planes_iq2xs(te, p0, p1, alpha)
    } else {
        vbmi_planes_iq2s(te, p0, p1, alpha)
    }
    te.vb_plane[0] = vbmi_const_bytes(te, mod, "dasllama.vbmi.{tag}.p0", p0)
    te.vb_plane[1] = vbmi_const_bytes(te, mod, "dasllama.vbmi.{tag}.p1", p1)
    var al : array<LLVMOpaqueValue?>
    al |> reserve(64)
    for (k in range(64)) {
        al |> push(LLVMConstInt(te.types.t_int8, uint64(k % 16 < length(alpha) ? alpha[k % 16] : 0), 0))
    }
    te.vb_alpha = vbmi_const_bytes(te, mod, "dasllama.vbmi.{tag}.alpha", al)
    if (te.vb_ksigns == null) {
        let ks = ksigns_iq2xs()
        var kv : array<LLVMOpaqueValue?>
        kv |> reserve(128)
        for (k in range(128)) {
            kv |> push(LLVMConstInt(te.types.t_int8, uint64(ks[k]), 0))
        }
        te.vb_ksigns = vbmi_const_bytes(te, mod, "dasllama.vbmi.ksigns", kv)
    }
}

//! an iq3 entry is one word of 4 weights: weights 0-1 and 2-3 per plane
def private vbmi_planes_iq3xxs(te : TileEmit; var p0, p1 : array<LLVMOpaqueValue?>; alpha : array<int>) {
    let g = iq3xxs_grid()
    for (e in range(256)) {
        vbmi_push_iq3(te, p0, p1, g[e], alpha)
    }
}

def private vbmi_planes_iq3s(te : TileEmit; var p0, p1 : array<LLVMOpaqueValue?>; alpha : array<int>) {
    let g = iq3s_grid()
    for (e in range(512)) {
        vbmi_push_iq3(te, p0, p1, g[e], alpha)
    }
}

//! an iq2 entry is two words: weights 0-3, 4-7 - one plane each
def private vbmi_planes_iq2xxs(te : TileEmit; var p0, p1 : array<LLVMOpaqueValue?>; alpha : array<int>) {
    let g = iq2xxs_grid2()
    for (e in range(256)) {
        vbmi_push_iq2(te, p0, p1, g[2 * e], g[2 * e + 1], alpha)
    }
}

def private vbmi_planes_iq2xs(te : TileEmit; var p0, p1 : array<LLVMOpaqueValue?>; alpha : array<int>) {
    let g = iq2xs_grid2()
    for (e in range(512)) {
        vbmi_push_iq2(te, p0, p1, g[2 * e], g[2 * e + 1], alpha)
    }
}

def private vbmi_planes_iq2s(te : TileEmit; var p0, p1 : array<LLVMOpaqueValue?>; alpha : array<int>) {
    let g = iq2s_grid2()
    for (e in range(1024)) {
        vbmi_push_iq2(te, p0, p1, g[2 * e], g[2 * e + 1], alpha)
    }
}

def private vbmi_push_iq3(te : TileEmit; var p0, p1 : array<LLVMOpaqueValue?>; word : uint; alpha : array<int>) {
    p0 |> push(LLVMConstInt(te.types.t_int8, uint64(vbmi_pack_word(word, alpha, 3, 2)), 0))
    p1 |> push(LLVMConstInt(te.types.t_int8, uint64(vbmi_pack_word(word >> 16u, alpha, 3, 2)), 0))
}

def private vbmi_push_iq2(te : TileEmit; var p0, p1 : array<LLVMOpaqueValue?>; w0, w1 : uint; alpha : array<int>) {
    p0 |> push(LLVMConstInt(te.types.t_int8, uint64(vbmi_pack_word(w0, alpha, 2, 4)), 0))
    p1 |> push(LLVMConstInt(te.types.t_int8, uint64(vbmi_pack_word(w1, alpha, 2, 4)), 0))
}

//! the grid's symbol alphabet: the halved iq3xxs magnitudes, iq3s's odd magnitudes, the iq2 family's three
def private vbmi_alphabet(kq : int) : array<int> {
    if (kq == 34) return <- [2, 6, 10, 14, 18, 22, 26, 31]
    if (kq == 33) return <- [1, 3, 5, 7, 9, 11, 13, 15]
    return <- [8, 25, 43]
}

//! the grid gemv's module-level tables: the format's grid, the ksigns +-1 table when the row form reads it, the lattice's
def private grid_emit_globals(var te : TileEmit; fmt : int; mod : LLVMOpaqueModule?) {
    if (fmt == 33) {
        iq3s_emit_globals(te, mod)
    } elif (fmt == 23) {
        iq2s_emit_globals(te, mod)
    } elif (fmt == 24) {
        iq2xs_emit_globals(te, mod)
    } elif (fmt == 25) {
        iq2xxs_emit_globals(te, mod)
    } else {
        iq3xxs_emit_globals(te, mod)
    }
    sign_pm1_emit_globals(te, mod)
    if (te.vbmi) {
        vbmi_emit_globals(te, mod)
    }
}

def private vbmi_tag(kq : int) : string => kq == 25 ? "iq2xxs" : (kq == 24 ? "iq2xs" : (kq == 23 ? "iq2s" : (kq == 34 ? "iq3xxs" : "iq3s")))

//! grid entries per format: the compact plane's byte count
def private vbmi_entries(kq : int) : int => (kq == 25 || kq == 34) ? 256 : (kq == 23 ? 1024 : 512)

//! a grid word's low `nbytes` weight bytes as packed symbols (weight t at bits t*bits); a byte outside the alphabet is a broken table
def private vbmi_pack_word(word : uint; alpha : array<int>; bits, nbytes : int) : int {
    var code = 0
    for (t in range(nbytes)) {
        let wb = int((word >> uint(8 * t)) & 0xFFu)
        let si = alpha |> find_index(wb)
        if (si < 0) {
            panic("vbmi lattice: grid byte {wb} is outside the alphabet")
        }
        code |= si << (bits * t)
    }
    return code
}

def private vbmi_const_bytes(te : TileEmit; mod : LLVMOpaqueModule?; name : string; var elems : array<LLVMOpaqueValue?>) : LLVMOpaqueValue? {
    var g = LLVMAddGlobal(mod, LLVMArrayType(te.types.t_int8, uint(length(elems))), name)
    LLVMSetInitializer(g, LLVMConstArray(te.types.t_int8, array_data_ptr(elems), uint(length(elems))))
    LLVMSetGlobalConstant(g, 1)
    LLVMSetLinkage(g, LLVMLinkage.LLVMPrivateLinkage)
    LLVMSetAlignment(g, 64u)
    return g
}

//! one 64-byte slice of a constant byte table (the lattice's register operands; loop-invariant, LLVM hoists them)
def private vbmi_table_part(var te : TileEmit; var tbl : LLVMOpaqueValue?; part : int; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    var p = LLVMBuildGEP2(b, te.types.t_int8, tbl, te.types->ConstI64(uint64(part * 64)), "")
    return LLVMBuildLoad2Aligned(b, te.vwi8, p, 64u, name)
}

def private vbmi_permi2b(var te : TileEmit; var lo, idx, hi : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    var args <- [lo, idx, hi]
    return LLVMBuildCall2(te.builder, te.permi2b_ty, te.permi2b_decl, args, name)
}

//! VPMULTISHIFTQB: byte j of each qword = the 8 bits of the source qword starting at bit ctrl[j]
def private vbmi_mshift(var te : TileEmit; var v : LLVMOpaqueValue?; ctrl : array<int>; name : string) : LLVMOpaqueValue? {
    var cs : array<LLVMOpaqueValue?>
    cs |> reserve(64)
    for (k in range(64)) {
        cs |> push(LLVMConstInt(te.types.t_int8, uint64(ctrl[k % 8]), 0))
    }
    var args <- [LLVMConstVector(array_data_ptr(cs), 64u), v]
    return LLVMBuildCall2(te.builder, te.mshift_ty, te.mshift_decl, args, name)
}

//! 64 indices through a byte table of `entries` (256 / 512 / 1024): VPERMI2B covers 128 entries per pair of
//! table registers, index bit 7 blends the pairs, and the caller's masks (the index's 9th and 10th bits, per
//! lane) blend the 256-entry halves above that
def private vbmi_lookup(var te : TileEmit; var tbl : LLVMOpaqueValue?; entries : int; var idx : LLVMOpaqueValue?; var masks : array<LLVMOpaqueValue?>; name : string) : LLVMOpaqueValue? { // nolint:LINT014 — var for non-const value handles: a const element cannot feed LLVMBuildSelect
    let b = te.builder
    var level <- [for (k in range(entries / 128)); vbmi_permi2b(te, vbmi_table_part(te, tbl, 2 * k, "{name}.t{2 * k}"), idx, vbmi_table_part(te, tbl, 2 * k + 1, "{name}.t{2 * k + 1}"), "{name}.l{k}")]
    var high = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, idx, LLVMConstNull(te.vwi8), "{name}.b7")
    var depth = 0
    while (length(level) > 1) {
        var sel = depth == 0 ? high : masks[depth - 1]
        var nxt <- [for (j in range(length(level) / 2)); LLVMBuildSelect(b, sel, level[2 * j + 1], level[2 * j], "{name}.s{depth}_{j}")]
        level |> clear()
        level |> push_from(nxt)
        depth++
    }
    return level[0]
}

//! the unsigned-weight dot with the signs as a byte mask on the activation copy: x' = (x ^ m) - m
def private rows_dot_masked(var te : TileEmit; var acc, w, m, xv : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    var xs = LLVMBuildSub(b, LLVMBuildXor(b, xv, m, ""), m, "signx")
    if (te.dotKind == DOT_VPDPBUSD) {
        var dargs <- [acc, w, xs]
        return LLVMBuildCall2(b, te.dp_ty, te.dp_decl, dargs, "dot")
    }
    var margs <- [w, xs]
    var pairs = LLVMBuildCall2(b, te.madd_ty, te.madd_decl, margs, "pairs")
    var wargs <- [pairs, ones_i16(te)]
    return LLVMBuildAdd(b, acc, LLVMBuildCall2(b, te.maddwd_ty, te.maddwd_decl, wargs, "quads"), "dot")
}

//! the block's 64-byte column `col` for rows qd*rv.. (row r's four bytes at r*4)
def private vbmi_col(var te : TileEmit; var wbP : LLVMOpaqueValue?; col, qd : int; name : string) : LLVMOpaqueValue? {
    let off = uint64(col * te.interleave * 4 + qd * (te.width / 8))
    return load_vec(te, te.vwi8, te.wg, LLVMBuildAdd(te.builder, wbP, te.types->ConstI64(off), ""), name)
}

//! the row's qh byte for this block (packed byte position 64 + blk: column 16 + blk/4, byte blk%4) broadcast
//! over its four lanes, then bit `bit0 + stride * (lane % 4)` as a lane mask - the iq3s / iq2s index bits above the qs byte
def private vbmi_qh_mask(var te : TileEmit; var qhCol : LLVMOpaqueValue?; blk, bit0, stride : int; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    let w8 = te.width / 8
    let idx <- [for (k in range(w8)); (k / 4) * 4 + blk % 4]
    var bytes = LLVMBuildShuffleVector(b, te.types, qhCol, qhCol, idx, "{name}.q")
    var bits <- [for (k in range(w8)); LLVMConstInt(te.types.t_int8, uint64(1 << (bit0 + stride * (k % 4))), 0)]
    var bitsv = LLVMConstVector(array_data_ptr(bits), uint(w8))
    return LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntNE, LLVMBuildAnd(b, bytes, bitsv, ""), LLVMConstNull(te.vwi8), name)
}

//! the row's sign bytes from its four 7-bit ksigns codes (lane r*4+l), parity included: one VPERMI2B over the table
def private vbmi_ksigns(var te : TileEmit; var codes : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    var ks0 = vbmi_table_part(te, te.vb_ksigns, 0, "ks0")
    var ks1 = vbmi_table_part(te, te.vb_ksigns, 1, "ks1")
    return vbmi_permi2b(te, ks0, LLVMBuildAnd(te.builder, codes, splat_i8w(te, 127), ""), ks1, name)
}

[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#vbmi-lattice")]
def private emit_grid_rows_blk_vbmi(var te : TileEmit; var wbP, xb : LLVMOpaqueValue?; blk : int; var a, a1 : LLVMOpaqueValue? [8]; iq2 : bool) { // nolint:STYLE037,STYLE038 — one block's decode: the five formats' index assembly and the shared expansion
    let b = te.builder
    let n = te.width / 64
    let w8 = te.width / 8
    let iq3 = te.kq == 33 || te.kq == 34
    let entries = vbmi_entries(te.kq)
    var alpha = LLVMBuildLoad2Aligned(b, te.vwi8, te.vb_alpha, 64u, "alpha")
    for (qd in range(te.rq)) {
        let ncol = iq3 ? 2 : 1
        var plo : LLVMOpaqueValue? [2]
        var phi : LLVMOpaqueValue? [2]
        var sgn : LLVMOpaqueValue?
        for (c in range(ncol)) {
            var idx : LLVMOpaqueValue?
            var masks : array<LLVMOpaqueValue?>
            if (te.kq == 25 || te.kq == 34) {
                idx = vbmi_col(te, wbP, 2 * blk + c, qd, "gi{blk}_{c}")
            } elif (te.kq == 33) {
                idx = vbmi_col(te, wbP, 2 * blk + c, qd, "gi{blk}_{c}")
                var qh = vbmi_col(te, wbP, 16 + blk / 4, qd, "qh{blk}")
                masks |> push(vbmi_qh_mask(te, qh, blk, 4 * c, 1, "qm{blk}_{c}"))
            } elif (te.kq == 23) {
                idx = vbmi_col(te, wbP, blk, qd, "gi{blk}")
                var qh = vbmi_col(te, wbP, 16 + blk / 4, qd, "qh{blk}")
                masks |> push(vbmi_qh_mask(te, qh, blk, 0, 2, "qm{blk}_0"))
                masks |> push(vbmi_qh_mask(te, qh, blk, 1, 2, "qm{blk}_1"))
                sgn = vbmi_col(te, wbP, 8 + blk, qd, "sg{blk}")
            } else {   // 24: u16 words - columns 2blk (l 0, 1) and 2blk+1 (l 2, 3), lane r*4+l takes word l's low then high byte
                var ca = vbmi_col(te, wbP, 2 * blk, qd, "wa{blk}")
                var cb = vbmi_col(te, wbP, 2 * blk + 1, qd, "wb{blk}")
                let lo <- [for (k in range(w8)); (k % 4 < 2 ? 0 : w8) + (k / 4) * 4 + 2 * (k % 2)]
                let hi <- [for (k in range(w8)); (k % 4 < 2 ? 0 : w8) + (k / 4) * 4 + 2 * (k % 2) + 1]
                idx = LLVMBuildShuffleVector(b, te.types, ca, cb, lo, "gi{blk}")
                var hib = LLVMBuildShuffleVector(b, te.types, ca, cb, hi, "gh{blk}")
                masks |> push(LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntNE, LLVMBuildAnd(b, hib, splat_i8w(te, 1), ""), LLVMConstNull(te.vwi8), "qm{blk}"))
                sgn = vbmi_ksigns(te, LLVMBuildLShr(b, hib, splat_i8w(te, 1), ""), "sg{blk}")
            }
            plo[c] = vbmi_lookup(te, te.vb_plane[0], entries, idx, masks, "plo{blk}_{c}")
            phi[c] = vbmi_lookup(te, te.vb_plane[1], entries, idx, masks, "phi{blk}_{c}")
        }
        if (te.kq == 33) {
            sgn = vbmi_col(te, wbP, 18 + blk, qd, "sg{blk}")
        } elif (te.kq == 25 || te.kq == 34) {   // the aux32's four 7-bit codes per row, two rows per qword
            var aux = vbmi_col(te, wbP, te.kq == 34 ? 16 + blk : 2 * blk + 1, qd, "aux{blk}")
            sgn = vbmi_ksigns(te, vbmi_mshift(te, aux, [0, 7, 14, 21, 32, 39, 46, 53], "sc{blk}"), "sg{blk}")
        }
        var acc : LLVMOpaqueValue? [2]
        var acc1 : LLVMOpaqueValue? [2]
        for (g in range(2)) {
            acc[g] = LLVMConstNull(te.vni32)
            acc1[g] = LLVMConstNull(te.vni32)
        }
        for (l in range(4)) {
            var xv = bcast_qword_x(te, LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32 + l * 8)), ""), "x8{blk}_{l}")
            for (g in range(2)) {
                let r0 = g * n
                var q : LLVMOpaqueValue?
                if (!iq3) {   // qword i <- row r0+i's low-plane byte, high-plane byte (the rest don't care)
                    let sel <- [for (k in range(w8)); (k % 8 == 1 ? w8 : 0) + (r0 + k / 8) * 4 + l]
                    q = LLVMBuildShuffleVector(b, te.types, plo[0], phi[0], sel, "wq{blk}_{l}_{g}")
                    q = LLVMBuildAnd(b, vbmi_mshift(te, q, [0, 2, 4, 6, 8, 10, 12, 14], "ws{blk}_{l}_{g}"), splat_i8w(te, 3), "")
                } else {      // qword i <- the two entries (h = 0, 1) of row r0+i, low then high plane each
                    let c = l / 2
                    let p0 = (2 * l) % 4
                    let sel <- [for (k in range(w8)); (k % 8 == 1 || k % 8 == 3 ? w8 : 0) + (r0 + k / 8) * 4 + p0 + (k % 8 == 2 || k % 8 == 3 ? 1 : 0)]
                    q = LLVMBuildShuffleVector(b, te.types, plo[c], phi[c], sel, "wq{blk}_{l}_{g}")
                    q = LLVMBuildAnd(b, vbmi_mshift(te, q, [0, 3, 8, 11, 16, 19, 24, 27], "ws{blk}_{l}_{g}"), splat_i8w(te, 7), "")
                }
                var sargs <- [alpha, q]
                var w = LLVMBuildCall2(b, te.shufb_ty, te.shufb_decl, sargs, "w{blk}_{l}_{g}")
                let ssel <- [for (k in range(w8)); (r0 + k / 8) * 4 + l]
                var sq = LLVMBuildShuffleVector(b, te.types, sgn, sgn, ssel, "sq{blk}_{l}_{g}")
                sq = LLVMBuildAnd(b, vbmi_mshift(te, sq, [0, 1, 2, 3, 4, 5, 6, 7], "sb{blk}_{l}_{g}"), splat_i8w(te, 1), "")
                var m = LLVMBuildSub(b, LLVMConstNull(te.vwi8), sq, "sm{blk}_{l}_{g}")
                let hi = iq2 && l >= 2
                var d = rows_dot_masked(te, hi ? acc1[g] : acc[g], w, m, xv)
                if (hi) {
                    acc1[g] = d
                } else {
                    acc[g] = d
                }
            }
        }
        a[qd] = LLVMBuildAdd(b, a[qd], pairwise_add_i32(te, acc[0], acc[1], "ap{blk}_{qd}"), "")
        if (iq2) {
            a1[qd] = LLVMBuildAdd(b, a1[qd], pairwise_add_i32(te, acc1[0], acc1[1], "ap1{blk}_{qd}"), "")
        }
    }
}

//! One block of a grid format's decode as ROW GROUPS: a weight-width vector is width/64 rows x 8 weights
//! straight from the grid words (one u64 grid entry per iq2 row), signed by +-1 bytes - the ksigns table row
//! per code or the plane's sign column (iq3s/iq2s); one pairwise add per group pair yields the fold's per-row lanes.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#grid-decode-forms")]
def private emit_grid_rows_blk(var te : TileEmit; var wbP, xb : LLVMOpaqueValue?; blk : int; var signCol : LLVMOpaqueValue? [2]; var a, a1 : LLVMOpaqueValue? [8]; iq2 : bool) {
    let b = te.builder
    let n = te.width / 64
    let colSigns = te.kq == 33 || te.kq == 23 || te.dotKind != DOT_SDOT   // x86: the sign column's mask beats four table loads + three inserts per group; the sdot lattice keeps the table for the ksigns formats
    for (qd in range(te.rq)) {
        var acc : LLVMOpaqueValue? [2]    // per row group; the lo strip (l < 2) when iq2
        var acc1 : LLVMOpaqueValue? [2]   // iq2: the hi strip (l >= 2)
        for (g in range(2)) {
            acc[g] = LLVMConstNull(te.vni32)
            acc1[g] = LLVMConstNull(te.vni32)
        }
        for (l in range(4)) {
            var xv = bcast_qword_x(te, LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32 + l * 8)), ""), "x8{blk}_{l}")
            for (g in range(2)) {
                let r0 = g * n
                var rows : array<LLVMOpaqueValue?>
                var sgs : array<LLVMOpaqueValue?>
                rows |> reserve(n)
                sgs |> reserve(n)
                for (i in range(n)) {
                    var base = LLVMBuildAdd(b, wbP, te.types->ConstI64(uint64((qd * te.rv + r0 + i) * 4)), "")
                    rows |> push(row_weights8(te, base, blk, l))
                    if (!colSigns) {
                        sgs |> push(sign_pm1_row(te, kq_row_sign_code(te, base, blk, l)))
                    }
                }
                var w = concat_rows(te, rows)
                var sg = colSigns ? sign_col_pm1(te, signCol[qd], r0, l, "sp{blk}_{l}_{qd}_{g}") : concat_rows(te, sgs)
                let hi = iq2 && l >= 2
                var d = grid_rows_dot(te, hi ? acc1[g] : acc[g], w, sg, xv)
                if (hi) {
                    acc1[g] = d
                } else {
                    acc[g] = d
                }
            }
        }
        a[qd] = LLVMBuildAdd(b, a[qd], pairwise_add_i32(te, acc[0], acc[1], "ap{blk}_{qd}"), "")
        if (iq2) {
            a1[qd] = LLVMBuildAdd(b, a1[qd], pairwise_add_i32(te, acc1[0], acc1[1], "ap1{blk}_{qd}"), "")
        }
    }
}

// One 256-weight SUPERBLOCK, IQ4_XS/iq3s grp<mr> form (te.kq = 44/33): mx4's LUT decode (44)
// or the byte-expanded signed panel (33; the gemv gathers it per superblock, the tile gets the
// runtime unpack) — dot_lane's sign-trick lattice, k4's fold with no min term.
def private emit_block_iq4xs(var te : TileEmit; var sbi : LLVMOpaqueValue?; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) {   // nolint:STYLE037,STYLE038 — one register-coupled superblock loop nest (the kqv2 emitters' shape)
    let b = te.builder
    let rq = te.rq
    let mr = te.interleave
    let w8 = te.width / 8
    let panel = te.kq == 33 || te.kq == 34 || te.kq == 23 || te.kq == 24 || te.kq == 25   // iq3s/iq3xxs/iq2s/iq2xs/iq2xxs: the byte-expanded signed panel replaces the nibble+LUT decode
    let iq2 = te.kq == 23 || te.kq == 24     // iq2s/iq2xs: per-16 UNSIGNED strips - lo/hi halves keep split accumulators
    let nl = te.kq == 45                     // iq4nl: the 44 LUT decode with q40's PER-BLOCK f16 d fold (no sub-scales, no min term)
    let gather = panel && te.gridPanel != null   // gemv mode: gather this superblock off the packed planes first
    // the decode shape composes row GROUPS straight from the grid words (emit_grid_rows_blk) - no panel
    let pairs = gather && tokCount == 1 && grid_rows_path(te)
    var wbase = te.wg
    var wb : LLVMOpaqueValue?
    var wbP : LLVMOpaqueValue?
    if (gather) {
        wbP = LLVMBuildMul(b, sbi, te.types->ConstI64(uint64(mr) * uint64(kq_gather_stride(te.kq))), "kq.wb")
        if (!pairs) {
            emit_kq_gather(te, sbi)
        }
        wbase = te.gridPanel
        wb = te.types->ConstI64(0ul)
    } else {
        wb = LLVMBuildMul(b, sbi, te.types->ConstI64(uint64(mr * (panel ? 256 : 128))), "wb")
    }
    var sb = LLVMBuildMul(b, sbi, te.types->ConstI64(uint64(mr * (nl ? 16 : 20))), "sb")
    var xb = LLVMBuildMul(b, sbi, te.types->ConstI64(0x100ul), "xb")
    var vri8 = LLVMVectorType(te.types.t_int8, uint(te.rv))
    var dv : LLVMOpaqueValue? [2]
    if (!nl) {   // one superblock d per row lane; iq4nl loads its per-block d inside the walk
        for (qd in range(rq)) {
            dv[qd] = load_f16_vec_at(te, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(2 * (qd * te.rv))), ""), "d{qd}")
        }
    }
    var d8nl : LLVMOpaqueValue? [4]
    if (nl) {   // hoist the per-token activation scales - the per-block fold consumes them 8x
        for (i in range(tokCount)) {
            let tk = tokBase + i
            var qp0 = LLVMBuildGEP2(b, te.types.t_float, te.xs[tk], sbi, "qp{tk}")
            d8nl[i] = splat_f32(te, LLVMBuildLoad2Aligned(b, te.types.t_float, qp0, 4u, "d8{tk}"), "d8v{tk}")
        }
    }
    var iacc : LLVMOpaqueValue? [8]
    for (i in range(tokCount * rq)) {
        iacc[i] = LLVMConstNull(te.vni32)
    }
    for (blk in range(8)) {
        var xoff0 = LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32)), "")
        var xoff1 = LLVMBuildAdd(b, xb, te.types->ConstI64(uint64(blk * 32 + 16)), "")
        var xv0 : LLVMOpaqueValue? [4]
        var xv1 : LLVMOpaqueValue? [4]
        for (i in range(tokCount)) {
            xv0[i] = load_v16i8(te, te.x[tokBase + i], xoff0, "x{tokBase + i}_{blk}lo")
            xv1[i] = load_v16i8(te, te.x[tokBase + i], xoff1, "x{tokBase + i}_{blk}hi")
        }
        var a : LLVMOpaqueValue? [8]     // [token * rq + qd]; tokCount*rq <= 8 (nrsplit*vq rail)
        var a1 : LLVMOpaqueValue? [8]    // iq2s: the hi-16 half's own accumulator (per-16 strips)
        for (i in range(tokCount * rq)) {
            a[i] = LLVMConstNull(te.vni32)
            a1[i] = LLVMConstNull(te.vni32)
        }
        var signCol : LLVMOpaqueValue? [2]   // this block's sign bytes, rows qd*rv.. at stride 4 - the gemv negates per vector, the tile's panel arrives signed
        if (gather && !(pairs && te.vbmi) && (!pairs || te.kq == 33 || te.kq == 23 || te.dotKind != DOT_SDOT)) {
            for (qd in range(rq)) {
                signCol[qd] = kq_sign_bytes(te, wbP, blk, qd, "sgc{blk}_{qd}")
            }
        }
        if (pairs && te.vbmi) {
            emit_grid_rows_blk_vbmi(te, wbP, xb, blk, a, a1, iq2)
        } elif (pairs) {
            te.gridRows = true
            emit_grid_rows_blk(te, wbP, xb, blk, signCol, a, a1, iq2)
            te.gridRows = false
        }
        for (j in range(pairs ? 0 : 4)) {
            for (qd in range(rq)) {
                var noff = LLVMBuildAdd(b, wb, te.types->ConstI64(uint64((blk * 16 + j * 4) * mr + qd * w8)), "")
                var wlo : LLVMOpaqueValue?
                var whi : LLVMOpaqueValue?
                if (panel) {
                    var hoff = LLVMBuildAdd(b, noff, te.types->ConstI64(uint64(128 * mr)), "")
                    wlo = load_vec(te, te.vwi8, wbase, noff, "wlo{blk}_{j * rq + qd}")
                    whi = load_vec(te, te.vwi8, wbase, hoff, "whi{blk}_{j * rq + qd}")
                    if (signCol[qd] != null) {
                        wlo = apply_sign_col(te, wlo, signCol[qd], j / 2, j % 2, "wlo{blk}_{j * rq + qd}s")
                        whi = apply_sign_col(te, whi, signCol[qd], 2 + j / 2, j % 2, "whi{blk}_{j * rq + qd}s")
                    }
                } else {
                    var nv = load_vec(te, te.vwi8, te.wg, noff, "nv{blk}_{j * rq + qd}")
                    var loIdx = LLVMBuildAnd(b, nv, splat_i8w(te, 15), "nib.lo")
                    var hiIdx = LLVMBuildLShr(b, nv, splat_i8w(te, 4), "nib.hi")
                    wlo = lut_lookup(te, loIdx, "wlo{blk}_{j * rq + qd}")
                    whi = lut_lookup(te, hiIdx, "whi{blk}_{j * rq + qd}")
                }
                var ulo = abs_w(te, wlo, "ulo{blk}_{j * rq + qd}")
                var uhi = abs_w(te, whi, "uhi{blk}_{j * rq + qd}")
                for (i in range(tokCount)) {
                    if (iq2) {
                        a[i * rq + qd] = dot_lane(te, a[i * rq + qd], wlo, ulo, xv0[i], j)
                        a1[i * rq + qd] = dot_lane(te, a1[i * rq + qd], whi, uhi, xv1[i], j)
                    } else {
                        a[i * rq + qd] = dot_lane(te, dot_lane(te, a[i * rq + qd], wlo, ulo, xv0[i], j), whi, uhi, xv1[i], j)
                    }
                }
            }
        }
        if (nl) {
            // per-block fold: f += sitofp(idot) * (d_blk * d8) - q40's shape, straight to float
            var dblk : LLVMOpaqueValue? [2]
            for (qd in range(rq)) {
                dblk[qd] = load_f16_vec_at(te, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(blk * 2 * mr + 2 * (qd * te.rv))), ""), "dnl{blk}_{qd}")
            }
            for (i in range(tokCount)) {
                for (qd in range(rq)) {
                    var dsn = LLVMBuildFMul(b, dblk[qd], d8nl[i], "dsn{tokBase + i}_{blk}_{qd}")
                    f[i * rq + qd] = fold_fma(te, LLVMBuildSIToFP(b, a[i * rq + qd], te.vnf32, ""), dsn, f[i * rq + qd], "f{tokBase + i}_{blk}_{qd}n")
                }
            }
        } else {
            var scv : LLVMOpaqueValue? [2]
            var scv1 : LLVMOpaqueValue? [2]   // iq2s: the hi half's strip
            for (qd in range(rq)) {
                if (iq2) {
                    var s0p = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(4 * mr + 2 * blk * mr + qd * te.rv)), ""), "s0p{blk}_{qd}")
                    var s1p = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(4 * mr + (2 * blk + 1) * mr + qd * te.rv)), ""), "s1p{blk}_{qd}")
                    scv[qd] = LLVMBuildZExt(b, LLVMBuildLoad2Aligned(b, vri8, s0p, 1u, ""), te.vni32, "sc{blk}_{qd}a")
                    scv1[qd] = LLVMBuildZExt(b, LLVMBuildLoad2Aligned(b, vri8, s1p, 1u, ""), te.vni32, "sc{blk}_{qd}b")
                    continue
                }
                var scp = LLVMBuildGEP2(b, te.types.t_int8, te.sg, LLVMBuildAdd(b, sb, te.types->ConstI64(uint64(4 * mr + blk * mr + qd * te.rv)), ""), "scp{blk}_{qd}")
                scv[qd] = LLVMBuildSExt(b, LLVMBuildLoad2Aligned(b, vri8, scp, 1u, ""), te.vni32, "sc{blk}_{qd}")
            }
            for (i in range(tokCount)) {
                for (qd in range(rq)) {
                    var ia = LLVMBuildMul(b, scv[qd], a[i * rq + qd], "")
                    if (iq2) {
                        ia = LLVMBuildAdd(b, ia, LLVMBuildMul(b, scv1[qd], a1[i * rq + qd], ""), "")
                    }
                    iacc[i * rq + qd] = LLVMBuildAdd(b, iacc[i * rq + qd], ia, "ia{tokBase + i}_{blk}_{qd}")
                }
            }
        }
    }
    if (nl) {
        return   // the per-block fold already landed everything in f
    }
    for (i in range(tokCount)) {
        let tk = tokBase + i
        var qp = LLVMBuildGEP2(b, te.types.t_float, te.xs[tk], sbi, "qp{tk}")
        var d8 = LLVMBuildLoad2Aligned(b, te.types.t_float, qp, 4u, "d8{tk}")
        var d8v = splat_f32(te, d8, "d8v{tk}")
        for (qd in range(rq)) {
            var ds = LLVMBuildFMul(b, dv[qd], d8v, "ds{tk}_{qd}")
            f[i * rq + qd] = fold_fma(te, LLVMBuildSIToFP(b, iacc[i * rq + qd], te.vnf32, ""), ds, f[i * rq + qd], "f{tk}_{qd}s")
        }
    }
}

// Block-emitter dispatch: the slice/loop machinery is quant-agnostic. The MMA form needs a
// token PAIR — single-token slices ride the sdot lattice instead. kq blocks are SUPERBLOCKS
// (the kq gemv/tile drive emit_slice with nb = n/256).
def private emit_one_block(var te : TileEmit; var bi : LLVMOpaqueValue?; var f : LLVMOpaqueValue? [8]; tokBase, tokCount : int) {
    if (te.kq == 44 || te.kq == 33 || te.kq == 34 || te.kq == 45 || te.kq == 23 || te.kq == 24 || te.kq == 25) {
        emit_block_iq4xs(te, bi, f, tokBase, tokCount)
    } elif (te.kq != 0) {
        emit_block_kqv2(te, bi, f, tokBase, tokCount)
    } elif (te.q51) {
        emit_block_q51(te, bi, f, tokBase, tokCount)
    } elif (te.mx4) {
        emit_block_mx4(te, bi, f, tokBase, tokCount)
    } elif (te.dotKind == DOT_SMMLA && tokCount >= 2) {
        emit_block_smmla(te, bi, f, tokBase, tokCount)
    } else {
        emit_block(te, bi, f, tokBase, tokCount)
    }
}

def private is_tile_signature(fn : Function const?) : bool {
    if (length(fn.arguments) != 10 || fn.result.baseType != Type.tVoid) return false
    for (i in range(6)) {
        if (fn.arguments[i]._type.baseType != Type.tPointer) return false
    }
    for (i in range(6, 10)) {
        if (fn.arguments[i]._type.baseType != Type.tInt64) return false
    }
    return true
}

// slice parameters shared by emit_slice and its caller
struct private SliceArgs {
    gc_impl : LLVMOpaqueValue?
    ctx : LLVMOpaqueContext?
    nb : LLVMOpaqueValue?
    d : LLVMOpaqueValue?
    g : LLVMOpaqueValue?
    t0 : LLVMOpaqueValue?
    yp : LLVMOpaqueValue?
    kstep : int
}

// One accumulator slice: the whole k loop for tokens [tokBase, tokBase+tokCount), then the
// per-token float-vector stores. `pred` must be terminator-free; returns the slice's merge
// block (also terminator-free). Main loop runs kstep blocks/iteration; remainder is a kstep-1 conditional chain.
def private emit_slice(var te : TileEmit; var sa : SliceArgs; var pred : LLVMOpaqueBasicBlock?; tokBase, tokCount : int; sfx : string) : LLVMOpaqueBasicBlock? { // nolint:STYLE038 — the block/phi plumbing of one tile slice is a single control-flow unit
    let b = te.builder
    let K = sa.kstep
    let nacc = tokCount * te.rq
    var head = LLVMAppendBasicBlockInContext(sa.ctx, sa.gc_impl, "head{sfx}")
    var body = LLVMAppendBasicBlockInContext(sa.ctx, sa.gc_impl, "body{sfx}")
    var checks : array<LLVMOpaqueBasicBlock?>
    var tails : array<LLVMOpaqueBasicBlock?>
    checks |> reserve(K - 1)
    tails |> reserve(K - 1)
    for (j in range(K - 1)) {
        let jsfx = j == 0 ? "" : "{j}"
        checks |> push(LLVMAppendBasicBlockInContext(sa.ctx, sa.gc_impl, "tail_check{jsfx}{sfx}"))
        tails |> push(LLVMAppendBasicBlockInContext(sa.ctx, sa.gc_impl, "tail_body{jsfx}{sfx}"))
    }
    var after = LLVMAppendBasicBlockInContext(sa.ctx, sa.gc_impl, "after{sfx}")

    LLVMPositionBuilderAtEnd(b, pred)
    LLVMBuildBr(b, head)

    // head: bi/f phis (all phis first — verifier rule); run the K-block body while bi+K <= nb
    LLVMPositionBuilderAtEnd(b, head)
    var biPhi = LLVMBuildPhi(b, te.types.t_int64, "bi{sfx}")
    var fPhi : LLVMOpaqueValue? [8]
    for (i in range(nacc)) {
        fPhi[i] = LLVMBuildPhi(b, te.vnf32, "fphi{tokBase + i / te.rq}")
    }
    var entryValsBi <- [LLVMConstNull(te.types.t_int64)]
    var entryBlocks <- [pred]
    LLVMAddIncoming(biPhi, entryValsBi, entryBlocks)
    for (i in range(nacc)) {
        var zv <- [LLVMConstNull(te.vnf32)]
        var zb <- [pred]
        LLVMAddIncoming(fPhi[i], zv, zb)
    }
    var biPK = LLVMBuildAdd(b, biPhi, te.types->ConstI64(uint64(K)), "biP{K}")
    var stepCond = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLE, biPK, sa.nb, "pair")
    LLVMBuildCondBr(b, stepCond, body, K > 1 ? checks[0] : after)

    // body: K blocks per iteration
    LLVMPositionBuilderAtEnd(b, body)
    var fCur : LLVMOpaqueValue? [8]
    for (i in range(nacc)) {
        fCur[i] = fPhi[i]
    }
    emit_one_block(te, biPhi, fCur, tokBase, tokCount)
    for (j in range(1, K)) {
        var bij = LLVMBuildAdd(b, biPhi, te.types->ConstI64(uint64(j)), "")
        emit_one_block(te, bij, fCur, tokBase, tokCount)
    }
    // the back edge starts wherever the block emitters LEFT the builder (a splitting emitter ends in its continuation block)
    var bodyEnd = LLVMGetInsertBlock(b)
    LLVMBuildBr(b, head)
    var bodyValsBi <- [biPK]
    var bodyBlocks <- [bodyEnd]
    LLVMAddIncoming(biPhi, bodyValsBi, bodyBlocks)
    for (i in range(nacc)) {
        var bv <- [fCur[i]]
        var bb <- [bodyEnd]
        LLVMAddIncoming(fPhi[i], bv, bb)
    }

    // remainder chain: check j falls through to `after` when no blocks remain, else tail body runs one more
    var mergeBlocks : array<LLVMOpaqueBasicBlock?>
    var mergeF : array<LLVMOpaqueValue? [8]>
    mergeBlocks |> reserve(K)
    mergeF |> reserve(K)
    var chainF : LLVMOpaqueValue? [8]
    for (i in range(nacc)) {
        chainF[i] = fPhi[i]
    }
    var chainBi = biPhi
    if (K == 1) {
        mergeBlocks |> push(head)
        mergeF |> push(chainF)
    }
    for (j in range(K - 1)) {
        LLVMPositionBuilderAtEnd(b, checks[j])
        var oddCond = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, chainBi, sa.nb, "odd")
        LLVMBuildCondBr(b, oddCond, tails[j], after)
        mergeBlocks |> push(checks[j])
        mergeF |> push(chainF)
        LLVMPositionBuilderAtEnd(b, tails[j])
        emit_one_block(te, chainBi, chainF, tokBase, tokCount)
        if (j + 1 < K - 1) {
            chainBi = LLVMBuildAdd(b, chainBi, te.types->ConstI64(1ul), "")
            LLVMBuildBr(b, checks[j + 1])
        } else {
            var tailEnd = LLVMGetInsertBlock(b)   // same rule: the tail body may have split its block
            LLVMBuildBr(b, after)
            mergeBlocks |> push(tailEnd)
            mergeF |> push(chainF)
        }
    }

    // after: merge phis (grouped first — verifier rule), then one float-vector store per token per row vector
    LLVMPositionBuilderAtEnd(b, after)
    var fF : LLVMOpaqueValue? [8]
    for (i in range(nacc)) {
        fF[i] = LLVMBuildPhi(b, te.vnf32, "ff{tokBase + i / te.rq}")
        for (mb, mf in mergeBlocks, mergeF) {
            var iv <- [mf[i]]
            var ib <- [mb]
            LLVMAddIncoming(fF[i], iv, ib)
        }
    }
    var col = LLVMBuildMul(b, sa.g, te.types->ConstI64(uint64(te.interleave)), "col")
    for (i in range(tokCount)) {
        let tk = tokBase + i
        var tRow = LLVMBuildAdd(b, sa.t0, te.types->ConstI64(uint64(tk)), "")
        var yoffBase = LLVMBuildAdd(b, LLVMBuildMul(b, tRow, sa.d, ""), col, "")
        for (qd in range(te.rq)) {
            var yoff = qd == 0 ? yoffBase : LLVMBuildAdd(b, yoffBase, te.types->ConstI64(uint64(qd * te.rv)), "")
            var yptr = LLVMBuildGEP2(b, te.types.t_float, sa.yp, yoff, qd == 0 ? "y{tk}" : "y{tk}_{qd}")
            LLVMBuildStoreAligned(b, fF[i * te.rq + qd], yptr, 4u)
        }
    }
    return after
}

// width-derived types + the per-ISA dot machinery for one generator invocation; false =
// decline (belt — perm_declines already gated the same rails). needMx4 additionally wires the
// LUT-dequant primitive (tbl1 / pshufb) and bakes the doubled-e2m1 LUT constant.
def private setup_tile_emit(var te : TileEmit; var gc : LlvmCodeCtx; p : TilePerm; needMx4 : bool) : bool { // nolint:STYLE037,STYLE038 — per-perm type/width/plane setup is one coherent table
    let rt = q8q8_repack_type(p.mr, p.bias, kgroup_of(p))
    te.builder = gc.jit.builder
    te.types = gc.jit.types
    te.interleave = rt.interleave
    te.dotKind = dot_kind(p.dotPrim)
    // mx4/kq/q51 planes are kgroup-independent — under an smmla perm they ride the plain sdot lattice
    if ((te.mx4 || te.q51 || te.kq != 0) && te.dotKind == DOT_SMMLA) {
        te.dotKind = DOT_SDOT
    }
    te.kgroup = (te.mx4 || te.q51 || te.kq != 0) ? 4 : rt.kgroup
    // the perm's bias applies to the Q8 weight plane only — mx4/kq/q51 planes never see the biased repack
    te.bias = (te.mx4 || te.q51 || te.kq != 0) ? 0 : p.bias
    te.width = (te.dotKind == DOT_SDOT || te.dotKind == DOT_SMMLA) ? 128 : p.width
    te.rv = te.width / 32
    te.rq = rt.interleave / te.rv
    te.v16i8 = LLVMVectorType(te.types.t_int8, 16u)
    te.v4i32 = te.types->LLVMInt4Type()
    te.vwi8 = LLVMVectorType(te.types.t_int8, uint(te.width / 8))
    te.vni32 = LLVMVectorType(te.types.t_int32, uint(te.rv))
    te.vnf32 = LLVMVectorType(te.types.t_float, uint(te.rv))
    te.vni16 = LLVMVectorType(te.types.t_int16, uint(te.rv))
    te.vnf16 = LLVMVectorType(LLVMHalfTypeInContext(gc.jit.ctx), uint(te.rv))
    let mod = LLVMGetGlobalParent(gc.impl)
    if (te.dotKind == DOT_SDOT || te.dotKind == DOT_SMMLA) {
        let id = LLVMLookupIntrinsicID("llvm.aarch64.neon.sdot")
        var declTypes <- [te.vni32, te.v16i8]
        te.sdot_decl = LLVMGetIntrinsicDeclaration(mod, id, declTypes)
        if (te.sdot_decl == null) return false      // no NEON dotprod on this target: decline
        var fnArgTypes <- [te.vni32, te.v16i8, te.v16i8]
        te.sdot_ty = LLVMFunctionType(te.vni32, fnArgTypes)
        if (te.dotKind == DOT_SMMLA) {
            let mmId = LLVMLookupIntrinsicID("llvm.aarch64.neon.smmla")
            var mmTypes <- [te.vni32, te.v16i8]
            te.smmla_decl = LLVMGetIntrinsicDeclaration(mod, mmId, mmTypes)
            if (te.smmla_decl == null) return false // lockstep with perm_declines' smmla rail
            var mmArgTypes <- [te.vni32, te.v16i8, te.v16i8]
            te.smmla_ty = LLVMFunctionType(te.vni32, mmArgTypes)
        }
        if (needMx4) {
            let tblId = LLVMLookupIntrinsicID("llvm.aarch64.neon.tbl1")
            var tblDeclTypes <- [te.v16i8]
            te.tbl_decl = LLVMGetIntrinsicDeclaration(mod, tblId, tblDeclTypes)
            if (te.tbl_decl == null) return false   // lockstep with perm_declines' tbl rail
            var tblArgTypes <- [te.v16i8, te.v16i8]
            te.tbl_ty = LLVMFunctionType(te.v16i8, tblArgTypes)
        }
    } else {
        var noTypes : array<LLVMOpaqueType?>
        var b2Types <- [te.vwi8, te.vwi8]
        if (te.dotKind != DOT_VPDPBSSD && te.bias == 0) {
            // the sign-trick machinery (|w| via llvm.abs, sign apply via VPSIGNB) — bias128 needs neither
            let absId = LLVMLookupIntrinsicID("llvm.abs")
            var absTypes <- [te.vwi8]
            te.abs_decl = LLVMGetIntrinsicDeclaration(mod, absId, absTypes)
            if (te.abs_decl == null) return false
            var absArgTypes <- [te.vwi8, te.types.t_int1]
            te.abs_ty = LLVMFunctionType(te.vwi8, absArgTypes)
            if (te.width == 256) {
                let psId = LLVMLookupIntrinsicID("llvm.x86.avx2.psign.b")
                te.psign_decl = LLVMGetIntrinsicDeclaration(mod, psId, noTypes)
                if (te.psign_decl == null) return false
                te.psign_ty = LLVMFunctionType(te.vwi8, b2Types)
            }
        }
        if (te.bias != 0) {
            // the gemv inline-bsum reduce (the tile reads the bsums plane instead)
            let redId = LLVMLookupIntrinsicID("llvm.vector.reduce.add")
            var redTypes <- [te.vni32]
            te.reduce_decl = LLVMGetIntrinsicDeclaration(mod, redId, redTypes)
            if (te.reduce_decl == null) return false
            var redArgTypes <- [te.vni32]
            te.reduce_ty = LLVMFunctionType(te.types.t_int32, redArgTypes)
        }
        te.pf = p.pf
        if (te.pf != 0) {
            var pty = LLVMPointerType(te.types.t_int8, 0u)
            var pfTypes <- [pty]
            te.prefetch_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.prefetch"), pfTypes)
            if (te.prefetch_decl == null) return false
            var pfArgTypes <- [pty, te.types.t_int32, te.types.t_int32, te.types.t_int32]
            te.prefetch_ty = LLVMFunctionType(te.types.t_void, pfArgTypes)
        }
        te.vbmi = p.grid == "vbmi"
        if (te.vbmi) {
            var b3Types <- [te.vwi8, te.vwi8, te.vwi8]
            te.permi2b_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.avx512.vpermi2var.qi.512"), noTypes)
            te.mshift_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.avx512.pmultishift.qb.512"), noTypes)
            te.shufb_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.avx512.pshuf.b.512"), noTypes)
            if (te.permi2b_decl == null || te.mshift_decl == null || te.shufb_decl == null) return false
            te.permi2b_ty = LLVMFunctionType(te.vwi8, b3Types)
            te.mshift_ty = LLVMFunctionType(te.vwi8, b2Types)
            te.shufb_ty = LLVMFunctionType(te.vwi8, b2Types)
        }
        if (te.dotKind == DOT_MADDUBS) {
            let maddId = LLVMLookupIntrinsicID(te.width == 256 ? "llvm.x86.avx2.pmadd.ub.sw" : "llvm.x86.avx512.pmaddubs.w.512")
            te.madd_decl = LLVMGetIntrinsicDeclaration(mod, maddId, noTypes)
            if (te.madd_decl == null) return false
            var vhi16 = LLVMVectorType(te.types.t_int16, uint(te.width / 16))
            te.madd_ty = LLVMFunctionType(vhi16, b2Types)
            let maddwdId = LLVMLookupIntrinsicID(te.width == 256 ? "llvm.x86.avx2.pmadd.wd" : "llvm.x86.avx512.pmaddw.d.512")
            te.maddwd_decl = LLVMGetIntrinsicDeclaration(mod, maddwdId, noTypes)
            if (te.maddwd_decl == null) return false
            var w2Types <- [vhi16, vhi16]
            te.maddwd_ty = LLVMFunctionType(te.vni32, w2Types)
        } else {
            let dpId = LLVMLookupIntrinsicID(te.dotKind == DOT_VPDPBSSD ? "llvm.x86.avx2.vpdpbssd.256"
                : (te.width == 256 ? "llvm.x86.avx512.vpdpbusd.256" : "llvm.x86.avx512.vpdpbusd.512"))
            te.dp_decl = LLVMGetIntrinsicDeclaration(mod, dpId, noTypes)
            if (te.dp_decl == null) return false
            var dpArgTypes <- [te.vni32, te.vwi8, te.vwi8]
            te.dp_ty = LLVMFunctionType(te.vni32, dpArgTypes)
        }
        if (needMx4) {
            let shId = LLVMLookupIntrinsicID(te.width == 256 ? "llvm.x86.avx2.pshuf.b" : "llvm.x86.avx512.pshuf.b.512")
            te.pshuf_decl = LLVMGetIntrinsicDeclaration(mod, shId, noTypes)
            if (te.pshuf_decl == null) return false
            te.pshuf_ty = LLVMFunctionType(te.vwi8, b2Types)
        }
    }
    if (needMx4) {
        // the nibble codebook as a constant vector (mx4: doubled e2m1; kq 44: kvalues_iq4nl) - no runtime table pointer; broadcast per 16-byte lane on x64
        let e2m1 = fixed_array(0, 1, 2, 3, 4, 6, 8, 12, 0, -1, -2, -3, -4, -6, -8, -12)
        let iq4nl = iq4nl_lut()
        let lanes = te.dotKind == DOT_SDOT ? 1 : te.width / 128
        var lutElems : array<LLVMOpaqueValue?>
        lutElems |> reserve(16 * lanes)
        for (_lane in range(lanes)) {
            for (i in range(16)) {
                let v = te.kq == 44 || te.kq == 45 ? int(iq4nl[i]) : e2m1[i]
                lutElems |> push(LLVMConstInt(te.types.t_int8, uint64(v & 255), 0))
            }
        }
        te.lut = LLVMConstVector(array_data_ptr(lutElems), uint(16 * lanes))
    }
    return true
}

// AMX machinery for the TMUL tile: virtual-tile x86_amx values via the .internal intrinsics
// (LLVM's codegen-prep materializes tmm allocation/ldtilecfg). A latch perm swaps in the
// RAW immediate-tmm intrinsics instead — fixed register plan, no backend config ops.
def private setup_amx_emit(var te : TileEmit; var gc : LlvmCodeCtx; p : TilePerm) : bool {
    let raw = p.latch != 0
    te.builder = gc.jit.builder
    te.types = gc.jit.types
    te.dotKind = DOT_AMX
    te.interleave = 16
    te.rv = 16
    te.rawTiles = raw
    te.bias = p.bias
    te.vni32 = LLVMVectorType(te.types.t_int32, 16u)
    te.vnf32 = LLVMVectorType(te.types.t_float, 16u)
    te.vni16 = LLVMVectorType(te.types.t_int16, 16u)
    te.vnf16 = LLVMVectorType(LLVMHalfTypeInContext(gc.jit.ctx), 16u)
    let mod = LLVMGetGlobalParent(gc.impl)
    var noTypes : array<LLVMOpaqueType?>
    var ptrTy = te.types->LLVMVoidPtrType()
    if (raw) {
        te.tload_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.tileloadd64"), noTypes)
        te.tdp_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID(p.bias != 0 ? "llvm.x86.tdpbsud" : "llvm.x86.tdpbssd"), noTypes)
        te.tstore_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.tilestored64"), noTypes)
        te.tzero_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.tilezero"), noTypes)
        if (te.tload_decl == null || te.tdp_decl == null || te.tstore_decl == null || te.tzero_decl == null) {
            return false      // belt — perm_declines already gated the same decls
        }
        var loadArgs <- [te.types.t_int8, ptrTy, te.types.t_int64]
        te.tload_ty = LLVMFunctionType(te.types.t_void, loadArgs)
        var dpArgs <- [te.types.t_int8, te.types.t_int8, te.types.t_int8]
        te.tdp_ty = LLVMFunctionType(te.types.t_void, dpArgs)
        var storeArgs <- [te.types.t_int8, ptrTy, te.types.t_int64]
        te.tstore_ty = LLVMFunctionType(te.types.t_void, storeArgs)
        var zeroArgs <- [te.types.t_int8]
        te.tzero_ty = LLVMFunctionType(te.types.t_void, zeroArgs)
        return true
    }
    te.t_amx = LLVMX86AMXTypeInContext(gc.jit.ctx)
    if (te.t_amx == null) return false
    te.tload_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.tileloadd64.internal"), noTypes)
    te.tdp_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID(p.bias != 0 ? "llvm.x86.tdpbsud.internal" : "llvm.x86.tdpbssd.internal"), noTypes)
    te.tstore_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.tilestored64.internal"), noTypes)
    te.tzero_decl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.tilezero.internal"), noTypes)
    if (te.tload_decl == null || te.tdp_decl == null || te.tstore_decl == null || te.tzero_decl == null) {
        return false      // belt — perm_declines already gated the same decls
    }
    var loadArgs <- [te.types.t_int16, te.types.t_int16, ptrTy, te.types.t_int64]
    te.tload_ty = LLVMFunctionType(te.t_amx, loadArgs)
    var dpArgs <- [te.types.t_int16, te.types.t_int16, te.types.t_int16, te.t_amx, te.t_amx, te.t_amx]
    te.tdp_ty = LLVMFunctionType(te.t_amx, dpArgs)
    var storeArgs <- [te.types.t_int16, te.types.t_int16, ptrTy, te.types.t_int64, te.t_amx]
    te.tstore_ty = LLVMFunctionType(te.types.t_void, storeArgs)
    var zeroArgs <- [te.types.t_int16, te.types.t_int16]
    te.tzero_ty = LLVMFunctionType(te.t_amx, zeroArgs)
    return true
}

// The raw register plan (latch perms): C tiles tmm0-3 by (r·nt+ts), A tiles tmm4+ts,
// B tiles tmm6+r; the cfg companion's palette sets exactly these shapes (C 16×64, A 16×32,
// B 8×64). Internal-SSA perms ignore the plan — the backend allocates tmm itself.
def private amx_tmm(te : TileEmit; v : int) : LLVMOpaqueValue? {
    return LLVMConstInt(te.types.t_int8, uint64(v), 0)
}

// one B-plane tile load: internal returns the virtual tile; raw loads into tmm6+r, null result
def private amx_load_b(var te : TileEmit; r : int; var wp : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    if (te.rawTiles) {
        var largs <- [amx_tmm(te, 6 + r), wp, te.types->ConstI64(64ul)]
        LLVMBuildCall2(b, te.tload_ty, te.tload_decl, largs, "")
        return null
    }
    var largs <- [te.types->ConstI16(int16(8)), te.types->ConstI16(int16(64)), wp, te.types->ConstI64(64ul)]
    return LLVMBuildCall2(b, te.tload_ty, te.tload_decl, largs, name)
}

// one A-activation tile load (stride = n): internal virtual tile; raw into tmm4+ts
def private amx_load_a(var te : TileEmit; ts : int; var xp, n : LLVMOpaqueValue?; name : string) : LLVMOpaqueValue? {
    let b = te.builder
    if (te.rawTiles) {
        var largs <- [amx_tmm(te, 4 + ts), xp, n]
        LLVMBuildCall2(b, te.tload_ty, te.tload_decl, largs, "")
        return null
    }
    var largs <- [te.types->ConstI16(int16(16)), te.types->ConstI16(int16(32)), xp, n]
    return LLVMBuildCall2(b, te.tload_ty, te.tload_decl, largs, name)
}

// one C chain head: zero + TDPBSSD. Internal returns the dotted virtual tile; raw runs on
// tmm(r·nt+ts) in place, null result
def private amx_dot(var te : TileEmit; nt, r, ts : int; var av, bv : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let b = te.builder
    if (te.rawTiles) {
        var zargs <- [amx_tmm(te, r * nt + ts)]
        LLVMBuildCall2(b, te.tzero_ty, te.tzero_decl, zargs, "")
        var dargs <- [amx_tmm(te, r * nt + ts), amx_tmm(te, 4 + ts), amx_tmm(te, 6 + r)]
        LLVMBuildCall2(b, te.tdp_ty, te.tdp_decl, dargs, "")
        return null
    }
    var zargs <- [te.types->ConstI16(int16(16)), te.types->ConstI16(int16(64))]
    var cz = LLVMBuildCall2(b, te.tzero_ty, te.tzero_decl, zargs, "cz")
    var dargs <- [te.types->ConstI16(int16(16)), te.types->ConstI16(int16(64)), te.types->ConstI16(int16(32)), cz, av, bv]
    return LLVMBuildCall2(b, te.tdp_ty, te.tdp_decl, dargs, "cd")
}

// spill one C tile to scratch (stride 64)
def private amx_store(var te : TileEmit; nt, r, ts : int; var cd, reg : LLVMOpaqueValue?) {
    let b = te.builder
    if (te.rawTiles) {
        var sargs <- [amx_tmm(te, r * nt + ts), reg, te.types->ConstI64(64ul)]
        LLVMBuildCall2(b, te.tstore_ty, te.tstore_decl, sargs, "")
        return
    }
    var sargs <- [te.types->ConstI16(int16(16)), te.types->ConstI16(int16(64)), reg, te.types->ConstI64(64ul), cd]
    LLVMBuildCall2(b, te.tstore_ty, te.tstore_decl, sargs, "")
}

// The TMUL batch tile: one call covers 16·nt tokens × nt grp16 row-groups (nt = nrsplit).
// Per 32-k block: nt B/A tile loads, nt² tilezero->TDPBSSD->tilestore-to-scratch, then the
// scale fold f32-accumulates INTO yp — yp IS the k-loop accumulator (no register holds 32×32 f32 across blocks).
def private emit_amx_tile(var gc : LlvmCodeCtx; p : TilePerm; f16s : bool) : bool { // nolint:STYLE038 — the AMX tile body is barrier-coupled; phases share tile registers
    var te = TileEmit(sgF16 = f16s)
    if (!setup_amx_emit(te, gc, p)) return false
    let b = gc.jit.builder
    let nt = p.nrsplit
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    var head = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "head")
    var body = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "body")
    var after = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "after")

    LLVMPositionBuilderAtEnd(b, entry)
    var yp = LLVMGetParam(gc.impl, 0u)
    var wg = LLVMGetParam(gc.impl, 1u)
    var sg = LLVMGetParam(gc.impl, 2u)
    var xqp = LLVMGetParam(gc.impl, 3u)
    var xsp = LLVMGetParam(gc.impl, 4u)
    var xbsp = LLVMGetParam(gc.impl, 5u)
    var n = LLVMGetParam(gc.impl, 6u)
    var d = LLVMGetParam(gc.impl, 7u)
    var g = LLVMGetParam(gc.impl, 8u)
    var t0 = LLVMGetParam(gc.impl, 9u)
    var nb = LLVMBuildSDiv(b, n, te.types->ConstI64(32ul), "nb")
    var col = LLVMBuildMul(b, g, te.types->ConstI64(16ul), "col")
    // C spill: one 16×64 tile region, reused per (token tile, row group) fold
    var scratch = LLVMBuildArrayAlloca(b, te.types.t_int8, te.types->ConstI64(1024ul), "cspill")
    LLVMSetAlignment(scratch, 64u)
    var n16 = LLVMBuildMul(b, n, te.types->ConstI64(16ul), "n16")
    var nb16 = LLVMBuildMul(b, nb, te.types->ConstI64(16ul), "nb16")
    var wplane : LLVMOpaqueValue? [2]     // adjacent row-group planes (+16·n each)
    var sgoff : LLVMOpaqueValue? [2]      // their scale-region offsets (+16·nb floats)
    var xbase : LLVMOpaqueValue? [2]      // A tile activation bases (rows t0 + ts·16 ..)
    for (r in range(nt)) {
        wplane[r] = r == 0 ? wg : LLVMBuildGEP2(b, te.types.t_int8, wg, n16, "wplane{r}")
        sgoff[r] = r == 0 ? LLVMConstNull(te.types.t_int64) : nb16
    }
    for (ts in range(nt)) {
        var trow = ts == 0 ? t0 : LLVMBuildAdd(b, t0, te.types->ConstI64(uint64(ts * 16)), "")
        xbase[ts] = LLVMBuildGEP2(b, te.types.t_int8, xqp, LLVMBuildMul(b, trow, n, ""), "xbase{ts}")
    }
    // fold bases: y rows start at t0·d + col, xs/xbs rows at t0·nb, walked with running pointers (see amx_fold_tile)
    var ybase = LLVMBuildGEP2(b, te.types.t_float, yp, LLVMBuildAdd(b, LLVMBuildMul(b, t0, d, ""), col, ""), "ybase")
    var xsbase = LLVMBuildGEP2(b, te.types.t_float, xsp, LLVMBuildMul(b, t0, nb, ""), "xsbase")
    var xbsbase : LLVMOpaqueValue?
    if (te.bias != 0) {
        xbsbase = LLVMBuildGEP2(b, te.types.t_int32, xbsp, LLVMBuildMul(b, t0, nb, ""), "xbsbase")
    }
    var zrow = ybase
    for (t in range(16 * nt)) {
        if (t != 0) {
            zrow = LLVMBuildGEP2(b, te.types.t_float, zrow, d, "zr{t}")
        }
        for (r in range(nt)) {
            var zp = r == 0 ? zrow : LLVMBuildGEP2(b, te.types.t_float, zrow, te.types->ConstI64(uint64(r * 16)), "")
            LLVMBuildStoreAligned(b, LLVMConstNull(te.vnf32), zp, 4u)
        }
    }
    LLVMBuildBr(b, head)

    LLVMPositionBuilderAtEnd(b, head)
    var biPhi = LLVMBuildPhi(b, te.types.t_int64, "bi")
    var entryVals <- [LLVMConstNull(te.types.t_int64)]
    var entryBlocks <- [entry]
    LLVMAddIncoming(biPhi, entryVals, entryBlocks)
    var more = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, biPhi, nb, "more")
    LLVMBuildCondBr(b, more, body, after)

    LLVMPositionBuilderAtEnd(b, body)
    // B tiles: block bi is a contiguous 512-byte region per group plane (8 k-groups × 16×4)
    var wboff = LLVMBuildMul(b, biPhi, te.types->ConstI64(512ul), "wboff")
    var Bs : LLVMOpaqueValue? [2]
    for (r in range(nt)) {
        var wp2 = LLVMBuildGEP2(b, te.types.t_int8, wplane[r], wboff, "wb{r}")
        Bs[r] = amx_load_b(te, r, wp2, "bt{r}")
    }
    var xoff = LLVMBuildMul(b, biPhi, te.types->ConstI64(32ul), "xoff")
    var As : LLVMOpaqueValue? [2]
    for (ts in range(nt)) {
        var xp2 = LLVMBuildGEP2(b, te.types.t_int8, xbase[ts], xoff, "xa{ts}")
        As[ts] = amx_load_a(te, ts, xp2, n, "at{ts}")
    }
    var bi16 = LLVMBuildMul(b, biPhi, te.types->ConstI64(16ul), "bi16")
    for (r in range(nt)) {
        // ws_vec: the block's 16 group scales, mr-contiguous in the grp plane — one zmm
        var sidx = r == 0 ? bi16 : LLVMBuildAdd(b, bi16, sgoff[r], "")
        var ws = load_scale_vec(te, sg, sidx, "ws{r}")
        for (ts in range(nt)) {
            var cd = amx_dot(te, nt, r, ts, As[ts], Bs[r])
            amx_store(te, nt, r, ts, cd, scratch)
            amx_fold_tile(te, scratch, biPhi, ws, r, ts, ybase, d, xsbase, xbsbase, nb)
        }
    }
    var biNext = LLVMBuildAdd(b, biPhi, te.types->ConstI64(1ul), "binext")
    LLVMBuildBr(b, head)
    var bodyVals <- [biNext]
    var bodyBlocks <- [body]
    LLVMAddIncoming(biPhi, bodyVals, bodyBlocks)

    LLVMPositionBuilderAtEnd(b, after)
    LLVMBuildRetVoid(b)
    return true
}

// Fold ONE spilled C tile into yp: cvt(i32 row)·ws·splat(xs[t][bi]) accumulated (the serial
// tile's fold order). Addressing uses RUNNING POINTERS, not precomputed arrays — those spilled
// ~140 movq reloads/k-iteration, 28% of the steady loop (2026-07-09 census).
def private amx_fold_tile(var te : TileEmit; var region, bi, ws : LLVMOpaqueValue?; r, ts : int;
                          var ybase, d, xsbase, xbsbase, nb : LLVMOpaqueValue?) {
    let b = te.builder
    // tile row 0: y at (ts·16)·d + r·16, xs/xbs at (ts·16)·nb + bi — then step d / nb per row
    var yoff = LLVMBuildMul(b, d, te.types->ConstI64(uint64(ts * 16)), "")
    if (r != 0) {
        yoff = LLVMBuildAdd(b, yoff, te.types->ConstI64(uint64(r * 16)), "")
    }
    var yrow = LLVMBuildGEP2(b, te.types.t_float, ybase, yoff, "yr{ts}_{r}")
    var xoff = LLVMBuildAdd(b, LLVMBuildMul(b, nb, te.types->ConstI64(uint64(ts * 16)), ""), bi, "")
    var xsp = LLVMBuildGEP2(b, te.types.t_float, xsbase, xoff, "xr{ts}")
    var xbp : LLVMOpaqueValue?
    if (te.bias != 0) {
        xbp = LLVMBuildGEP2(b, te.types.t_int32, xbsbase, xoff, "br{ts}")
    }
    for (t16 in range(16)) {
        let t = ts * 16 + t16
        if (t16 != 0) {   // advance one token row: y by d floats, xs/xbs by nb elements
            yrow = LLVMBuildGEP2(b, te.types.t_float, yrow, d, "yr{t}_{r}")
            xsp = LLVMBuildGEP2(b, te.types.t_float, xsp, nb, "xr{t}")
            if (te.bias != 0) {
                xbp = LLVMBuildGEP2(b, te.types.t_int32, xbp, nb, "br{t}")
            }
        }
        var crp = t16 == 0 ? region : LLVMBuildGEP2(b, te.types.t_int8, region, te.types->ConstI64(uint64(t16 * 64)), "")
        var crow = LLVMBuildLoad2Aligned(b, te.vni32, crp, 64u, "c{t}_{r}")
        if (te.bias != 0) {
            var bsv = LLVMBuildLoad2Aligned(b, te.types.t_int32, xbp, 4u, "bsum{t}")
            crow = LLVMBuildAdd(b, crow, splat_i32(te, bsv, "bss{t}"), "cb{t}_{r}")
        }
        var cf = LLVMBuildSIToFP(b, crow, te.vnf32, "cf{t}_{r}")
        var q = LLVMBuildLoad2Aligned(b, te.types.t_float, xsp, 4u, "q{t}")
        var sq = LLVMBuildFMul(b, ws, splat_f32(te, q, "qs{t}"), "")
        var yv = LLVMBuildLoad2Aligned(b, te.vnf32, yrow, 4u, "")
        LLVMBuildStoreAligned(b, fold_fma(te, cf, sq, yv, "f{t}_{r}"), yrow, 4u)
    }
}

// The software-pipelined TMUL batch tile (double-buffered C): same contract as
// emit_amx_tile, but the per-block scale fold is deferred one k-iteration — block bi's
// TDPBSSD spills into C[cur] while block bi−1's fold reads C[pre], overlapping TMUL with the vector fold.
def private emit_amx_tile_pipelined(var gc : LlvmCodeCtx; p : TilePerm; f16s : bool) : bool { // nolint:STYLE037,STYLE038 — the software-pipelined AMX schedule is one register-coupled loop
    var te = TileEmit(sgF16 = f16s)
    if (!setup_amx_emit(te, gc, p)) return false
    let b = gc.jit.builder
    let nt = p.nrsplit
    let ntiles = nt * nt
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    var head = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "head")
    var body = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "body")
    var after = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "after")

    LLVMPositionBuilderAtEnd(b, entry)
    var yp = LLVMGetParam(gc.impl, 0u)
    var wg = LLVMGetParam(gc.impl, 1u)
    var sg = LLVMGetParam(gc.impl, 2u)
    var xqp = LLVMGetParam(gc.impl, 3u)
    var xsp = LLVMGetParam(gc.impl, 4u)
    var xbsp = LLVMGetParam(gc.impl, 5u)
    var n = LLVMGetParam(gc.impl, 6u)
    var d = LLVMGetParam(gc.impl, 7u)
    var g = LLVMGetParam(gc.impl, 8u)
    var t0 = LLVMGetParam(gc.impl, 9u)
    var nb = LLVMBuildSDiv(b, n, te.types->ConstI64(32ul), "nb")
    var col = LLVMBuildMul(b, g, te.types->ConstI64(16ul), "col")
    // C spill: TWO ntiles-deep tile regions — the double buffer the fold deferral swaps over
    var scratch = LLVMBuildArrayAlloca(b, te.types.t_int8, te.types->ConstI64(uint64(2 * ntiles * 1024)), "cspill")
    LLVMSetAlignment(scratch, 64u)
    var buf1 = LLVMBuildGEP2(b, te.types.t_int8, scratch, te.types->ConstI64(uint64(ntiles * 1024)), "buf1")
    var n16 = LLVMBuildMul(b, n, te.types->ConstI64(16ul), "n16")
    var nb16 = LLVMBuildMul(b, nb, te.types->ConstI64(16ul), "nb16")
    var wplane : LLVMOpaqueValue? [2]     // adjacent row-group planes (+16·n each)
    var sgoff : LLVMOpaqueValue? [2]      // their scale-region offsets (+16·nb floats)
    var xbase : LLVMOpaqueValue? [2]      // A tile activation bases (rows t0 + ts·16 ..)
    for (r in range(nt)) {
        wplane[r] = r == 0 ? wg : LLVMBuildGEP2(b, te.types.t_int8, wg, n16, "wplane{r}")
        sgoff[r] = r == 0 ? LLVMConstNull(te.types.t_int64) : nb16
    }
    for (ts in range(nt)) {
        var trow = ts == 0 ? t0 : LLVMBuildAdd(b, t0, te.types->ConstI64(uint64(ts * 16)), "")
        xbase[ts] = LLVMBuildGEP2(b, te.types.t_int8, xqp, LLVMBuildMul(b, trow, n, ""), "xbase{ts}")
    }
    // fold bases: y rows start at t0·d + col, xs/xbs rows at t0·nb, walked with running pointers (see amx_fold_tile)
    var ybase = LLVMBuildGEP2(b, te.types.t_float, yp, LLVMBuildAdd(b, LLVMBuildMul(b, t0, d, ""), col, ""), "ybase")
    var xsbase = LLVMBuildGEP2(b, te.types.t_float, xsp, LLVMBuildMul(b, t0, nb, ""), "xsbase")
    var xbsbase : LLVMOpaqueValue?
    if (te.bias != 0) {
        xbsbase = LLVMBuildGEP2(b, te.types.t_int32, xbsp, LLVMBuildMul(b, t0, nb, ""), "xbsbase")
    }
    var zrow = ybase
    for (t in range(16 * nt)) {
        if (t != 0) {
            zrow = LLVMBuildGEP2(b, te.types.t_float, zrow, d, "zr{t}")
        }
        for (r in range(nt)) {
            var zp = r == 0 ? zrow : LLVMBuildGEP2(b, te.types.t_float, zrow, te.types->ConstI64(uint64(r * 16)), "")
            LLVMBuildStoreAligned(b, LLVMConstNull(te.vnf32), zp, 4u)
        }
    }
    // peel: block 0's TMUL chain into buf0 (its fold runs in the first loop iteration)
    var Bs : LLVMOpaqueValue? [2]
    var As : LLVMOpaqueValue? [2]
    for (r in range(nt)) {
        Bs[r] = amx_load_b(te, r, wplane[r], "bt0{r}")
    }
    for (ts in range(nt)) {
        As[ts] = amx_load_a(te, ts, xbase[ts], n, "at0{ts}")
    }
    for (r in range(nt)) {
        for (ts in range(nt)) {
            var cd = amx_dot(te, nt, r, ts, As[ts], Bs[r])
            let idx = r * nt + ts
            var reg = idx == 0 ? scratch : LLVMBuildGEP2(b, te.types.t_int8, scratch, te.types->ConstI64(uint64(idx * 1024)), "")
            amx_store(te, nt, r, ts, cd, reg)
        }
    }
    LLVMBuildBr(b, head)

    LLVMPositionBuilderAtEnd(b, head)
    var biPhi = LLVMBuildPhi(b, te.types.t_int64, "bi")
    var cPrePhi = LLVMBuildPhi(b, te.types->LLVMVoidPtrType(), "cpre")
    var cCurPhi = LLVMBuildPhi(b, te.types->LLVMVoidPtrType(), "ccur")
    var entryVals <- [te.types->ConstI64(1ul)]
    var entryBlocks <- [entry]
    LLVMAddIncoming(biPhi, entryVals, entryBlocks)
    var preEntryVals <- [scratch]
    LLVMAddIncoming(cPrePhi, preEntryVals, entryBlocks)
    var curEntryVals <- [buf1]
    LLVMAddIncoming(cCurPhi, curEntryVals, entryBlocks)
    var more = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, biPhi, nb, "more")
    LLVMBuildCondBr(b, more, body, after)

    LLVMPositionBuilderAtEnd(b, body)
    var ii = LLVMBuildSub(b, biPhi, te.types->ConstI64(1ul), "ii")
    var ii16 = LLVMBuildMul(b, ii, te.types->ConstI64(16ul), "ii16")
    // B tiles: block bi is a contiguous 512-byte region per group plane (8 k-groups × 16×4)
    var wboff = LLVMBuildMul(b, biPhi, te.types->ConstI64(512ul), "wboff")
    for (r in range(nt)) {
        var wp2 = LLVMBuildGEP2(b, te.types.t_int8, wplane[r], wboff, "wb{r}")
        Bs[r] = amx_load_b(te, r, wp2, "bt{r}")
    }
    var xoff = LLVMBuildMul(b, biPhi, te.types->ConstI64(32ul), "xoff")
    for (ts in range(nt)) {
        var xp2 = LLVMBuildGEP2(b, te.types.t_int8, xbase[ts], xoff, "xa{ts}")
        As[ts] = amx_load_a(te, ts, xp2, n, "at{ts}")
    }
    for (r in range(nt)) {
        // block bi−1's ws vector — the deferred fold's group scales (one zmm per r plane)
        var sidx = r == 0 ? ii16 : LLVMBuildAdd(b, ii16, sgoff[r], "")
        var ws = load_scale_vec(te, sg, sidx, "ws{r}")
        for (ts in range(nt)) {
            let idx = r * nt + ts
            // the deferral: fold block bi−1's tile from C[pre] while the TMUL chain owns block bi
            var preReg = idx == 0 ? cPrePhi : LLVMBuildGEP2(b, te.types.t_int8, cPrePhi, te.types->ConstI64(uint64(idx * 1024)), "")
            amx_fold_tile(te, preReg, ii, ws, r, ts, ybase, d, xsbase, xbsbase, nb)
            var cd = amx_dot(te, nt, r, ts, As[ts], Bs[r])
            var curReg = idx == 0 ? cCurPhi : LLVMBuildGEP2(b, te.types.t_int8, cCurPhi, te.types->ConstI64(uint64(idx * 1024)), "")
            amx_store(te, nt, r, ts, cd, curReg)
        }
    }
    var biNext = LLVMBuildAdd(b, biPhi, te.types->ConstI64(1ul), "binext")
    LLVMBuildBr(b, head)
    var bodyVals <- [biNext]
    var bodyBlocks <- [body]
    LLVMAddIncoming(biPhi, bodyVals, bodyBlocks)
    var preBodyVals <- [cCurPhi]      // the swap: last-written buffer becomes next pre
    LLVMAddIncoming(cPrePhi, preBodyVals, bodyBlocks)
    var curBodyVals <- [cPrePhi]
    LLVMAddIncoming(cCurPhi, curBodyVals, bodyBlocks)

    LLVMPositionBuilderAtEnd(b, after)
    // final fold: block nb−1's spill, post-swap — cPre holds the last-written buffer
    var iiF = LLVMBuildSub(b, nb, te.types->ConstI64(1ul), "iif")
    var iiF16 = LLVMBuildMul(b, iiF, te.types->ConstI64(16ul), "iif16")
    for (r in range(nt)) {
        var sidx = r == 0 ? iiF16 : LLVMBuildAdd(b, iiF16, sgoff[r], "")
        var ws = load_scale_vec(te, sg, sidx, "wsf{r}")
        for (ts in range(nt)) {
            let idx = r * nt + ts
            var preReg = idx == 0 ? cPrePhi : LLVMBuildGEP2(b, te.types.t_int8, cPrePhi, te.types->ConstI64(uint64(idx * 1024)), "")
            amx_fold_tile(te, preReg, iiF, ws, r, ts, ybase, d, xsbase, xbsbase, nb)
        }
    }
    LLVMBuildRetVoid(b)
    return true
}

// the tile emitter shared by the f32 and f16-scale entries: f16s swaps ONLY the scale-fold
// load (load_scale_vec) — same lattice, same fold order, same shared decline rails
def private tile_gen_impl(var gc : LlvmCodeCtx; f16s : bool) : bool {
    // decline on a foreign signature or any shared rail — the reference body compiles instead
    if (!is_tile_signature(gc.fn)) return false
    let p = parse_perm(gc)
    if (perm_declines(gc, p)) return false
    if (dot_kind(p.dotPrim) == DOT_AMX) {
        return p.pipe != 0 ? emit_amx_tile_pipelined(gc, p, f16s) : emit_amx_tile(gc, p, f16s)
    }

    var te = TileEmit(sgF16 = f16s)
    if (!setup_tile_emit(te, gc, p, false)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")

    // entry: per-token activation/scale bases; nb = n / 32
    LLVMPositionBuilderAtEnd(b, entry)
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = p.kstep)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    te.wg = LLVMGetParam(gc.impl, 1u)
    te.sg = LLVMGetParam(gc.impl, 2u)
    let xqp = LLVMGetParam(gc.impl, 3u)
    let xsp = LLVMGetParam(gc.impl, 4u)
    let xbsp = LLVMGetParam(gc.impl, 5u)
    let n = LLVMGetParam(gc.impl, 6u)
    sa.d = LLVMGetParam(gc.impl, 7u)
    sa.g = LLVMGetParam(gc.impl, 8u)
    sa.t0 = LLVMGetParam(gc.impl, 9u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(32ul), "nb")
    for (tk in range(4)) {
        var tRow = LLVMBuildAdd(b, sa.t0, te.types->ConstI64(uint64(tk)), "")
        te.x[tk] = LLVMBuildGEP2(b, te.types.t_int8, xqp, LLVMBuildMul(b, tRow, n, ""), "x{tk}")
        te.xs[tk] = LLVMBuildGEP2(b, te.types.t_float, xsp, LLVMBuildMul(b, tRow, sa.nb, ""), "xs{tk}")
        if (te.bias != 0) {
            // the bsums plane (−128·Σx i32 per token/block) — built by the wrapper for biased stamps only
            te.xbs[tk] = LLVMBuildGEP2(b, te.types.t_int32, xbsp, LLVMBuildMul(b, tRow, sa.nb, ""), "xbs{tk}")
        }
    }

    // one k-loop slice per nrsplit tokens (nrsplit=4: single slice, the M2 shape)
    var pred = entry
    var tokBase = 0
    while (tokBase < 4) {
        pred = emit_slice(te, sa, pred, tokBase, p.nrsplit, tokBase == 0 ? "" : "_{tokBase}")
        tokBase += p.nrsplit
    }
    LLVMPositionBuilderAtEnd(b, pred)
    LLVMBuildRetVoid(b)
    return true
}

def private tile_gen(var gc : LlvmCodeCtx) : bool => tile_gen_impl(gc, false)

// the wscale_f16 tile twin (stamped as a companion with the SAME perm): the binary16 group-
// scale plane, widened in the fold load — bit-identical over the plane the halfwords widen to
def private tile_s16_gen(var gc : LlvmCodeCtx) : bool => tile_gen_impl(gc, true)

def private is_layout_signature(fn : Function const?) : bool {
    return empty(fn.arguments) && fn.result.baseType == Type.tInt
}

// The layout companion: `() : int` returning the perm's repack interleave (mr) — the
// runtime repack asks it which layout the stamped tile reads. Declines via the SAME
// perm_declines as tile_gen, so both sides fall back together.
def private layout_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_layout_signature(gc.fn)) return false
    let p = parse_perm(gc)
    if (perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    LLVMPositionBuilderAtEnd(gc.jit.builder, entry)
    LLVMBuildRet(gc.jit.builder, gc.jit.types->ConstI32(uint64(q8q8_repack_type(p.mr).interleave)))
    return true
}

// The wbias companion: `() : int` returning the perm's weight-plane bias (128 for bias128
// vpdpbusd perms, 0 otherwise) — the runtime repack and mx4->Q8 expand ask it how the plane
// bytes are baked; declines via the SAME perm_declines.
def private wbias_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_layout_signature(gc.fn)) return false
    let p = parse_perm(gc)
    if (perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    LLVMPositionBuilderAtEnd(gc.jit.builder, entry)
    LLVMBuildRet(gc.jit.builder, gc.jit.types->ConstI32(uint64(p.bias)))
    return true
}

// The kgroup companion: `() : int` returning the perm's plane kgroup — 8 for smmla perms
// (row-pair × 8-k layout), 4 for every vector-dot perm. The runtime repack and mx4->Q8
// expand ask it how the plane groups its k-bytes; declines via the SAME perm_declines.
def private kgroup_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_layout_signature(gc.fn)) return false
    let p = parse_perm(gc)
    if (perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    LLVMPositionBuilderAtEnd(gc.jit.builder, entry)
    LLVMBuildRet(gc.jit.builder, gc.jit.types->ConstI32(uint64(q8q8_repack_type(p.mr, p.bias, kgroup_of(p)).kgroup)))
    return true
}

// The tokstep companion: `() : int` returning the tokens one tile call covers — 16·nrsplit
// for amx perms (macros are square in tile units), 4 for every vector perm. The batch
// wrapper walks its (token, group) space off it; declines via the SAME perm_declines.
def private tokstep_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_layout_signature(gc.fn)) return false
    let p = parse_perm(gc)
    if (perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    LLVMPositionBuilderAtEnd(gc.jit.builder, entry)
    let ts = dot_kind(p.dotPrim) == DOT_AMX ? 16 * p.nrsplit : 4
    LLVMBuildRet(gc.jit.builder, gc.jit.types->ConstI32(uint64(ts)))
    return true
}

def private is_witness_signature(fn : Function const?) : bool {
    return empty(fn.arguments) && fn.result.baseType == Type.tBool
}

// The witness: `() : bool` — did the stamped family actually EMIT on this target? Reference
// body returns false, generated body returns true, declining via the SAME perm_declines —
// backend selection reads it at SELECTION time (never [init]), never load-selecting a declined stamp.
def private witness_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_witness_signature(gc.fn)) return false
    let p = parse_perm(gc)
    if (perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    let b = gc.jit.builder
    LLVMPositionBuilderAtEnd(b, entry)
    if (dot_kind(p.dotPrim) == DOT_AMX) {
        // TMUL executes only after the per-process XTILEDATA grant, so the arch_prctl syscall IS the witness
        let mod = LLVMGetGlobalParent(gc.impl)
        var scTy : LLVMOpaqueType?
        var argTypes <- [gc.jit.types.t_int64, gc.jit.types.t_int64, gc.jit.types.t_int64]
        scTy = LLVMFunctionType(gc.jit.types.t_int64, argTypes)
        var sc = LLVMGetNamedFunction(mod, "syscall")
        if (sc == null) {
            sc = LLVMAddFunction(mod, "syscall", scTy)
        }
        var cargs <- [gc.jit.types->ConstI64(158ul), gc.jit.types->ConstI64(0x1023ul), gc.jit.types->ConstI64(18ul)]
        var rc = LLVMBuildCall2(b, scTy, sc, cargs, "xtile")
        var ok = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntEQ, rc, LLVMConstNull(gc.jit.types.t_int64), "ok")
        LLVMBuildRet(b, ok)
    } else {
        LLVMBuildRet(b, LLVMConstInt(gc.jit.types.t_int1, 1ul, 0))
    }
    return true
}

def private is_cfg_signature(fn : Function const?) : bool {
    return empty(fn.arguments) && fn.result.baseType == Type.tVoid
}

// The tile-config companion: `() : void`. For latch=1 it LDTILECFGs the static 8-tile
// palette (amx_tmm's plan) — those stamps carry no per-call config/TILERELEASE, so the
// batch wrapper calls this once per dispatch chunk. Every other perm is a bare `ret void`.
def private cfg_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_cfg_signature(gc.fn)) return false
    let p = parse_perm(gc)
    if (perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    let b = gc.jit.builder
    LLVMPositionBuilderAtEnd(b, entry)
    if (dot_kind(p.dotPrim) == DOT_AMX && p.latch != 0) {
        let mod = LLVMGetGlobalParent(gc.impl)
        var glob = LLVMGetNamedGlobal(mod, "dasllama.amx.tilecfg")
        if (glob == null) {
            // the 64-byte LDTILECFG image: palette 1, colsb u16 at [16+2i], rows u8 at [48+i]
            var cfgb : int[64]
            cfgb[0] = 1
            for (i in range(8)) {
                cfgb[16 + 2 * i] = i == 4 || i == 5 ? 32 : 64
                cfgb[48 + i] = i >= 6 ? 8 : 16
            }
            var elems : array<LLVMOpaqueValue?>
            elems |> reserve(64)
            for (i in range(64)) {
                elems |> push(LLVMConstInt(gc.jit.types.t_int8, uint64(cfgb[i]), 0))
            }
            glob = LLVMAddGlobal(mod, LLVMArrayType(gc.jit.types.t_int8, 64u), "dasllama.amx.tilecfg")
            LLVMSetInitializer(glob, LLVMConstArray(gc.jit.types.t_int8, array_data_ptr(elems), 64u))
            LLVMSetGlobalConstant(glob, 1)
            LLVMSetLinkage(glob, LLVMLinkage.LLVMPrivateLinkage)
            LLVMSetAlignment(glob, 64u)
        }
        var noTypes : array<LLVMOpaqueType?>
        var cfgDecl = LLVMGetIntrinsicDeclaration(mod, LLVMLookupIntrinsicID("llvm.x86.ldtilecfg"), noTypes)
        var cfgArgs <- [gc.jit.types->LLVMVoidPtrType()]
        var cfgTy = LLVMFunctionType(gc.jit.types.t_void, cfgArgs)
        var cargs <- [glob]
        LLVMBuildCall2(b, cfgTy, cfgDecl, cargs, "")
    }
    LLVMBuildRetVoid(b)
    return true
}

def private is_gemv_signature(fn : Function const?) : bool {
    if (length(fn.arguments) != 8 || fn.result.baseType != Type.tVoid) return false
    for (i in range(5)) {
        if (fn.arguments[i]._type.baseType != Type.tPointer) return false
    }
    for (i in range(5, 8)) {
        if (fn.arguments[i]._type.baseType != Type.tInt64) return false
    }
    return true
}

// The GEMV rows-range core: rows [rb, re) of one region, rb/re multiples of the perm's mr,
// group loop INSIDE the function. Per group it's emit_slice with a single token and gkstep
// blocks per K-iteration. Declines via the SAME perm_declines as the tile.
def private gemv_gen_impl(var gc : LlvmCodeCtx; f16s : bool) : bool {
    if (!is_gemv_signature(gc.fn)) return false
    let p0 = parse_perm(gc)
    if (perm_declines(gc, p0)) return false
    let p = companion_perm(p0)

    var te = TileEmit(sgF16 = f16s)
    if (!setup_tile_emit(te, gc, p, false)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    var ghead = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "ghead")
    var gexit = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "gexit")

    // entry: group range off the row range; d=0/t0=0 turn emit_slice's store offset into the absolute row g*mr
    LLVMPositionBuilderAtEnd(b, entry)
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = p.gkstep)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    let wp = LLVMGetParam(gc.impl, 1u)
    let sp = LLVMGetParam(gc.impl, 2u)
    te.x[0] = LLVMGetParam(gc.impl, 3u)
    te.xs[0] = LLVMGetParam(gc.impl, 4u)
    let n = LLVMGetParam(gc.impl, 5u)
    let rbv = LLVMGetParam(gc.impl, 6u)
    let rev = LLVMGetParam(gc.impl, 7u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(32ul), "nb")
    sa.d = te.types->ConstI64(0ul)
    sa.t0 = te.types->ConstI64(0ul)
    let mrC = te.types->ConstI64(uint64(te.interleave))
    var g0 = LLVMBuildSDiv(b, rbv, mrC, "g0")
    var g1 = LLVMBuildSDiv(b, rev, mrC, "g1")
    var nonempty = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, g0, g1, "any")
    LLVMBuildCondBr(b, nonempty, ghead, gexit)

    // ghead: group phi + weight/scale bases; emit_slice re-enters with zeroed accumulators each group
    LLVMPositionBuilderAtEnd(b, ghead)
    var gPhi = LLVMBuildPhi(b, te.types.t_int64, "g")
    var gEntryVals <- [g0]
    var gEntryBlocks <- [entry]
    LLVMAddIncoming(gPhi, gEntryVals, gEntryBlocks)
    sa.g = gPhi
    var gRow = LLVMBuildMul(b, gPhi, mrC, "grow")
    te.wg = LLVMBuildGEP2(b, te.types.t_int8, wp, LLVMBuildMul(b, gRow, n, ""), "wg")
    te.sg = LLVMBuildGEP2(b, f16s ? te.types.t_int16 : te.types.t_float, sp, LLVMBuildMul(b, gRow, sa.nb, ""), "sg")

    var after = emit_slice(te, sa, ghead, 0, 1, "")
    LLVMPositionBuilderAtEnd(b, after)
    var gNext = LLVMBuildAdd(b, gPhi, te.types->ConstI64(1ul), "gnext")
    var more = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, gNext, g1, "more")
    LLVMBuildCondBr(b, more, ghead, gexit)
    var gLoopVals <- [gNext]
    var gLoopBlocks <- [after]
    LLVMAddIncoming(gPhi, gLoopVals, gLoopBlocks)

    LLVMPositionBuilderAtEnd(b, gexit)
    LLVMBuildRetVoid(b)
    return true
}

def private gemv_gen(var gc : LlvmCodeCtx) : bool => gemv_gen_impl(gc, false)

// the wscale_f16 rows-core twin (stamped as a companion with the SAME perm) — see tile_s16_gen
def private gemv_s16_gen(var gc : LlvmCodeCtx) : bool => gemv_gen_impl(gc, true)

// The MXFP4 GEMV: same rows-range walk as gemv_gen over grp<mr>-interleaved nibble/exponent
// planes — emit_block_mx4 swaps the weight load for LUT dequant (tbl1/pshufb) and the scale
// load for the in-register e8m0 fold; loop machinery, gkstep, and decline rails are shared.
def private mx4_gemv_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_gemv_signature(gc.fn)) return false
    let p0 = parse_perm(gc)
    if (perm_declines(gc, p0)) return false
    let p = companion_perm(p0)

    var te = TileEmit(mx4 = true)
    if (!setup_tile_emit(te, gc, p, true)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    var ghead = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "ghead")
    var gexit = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "gexit")

    // entry: group range off the row range; plane strides are nibble n/2 and exponent n/32
    LLVMPositionBuilderAtEnd(b, entry)
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = p.gkstep)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    let wn = LLVMGetParam(gc.impl, 1u)
    let we = LLVMGetParam(gc.impl, 2u)
    te.x[0] = LLVMGetParam(gc.impl, 3u)
    te.xs[0] = LLVMGetParam(gc.impl, 4u)
    let n = LLVMGetParam(gc.impl, 5u)
    let rbv = LLVMGetParam(gc.impl, 6u)
    let rev = LLVMGetParam(gc.impl, 7u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(32ul), "nb")
    var nrow = LLVMBuildSDiv(b, n, te.types->ConstI64(2ul), "nrow")
    sa.d = te.types->ConstI64(0ul)
    sa.t0 = te.types->ConstI64(0ul)
    let mrC = te.types->ConstI64(uint64(te.interleave))
    var g0 = LLVMBuildSDiv(b, rbv, mrC, "g0")
    var g1 = LLVMBuildSDiv(b, rev, mrC, "g1")
    var nonempty = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, g0, g1, "any")
    LLVMBuildCondBr(b, nonempty, ghead, gexit)

    LLVMPositionBuilderAtEnd(b, ghead)
    var gPhi = LLVMBuildPhi(b, te.types.t_int64, "g")
    var gEntryVals <- [g0]
    var gEntryBlocks <- [entry]
    LLVMAddIncoming(gPhi, gEntryVals, gEntryBlocks)
    sa.g = gPhi
    var gRow = LLVMBuildMul(b, gPhi, mrC, "grow")
    te.wg = LLVMBuildGEP2(b, te.types.t_int8, wn, LLVMBuildMul(b, gRow, nrow, ""), "wg")
    te.sg = LLVMBuildGEP2(b, te.types.t_int8, we, LLVMBuildMul(b, gRow, sa.nb, ""), "eg")

    var after = emit_slice(te, sa, ghead, 0, 1, "")
    LLVMPositionBuilderAtEnd(b, after)
    var gNext = LLVMBuildAdd(b, gPhi, te.types->ConstI64(1ul), "gnext")
    var more = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, gNext, g1, "more")
    LLVMBuildCondBr(b, more, ghead, gexit)
    var gLoopVals <- [gNext]
    var gLoopBlocks <- [after]
    LLVMAddIncoming(gPhi, gLoopVals, gLoopBlocks)

    LLVMPositionBuilderAtEnd(b, gexit)
    LLVMBuildRetVoid(b)
    return true
}

// The mx4 TILE: tile_gen's 4-token walk over the group-base nibble/exponent planes —
// emit_block_mx4 dequants each weight block-group ONCE (LUT + e8m0 fold) and reuses it
// across the 4 activation rows. amx winner perms ride the busd lattice via companion_perm.
def private mx4_tile_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_tile_signature(gc.fn)) return false
    let p0 = parse_perm(gc)
    if (perm_declines(gc, p0)) return false
    let p = companion_perm(p0)

    var te = TileEmit(mx4 = true)
    if (!setup_tile_emit(te, gc, p, true)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")

    // entry: per-token activation/scale bases; wn/we arrive pre-offset to the group
    LLVMPositionBuilderAtEnd(b, entry)
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = p.kstep)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    te.wg = LLVMGetParam(gc.impl, 1u)
    te.sg = LLVMGetParam(gc.impl, 2u)
    let xqp = LLVMGetParam(gc.impl, 3u)
    let xsp = LLVMGetParam(gc.impl, 4u)
    let n = LLVMGetParam(gc.impl, 6u)
    sa.d = LLVMGetParam(gc.impl, 7u)
    sa.g = LLVMGetParam(gc.impl, 8u)
    sa.t0 = LLVMGetParam(gc.impl, 9u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(32ul), "nb")
    for (tk in range(4)) {
        var tRow = LLVMBuildAdd(b, sa.t0, te.types->ConstI64(uint64(tk)), "")
        te.x[tk] = LLVMBuildGEP2(b, te.types.t_int8, xqp, LLVMBuildMul(b, tRow, n, ""), "x{tk}")
        te.xs[tk] = LLVMBuildGEP2(b, te.types.t_float, xsp, LLVMBuildMul(b, tRow, sa.nb, ""), "xs{tk}")
    }

    // one k-loop slice per nrsplit tokens — the Q8 tile's walk over the mx4 block form
    var pred = entry
    var tokBase = 0
    while (tokBase < 4) {
        pred = emit_slice(te, sa, pred, tokBase, p.nrsplit, tokBase == 0 ? "" : "_{tokBase}")
        tokBase += p.nrsplit
    }
    LLVMPositionBuilderAtEnd(b, pred)
    LLVMBuildRetVoid(b)
    return true
}

def private is_kq_gemv_signature(fn : Function const?) : bool {
    if (length(fn.arguments) != 9 || fn.result.baseType != Type.tVoid) return false
    for (i in range(6)) {
        if (fn.arguments[i]._type.baseType != Type.tPointer) return false
    }
    for (i in range(6, 9)) {
        if (fn.arguments[i]._type.baseType != Type.tInt64) return false
    }
    return true
}

// The K-quant rows-range GEMV over the grp<mr> kq planes — same group walk as gemv_gen, but
// the block unit is a 256-weight SUPERBLOCK (nb = n/256, kstep pinned 1). Unpacks are generic
// IR riding kq_dot_lane, so it declines via the SAME perm_declines — no new rails.
def private kq_gemv_gen_impl(var gc : LlvmCodeCtx; fmt : int) : bool {
    if (!is_kq_gemv_signature(gc.fn)) return false
    let p0 = parse_perm(gc)
    if (perm_declines(gc, p0)) return false
    let p = companion_perm(p0)

    var te = TileEmit(kq = fmt)
    if (!setup_tile_emit(te, gc, p, fmt == 44 || fmt == 45)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    var ghead = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "ghead")
    var gexit = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "gexit")

    // entry: group range off the row range; strides are disk footprints (quants 128/160/192B, scales 20/20/18B)
    LLVMPositionBuilderAtEnd(b, entry)
    if (fmt == 33 || fmt == 34 || fmt == 23 || fmt == 24 || fmt == 25) {
        // iq3s/iq3xxs/iq2s/iq2xs: the gemv gathers each superblock into an alloca panel (the tile gets the runtime unpack)
        te.gfn = gc.impl
        te.gctx = gc.jit.ctx
        grid_emit_globals(te, fmt, LLVMGetGlobalParent(gc.impl))
        te.gridPanel = LLVMBuildAlloca(b, LLVMArrayType(gc.jit.types.t_int8, uint(256 * te.interleave)), "grid.panel")
        LLVMSetAlignment(te.gridPanel, 64u)   // a cache line: at 32, half the width-512 panel loads straddle one
    }
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = 1)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    let kqp = LLVMGetParam(gc.impl, 1u)
    let ksp = LLVMGetParam(gc.impl, 2u)
    te.x[0] = LLVMGetParam(gc.impl, 3u)
    te.xs[0] = LLVMGetParam(gc.impl, 4u)
    te.xbs[0] = LLVMGetParam(gc.impl, 5u)
    let n = LLVMGetParam(gc.impl, 6u)
    let rbv = LLVMGetParam(gc.impl, 7u)
    let rev = LLVMGetParam(gc.impl, 8u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(0x100ul), "nsb")
    var qrow = LLVMBuildMul(b, sa.nb, te.types->ConstI64(uint64(kq_qsb(fmt))), "qrow")
    var srow = LLVMBuildMul(b, sa.nb, te.types->ConstI64(uint64(kq_ssb(fmt))), "srow")
    sa.d = te.types->ConstI64(0ul)
    sa.t0 = te.types->ConstI64(0ul)
    let mrC = te.types->ConstI64(uint64(te.interleave))
    var g0 = LLVMBuildSDiv(b, rbv, mrC, "g0")
    var g1 = LLVMBuildSDiv(b, rev, mrC, "g1")
    var nonempty = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, g0, g1, "any")
    LLVMBuildCondBr(b, nonempty, ghead, gexit)

    LLVMPositionBuilderAtEnd(b, ghead)
    var gPhi = LLVMBuildPhi(b, te.types.t_int64, "g")
    var gEntryVals <- [g0]
    var gEntryBlocks <- [entry]
    LLVMAddIncoming(gPhi, gEntryVals, gEntryBlocks)
    sa.g = gPhi
    var gRow = LLVMBuildMul(b, gPhi, mrC, "grow")
    te.wg = LLVMBuildGEP2(b, te.types.t_int8, kqp, LLVMBuildMul(b, gRow, qrow, ""), "wg")
    te.sg = LLVMBuildGEP2(b, te.types.t_int8, ksp, LLVMBuildMul(b, gRow, srow, ""), "sg")

    var after = emit_slice(te, sa, ghead, 0, 1, "")
    LLVMPositionBuilderAtEnd(b, after)
    var gNext = LLVMBuildAdd(b, gPhi, te.types->ConstI64(1ul), "gnext")
    var more = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, gNext, g1, "more")
    LLVMBuildCondBr(b, more, ghead, gexit)
    var gLoopVals <- [gNext]
    var gLoopBlocks <- [after]
    LLVMAddIncoming(gPhi, gLoopVals, gLoopBlocks)

    LLVMPositionBuilderAtEnd(b, gexit)
    LLVMBuildRetVoid(b)
    return true
}

def private k4_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 4)
def private k5_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 5)
def private k6_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 6)
def private q40_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 40)

// The q51 rows-range GEMV over the grp<mr> q51 planes — kq_gemv_gen_impl's walk with the
// 32-weight block unit (nb = n/32, row strides 20B quants / 4B scales per block). The block
// dots ride kq_dot_lane, so every lattice leg (sdot/maddubs/vpdpbusd) emits.
def private q51_gemv_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_kq_gemv_signature(gc.fn)) return false
    let p0 = parse_perm(gc)
    if (perm_declines(gc, p0)) return false
    let p = companion_perm(p0)

    var te = TileEmit(q51 = true)
    if (!setup_tile_emit(te, gc, p, false)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    var ghead = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "ghead")
    var gexit = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "gexit")

    LLVMPositionBuilderAtEnd(b, entry)
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = 1)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    let qgp = LLVMGetParam(gc.impl, 1u)
    let sgp = LLVMGetParam(gc.impl, 2u)
    te.x[0] = LLVMGetParam(gc.impl, 3u)
    te.xs[0] = LLVMGetParam(gc.impl, 4u)
    te.xbs[0] = LLVMGetParam(gc.impl, 5u)
    let n = LLVMGetParam(gc.impl, 6u)
    let rbv = LLVMGetParam(gc.impl, 7u)
    let rev = LLVMGetParam(gc.impl, 8u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(0x20ul), "nb")
    var qrow = LLVMBuildMul(b, sa.nb, te.types->ConstI64(20ul), "qrow")
    var srow = LLVMBuildMul(b, sa.nb, te.types->ConstI64(4ul), "srow")
    sa.d = te.types->ConstI64(0ul)
    sa.t0 = te.types->ConstI64(0ul)
    let mrC = te.types->ConstI64(uint64(te.interleave))
    var g0 = LLVMBuildSDiv(b, rbv, mrC, "g0")
    var g1 = LLVMBuildSDiv(b, rev, mrC, "g1")
    var nonempty = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, g0, g1, "any")
    LLVMBuildCondBr(b, nonempty, ghead, gexit)

    LLVMPositionBuilderAtEnd(b, ghead)
    var gPhi = LLVMBuildPhi(b, te.types.t_int64, "g")
    var gEntryVals <- [g0]
    var gEntryBlocks <- [entry]
    LLVMAddIncoming(gPhi, gEntryVals, gEntryBlocks)
    sa.g = gPhi
    var gRow = LLVMBuildMul(b, gPhi, mrC, "grow")
    te.wg = LLVMBuildGEP2(b, te.types.t_int8, qgp, LLVMBuildMul(b, gRow, qrow, ""), "wg")
    te.sg = LLVMBuildGEP2(b, te.types.t_int8, sgp, LLVMBuildMul(b, gRow, srow, ""), "sg")

    var after = emit_slice(te, sa, ghead, 0, 1, "")
    LLVMPositionBuilderAtEnd(b, after)
    var gNext = LLVMBuildAdd(b, gPhi, te.types->ConstI64(1ul), "gnext")
    var more = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLT, gNext, g1, "more")
    LLVMBuildCondBr(b, more, ghead, gexit)
    var gLoopVals <- [gNext]
    var gLoopBlocks <- [after]
    LLVMAddIncoming(gPhi, gLoopVals, gLoopBlocks)

    LLVMPositionBuilderAtEnd(b, gexit)
    LLVMBuildRetVoid(b)
    return true
}

// The K-quant 4-token TILE (weight-stationary prefill): same 10-arg contract as the q8 tile;
// emit_block_kqv2 unpacks each weight vector ONCE and dots it against every token, bit-exact
// vs 4 per-token GEMVs. Declines via the SAME perm_declines — no new rails.
def private kq_tile_gen_impl(var gc : LlvmCodeCtx; fmt : int) : bool {
    if (!is_tile_signature(gc.fn)) return false
    let p0 = parse_perm(gc)
    if (perm_declines(gc, p0)) return false
    let p = companion_perm(p0)

    var te = TileEmit(kq = fmt, kqBytes = fmt == 5 || fmt == 6)   // only the k5/k6 tiles get the byte-expanded panel
    if (!setup_tile_emit(te, gc, p, fmt == 44 || fmt == 45)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")

    // entry: per-token activation/scale/bsum bases; the block unit is a superblock (nb = n/256)
    LLVMPositionBuilderAtEnd(b, entry)
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = 1)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    te.wg = LLVMGetParam(gc.impl, 1u)
    te.sg = LLVMGetParam(gc.impl, 2u)
    let xqp = LLVMGetParam(gc.impl, 3u)
    let xsp = LLVMGetParam(gc.impl, 4u)
    let xbsp = LLVMGetParam(gc.impl, 5u)
    let n = LLVMGetParam(gc.impl, 6u)
    sa.d = LLVMGetParam(gc.impl, 7u)
    sa.g = LLVMGetParam(gc.impl, 8u)
    sa.t0 = LLVMGetParam(gc.impl, 9u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(0x100ul), "nsb")
    // kq v2 activations are Q8_K-form: scale plane per-superblock (n/256/token), bsums per-16
    var nb16 = LLVMBuildSDiv(b, n, te.types->ConstI64(16ul), "nb16")
    for (tk in range(4)) {
        var tRow = LLVMBuildAdd(b, sa.t0, te.types->ConstI64(uint64(tk)), "")
        te.x[tk] = LLVMBuildGEP2(b, te.types.t_int8, xqp, LLVMBuildMul(b, tRow, n, ""), "x{tk}")
        te.xs[tk] = LLVMBuildGEP2(b, te.types.t_float, xsp, LLVMBuildMul(b, tRow, sa.nb, ""), "xs{tk}")
        te.xbs[tk] = LLVMBuildGEP2(b, te.types.t_int32, xbsp, LLVMBuildMul(b, tRow, nb16, ""), "xbs{tk}")
    }

    // one k-loop slice per nrsplit tokens (the q8 tile's slice shape)
    var pred = entry
    var tokBase = 0
    while (tokBase < 4) {
        pred = emit_slice(te, sa, pred, tokBase, p.nrsplit, tokBase == 0 ? "" : "_{tokBase}")
        tokBase += p.nrsplit
    }
    LLVMPositionBuilderAtEnd(b, pred)
    LLVMBuildRetVoid(b)
    return true
}

// iq4xs rides the kq gemv/tile walks with emit_block_iq4xs as the block body (LUT decode +
// sign-trick dots); the tbl1/pshufb rail is the mx4 companion's, already in perm_declines.
def private iq4xs_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 44)
def private iq4xs_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 44)

// k3 rides the kq walks through emit_block_kqv2's k6 arms (the k3 flag swaps the quant compose and the offset)
def private k3_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 3)
def private k3_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 3)

// iq3s: the gemv gathers per superblock into an alloca panel (emit_iq3s_gather); the tile
// reads the runtime-unpacked panel — both then ride the iq4xs block lattice.
def private iq3s_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 33)
def private iq3s_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 33)

// iq3xxs: the gemv gathers per superblock into the alloca panel (emit_iq3xxs_gather - the
// halved grid + ksigns); the tile reads the runtime-unpacked panel - the iq3s route exactly.
def private iq3xxs_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 34)
def private iq3xxs_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 34)

// iq4nl rides the iq4xs LUT block body with q40's per-block f16 d fold (the `nl` mode) over
// the packed grp planes - no panel, no gather, no sub-scales.
def private iq4nl_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 45)
def private iq4nl_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 45)

// k2 rides the kq walks through emit_block_kqv2's per-16 arms (the k2 flag swaps the 2-bit
// compose, the nibble scale extracts, and the epilogue to k4/k5's dmin fold)
def private k2_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 2)
def private k2_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 2)

// iq2s rides the iq4xs walk through the panel route: emit_iq2s_gather feeds the shared
// byte-expanded panel; per-16 UNSIGNED strips keep split lo/hi accumulators (the iq2 flag)
def private iq2s_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 23)
def private iq2s_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 23)

// iq2xs rides the iq4xs walk through the panel route: emit_iq2xs_gather feeds the shared
// byte-expanded panel; the iq2 flag's split accumulators and per-16 strips are iq2s's.
def private iq2xs_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 24)
def private iq2xs_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 24)

def private k4_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 4)
def private k5_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 5)
def private k6_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 6)
def private q40_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 40)

// The q51 4-token TILE: the kq tile walk with the 32-weight block unit and Q8_0-form
// activations (per-32 xs floats + per-32 asum ints — emit_block_q51's contract).
def private q51_tile_gen(var gc : LlvmCodeCtx) : bool {
    if (!is_tile_signature(gc.fn)) return false
    let p0 = parse_perm(gc)
    if (perm_declines(gc, p0)) return false
    let p = companion_perm(p0)

    var te = TileEmit(q51 = true)
    if (!setup_tile_emit(te, gc, p, false)) return false

    let b = gc.jit.builder
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")

    // entry: per-token activation/scale/asum bases; the block unit is 32 weights (nb = n/32)
    LLVMPositionBuilderAtEnd(b, entry)
    var sa = SliceArgs(gc_impl = gc.impl, ctx = gc.jit.ctx, kstep = 1)
    sa.yp = LLVMGetParam(gc.impl, 0u)
    te.wg = LLVMGetParam(gc.impl, 1u)
    te.sg = LLVMGetParam(gc.impl, 2u)
    let xqp = LLVMGetParam(gc.impl, 3u)
    let xsp = LLVMGetParam(gc.impl, 4u)
    let xbsp = LLVMGetParam(gc.impl, 5u)
    let n = LLVMGetParam(gc.impl, 6u)
    sa.d = LLVMGetParam(gc.impl, 7u)
    sa.g = LLVMGetParam(gc.impl, 8u)
    sa.t0 = LLVMGetParam(gc.impl, 9u)
    sa.nb = LLVMBuildSDiv(b, n, te.types->ConstI64(0x20ul), "nb")
    for (tk in range(4)) {
        var tRow = LLVMBuildAdd(b, sa.t0, te.types->ConstI64(uint64(tk)), "")
        te.x[tk] = LLVMBuildGEP2(b, te.types.t_int8, xqp, LLVMBuildMul(b, tRow, n, ""), "x{tk}")
        te.xs[tk] = LLVMBuildGEP2(b, te.types.t_float, xsp, LLVMBuildMul(b, tRow, sa.nb, ""), "xs{tk}")
        te.xbs[tk] = LLVMBuildGEP2(b, te.types.t_int32, xbsp, LLVMBuildMul(b, tRow, sa.nb, ""), "xbs{tk}")
    }

    // one k-loop slice per nrsplit tokens (the q8 tile's slice shape)
    var pred = entry
    var tokBase = 0
    while (tokBase < 4) {
        pred = emit_slice(te, sa, pred, tokBase, p.nrsplit, tokBase == 0 ? "" : "_{tokBase}")
        tokBase += p.nrsplit
    }
    LLVMPositionBuilderAtEnd(b, pred)
    LLVMBuildRetVoid(b)
    return true
}

// Called from llvm_user_modules::register_user_llvm_code_generators — the JIT invokes that in the
// context that reads the registry, so the direct call lands in the right table copy.

// iq2xxs rides the iq4xs walk through the panel route: emit_iq2xxs_gather feeds the shared
// byte-expanded panel; per-32 UNSIGNED strips ride the iq3s/iq3xxs scale fold (the strip
// bytes stay under 128, so the shared SExt load reads them exactly).
def private iq2xxs_gemv_gen(var gc : LlvmCodeCtx) : bool => kq_gemv_gen_impl(gc, 25)
def private iq2xxs_tile_gen(var gc : LlvmCodeCtx) : bool => kq_tile_gen_impl(gc, 25)

[macro_function]
def public register_dasllama_gemm_generators {
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_tile", @@tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_layout", @@layout_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_wbias", @@wbias_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_kgroup", @@kgroup_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_tokstep", @@tokstep_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_amx_cfg", @@cfg_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_gemv", @@gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_tile_s16", @@tile_s16_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_gemv_s16", @@gemv_s16_gen)
    register_llvm_code_generator("dasllama_gemm_gen::mx4_gemv", @@mx4_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::mx4_tile", @@mx4_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k4_gemv", @@k4_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k5_gemv", @@k5_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k6_gemv", @@k6_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q51_gemv", @@q51_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q51_tile", @@q51_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k4_tile", @@k4_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k5_tile", @@k5_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k6_tile", @@k6_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q40_gemv", @@q40_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q40_tile", @@q40_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq4xs_gemv", @@iq4xs_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq4xs_tile", @@iq4xs_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k3_gemv", @@k3_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k3_tile", @@k3_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq3s_gemv", @@iq3s_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq3s_tile", @@iq3s_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq3xxs_gemv", @@iq3xxs_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq3xxs_tile", @@iq3xxs_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq4nl_gemv", @@iq4nl_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq4nl_tile", @@iq4nl_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k2_gemv", @@k2_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::k2_tile", @@k2_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq2s_gemv", @@iq2s_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq2s_tile", @@iq2s_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq2xs_gemv", @@iq2xs_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq2xs_tile", @@iq2xs_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq2xxs_gemv", @@iq2xxs_gemv_gen)
    register_llvm_code_generator("dasllama_gemm_gen::iq2xxs_tile", @@iq2xxs_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::q8q8_witness", @@witness_gen)
}
