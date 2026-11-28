options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kqformat shared public

// The K-quant format taxonomy — ONE enum, ONE stride truth, ONE id space. This module requires
// nothing dasllama: everything format-shaped keys off it (convert reads codec strides, repack the
// layouts, layout/common the loader walks, gemm_schema the kernel side). Int format ids exist
// only at the IR/kernel-param boundary — cast there via kq_schema_id, never carry an int format
// across a module seam.

require dasllama/dasllama_lint public
require daslib/enum_trait   // kq_fmt_of_name walks the enum
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require strings
require daslib/strings_boost

//! Per-weight storage format under a q8-mode load with native planes: q8 = qblob/qscales;
//! k4/k5/k6/q40/iq4xs/k3/iq3s/iq3xxs/iq4nl/k2/iq2s/iq2xs/iq2xxs = superblock plane pairs (mixed PER TENSOR); q51 = native Q5_1 per-32
//! planes; mx4 = the native MXFP4 expert planes (`Model.mxq` / `mxs` under `experts_mx4` - the GPU tiers' stack tag, never a
//! per-tensor tag the loader writes); iq4nl32 = native IQ4_NL per-32
//! planes (any dense, expert or table plane whose rows are off the
//! 256 lattice and on the 32 one - iq4nl's disk blocks with q51's
//! plane form); q4032 = native Q4_0 per-32 planes (a Q4_0 plane off
//! the 256 lattice - iq4nl32's plane form under the linear codebook);
//! q5032 = native Q5_0 per-32 planes on any 32-multiple row
//! (q51's quant plane - 16 nibble bytes + the qh word - under one f16 d, weight d x (q - 16)); nv4 = native NVFP4 superblock
//! planes (four 64-weight disk blocks a superblock: e2m1 nibbles re-paired into the k4 tiling, sixteen UE4M3 scales - one a
//! 16-weight sub-block - verbatim); mlx4 / mlx8 / mlx4b / mlx8b / mlx6b = a safetensors affine triplet's per-32 planes (4-,
//! 8- or 6-bit unsigned codes verbatim, the 64-group's F16 or BF16 (scale, bias) pair shared by its two blocks, w = scale x q +
//! bias; mlx6b is the 6-bit codes under BF16); q20 = native Q2_0 per-32 planes (2-bit codes in the disk order, eight bytes a
//! block, one f16 d a 64-weight group - a scale unit serving two blocks - weight d x (q - 1)); q10 = native Q1_0 per-32 planes
//! (sign bits in the disk order, four bytes a block, one f16 d a 128-weight group - a scale unit serving four blocks - weight
//! +d where the bit is set, -d where it is clear); iq1s / iq1m = native IQ1_S / IQ1_M superblock planes (the disk qs and qh
//! bytes verbatim as the quant plane, IQ1_S's f16 d or IQ1_M's four scale words verbatim as the scale plane; weight
//! scale x (grid value + delta) off the 2048-entry ternary grid `iq1s_grid`); tq2 = native TQ2_0 superblock planes (Q2_K's
//! 2-bit lanes verbatim under one f16 d a superblock, weight d x (q - 1)); tq1 = native TQ1_0 superblock planes (the 48
//! five-trit qs bytes and the 4 four-trit qh bytes verbatim under one f16 d a superblock, weight d x (q - 1)).
//! New members append — the int value is the device stack tag and the image plane id.
enum KqFmt : uint8 {
    q8
    k4
    k5
    k6
    q40
    q51
    iq4xs
    k3
    iq3s
    iq3xxs
    iq4nl
    k2
    iq2s
    iq2xs
    iq2xxs
    mx4
    iq4nl32
    iq3s4
    q4032
    q5032
    nv4
    mlx4
    mlx8
    mlx4b
    mlx8b
    mlx6b
    q20
    q10
    iq1s
    iq1m
    tq2
    tq1
}

//! One empty tag type per KqFmt member - the overload key of every per-format function family
//! (the plane codec, the grp repack); `kq_fmt_stamp` binds the member's tag, so a family is a
//! set of overloads and a format without one fails to compile at the stamp, never at runtime.
struct KqTag_q8 {}
struct KqTag_k4 {}
struct KqTag_k5 {}
struct KqTag_k6 {}
struct KqTag_q40 {}
struct KqTag_q51 {}
struct KqTag_iq4xs {}
struct KqTag_k3 {}
struct KqTag_iq3s {}
struct KqTag_iq3xxs {}
struct KqTag_iq4nl {}
struct KqTag_k2 {}
struct KqTag_iq2s {}
struct KqTag_iq2xs {}
struct KqTag_iq2xxs {}
struct KqTag_mx4 {}
struct KqTag_iq4nl32 {}
struct KqTag_iq3s4 {}
struct KqTag_q4032 {}
struct KqTag_q5032 {}
struct KqTag_nv4 {}
struct KqTag_mlx4 {}
struct KqTag_mlx8 {}
struct KqTag_mlx4b {}
struct KqTag_mlx8b {}
struct KqTag_mlx6b {}
struct KqTag_q20 {}
struct KqTag_q10 {}
struct KqTag_iq1s {}
struct KqTag_iq1m {}
struct KqTag_tq2 {}
struct KqTag_tq1 {}

[call_macro(name = "kq_fmt_stamp")]
class KqFmtStamp : AstCallMacro {
    //! ``kq_fmt_stamp(fmt) <| $(F) { ... }`` stamps the block once per ``KqFmt`` member behind a ladder on
    //! ``fmt``, ``F`` bound to the member's tag value (``KqTag_k4()`` ...), so a per-format overload family
    //! called on ``F`` resolves at compile time and a member without an overload fails the compile.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(length(expr.arguments) == 2 && expr.arguments[1] is ExprMakeBlock, prog, expr.at,
            "expecting kq_fmt_stamp(fmt) followed by a block")
        macro_verify(expr.arguments[0] is ExprVar, prog, expr.at,
            "kq_fmt_stamp takes a plain variable for the format, got {describe(expr.arguments[0])}")
        let emb = expr.arguments[1] as ExprMakeBlock
        macro_verify(emb._block is ExprBlock, prog, expr.at, "expecting a block body, got {describe(emb._block)}")
        var blk = emb._block as ExprBlock
        macro_verify(length(blk.arguments) == 1 && empty(blk.finalList), prog, expr.at,
            "expecting one block argument, the format tag, and no finally section")
        let tp = "{blk.arguments[0].name}"
        var fmt = expr.arguments[0]
        var ladder : ExpressionPtr
        for (f in type<KqFmt>) {
            let tagName = "KqTag_{f}"
            var arm = bind_block_argument(blk, tp, qmacro($c(tagName)()), expr.at)
            if (ladder == null) {
                ladder = arm
            } else {
                ladder = qmacro_expr() {
                    if ($e(fmt) == $v(f)) {
                        $e(arm)
                    } else {
                        $e(ladder)
                    }
                }
            }
        }
        return ladder
    }
}

[function_macro(name = "b32_portable_slots")]
class B32PortableSlots : AstFunctionAnnotation {
    //! ``[b32_portable_slots] def f {}`` gives each per-32 member (``kq_dense_b32``) its three portable kernels -
    //! ``<fmt>q8_batch_kernel``, ``<fmt>q8_groupn_kernel``, ``<fmt>q8_batch_groupn_kernel``, the ``b32_*_kernel`` shapes on the
    //! member's tag - and makes ``f``'s body their ``register_b32_portable`` calls, in ``KqFmt`` order
    def override apply(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        var body : array<ExpressionPtr>
        for (f in type<KqFmt>) {
            continue if (!kq_dense_b32(f))
            let tag = "KqTag_{f}"
            let stem = "{f}q8"
            var fns <- [
                qmacro_function("{stem}_batch_kernel") $(var yp : float?; wq : uint8 const?; ws : uint8 const?; xqp : int8 const?; xsp : float const?;
                                                         n : int64; d : int64; ntok : int64) {
                    b32_batch_kernel(yp, wq, ws, xqp, xsp, n, d, ntok, $c(tag)())
                },
                qmacro_function("{stem}_groupn_kernel") $(var yp : float?; wq : uint8 const?; ws : uint8 const?; offs : int64 const?; nregions : int64;
                                                          xqp : int8 const?; xsp : float const?; n : int64; d : int64) {
                    b32_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, $c(tag)())
                },
                qmacro_function("{stem}_batch_groupn_kernel") $(var yp : float?; wq : uint8 const?; ws : uint8 const?; offs : int64 const?;
                                                                nregions : int64; xqp : int8 const?; xsp : float const?; n : int64; d : int64) {
                    b32_batch_groupn_kernel(yp, wq, ws, offs, nregions, xqp, xsp, n, d, $c(tag)())
                }
            ]
            for (fn in fns) {
                fn.flags |= FunctionFlags.generated | FunctionFlags.privateFunction
                if (!(compiling_module() |> add_function(fn))) {
                    errors := "[b32_portable_slots] failed to add {fn.name}"
                    return false
                }
            }
            var batch = new ExprAddr(at = func.at, target := "{stem}_batch_kernel")
            var groupn = new ExprAddr(at = func.at, target := "{stem}_groupn_kernel")
            var batch_groupn = new ExprAddr(at = func.at, target := "{stem}_batch_groupn_kernel")
            body |> push <| qmacro_expr() {
                register_b32_portable($v(f), B32Slots(batch = $e(batch), groupn = $e(groupn), batch_groupn = $e(batch_groupn)))
            }
        }
        func.body = make_expression_block(body)
        func.body |> force_at(func.at)
        return true
    }
}

[function_macro(name = "grid_words")]
class GridWordsMacro : AstFunctionAnnotation {
    //! ``[grid_words(of = "iq3s_grid")] def f(i : int) : uint {}`` gives ``f`` the body ``let tbl = <the table's literal>; return tbl[i]``:
    //! the shader-side per-word form of a kqformat grid table, baked from this module's one literal when the kernel home
    //! compiles (a shader function cannot return a fixed array, so the per-word form carries the literal inline)
    def override apply(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        let of = find_arg(args, "of")
        if (!(of is tString)) {
            errors := "[grid_words] takes of = \"<table function>\""
            return false
        }
        if (length(func.arguments) != 1) {
            errors := "[grid_words] {func.name}: one index argument expected"
            return false
        }
        let tname = of as tString
        var lit : ExpressionPtr
        find_compiling_module("dasllama_kqformat") |> for_each_function(tname) $(var fn) {
            if (fn.body is ExprBlock) {
                var blk = fn.body as ExprBlock
                if (length(blk.list) == 1 && blk.list[0] is ExprReturn) {
                    var ret = blk.list[0] as ExprReturn
                    lit := ret.subexpr
                }
            }
        }
        if (lit == null) {
            errors := "[grid_words] {func.name}: no `def {tname}` in dasllama_kqformat whose body is one `return fixed_array<uint>(...)`"
            return false
        }
        let argn = "{func.arguments[0].name}"
        var qb <- qmacro_block() {
            let tbl = $e(lit)
            return tbl[$i(argn)]
        }
        qb |> force_at(func.at)
        func.body <- qb
        return true
    }
}

//! the formats whose cm2 e stamp is the m stamp byte for byte (their k step is 64, the m tile's), so the e ladder takes the m class; the seven grid-codebook formats and mx4 step 32 on the e column and keep a stamp of their own
let KQ_CM2E_ALIASES_M = "q8 k4 k5 k6 q40 iq4xs iq3s4 iq4nl k3 k2 tq2 tq1 q51 q4032 q5032 iq4nl32 nv4 q20 q10"

def kq_cm2e_aliases_m(fmt_name : string) : bool => word_in(KQ_CM2E_ALIASES_M, fmt_name)

//! the stem of a format's stamp functions in a stamp family - `batch` (the batch tiles) or `gemv` (the decode GEMVs):
//! `<verb>_<stem>_<tail>`, the stem `<fmt>_<family>` on q8 and the per-32 formats, `kq_<family>_<fmt>` on a kq format
def kq_tile_stem(fmt_name : string; family : string = "batch") : string =>
    (fmt_name == "q8" || fmt_name == "q51" || fmt_name == "mx4" || fmt_name == "iq4nl32" || fmt_name == "q4032" || fmt_name == "q5032"
        || fmt_name == "q20" || fmt_name == "q10" || word_in(KQ_DISCRETE_UNSTAMPED, fmt_name)) ? "{fmt_name}_{family}" : "kq_{family}_{fmt_name}"

let private STAMP_INST = "_::"   //! a placeholder written `_::<prefix>...` names a target resolved where its generic body is instantiated

//! a `<prefix><verb>[_<tail>]` placeholder -> `<verb>_<stem>_<tail>`, the tail the placeholder's or, where it names none, `arm_tail`;
//! a `_::` ahead of the placeholder stays ahead of the target
def private stamp_target(placeholder, prefix, stem, arm_tail : string) : string {
    let inst = placeholder |> starts_with(STAMP_INST)
    let rest = slice(placeholder, length(prefix) + (inst ? length(STAMP_INST) : 0))
    let cut = find(rest, "_")
    let verb = cut < 0 ? rest : slice(rest, 0, cut)
    let tail = cut < 0 ? arm_tail : slice(rest, cut + 1)
    return "{inst ? STAMP_INST : ""}{verb}_{stem}_{tail}"
}

//! `tile_<verb>[_<tail>]` -> the format's `<verb>_<stem>_<tail>` in `family`; a `cm2e_cls` tail on an aliasing format takes `cm2m_cls`
def private kq_tile_target(placeholder, fmt_name, family, arm_tail : string) : string {
    let target = stamp_target(placeholder, "tile_", kq_tile_stem(fmt_name, family), arm_tail)
    return target |> ends_with("_cm2e_cls") && kq_cm2e_aliases_m(fmt_name) ? "{slice(target, 0, length(target) - length("cm2e_cls"))}cm2m_cls" : target
}

class private PrefixCallNames : AstVisitor {
    prefix : string
    names : array<string>
    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        let n = "{expr.name}"
        let bare = n |> starts_with(STAMP_INST) ? slice(n, length(STAMP_INST)) : n
        if (bare |> starts_with(prefix) && find_index(names, n) < 0) {
            names |> push(n)
        }
        return <- expr
    }
}

//! the distinct names of the calls a block makes that start with `prefix` (a stamp macro's placeholders)
def private placeholder_names(var root : ExpressionPtr; prefix : string) : array<string> {
    var collector = new PrefixCallNames(prefix = prefix)
    make_visitor(*collector) $(adapter) {
        visit_expression(root, adapter)
    }
    var names <- collector.names
    unsafe {
        delete collector
    }
    return <- names
}

//! the block cloned with every placeholder in `names` renamed by `target`
def private renamed_arm(at : LineInfo; root : ExpressionPtr; names : array<string>; target : block<(n : string) : string>) : ExpressionPtr {
    var body = clone_expression(root)
    return apply_template(at, body) $(var r : Template) {
        for (n in names) {
            r |> renameCall(n, target |> invoke(n))
        }
    }
}

//! `ladder` behind one more arm: `arm` where `key` reads `ord`, else what `ladder` already picks
def private ladder_arm(var key, arm, ladder : ExpressionPtr; ord : int) : ExpressionPtr {
    return qmacro_expr() {
        if ($e(key) == $v(ord)) {
            $e(arm)
        } else {
            $e(ladder)
        }
    }
}

def private word_in(list, word : string) : bool {
    for (w in split(list, " ")) {
        return true if (w == word)
    }
    return false
}

//! the members Vulkan and CUDA stamp no kernel for (Metal stamps them): those homes' loads decline their planes by name
let KQ_DISCRETE_UNSTAMPED = "mlx4 mlx8 mlx4b mlx8b mlx6b"

//! a member the discrete GPU homes stamp no kernel for
def kq_discrete_unstamped(f : KqFmt) : bool => word_in(KQ_DISCRETE_UNSTAMPED, "{f}")

//! whether a stamp macro's skip words drop member `f`: the member named, `!b32` and `f` no per-32 format, or - on a ladder
//! whose words do not name the `metal` home - a member the discrete homes do not stamp
def private stamp_skips(skip : string; f : KqFmt) : bool => (word_in(skip, "{f}") || (word_in(skip, "!b32") && !kq_dense_b32(f))
    || (kq_discrete_unstamped(f) && !word_in(skip, "metal")))

//! a stamp macro's word list: a string literal, or a module `let` of one - in the module the call compiles in, else in the first module of the program that declares it
def private macro_words(prog : ProgramPtr; arg : ExpressionPtr) : string {
    if (arg is ExprConstString) {
        return string((arg as ExprConstString).value)
    }
    var words = ""
    if (arg is ExprVar) {
        let name = "{(arg as ExprVar).name}"
        for_each_global(prog.getThisModule) $(v) {
            if ("{v.name}" == name && v.init is ExprConstString) {
                words = string((v.init as ExprConstString).value)
            }
        }
        if (empty(words)) {   // a generic's instance compiles in the module that instances it; the `let` sits in the module that wrote the call
            get_ptr(prog) |> for_each_module() $(m) {
                for_each_global(m) $(v) {
                    if (empty(words) && "{v.name}" == name && v.init is ExprConstString) {
                        words = string((v.init as ExprConstString).value)
                    }
                }
            }
        }
    }
    return words
}

//! `macro_words`' words, empty ones dropped; no argument reads none
def private macro_word_list(prog : ProgramPtr; arg : ExpressionPtr) : array<string> =>
    <- arg == null ? array<string>() : [for (w in split(macro_words(prog, arg), " ")); w; where !empty(w)]

//! the block of an argument-less `<| $() { ... }` the stamp macros take last
def private stamp_block(prog : ProgramPtr; var expr : ExprCallMacro?; what : string) : ExpressionPtr {
    var emb = expr.arguments[length(expr.arguments) - 1] as ExprMakeBlock
    macro_verify(emb._block is ExprBlock, prog, expr.at, "{what}: expecting a block body, got {describe(emb._block)}")
    let blk = emb._block as ExprBlock
    macro_verify(empty(blk.arguments) && empty(blk.finalList), prog, expr.at, "{what}: expecting an argument-less block and no finally section")
    return emb._block
}

[call_macro(name = "kq_tile_stamp")]
class KqTileStamp : AstCallMacro {
    //! ``kq_tile_stamp(fmt[, skip[, family[, tail, tails]]]) <| $() { ... tile_<verb>_<tail>(...) ... }``: the block once per KqFmt member
    //! on a ladder over ``fmt``, each placeholder renamed by ``kq_tile_target``; ``skip`` members (``!b32``: all but the per-32 ones) get
    //! no arm, nor do the members the discrete homes do not stamp unless ``skip`` names the ``metal`` home. With ``tail``/``tails`` a ``tile_<verb>`` placeholder takes arm ``j``'s word from ``tails`` on an inner ladder over ``tail``, ``<fmt>/<tail>`` skips a pair
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        let nargs = length(expr.arguments)
        macro_verify((nargs >= 2 && nargs <= 4 || nargs == 6) && expr.arguments[nargs - 1] is ExprMakeBlock, prog, expr.at,
            "expecting kq_tile_stamp(fmt[, skip[, family[, tail, tails]]]) followed by a block")
        macro_verify(expr.arguments[0] is ExprVar, prog, expr.at,
            "kq_tile_stamp takes a plain variable for the format, got {describe(expr.arguments[0])}")
        let skip = nargs >= 3 ? macro_words(prog, expr.arguments[1]) : ""
        let family = nargs >= 4 ? macro_words(prog, expr.arguments[2]) : "batch"
        macro_verify(!empty(family), prog, expr.at, "kq_tile_stamp's family is a string literal or a module `let` of one")
        let tails <- macro_word_list(prog, nargs == 6 ? expr.arguments[4] : null)
        macro_verify(nargs != 6 || (!empty(tails) && expr.arguments[3] is ExprVar), prog, expr.at,
            "kq_tile_stamp's tail key is a plain variable and its tails a string literal or a module `let` of one")
        var root = stamp_block(prog, expr, "kq_tile_stamp")
        let names <- placeholder_names(root, "tile_")
        macro_verify(!empty(names), prog, expr.at, "kq_tile_stamp: the block calls no `tile_<verb>_<tail>` placeholder")
        var fmt = expr.arguments[0]
        var ladder : ExpressionPtr
        for (f in type<KqFmt>) {
            let fname = "{f}"
            continue if (stamp_skips(skip, f))
            var arm : ExpressionPtr
            if (empty(tails)) {
                arm = renamed_arm(expr.at, root, names) $(n) => kq_tile_target(n, fname, family, "")
            } else {
                var tail = expr.arguments[3]
                arm = qmacro_block() {
                    panic("kq_tile_stamp: tail {$e(tail)} has no tile arm for {$v(fname)}")
                }
                for (t, j in tails, count()) {
                    continue if (word_in(skip, "{fname}/{t}"))
                    var tarm = renamed_arm(expr.at, root, names) $(n) => kq_tile_target(n, fname, family, t)
                    arm = ladder_arm(tail, tarm, arm, j)
                }
            }
            if (ladder == null) {
                ladder = qmacro_block() {
                    if ($e(fmt) != int($v(f))) {
                        panic("kq_tile_stamp: format slot {$e(fmt)} has no tile arm")
                    }
                    $e(arm)
                }
            } else {
                ladder = qmacro_expr() {
                    if ($e(fmt) == int($v(f))) {
                        $e(arm)
                    } else {
                        $e(ladder)
                    }
                }
            }
        }
        return ladder
    }
}

[call_macro(name = "kq_tile_stamped")]
class KqTileStamped : AstCallMacro {
    //! ``kq_tile_stamped(fmt, skip, tail, tails)``: whether ``kq_tile_stamp`` over the same ``skip`` and ``tails`` has an arm for the
    //! pair (``fmt``, ``tail``) - the pairs read at compile time off the words the stamp itself reads
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(length(expr.arguments) == 4 && expr.arguments[0] is ExprVar && expr.arguments[2] is ExprVar, prog, expr.at,
            "expecting kq_tile_stamped(fmt, skip, tail, tails), the format and the tail plain variables")
        let skip = macro_words(prog, expr.arguments[1])
        let tails <- macro_word_list(prog, expr.arguments[3])
        macro_verify(!empty(tails), prog, expr.at, "kq_tile_stamped's tails are a string literal or a module `let` of one")
        var fmt = expr.arguments[0]
        var tail = expr.arguments[2]
        var any = qmacro(false)
        for (f in type<KqFmt>) {
            continue if (stamp_skips(skip, f))
            for (t, j in tails, count()) {
                continue if (word_in(skip, "{f}/{t}"))
                any = qmacro($e(any) || ($e(fmt) == int($v(f)) && $e(tail) == $v(j)))
            }
        }
        return any
    }
}

[call_macro(name = "stamp_ladder")]
class StampLadder : AstCallMacro {
    //! ``stamp_ladder(key, stems) <| $() { ... stamp_<verb>_<tail>(...) ... }``: the block once per word of ``stems`` on a ladder over
    //! the int variable ``key``, arm ``i`` renaming each placeholder to ``<verb>_<word i>_<tail>``; a ``-`` word, and a key past
    //! every word, panics
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(length(expr.arguments) == 3 && expr.arguments[2] is ExprMakeBlock, prog, expr.at,
            "expecting stamp_ladder(key, stems) followed by a block")
        macro_verify(expr.arguments[0] is ExprVar, prog, expr.at, "stamp_ladder takes a plain variable for the key, got {describe(expr.arguments[0])}")
        let stems <- macro_word_list(prog, expr.arguments[1])
        macro_verify(!empty(stems), prog, expr.at, "stamp_ladder's stems are a string literal or a module `let` of one")
        var root = stamp_block(prog, expr, "stamp_ladder")
        let names <- placeholder_names(root, "stamp_")
        macro_verify(!empty(names), prog, expr.at, "stamp_ladder: the block calls no `stamp_<verb>_<tail>` placeholder")
        var key = expr.arguments[0]
        var ladder = qmacro_block() {
            panic("stamp_ladder: key {$e(key)} has no stamp")
        }
        for (s, i in stems, count()) {
            continue if (s == "-")
            var arm = renamed_arm(expr.at, root, names) $(n) => stamp_target(n, "stamp_", s, "")
            ladder = ladder_arm(key, arm, ladder, i)
        }
        return ladder
    }
}

// upstream (ggml) tensor data type ids - the subset the loader reads
let GGML_TYPE_F32 = 0
let GGML_TYPE_F16 = 1
let GGML_TYPE_Q4_0 = 2
let GGML_TYPE_Q5_0 = 6
let GGML_TYPE_Q5_1 = 7
let GGML_TYPE_Q8_0 = 8
let GGML_TYPE_Q3_K = 11
let GGML_TYPE_Q2_K = 10
let GGML_TYPE_IQ3_S = 21
let GGML_TYPE_IQ2_S = 22
let GGML_TYPE_IQ2_XS = 17
let GGML_TYPE_IQ2_XXS = 16
let GGML_TYPE_IQ3_XXS = 18
let GGML_TYPE_IQ4_NL = 20
let GGML_TYPE_Q4_K = 12
let GGML_TYPE_Q5_K = 13
let GGML_TYPE_Q6_K = 14
let GGML_TYPE_IQ4_XS = 23
let GGML_TYPE_BF16 = 30
let GGML_TYPE_MXFP4 = 39
let GGML_TYPE_NVFP4 = 40
let GGML_TYPE_Q2_0 = 42
let GGML_TYPE_Q1_0 = 41
let GGML_TYPE_IQ1_S = 19
let GGML_TYPE_IQ1_M = 29
let GGML_TYPE_TQ2_0 = 35
let GGML_TYPE_TQ1_0 = 34
// ... and the ids the load declines by name: no reader arm and no `KqFmt` member carries them
let GGML_TYPE_Q4_1 = 3
//! the loader's own type ids for a safetensors affine triplet (u32 codes, the 64-group scale and bias planes beside them),
//! outside ggml's id range: 4- and 8-bit codes under F16 or BF16 scales; a tensor of one counts its code bytes
let GGML_TYPE_MLX4 = 1004
let GGML_TYPE_MLX8 = 1008
let GGML_TYPE_MLX4B = 1104
let GGML_TYPE_MLX8B = 1108
let GGML_TYPE_MLX6B = 1106   //! 6-bit codes under BF16 scales: no F16 6-bit member exists
//! the n-gram table's sharded affine rows: 4-bit codes in 32-groups under BF16 pairs, each row shard its codes, then its scales,
//! then its biases; no member serves it - only the n-gram row gather reads it
let GGML_TYPE_NGRAM_A4G32 = 1132
//! the bytes one 32-group of an n-gram table row takes across the three planes: 16 code bytes, a BF16 scale, a BF16 bias
let NGRAM_A4G32_BLOCK = 20l
//! the weights one affine scale and bias pair covers
let MLX_GROUP = 64l

//! the code width of an affine triplet's loader type; 0 for any other type
def mlx_affine_bits(gt : int) : int64 {
    return 4l if (gt == GGML_TYPE_MLX4 || gt == GGML_TYPE_MLX4B)
    return 8l if (gt == GGML_TYPE_MLX8 || gt == GGML_TYPE_MLX8B)
    return 6l if (gt == GGML_TYPE_MLX6B)
    return 0l
}

//! the loader type of an affine triplet of `bits`-bit codes in `group`-weight groups under BF16 (`bf16`) or F16 scales; -1
//! for a geometry no member serves
def mlx_affine_ggml_type(bits, group : int; bf16 : bool) : int {
    return -1 if (int64(group) != MLX_GROUP)
    if (bits == 4) {
        return bf16 ? GGML_TYPE_MLX4B : GGML_TYPE_MLX4
    }
    if (bits == 8) {
        return bf16 ? GGML_TYPE_MLX8B : GGML_TYPE_MLX8
    }
    if (bits == 6) {
        return bf16 ? GGML_TYPE_MLX6B : -1
    }
    return -1
}

//! One format's descriptor row - every per-format number a consumer keys off. The stride unit is
//! one 256-weight superblock on the kq lattice and one 32-weight block for q8/q51/mx4; `qsb` is 0
//! where no stride-walked plane pair exists (q8), `schema_id` -1 where no kernel takes an int id.
struct KqDesc {
    fmt : KqFmt
    qsb : int64          //!< quant-plane bytes per stride unit
    ssb : int64          //!< scale-plane bytes per scale unit: `sgroup` stride units
    sgroup : int64       //!< stride units one scale unit serves
    elems : int64        //!< weights per stride unit
    disk_bytes : int64   //!< bytes one scale unit's `sgroup` stride units take on disk (the ggml block bytes)
    ggml_type : int      //!< the ggml type the planes convert from
    schema_id : int      //!< the kernel/IR format id
    stream_code : int    //!< the loader's stream/repack region tag
}

//! the two stride units a device plane strides its rows and activations by: the Q8_K-form superblock and the Q8_0-form block
let KQ_SB_ELEMS = 256l
let KQ_B32_ELEMS = 32l
let KQ_B32_ELEMS_U = 32u   //! the Q8_0 block width as a kernel body reads it (a module `let` is a stamp; the int64 form is the host's)

//! a per-32 device block: quant words (16 nibble bytes, then q51's
//! and q5032's qh word) and scale bytes (q51's d | m, mx4's E8M0, the
//! others' d) - its disk bytes, pinned a row each in test_kqformat
let KQ_B32_QWORDS = 4u
let KQ_Q51_QWORDS = 5u
let KQ_Q51_SBYTES = 4u
let KQ_IQ4NL32_SBYTES = 2u
let KQ_Q4032_SBYTES = 2u
let KQ_Q5032_QWORDS = 5u
let KQ_Q5032_SBYTES = 2u
let KQ_MX4_SBYTES = 1u
//! q20's device block: two code words a 32-block, and its f16 d one scale unit a `KQ_Q20_SGROUP`-block group
let KQ_Q20_QWORDS = 2u
let KQ_Q20_SBYTES = 2u
let KQ_Q20_SGROUP = 2u
//! q10's device block: one sign word a 32-block, and its f16 d one scale unit a `KQ_Q10_SGROUP`-block group
let KQ_Q10_QWORDS = 1u
let KQ_Q10_SBYTES = 2u
let KQ_Q10_SGROUP = 4u

//! The descriptor row of a format - a new format is one row here plus its planes; every
//! per-format accessor below is a field of this row.
def kq_desc(f : KqFmt) : KqDesc {   // nolint:STYLE037,STYLE038 - one arm per KqFmt member, the honest shape
    if (f == KqFmt.k4) {
        return KqDesc(fmt = f, qsb = K4_QSB, ssb = K4_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 144l, ggml_type = GGML_TYPE_Q4_K, schema_id = 4, stream_code = 4)
    }
    if (f == KqFmt.k5) {
        return KqDesc(fmt = f, qsb = K5_QSB, ssb = K5_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 176l, ggml_type = GGML_TYPE_Q5_K, schema_id = 5, stream_code = 5)
    }
    if (f == KqFmt.k6) {
        return KqDesc(fmt = f, qsb = K6_QSB, ssb = K6_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 210l, ggml_type = GGML_TYPE_Q6_K, schema_id = 6, stream_code = 6)
    }
    if (f == KqFmt.q40) {
        return KqDesc(fmt = f, qsb = Q40_QSB, ssb = Q40_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 144l, ggml_type = GGML_TYPE_Q4_0, schema_id = 40, stream_code = 40)
    }
    if (f == KqFmt.iq4xs) {
        return KqDesc(fmt = f, qsb = IQ4XS_QSB, ssb = IQ4XS_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 136l, ggml_type = GGML_TYPE_IQ4_XS, schema_id = 44, stream_code = 44)
    }
    if (f == KqFmt.k3) {
        return KqDesc(fmt = f, qsb = K3_QSB, ssb = K3_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 110l, ggml_type = GGML_TYPE_Q3_K, schema_id = 3, stream_code = 3)
    }
    if (f == KqFmt.iq3s) {
        return KqDesc(fmt = f, qsb = IQ3S_QSB, ssb = IQ3S_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 110l, ggml_type = GGML_TYPE_IQ3_S, schema_id = 33, stream_code = 33)
    }
    if (f == KqFmt.iq3xxs) {
        return KqDesc(fmt = f, qsb = IQ3XXS_QSB, ssb = IQ3XXS_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 98l, ggml_type = GGML_TYPE_IQ3_XXS, schema_id = 34, stream_code = 34)
    }
    if (f == KqFmt.iq4nl) {
        return KqDesc(fmt = f, qsb = Q40_QSB, ssb = Q40_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 144l, ggml_type = GGML_TYPE_IQ4_NL, schema_id = 45, stream_code = 45)
    }
    if (f == KqFmt.k2) {
        return KqDesc(fmt = f, qsb = K2_QSB, ssb = K2_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 84l, ggml_type = GGML_TYPE_Q2_K, schema_id = 2, stream_code = K2_STREAM_TAG)
    }
    if (f == KqFmt.iq2s) {
        return KqDesc(fmt = f, qsb = IQ2S_QSB, ssb = IQ2S_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 82l, ggml_type = GGML_TYPE_IQ2_S, schema_id = 23, stream_code = 23)
    }
    if (f == KqFmt.iq2xs) {
        return KqDesc(fmt = f, qsb = IQ2XS_QSB, ssb = IQ2XS_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 74l, ggml_type = GGML_TYPE_IQ2_XS, schema_id = 24, stream_code = 24)
    }
    if (f == KqFmt.iq2xxs) {
        return KqDesc(fmt = f, qsb = IQ2XXS_QSB, ssb = IQ2XXS_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 66l, ggml_type = GGML_TYPE_IQ2_XXS, schema_id = 25, stream_code = 25)
    }
    if (f == KqFmt.q51) {
        return KqDesc(fmt = f, qsb = Q51_QB, ssb = Q51_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 24l, ggml_type = GGML_TYPE_Q5_1, schema_id = -1, stream_code = 2)
    }
    if (f == KqFmt.mx4) {
        return KqDesc(fmt = f, qsb = MX4_QB, ssb = MX4_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 17l, ggml_type = GGML_TYPE_MXFP4, schema_id = -1, stream_code = MX4_STREAM_TAG)
    }
    if (f == KqFmt.iq4nl32) {
        return KqDesc(fmt = f, qsb = IQ4NL32_QB, ssb = IQ4NL32_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 18l, ggml_type = GGML_TYPE_IQ4_NL, schema_id = -1, stream_code = IQ4NL32_STREAM_TAG)
    }
    if (f == KqFmt.iq3s4) {
        return KqDesc(fmt = f, qsb = IQ4XS_QSB, ssb = IQ3S_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 110l, ggml_type = GGML_TYPE_IQ3_S, schema_id = 35, stream_code = 35)
    }
    if (f == KqFmt.q4032) {
        return KqDesc(fmt = f, qsb = Q4032_QB, ssb = Q4032_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 18l, ggml_type = GGML_TYPE_Q4_0, schema_id = -1, stream_code = Q4032_STREAM_TAG)
    }
    if (f == KqFmt.q5032) {
        return KqDesc(fmt = f, qsb = Q5032_QB, ssb = Q5032_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 22l, ggml_type = GGML_TYPE_Q5_0, schema_id = -1, stream_code = Q5032_STREAM_TAG)
    }
    if (f == KqFmt.nv4) {
        return KqDesc(fmt = f, qsb = NV4_QSB, ssb = NV4_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 144l, ggml_type = GGML_TYPE_NVFP4, schema_id = 49, stream_code = 49)
    }
    if (f == KqFmt.mlx4 || f == KqFmt.mlx4b) {
        return KqDesc(fmt = f, qsb = MLX4_QB, ssb = MLX_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 16l, ggml_type = f == KqFmt.mlx4 ? GGML_TYPE_MLX4 : GGML_TYPE_MLX4B,
            schema_id = -1, stream_code = f == KqFmt.mlx4 ? MLX4_STREAM_TAG : MLX4B_STREAM_TAG)
    }
    if (f == KqFmt.mlx8 || f == KqFmt.mlx8b) {
        return KqDesc(fmt = f, qsb = MLX8_QB, ssb = MLX_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 32l, ggml_type = f == KqFmt.mlx8 ? GGML_TYPE_MLX8 : GGML_TYPE_MLX8B,
            schema_id = -1, stream_code = f == KqFmt.mlx8 ? MLX8_STREAM_TAG : MLX8B_STREAM_TAG)
    }
    if (f == KqFmt.mlx6b) {
        return KqDesc(fmt = f, qsb = MLX6_QB, ssb = MLX_SB, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 24l, ggml_type = GGML_TYPE_MLX6B,
            schema_id = -1, stream_code = MLX6B_STREAM_TAG)
    }
    if (f == KqFmt.q20) {
        return KqDesc(fmt = f, qsb = Q20_QB, ssb = Q20_SB, sgroup = Q20_SGROUP, elems = KQ_B32_ELEMS, disk_bytes = 18l, ggml_type = GGML_TYPE_Q2_0,
            schema_id = -1, stream_code = Q20_STREAM_TAG)
    }
    if (f == KqFmt.q10) {
        return KqDesc(fmt = f, qsb = Q10_QB, ssb = Q10_SB, sgroup = Q10_SGROUP, elems = KQ_B32_ELEMS, disk_bytes = 18l, ggml_type = GGML_TYPE_Q1_0,
            schema_id = -1, stream_code = Q10_STREAM_TAG)
    }
    if (f == KqFmt.iq1s) {
        return KqDesc(fmt = f, qsb = IQ1S_QSB, ssb = IQ1S_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 50l, ggml_type = GGML_TYPE_IQ1_S, schema_id = 13, stream_code = 13)
    }
    if (f == KqFmt.iq1m) {
        return KqDesc(fmt = f, qsb = IQ1M_QSB, ssb = IQ1M_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 56l, ggml_type = GGML_TYPE_IQ1_M, schema_id = 16, stream_code = 16)
    }
    if (f == KqFmt.tq2) {
        return KqDesc(fmt = f, qsb = TQ2_QSB, ssb = TQ2_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 66l, ggml_type = GGML_TYPE_TQ2_0, schema_id = 22, stream_code = 22)
    }
    if (f == KqFmt.tq1) {
        return KqDesc(fmt = f, qsb = TQ1_QSB, ssb = TQ1_SSB, sgroup = 1l, elems = KQ_SB_ELEMS, disk_bytes = 54l, ggml_type = GGML_TYPE_TQ1_0, schema_id = 21, stream_code = 21)
    }
    return KqDesc(fmt = KqFmt.q8, qsb = 0l, ssb = 0l, sgroup = 1l, elems = KQ_B32_ELEMS, disk_bytes = 34l, ggml_type = GGML_TYPE_Q8_0, schema_id = -1, stream_code = 0)
}

//! The superblock-lattice formats (Q8_K-form activations, % 256 rows, repack + stamped kq
//! kernels). q51 deliberately fails this: it rides per-32 planes with Q8_0-form activations —
//! a `fmt != KqFmt.q8` test does not imply the kq lattice; branch on kq_sb where it does.
def kq_sb(f : KqFmt) : bool => kq_desc(f).elems == KQ_SB_ELEMS

//! Parses an enum member's spelling (`k4`, `iq2xxs`, ...) into `out`; false for any other string.
def kq_fmt_of_name(name : string; var out : KqFmt&) : bool => kq_fmt_where(out) <| $(f : KqFmt) => "{f}" == name

//! The per-32 formats: 32-weight blocks with Q8_0-form activations (q8, q51, mx4), off the kq lattice.
def kq_block32(f : KqFmt) : bool => !kq_sb(f)

//! The same predicate over the int id space the GPU drivers carry per stack/plane.
def kq_block32(fmt : int) : bool => !kq_sb(fmt)

//! the block a device plane of the format strides its rows and activations by (`kq_elems`, KQ_B32_ELEMS on the Q8_0-form
//! planes, KQ_SB_ELEMS on the superblock forms) - by the format, by its arena slot, or by the superblock flag a feed carries
def kq_unit(f : KqFmt) : int64 => kq_elems(f)

def kq_unit(fmt : int) : int64 => kq_unit(kq_sb(fmt))

def kq_unit(sb : bool) : int64 => sb ? KQ_SB_ELEMS : KQ_B32_ELEMS

//! a row of `n` elements is placeable on a device plane when the unit divides it; a caller DECLINES the other rows to the CPU
def kq_rows_ok(n : int64; f : KqFmt) : bool => n % kq_unit(f) == 0l

def kq_rows_ok(n : int64; fmt : int) : bool => n % kq_unit(fmt) == 0l

//! the blocks `rows` placeable rows of `n` elements take on the plane
def kq_blocks_for(n, rows : int64; f : KqFmt) : int64 {
    if (!kq_rows_ok(n, f)) {
        panic("dasLLAMA: a {f} row of {n} elements is not a multiple of its {kq_unit(f)}-element block")
    }
    return rows * (n / kq_unit(f))
}

def kq_blocks_for(n, rows : int64; fmt : int) : int64 {
    if (!kq_rows_ok(n, fmt)) {
        panic("dasLLAMA: a row of {n} elements on format slot {fmt} is not a multiple of its {kq_unit(fmt)}-element block")
    }
    return rows * (n / kq_unit(fmt))
}

//! the KqFmt slot count of a per-format table (int(KqFmt) indexes it; members append, so the last is the bound)
let KQ_FMT_COUNT = int(KqFmt.tq1) + 1

//! the format an `int(KqFmt)` slot names - the per-format slot the GPU drivers and the image's plane tables index by; the caller holds the slot inside the enum's range
def kq_fmt_of_slot(slot : int) : KqFmt => unsafe(reinterpret<KqFmt>(uint8(slot)))

//! The same predicate over the int id space the GPU drivers carry per stack/plane (pure: it sits in asserts).
def kq_sb(fmt : int) : bool => fmt >= 0 && fmt < KQ_FMT_COUNT && kq_sb(kq_fmt_of_slot(fmt))

//! an expert plane format the expert chain and the host twin read: q8, a superblock format, a per-32 one, mx4 (the GEMVs and the batch tiles)
def public expert_chain_fmt_ok(f : KqFmt) : bool => f == KqFmt.q8 || kq_sb(f) || kq_dense_b32(f) || f == KqFmt.mx4

//! the same predicate over the int slot the GPU drivers carry per plane: the CUDA driver's arenas and the host twin serve exactly these
def public expert_chain_slot_ok(fmt : int) : bool => expert_chain_fmt_ok(kq_fmt_of_slot(fmt))

//! the formats the CUDA home's MMQ leaves serve on a window GEMM site (int8 over Q8_0 feeds; NVFP4 on the FP4 tensor cores), in the
//! leaf table's order, and their names for the load log; `kq_mmq_leaf_index` is -1 off the table
let KQ_MMQ_LEAF_FMTS <- [KqFmt.q8, KqFmt.k4, KqFmt.iq4xs, KqFmt.q40, KqFmt.iq4nl, KqFmt.q51, KqFmt.q5032, KqFmt.q4032, KqFmt.iq4nl32, KqFmt.nv4]
let KQ_MMQ_LEAF_NAMES <- ["Q8_0", "Q4_K", "IQ4_XS", "Q4_0", "IQ4_NL", "Q5_1", "Q5_0", "Q4_0 (per-32)", "IQ4_NL (per-32)", "NVFP4 (FP4 tensor cores)"]
def kq_mmq_leaf_index(f : KqFmt) : int => find_index(KQ_MMQ_LEAF_FMTS, f)
def kq_mmq_leaf(f : KqFmt) : bool => kq_mmq_leaf_index(f) >= 0

//! the leaves only `DASLLAMA_CUDA_MMQ=2` arms: their windows did not beat the cm2 tiles on every measured card
def kq_mmq_raced(f : KqFmt) : bool => f == KqFmt.q51 || f == KqFmt.q5032

//! the leaves with a min term: their reduction folds sixteen blocks at a time, so a row is a multiple of 512
def kq_mmq_min(f : KqFmt) : bool => f == KqFmt.k4 || f == KqFmt.q51

//! a per-32 plane format (Q5_1, Q5_0, per-32 IQ4_NL and Q4_0, Q2_0, Q1_0, the affine members): its own plane pair, never a superblock bind
def kq_dense_b32(fmt : KqFmt) : bool => (fmt == KqFmt.q51 || fmt == KqFmt.iq4nl32
    || fmt == KqFmt.q4032 || fmt == KqFmt.q5032 || fmt == KqFmt.q20 || fmt == KqFmt.q10 || kq_mlx_affine(fmt))

//! a per-32 member whose scale unit serves a block group (q20's two blocks, q10's four): disk-order planes, rows whole in groups
def kq_grouped_b32(fmt : KqFmt) : bool => kq_sgroup(fmt) > 1l

//! an affine member: per-32 planes whose block pair shares one 64-group (scale, bias) pair
def kq_mlx_affine(fmt : KqFmt) : bool => fmt == KqFmt.mlx4 || fmt == KqFmt.mlx8 || fmt == KqFmt.mlx4b || fmt == KqFmt.mlx8b || fmt == KqFmt.mlx6b

//! an affine member's code width - 4, 6 or 8 - and 0 off the affine members
def kq_mlx_bits(fmt : KqFmt) : int => int(mlx_affine_bits(kq_desc(fmt).ggml_type))

//! an affine member whose scale and bias halves are BF16; the other affine members carry F16 halves
def kq_mlx_bf16(fmt : KqFmt) : bool => fmt == KqFmt.mlx4b || fmt == KqFmt.mlx8b || fmt == KqFmt.mlx6b

//! The same predicate over the int slot space the GPU drivers carry.
def kq_dense_b32(fmt : int) : bool => (fmt >= 0 && fmt < KQ_FMT_COUNT
    && kq_dense_b32(kq_fmt_of_slot(fmt)))

//! KqFmt -> the kernel/IR format id space — the ONE bridge between the enum and the int ids the
//! generated kernels take as runtime params. Mnemonic ids: a K-quant is its bit width (3/4/5/6),
//! Q4_0 is 40, an i-quant is bit width x 10 + a variant digit (IQ4_XS = 44, IQ3_S = 33). q8/q51 panic.
def kq_schema_id(f : KqFmt) : int {
    let id = kq_desc(f).schema_id
    if (id < 0) {
        panic("kq_schema_id: not a kq superblock format")
    }
    return id
}

//! The first format the predicate admits, into `out`; false when none does - the one walk every
//! key-to-format lookup (name, kernel id, stream code) runs.
def kq_fmt_where(var out : KqFmt&; blk : block<(f : KqFmt) : bool>) : bool {
    for (f in type<KqFmt>) {
        if (invoke(blk, f)) {
            out = f
            return true
        }
    }
    return false
}

//! The format a kernel/IR id names; false for an id no format carries.
def kq_fmt_of_id(id : int; var out : KqFmt&) : bool => id >= 0 && kq_fmt_where(out) <| $(f : KqFmt) => kq_desc(f).schema_id == id

//! The format of a kernel/IR id, or a panic naming the asking site (`what`) on an id no row carries.
def kq_fmt_of_id(id : int; what : string) : KqFmt {
    var f = KqFmt.q8
    if (!kq_fmt_of_id(id, f)) {
        panic("{what}: unknown kq format id {id}")
    }
    return f
}

//! The ggml type a kq format's plane converts from.
def kq_ggml_type(f : KqFmt) : int => kq_desc(f).ggml_type

// ===== plane stride truth (per 256-weight superblock row unless noted) =====

let K4_QSB = 128l   // Q4_K quant plane: 128 nibble bytes
let K4_LEVEL_MAX = 15   //! Q4_K's top quant level: four bits a weight
let K5_LEVEL_MAX = 31   //! Q5_K's: five
let K4_SSB = 20l    // Q4_K scale plane: 16B disk block + 4B pad, decoded in place at repack
let K5_QSB = 160l   // Q5_K quant plane: 128 nibble bytes + 32 qh bytes
let K5_QH_B32 = (K5_QSB - K4_QSB) / (KQ_SB_ELEMS / KQ_B32_ELEMS)   //! a k5 superblock's qh bytes a 32-block (its 32 past the nibbles, over eight blocks)
let K5_SSB = 20l    // Q5_K scale plane: same 20B row as k4
let K45_HEAD_BYTES = 4l   //! a Q4_K / Q5_K disk scale block's head: f16 d, f16 dmin
let K45_PACK_BYTES = 12   //! a Q4_K / Q5_K scale block's eight (scale, min) pairs after the head, six bits each
let K45_SUBS = 8          //! a Q4_K / Q5_K superblock's sub-blocks, 32 weights each
//! the k4 / k5 device scale record (`KQ_DEV_SSB` bytes): f16 d, f16 dmin, the eight 6-bit sub-scales decoded to a byte each at +4, the eight mins at +12
let KQ_SREC_SCALES_OFF = 4l
let KQ_SREC_MINS_OFF = 12l
let K6_QSB = 192l   // Q6_K quant plane: 128 ql + 64 qh bytes
let K6_SSB = 18l    // Q6_K scale plane: 16 int8 sub-scales + f16 d, native
let K6_D_AT = K6_SSB - 2l   //! a Q6_K scale block's f16 d, after its sixteen int8 sub-scales
let K6_QL_BYTES = 128l    //! a Q6_K superblock's low-nibble bytes, ahead of the qh bytes
let K6_QH_BYTES = K6_QSB - K6_QL_BYTES   //! its qh bytes: two bits a weight, a half of them a 128-weight half
let Q40_QSB = 128l  // Q4_0 quant plane: 128 nibble bytes (k4 tiling)
let Q40_SSB = 16l   // Q4_0 scale plane: 8 x f16 d, verbatim
let Q51_QB = 20l    // q5_1 quant plane PER 32-BLOCK: 16 nibble bytes (k/k+16 pairing) + 4 qh
let Q51_SB = 4l     // q5_1 scale plane PER 32-BLOCK: f16 d + f16 m
let MX4_QB = 16l    // MXFP4 quant plane PER 32-BLOCK: 16 nibble bytes (element j low, j + 16 high), the disk order
let MX4_SB = 1l     // MXFP4 scale plane PER 32-BLOCK: one E8M0 byte
let IQ4NL32_QB = 16l   // IQ4_NL quant plane PER 32-BLOCK: 16 nibble bytes (element j low, j + 16 high), the disk order
let IQ4NL32_SB = 2l    // IQ4_NL scale plane PER 32-BLOCK: f16 d
let Q4032_QB = 16l     // Q4_0 quant plane PER 32-BLOCK: 16 nibble bytes (element j low, j + 16 high), the disk order
let Q4032_SB = 2l      // Q4_0 scale plane PER 32-BLOCK: f16 d
let Q5032_QB = 20l     // Q5_0 quant plane PER 32-BLOCK: q51's form - 16 nibble bytes (element j low, j + 16 high), then the qh word
let Q5032_SB = 2l      // Q5_0 scale plane PER 32-BLOCK: f16 d
let MLX4_QB = 16l      //! affine 4-bit quant plane PER 32-BLOCK: the codes' four u32 words verbatim (element k at bits 4k..4k+3)
let MLX8_QB = 32l      //! affine 8-bit quant plane PER 32-BLOCK: one code byte an element, in order
let MLX6_QB = 24l      //! affine 6-bit quant plane PER 32-BLOCK: the codes' six u32 words verbatim (element k at bits 6k..6k+5 of the little-endian stream)
let MLX_SB = 2l        //! affine scale plane PER 32-BLOCK: half of the 64-group's (scale, bias) half pair, which its two blocks share
let MLX_PAIR_BYTES = 4l   //! one 64-group's (scale, bias) pair: two halves, scale first
let Q20_QB = 8l        //! Q2_0 quant plane PER 32-BLOCK: the 2-bit codes in the disk order (element j at byte j / 4, bits (j % 4) x 2)
let Q20_SB = 2l        //! Q2_0 scale plane PER SCALE UNIT: the 64-weight group's f16 d
let Q20_SGROUP = 2l    //! the 32-blocks one Q2_0 scale unit serves: the disk block's 64 weights
let Q20_CODE_BITS = 2         //! a Q2_0 code's bits
let Q20_CODE_MASK = 3         //! ... its mask
let Q20_CODES_A_BYTE = 4l     //! ... the codes a byte holds: element j at bits (j % 4) x 2 of byte j / 4
let Q20_ZERO_CODE = 1         //! the code of the zero level: weight (q - 1) x d

//! code slot `k` of a Q2_0 code byte `b` as its signed level q - 1 (-1, 0, 1, 2); generic, to inline in the caller's partition
def q20_level(b : auto(BT); k : auto(KT)) : int => ((int(b) >> (int(k) * Q20_CODE_BITS)) & Q20_CODE_MASK) - Q20_ZERO_CODE
let Q10_QB = 4l        //! Q1_0 quant plane PER 32-BLOCK: the sign bits in the disk order (element j at bit j % 8 of byte j / 8)
let Q10_SB = 2l        //! Q1_0 scale plane PER SCALE UNIT: the 128-weight group's f16 d
let Q10_SGROUP = 4l    //! the 32-blocks one Q1_0 scale unit serves: the disk block's 128 weights
let Q10_BITS_A_BYTE = 8l   //! the sign bits a Q1_0 byte holds: element j at bit j % 8 of byte j / 8

//! bit `k` of a Q1_0 sign byte `b` as its level, +1 set and -1 clear; generic, to inline in the caller's partition
def q10_level(b : auto(BT); k : auto(KT)) : int => ((int(b) >> int(k)) & 1) * 2 - 1

//! the codes one quant byte of a low-bit member holds: element j at slot j % this of byte j / this
def lowbit_codes_a_byte(_f : KqTag_q20) : int64 => Q20_CODES_A_BYTE
def lowbit_codes_a_byte(_f : KqTag_q10) : int64 => Q10_BITS_A_BYTE

//! code slot `k` of quant byte `b` as the member's signed level: q20's q - 1, q10's +-1; generic in `b` and `k`, as the levels are
def lowbit_level(b : auto(BT); k : auto(KT); _f : KqTag_q20) : int => q20_level(b, k)
def lowbit_level(b : auto(BT); k : auto(KT); _f : KqTag_q10) : int => q10_level(b, k)
let NV4_QSB = 128l     //! NVFP4 quant plane: 128 nibble bytes a superblock, in the k4 tiling
let NV4_SSB = 16l      //! NVFP4 scale plane: 16 UE4M3 bytes a superblock, one a 16-weight sub-block
let NV4_UNIT_QUADS = 9u    //! NVFP4 device unit: a superblock's eight nibble quads in the file's pairing, then its scale quad
let NV4_SCALE_QUAD = 8u    //! ... the scale quad's place in the unit
let NV4_DEV_QSB = NV4_QSB + NV4_SSB   //! ... the unit's bytes: the device quant plane's stride a superblock
let NV4_DEV_SSB = 4l       //! NVFP4 device scale plane: the per-tensor scale word a superblock
//! a per-32 block's nibble bytes: elem j low in byte j, j + 16 high
let B32_NIB_BYTES = int64(KQ_B32_QWORDS) * 4l
//! q51's and q5032's qh word after the nibbles: elem k's bit 4 at k
let B5_QH_BYTES = Q5032_QB - B32_NIB_BYTES
//! the widest per-32 block's bytes (mlx8's quant block, q51's scale block): a one-block scratch
let B32_MAX_QB = int(MLX8_QB)
let B32_MAX_SB = int(Q51_SB)
let IQ4XS_QSB = 128l   // IQ4_XS quant plane: 128 nibble bytes, the disk k/k+16 pairing (q40 tiling)
let IQ4XS_SSB = 20l    // IQ4_XS scale plane: f16 d, 2 pad, 8 x int8 (ls - 32), 8 pad — the k4 decoded-row shape
let K3_QSB = 96l    // Q3_K quant plane: 64 qs bytes (four 2-bit lanes each) + 32 hmask bytes, verbatim disk order
let K3_SSB = 18l    // Q3_K scale plane: 16 x int8 (6-bit scale - 32, decoded at transcode) + f16 d — the k6 row
let IQ3S_QSB = 104l   // IQ3_S quant plane: [64 grid-index qs][8 qh ninth bits][32 sign bytes], verbatim disk order
let IQ3S_SSB = 20l    // IQ3_S scale plane: f16 d, 2 pad, 8 x int8 (1 + 2s), 8 pad — the iq4xs/k4 row shape
let IQ3XXS_QSB = 96l  // IQ3_XXS quant plane: [64 grid-index qs][32 aux bytes: per block 4x7-bit sign indices + 4-bit scale], verbatim disk order
let IQ3XXS_SSB = 20l  // IQ3_XXS scale plane: f16 d HALVED, 2 pad, 8 x uint8 (2*ls + 1), 8 pad — the iq3s row shape and fold (the format's global 0.25 = the halved d x the halved grid)
let K2_QSB = 64l    // Q2_K quant plane: 64 qs bytes (four 2-bit lanes each), verbatim disk order
let K2_SSB = 20l    // Q2_K scale plane: [f16 d][f16 dmin][16 sc/min nibble-pair bytes] - the disk scale block, header first
let IQ2S_QSB = 72l    // IQ2_S quant plane: [32 grid-index low bytes][32 sign bytes][8 qh], verbatim disk order
let IQ2S_SSB = 20l    // IQ2_S scale plane: f16 d EIGHTH-ed (the disk (0.5+ls)*0.25 = (2ls+1)/8), 2 pad, 16 x uint8 (2ls+1) - per-16 strips in the 20B row
let IQ2XS_QSB = 64l   // IQ2_XS quant plane: 32 u16 words ([9-bit grid index][7-bit ksigns index]), verbatim disk order
let IQ2XS_SSB = 20l   // IQ2_XS scale plane: the iq2s row exactly - f16 d EIGHTH-ed, 2 pad, 16 x uint8 (2ls+1) per-16 strips
let IQ2XXS_QSB = 64l   // IQ2_XXS quant plane: 8 blocks x [4 iq2xxs_grid byte indices][aux32: 4 x 7-bit ksigns + 4-bit ls], verbatim disk order
let IQ2XXS_SSB = 20l   // IQ2_XXS scale plane: the iq3xxs row shape - f16 d EIGHTH-ed, 2 pad, 8 x uint8 (2ls+1) per-32 strips, 8 pad
let IQ1_QS_BYTES = 32l   //! an IQ1_S / IQ1_M superblock's grid-index low bytes, one an octet, ahead of its qh bytes in the quant plane
let IQ1S_QSB = 48l   //! IQ1_S quant plane: [32 qs][8 LE u16 qh words, one a 32-block: index high bits, 3-bit scale, delta sign], verbatim disk order
let IQ1S_SSB = 2l    //! IQ1_S scale plane: the f16 d, verbatim
let IQ1M_QSB = 48l   //! IQ1_M quant plane: [32 qs][16 qh bytes, a nibble an octet: index high bits, delta sign], verbatim disk order
let IQ1M_SSB = 8l    //! IQ1_M scale plane: the four LE u16 scale words verbatim (four 3-bit half-block scales each, the f16 d's nibbles on top)
let IQ1_QWORDS = 12u   //! an IQ1_S / IQ1_M quant plane's words a superblock (`IQ1S_QSB` / 4): the device kernels' quant row stride
let IQ1_QH_WORD = 8u   //! ... and the word its qh bytes start at (`IQ1_QS_BYTES` / 4): block b's 16 qh bits the half b % 2 of word 8 + b / 2
let IQ1M_SWORDS = 2u   //! IQ1_M's scale plane words a superblock (`IQ1M_SSB` / 4)
let TQ2_QSB = 64l      //! TQ2_0 quant plane: 64 qs bytes (four 2-bit lanes each, Q2_K's lane order), verbatim disk order
let TQ2_SSB = 2l       //! TQ2_0 scale plane: the f16 d, nothing else - every sub-scale and min is 1 and dmin is d
let TQ2_ZERO = 1       //! the TQ2_0 code of a zero weight: w = d x (q - TQ2_ZERO)
let TQ2_K2_PAIR = 1 | (TQ2_ZERO << 4)   //! TQ2_0 as k2's block: every group's sub-scale 1 (low nibble) and min TQ2_ZERO (high nibble)
let TQ1_QSB = 52l      //! TQ1_0 quant plane: the 48 qs bytes (five trits each) then the 4 qh bytes (four), verbatim disk order
let TQ1_SSB = 2l       //! TQ1_0 scale plane: the f16 d, nothing else - tq2's row
let TQ1_QWORDS = 13u   //! TQ1_0 quant plane words a superblock (`TQ1_QSB` / 4): the device kernels' quant row stride

// IQ4_NL reuses Q40_QSB/Q40_SSB outright: the disk bytes are q40's (f16 d + 16 nibble bytes
// per 32-block, the k/k+16 pairing) — only the nibble SEMANTICS differ (IQ4NL_LUT, no -8).

let TQ1_DWORDS = 64     //! a TQ1_0 superblock's weight dwords, `tq1_dword_trit`'s domain
let TQ1_HEAD_COLS = 8   //! TQ1_0's head: quant columns 0 .. 7 (qs bytes 0 .. 31), five trits a byte - dwords 0 .. 39
let TQ1_MID_COLS = 4    //! ... columns 8 .. 11 (qs bytes 32 .. 47), five trits a byte - dwords 40 .. 59
let TQ1_QH_COL = 12     //! ... column 12, the qh bytes, four trits a byte - dwords 60 .. 63
let TQ1_HEAD_DWORDS = 40   //! `TQ1_HEAD_COLS` x 5
let TQ1_QS_DWORDS = 60     //! ... then `TQ1_MID_COLS` x 5
let TQ1_QH_BYTE = 48       //! the qh bytes' first byte in the quant row: `TQ1_QH_COL` x 4

//! the column (`.x`, of the TQ1_0 quant row's 13 four-byte ones) and trit (`.y`) carrying superblock elements 4 w .. 4 w + 3
def tq1_dword_trit(w : int) : int2 =>
    w < TQ1_HEAD_DWORDS ? int2(w % TQ1_HEAD_COLS, w / TQ1_HEAD_COLS) : (w < TQ1_QS_DWORDS ? int2((w - TQ1_HEAD_DWORDS) % TQ1_MID_COLS + TQ1_HEAD_COLS, (w - TQ1_HEAD_DWORDS) / TQ1_MID_COLS) : int2(TQ1_QH_COL, w - TQ1_QS_DWORDS))

//! 3^n, the multiplier of TQ1_0 digit n (0 to 4): byte n of 0x1B090301, then 81 - 32-bit words, so a shader body reads it too
def tq1_pow3(n : auto(TN)) : int => int(n) < 4 ? int((0x1B090301u >> uint(8 * int(n))) & 0xFFu) : 81

//! trit `n` (0 to 4) of TQ1_0 byte `b`, a code in {0, 1, 2}: (b x 3^n wrapped to 8 bits) x 3 >> 8
def tq1_trit(b : auto(TB); n : int) : int => (((int(b) * tq1_pow3(n)) & 255) * 3) >> 8



//! The IQ4_NL / IQ4_XS nibble codebook (upstream's kvalues_iq4nl): weight = scale x lut[nibble], as
//! a per-call local (16 bytes on the caller's frame) - the form every worker-run kernel reads.
def iq4nl_lut() : int8[16] => fixed_array<int8>(-127, -104, -83, -65, -49, -35, -22, -10, 1, 13, 25, 38, 53, 69, 89, 113)

//! The codebook's main-context copy (tests, oracles, the emitter's constant bake): a kernel that
//! can run on a forked worker reads a module global as zeros - kernels take `iq4nl_lut()`.
let IQ4NL_LUT = iq4nl_lut()

//! The iq3s4 nibble codebook: the sixteen signed odd values an IQ3_S weight takes (grid magnitude
//! 1..15 under its sign bit), lut[q] = 2q - 15 - so an IQ3_S superblock is served as an
//! iq4xs-shaped plane (4-bit index, the iq3s scale row) and decoded by the LUT kernels, exactly.
def iq3s4_lut() : int8[16] => fixed_array<int8>(-15, -13, -11, -9, -7, -5, -3, -1, 1, 3, 5, 7, 9, 11, 13, 15)

//! iq3s4_lut's main-context copy (tests, oracles, the emitter's constant bake).
let IQ3S4_LUT = iq3s4_lut()

//! The Q4_0 nibble read as a codebook (weight = d x (q - 8)): the q4032 planes are iq4nl32's block form, so every per-32 codebook
//! kernel serves them under this table - exact, the levels are int8
def q40_lut() : int8[16] => fixed_array<int8>(-8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7)

//! q40_lut's main-context copy (tests, oracles).
let Q40_LUT = q40_lut()

//! The MXFP4 nibble codebook: the e2m1 values DOUBLED (nibble -> +-{0, .5, 1, 1.5, 2, 3, 4, 6} x 2, OCP MX
//! spec) as int8; the halved E8M0 scale (`e8m0_half`) compensates the doubling.
def mx4_lut() : int8[16] => fixed_array<int8>(0, 1, 2, 3, 4, 6, 8, 12, 0, -1, -2, -3, -4, -6, -8, -12)

//! mx4_lut's main-context copy (tests, oracles).
let MX4_LUT = mx4_lut()

//! The iq3s4 index of one signed odd IQ3_S weight value.
def iq3s4_index(v : int) : int => (v + 15) / 2

//! The 6-bit sub-scale/min pair j of a 12-byte packed Q4_K/Q5_K scale block at sp - the one
//! decode the dots, repacks, dequants and the Q5_K encoder share.
def k4_sc_mn(sp : uint8 const?; j : int64) : int2 {
    unsafe {
        if (j < 4l) {
            return int2(int(sp[j]) & 63, int(sp[j + 4l]) & 63)
        }
        let sc = (int(sp[j + 4l]) & 15) | ((int(sp[j - 4l]) >> 6) << 4)
        let mn = (int(sp[j + 4l]) >> 4) | ((int(sp[j]) >> 6) << 4)
        return int2(sc, mn)
    }
}

//! The IQ3_S grid (upstream's iq3s_grid): entry qs | (qh bit << 8) packs FOUR weight magnitudes as
//! bytes; the per-element sign rides the block's own sign bytes. Per-call local (2 KB on the
//! caller's frame) - the form every worker-run kernel reads; IQ3S_GRID is the main-context copy.
def iq3s_grid() : uint[512] {
    return fixed_array<uint>(
        0x01010101u, 0x01010103u, 0x01010105u, 0x0101010bu, 0x0101010fu, 0x01010301u, 0x01010303u, 0x01010305u,
        0x01010309u, 0x0101030du, 0x01010501u, 0x01010503u, 0x0101050bu, 0x01010707u, 0x01010901u, 0x01010905u,
        0x0101090bu, 0x0101090fu, 0x01010b03u, 0x01010b07u, 0x01010d01u, 0x01010d05u, 0x01010f03u, 0x01010f09u,
        0x01010f0fu, 0x01030101u, 0x01030103u, 0x01030105u, 0x01030109u, 0x01030301u, 0x01030303u, 0x0103030bu,
        0x01030501u, 0x01030507u, 0x0103050fu, 0x01030703u, 0x0103070bu, 0x01030909u, 0x01030d03u, 0x01030d0bu,
        0x01030f05u, 0x01050101u, 0x01050103u, 0x0105010bu, 0x0105010fu, 0x01050301u, 0x01050307u, 0x0105030du,
        0x01050503u, 0x0105050bu, 0x01050701u, 0x01050709u, 0x01050905u, 0x0105090bu, 0x0105090fu, 0x01050b03u,
        0x01050b07u, 0x01050f01u, 0x01050f07u, 0x01070107u, 0x01070303u, 0x0107030bu, 0x01070501u, 0x01070505u,
        0x01070703u, 0x01070707u, 0x0107070du, 0x01070909u, 0x01070b01u, 0x01070b05u, 0x01070d0fu, 0x01070f03u,
        0x01070f0bu, 0x01090101u, 0x01090307u, 0x0109030fu, 0x01090503u, 0x01090509u, 0x01090705u, 0x01090901u,
        0x01090907u, 0x01090b03u, 0x01090f01u, 0x010b0105u, 0x010b0109u, 0x010b0501u, 0x010b0505u, 0x010b050du,
        0x010b0707u, 0x010b0903u, 0x010b090bu, 0x010b090fu, 0x010b0d0du, 0x010b0f07u, 0x010d010du, 0x010d0303u,
        0x010d0307u, 0x010d0703u, 0x010d0b05u, 0x010d0f03u, 0x010f0101u, 0x010f0105u, 0x010f0109u, 0x010f0501u,
        0x010f0505u, 0x010f050du, 0x010f0707u, 0x010f0b01u, 0x010f0b09u, 0x03010101u, 0x03010103u, 0x03010105u,
        0x03010109u, 0x03010301u, 0x03010303u, 0x03010307u, 0x0301030bu, 0x0301030fu, 0x03010501u, 0x03010505u,
        0x03010703u, 0x03010709u, 0x0301070du, 0x03010b09u, 0x03010b0du, 0x03010d03u, 0x03010f05u, 0x03030101u,
        0x03030103u, 0x03030107u, 0x0303010du, 0x03030301u, 0x03030309u, 0x03030503u, 0x03030701u, 0x03030707u,
        0x03030903u, 0x03030b01u, 0x03030b05u, 0x03030f01u, 0x03030f0du, 0x03050101u, 0x03050305u, 0x0305030bu,
        0x0305030fu, 0x03050501u, 0x03050509u, 0x03050705u, 0x03050901u, 0x03050907u, 0x03050b0bu, 0x03050d01u,
        0x03050f05u, 0x03070103u, 0x03070109u, 0x0307010fu, 0x03070301u, 0x03070307u, 0x03070503u, 0x0307050fu,
        0x03070701u, 0x03070709u, 0x03070903u, 0x03070d05u, 0x03070f01u, 0x03090107u, 0x0309010bu, 0x03090305u,
        0x03090309u, 0x03090703u, 0x03090707u, 0x03090905u, 0x0309090du, 0x03090b01u, 0x03090b09u, 0x030b0103u,
        0x030b0301u, 0x030b0307u, 0x030b0503u, 0x030b0701u, 0x030b0705u, 0x030b0b03u, 0x030d0501u, 0x030d0509u,
        0x030d050fu, 0x030d0909u, 0x030d090du, 0x030f0103u, 0x030f0107u, 0x030f0301u, 0x030f0305u, 0x030f0503u,
        0x030f070bu, 0x030f0903u, 0x030f0d05u, 0x030f0f01u, 0x05010101u, 0x05010103u, 0x05010107u, 0x0501010bu,
        0x0501010fu, 0x05010301u, 0x05010305u, 0x05010309u, 0x0501030du, 0x05010503u, 0x05010507u, 0x0501050fu,
        0x05010701u, 0x05010705u, 0x05010903u, 0x05010907u, 0x0501090bu, 0x05010b01u, 0x05010b05u, 0x05010d0fu,
        0x05010f01u, 0x05010f07u, 0x05010f0bu, 0x05030101u, 0x05030105u, 0x05030301u, 0x05030307u, 0x0503030fu,
        0x05030505u, 0x0503050bu, 0x05030703u, 0x05030709u, 0x05030905u, 0x05030b03u, 0x05050103u, 0x05050109u,
        0x0505010fu, 0x05050503u, 0x05050507u, 0x05050701u, 0x0505070fu, 0x05050903u, 0x05050b07u, 0x05050b0fu,
        0x05050f03u, 0x05050f09u, 0x05070101u, 0x05070105u, 0x0507010bu, 0x05070303u, 0x05070505u, 0x05070509u,
        0x05070703u, 0x05070707u, 0x05070905u, 0x05070b01u, 0x05070d0du, 0x05090103u, 0x0509010fu, 0x05090501u,
        0x05090507u, 0x05090705u, 0x0509070bu, 0x05090903u, 0x05090f05u, 0x05090f0bu, 0x050b0109u, 0x050b0303u,
        0x050b0505u, 0x050b070fu, 0x050b0901u, 0x050b0b07u, 0x050b0f01u, 0x050d0101u, 0x050d0105u, 0x050d010fu,
        0x050d0503u, 0x050d0b0bu, 0x050d0d03u, 0x050f010bu, 0x050f0303u, 0x050f050du, 0x050f0701u, 0x050f0907u,
        0x050f0b01u, 0x07010105u, 0x07010303u, 0x07010307u, 0x0701030bu, 0x0701030fu, 0x07010505u, 0x07010703u,
        0x07010707u, 0x0701070bu, 0x07010905u, 0x07010909u, 0x0701090fu, 0x07010b03u, 0x07010d07u, 0x07010f03u,
        0x07030103u, 0x07030107u, 0x0703010bu, 0x07030309u, 0x07030503u, 0x07030507u, 0x07030901u, 0x07030d01u,
        0x07030f05u, 0x07030f0du, 0x07050101u, 0x07050305u, 0x07050501u, 0x07050705u, 0x07050709u, 0x07050b01u,
        0x07070103u, 0x07070301u, 0x07070309u, 0x07070503u, 0x07070507u, 0x0707050fu, 0x07070701u, 0x07070903u,
        0x07070907u, 0x0707090fu, 0x07070b0bu, 0x07070f07u, 0x07090107u, 0x07090303u, 0x0709030du, 0x07090505u,
        0x07090703u, 0x07090b05u, 0x07090d01u, 0x07090d09u, 0x070b0103u, 0x070b0301u, 0x070b0305u, 0x070b050bu,
        0x070b0705u, 0x070b0909u, 0x070b0b0du, 0x070b0f07u, 0x070d030du, 0x070d0903u, 0x070f0103u, 0x070f0107u,
        0x070f0501u, 0x070f0505u, 0x070f070bu, 0x09010101u, 0x09010109u, 0x09010305u, 0x09010501u, 0x09010509u,
        0x0901050fu, 0x09010705u, 0x09010903u, 0x09010b01u, 0x09010f01u, 0x09030105u, 0x0903010fu, 0x09030303u,
        0x09030307u, 0x09030505u, 0x09030701u, 0x0903070bu, 0x09030907u, 0x09030b03u, 0x09030b0bu, 0x09050103u,
        0x09050107u, 0x09050301u, 0x0905030bu, 0x09050503u, 0x09050707u, 0x09050901u, 0x09050b0fu, 0x09050d05u,
        0x09050f01u, 0x09070109u, 0x09070303u, 0x09070307u, 0x09070501u, 0x09070505u, 0x09070703u, 0x0907070bu,
        0x09090101u, 0x09090105u, 0x09090509u, 0x0909070fu, 0x09090901u, 0x09090f03u, 0x090b010bu, 0x090b010fu,
        0x090b0503u, 0x090b0d05u, 0x090d0307u, 0x090d0709u, 0x090d0d01u, 0x090f0301u, 0x090f030bu, 0x090f0701u,
        0x090f0907u, 0x090f0b03u, 0x0b010105u, 0x0b010301u, 0x0b010309u, 0x0b010505u, 0x0b010901u, 0x0b010909u,
        0x0b01090fu, 0x0b010b05u, 0x0b010d0du, 0x0b010f09u, 0x0b030103u, 0x0b030107u, 0x0b03010bu, 0x0b030305u,
        0x0b030503u, 0x0b030705u, 0x0b030f05u, 0x0b050101u, 0x0b050303u, 0x0b050507u, 0x0b050701u, 0x0b05070du,
        0x0b050b07u, 0x0b070105u, 0x0b07010fu, 0x0b070301u, 0x0b07050fu, 0x0b070909u, 0x0b070b03u, 0x0b070d0bu,
        0x0b070f07u, 0x0b090103u, 0x0b090109u, 0x0b090501u, 0x0b090705u, 0x0b09090du, 0x0b0b0305u, 0x0b0b050du,
        0x0b0b0b03u, 0x0b0b0b07u, 0x0b0d0905u, 0x0b0f0105u, 0x0b0f0109u, 0x0b0f0505u, 0x0d010303u, 0x0d010307u,
        0x0d01030bu, 0x0d010703u, 0x0d010707u, 0x0d010d01u, 0x0d030101u, 0x0d030501u, 0x0d03050fu, 0x0d030d09u,
        0x0d050305u, 0x0d050709u, 0x0d050905u, 0x0d050b0bu, 0x0d050d05u, 0x0d050f01u, 0x0d070101u, 0x0d070309u,
        0x0d070503u, 0x0d070901u, 0x0d09050bu, 0x0d090907u, 0x0d090d05u, 0x0d0b0101u, 0x0d0b0107u, 0x0d0b0709u,
        0x0d0b0d01u, 0x0d0d010bu, 0x0d0d0901u, 0x0d0f0303u, 0x0d0f0307u, 0x0f010101u, 0x0f010109u, 0x0f01010fu,
        0x0f010501u, 0x0f010505u, 0x0f01070du, 0x0f010901u, 0x0f010b09u, 0x0f010d05u, 0x0f030105u, 0x0f030303u,
        0x0f030509u, 0x0f030907u, 0x0f03090bu, 0x0f050103u, 0x0f050109u, 0x0f050301u, 0x0f05030du, 0x0f050503u,
        0x0f050701u, 0x0f050b03u, 0x0f070105u, 0x0f070705u, 0x0f07070bu, 0x0f070b07u, 0x0f090103u, 0x0f09010bu,
        0x0f090307u, 0x0f090501u, 0x0f090b01u, 0x0f0b0505u, 0x0f0b0905u, 0x0f0d0105u, 0x0f0d0703u, 0x0f0f0101u)
}

//! Main-context use only (tests, oracles, the emitter's constant bake) - a forked worker reads
//! a module global as zeros; kernels take `iq3s_grid()` instead.
let IQ3S_GRID = iq3s_grid()

//! The IQ3_XXS grid, every byte HALVED (upstream levels are even, 4..62 -> 2..31): one qs byte =
//! FOUR magnitudes; the halved grid + halved stored d absorb upstream's 0.25 exactly, so the fold
//! is iq3s's d x strip x byte. Per-call local (1 KB) - the worker-safe form kernels read.
def iq3xxs_grid() : uint[256] {
    return fixed_array<uint>(
        0x02020202u, 0x0202020au, 0x02020212u, 0x02020606u, 0x0202060eu, 0x0202061fu, 0x02020a02u, 0x02020a0au,
        0x02020e06u, 0x0202120au, 0x02021f0eu, 0x02021f16u, 0x02060206u, 0x0206020eu, 0x02060602u, 0x0206060au,
        0x02060a06u, 0x02060a16u, 0x02060e02u, 0x02060e0au, 0x02061206u, 0x02061612u, 0x02061f02u, 0x020a0202u,
        0x020a020au, 0x020a0212u, 0x020a0606u, 0x020a0a02u, 0x020a0a0au, 0x020a0e06u, 0x020a0e0eu, 0x020a0e1fu,
        0x020a1606u, 0x020a161fu, 0x020a1f16u, 0x020e0206u, 0x020e021fu, 0x020e0602u, 0x020e060au, 0x020e0a16u,
        0x020e1f02u, 0x0212060eu, 0x02120e1fu, 0x02121212u, 0x0212161fu, 0x02121f0eu, 0x02121f16u, 0x02160206u,
        0x0216021fu, 0x02160e0au, 0x0216160au, 0x021a0e16u, 0x021a1a12u, 0x021f0602u, 0x021f0612u, 0x021f061au,
        0x021f120eu, 0x021f1a06u, 0x06020206u, 0x0602020eu, 0x06020602u, 0x0602060au, 0x06020a06u, 0x06020a0eu,
        0x06020e02u, 0x06020e0au, 0x06020e12u, 0x0602121fu, 0x06021602u, 0x06060202u, 0x0606020au, 0x06060606u,
        0x06060a02u, 0x06060a0au, 0x060a0206u, 0x060a020eu, 0x060a0602u, 0x060a060au, 0x060a0a06u, 0x060a0e02u,
        0x060a1f0au, 0x060e0202u, 0x060e020au, 0x060e0a02u, 0x060e0e06u, 0x060e121au, 0x060e1a1au, 0x06120206u,
        0x06120216u, 0x06121602u, 0x06160a02u, 0x06160a12u, 0x0616121au, 0x06161f06u, 0x061a0216u, 0x061f0a0au,
        0x061f1202u, 0x0a020202u, 0x0a02020au, 0x0a020606u, 0x0a02060eu, 0x0a020a02u, 0x0a020a0au, 0x0a020a1au,
        0x0a020e06u, 0x0a02120au, 0x0a060206u, 0x0a06020eu, 0x0a060216u, 0x0a060602u, 0x0a06060au, 0x0a060a06u,
        0x0a060e02u, 0x0a061a0eu, 0x0a061a1fu, 0x0a061f02u, 0x0a0a0202u, 0x0a0a020au, 0x0a0a0606u, 0x0a0a061fu,
        0x0a0a0a02u, 0x0a0a0a0au, 0x0a0a0e1fu, 0x0a0a1202u, 0x0a0a1616u, 0x0a0e0206u, 0x0a0e0602u, 0x0a0e0612u,
        0x0a0e1f02u, 0x0a0e1f12u, 0x0a120e16u, 0x0a12160eu, 0x0a16020eu, 0x0a160a1fu, 0x0a161206u, 0x0a161f12u,
        0x0a1f0206u, 0x0a1f020eu, 0x0a1f061au, 0x0a1f1216u, 0x0e020206u, 0x0e020602u, 0x0e02060au, 0x0e020a06u,
        0x0e020a0eu, 0x0e021602u, 0x0e021a16u, 0x0e021f0au, 0x0e060202u, 0x0e06020au, 0x0e060a02u, 0x0e060e06u,
        0x0e061212u, 0x0e06121au, 0x0e0a0206u, 0x0e0a020eu, 0x0e0a0602u, 0x0e0a0a16u, 0x0e0a160au, 0x0e0a1f0au,
        0x0e0e0606u, 0x0e0e0e0eu, 0x0e120e02u, 0x0e12121fu, 0x0e121f0au, 0x0e160202u, 0x0e16021au, 0x0e160a0au,
        0x0e161616u, 0x0e1a0612u, 0x0e1a0e1au, 0x0e1a1a0eu, 0x0e1f0e0eu, 0x0e1f1a02u, 0x12020212u, 0x1202061fu,
        0x12020e16u, 0x12020e1fu, 0x1202160eu, 0x1202161fu, 0x12061f12u, 0x120a0a02u, 0x120a0e1fu, 0x120a1202u,
        0x120a1a02u, 0x120a1a1au, 0x120e021fu, 0x120e1216u, 0x12120212u, 0x12121606u, 0x12121a12u, 0x12160a16u,
        0x1216120eu, 0x12161f02u, 0x121f0216u, 0x121f0602u, 0x121f060au, 0x121f0e02u, 0x1602060au, 0x16021206u,
        0x16021f02u, 0x16060202u, 0x1606021au, 0x16060a1au, 0x16061616u, 0x160a0612u, 0x160a0e0au, 0x160a1f0au,
        0x160e020au, 0x160e160eu, 0x16120602u, 0x16120a0eu, 0x16120a1fu, 0x16121f0au, 0x1616020au, 0x16160e06u,
        0x161a1602u, 0x161f0a12u, 0x161f120au, 0x1a020a12u, 0x1a021212u, 0x1a02121au, 0x1a021a12u, 0x1a060a06u,
        0x1a061a06u, 0x1a0a061fu, 0x1a0a1a12u, 0x1a0e0e02u, 0x1a0e0e1au, 0x1a121212u, 0x1a160216u, 0x1a16160au,
        0x1a1a0e0eu, 0x1a1f020eu, 0x1a1f0a06u, 0x1f02020eu, 0x1f020216u, 0x1f02021fu, 0x1f020602u, 0x1f020e0au,
        0x1f02160au, 0x1f060a1au, 0x1f061202u, 0x1f0a060au, 0x1f0a1216u, 0x1f0a160au, 0x1f0e0202u, 0x1f0e0616u,
        0x1f0e0e0eu, 0x1f0e1a02u, 0x1f120a06u, 0x1f121206u, 0x1f160202u, 0x1f16020au, 0x1f160a12u, 0x1f1a0e02u)
}

//! Main-context copy of the halved IQ3_XXS grid (tests, oracles, emitter bakes).
let IQ3XXS_GRID = iq3xxs_grid()

//! upstream's ksigns_iq2xs: 7-bit sign index -> 8 sign bits (bit 7 = even parity). Shared by the
//! IQ3_XXS and IQ2 families. Per-call local - the worker-safe form.
def ksigns_iq2xs() : uint8[128] {
    return fixed_array<uint8>(
        0x00, 0x81, 0x82, 0x03, 0x84, 0x05, 0x06, 0x87, 0x88, 0x09, 0x0a, 0x8b, 0x0c, 0x8d, 0x8e, 0x0f,
        0x90, 0x11, 0x12, 0x93, 0x14, 0x95, 0x96, 0x17, 0x18, 0x99, 0x9a, 0x1b, 0x9c, 0x1d, 0x1e, 0x9f,
        0xa0, 0x21, 0x22, 0xa3, 0x24, 0xa5, 0xa6, 0x27, 0x28, 0xa9, 0xaa, 0x2b, 0xac, 0x2d, 0x2e, 0xaf,
        0x30, 0xb1, 0xb2, 0x33, 0xb4, 0x35, 0x36, 0xb7, 0xb8, 0x39, 0x3a, 0xbb, 0x3c, 0xbd, 0xbe, 0x3f,
        0xc0, 0x41, 0x42, 0xc3, 0x44, 0xc5, 0xc6, 0x47, 0x48, 0xc9, 0xca, 0x4b, 0xcc, 0x4d, 0x4e, 0xcf,
        0x50, 0xd1, 0xd2, 0x53, 0xd4, 0x55, 0x56, 0xd7, 0xd8, 0x59, 0x5a, 0xdb, 0x5c, 0xdd, 0xde, 0x5f,
        0x60, 0xe1, 0xe2, 0x63, 0xe4, 0x65, 0x66, 0xe7, 0xe8, 0x69, 0x6a, 0xeb, 0x6c, 0xed, 0xee, 0x6f,
        0xf0, 0x71, 0x72, 0xf3, 0x74, 0xf5, 0xf6, 0x77, 0x78, 0xf9, 0xfa, 0x7b, 0xfc, 0x7d, 0x7e, 0xff)
}

//! Main-context copy of ksigns_iq2xs.
let KSIGNS_IQ2XS = ksigns_iq2xs()

//! The signed-grid octet the grid formats share (iq3s, iq3xxs, iq2s, iq2xs, iq2xxs): two grid
//! words carry eight magnitudes and `sgn`'s bit j flips element j. Generic in the operand.
def grid_octet_dot(g1, g2 : uint; sgn : int; a : auto(TA) const?; ab : int64) : int {
    var acc = 0
    unsafe {
        for (j in range64(4l)) {
            let w1 = int((g1 >> uint(8l * j)) & 0xFFu)
            let w2 = int((g2 >> uint(8l * j)) & 0xFFu)
            acc += (((sgn >> int(j)) & 1) != 0 ? -w1 : w1) * int(a[ab + j])
            acc += (((sgn >> int(4l + j)) & 1) != 0 ? -w2 : w2) * int(a[ab + 4l + j])
        }
    }
    return acc
}

//! The octet dequantized: dst[at .. at + 8) = db * magnitude * sign, ggml's own float order.
def store_grid_pair(var dst : auto(TD); at : int64; db : float; g1, g2 : uint; sgn : int) {
    unsafe {
        for (j in range64(4l)) {
            dst[at + j] = db * float((g1 >> uint(8l * j)) & 0xFFu) * (((sgn >> int(j)) & 1) != 0 ? -1.0 : 1.0)
            dst[at + 4l + j] = db * float((g2 >> uint(8l * j)) & 0xFFu) * (((sgn >> int(4l + j)) & 1) != 0 ? -1.0 : 1.0)
        }
    }
}

//! The 16 byte-select masks the panel unpacks negate through: mask n selects the bytes whose
//! sign bit is set in nibble n. Built per call, never a module global (a forked worker reads a
//! `let` global as zero).
def sign_masks_iq() : uint[16] => fixed_array<uint>(0x00000000u, 0x000000FFu, 0x0000FF00u, 0x0000FFFFu, 0x00FF0000u, 0x00FF00FFu, 0x00FFFF00u, 0x00FFFFFFu, 0xFF000000u, 0xFF0000FFu, 0xFF00FF00u, 0xFF00FFFFu, 0xFFFF0000u, 0xFFFF00FFu, 0xFFFFFF00u, 0xFFFFFFFFu)

//! The octet into the byte-expanded signed panel at superblock base `ob`: each word's four
//! magnitudes negated byte-wise through the sign nibble's mask (odd 1..15, no cross-byte carry),
//! one u32 store each; element l*8 + half*4 + t of (blk, row r) lands in the lo half below 16.
def store_grid_panel_pair(var dst : auto(TD)?; ob, blk, l, r, mr : int64; g1, g2 : uint; sgn : int; smask : uint[16]) {
    unsafe {
        let m1 = smask[sgn & 15]
        let m2 = smask[(sgn >> 4) & 15]
        let e1 = l * 8l
        let e2 = l * 8l + 4l
        let d1 = (e1 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e1 % 16l) / 4l) * mr + r) * 4l
        let d2 = (e2 < 16l ? 0l : 128l * mr) + ((blk * 4l + (e2 % 16l) / 4l) * mr + r) * 4l
        var wp1 = reinterpret<uint?>(dst + (ob + d1))
        var wp2 = reinterpret<uint?>(dst + (ob + d2))
        wp1[0] = (g1 ^ m1) + (m1 & 0x01010101u)
        wp2[0] = (g2 ^ m2) + (m2 & 0x01010101u)
    }
}

//! Halve an f16 bit pattern exactly (exponent decrement; subnormal results truncate-halve).
//! The IQ3_XXS transcode folds upstream's 0.25 into the stored d (one half) and the grid (the other).
def f16_half_bits(u : uint) : uint {
    let e = (u >> 10u) & 0x1Fu
    if (e >= 2u) {
        return u - 0x400u
    }
    if (e == 1u) {
        return (u & 0x8000u) | ((0x400u | (u & 0x3FFu)) >> 1u)
    }
    return (u & 0x8000u) | ((u & 0x3FFu) >> 1u)
}


//! The IQ2_S grid (upstream's iq2s_grid, 1024 u64 entries as low/high uint pairs): a 10-bit index
//! (qs byte | qh 2 bits) selects EIGHT magnitudes ({8, 25, 43}); signs ride the block's own
//! sign bytes. Per-call local (8 KB) - the worker-safe form; IQ2S_GRID is the main-context copy.
def iq2s_grid2() : uint[2048] {   // nolint:STYLE038 - a 2048-word data table, not splittable
    return fixed_array<uint>(
        0x08080808, 0x08080808, 0x0808082b, 0x08080808, 0x08081919, 0x08080808, 0x08082b08, 0x08080808,
        0x08082b2b, 0x08080808, 0x08190819, 0x08080808, 0x08191908, 0x08080808, 0x0819192b, 0x08080808,
        0x08192b19, 0x08080808, 0x082b0808, 0x08080808, 0x082b082b, 0x08080808, 0x082b1919, 0x08080808,
        0x082b2b08, 0x08080808, 0x19080819, 0x08080808, 0x19081908, 0x08080808, 0x1908192b, 0x08080808,
        0x19082b19, 0x08080808, 0x19190808, 0x08080808, 0x1919082b, 0x08080808, 0x19191919, 0x08080808,
        0x19192b08, 0x08080808, 0x192b0819, 0x08080808, 0x192b1908, 0x08080808, 0x192b192b, 0x08080808,
        0x192b2b19, 0x08080808, 0x2b080808, 0x08080808, 0x2b08082b, 0x08080808, 0x2b081919, 0x08080808,
        0x2b082b08, 0x08080808, 0x2b190819, 0x08080808, 0x2b191908, 0x08080808, 0x2b2b0808, 0x08080808,
        0x2b2b1919, 0x08080808, 0x2b2b2b2b, 0x08080808, 0x08080819, 0x08080819, 0x08081908, 0x08080819,
        0x0808192b, 0x08080819, 0x08082b19, 0x08080819, 0x08190808, 0x08080819, 0x0819082b, 0x08080819,
        0x08191919, 0x08080819, 0x08192b08, 0x08080819, 0x082b0819, 0x08080819, 0x082b1908, 0x08080819,
        0x19080808, 0x08080819, 0x1908082b, 0x08080819, 0x19081919, 0x08080819, 0x19082b08, 0x08080819,
        0x19190819, 0x08080819, 0x19191908, 0x08080819, 0x1919192b, 0x08080819, 0x19192b19, 0x08080819,
        0x192b0808, 0x08080819, 0x192b1919, 0x08080819, 0x192b2b08, 0x08080819, 0x2b080819, 0x08080819,
        0x2b081908, 0x08080819, 0x2b190808, 0x08080819, 0x2b19082b, 0x08080819, 0x2b191919, 0x08080819,
        0x2b2b0819, 0x08080819, 0x2b2b1908, 0x08080819, 0x08080808, 0x0808082b, 0x0808082b, 0x0808082b,
        0x08081919, 0x0808082b, 0x08082b08, 0x0808082b, 0x08190819, 0x0808082b, 0x08191908, 0x0808082b,
        0x082b0808, 0x0808082b, 0x082b2b2b, 0x0808082b, 0x19080819, 0x0808082b, 0x19081908, 0x0808082b,
        0x1908192b, 0x0808082b, 0x19082b19, 0x0808082b, 0x19190808, 0x0808082b, 0x19191919, 0x0808082b,
        0x2b080808, 0x0808082b, 0x2b081919, 0x0808082b, 0x2b082b2b, 0x0808082b, 0x2b191908, 0x0808082b,
        0x2b2b082b, 0x0808082b, 0x08080819, 0x08081908, 0x08081908, 0x08081908, 0x0808192b, 0x08081908,
        0x08082b19, 0x08081908, 0x08190808, 0x08081908, 0x0819082b, 0x08081908, 0x08191919, 0x08081908,
        0x08192b08, 0x08081908, 0x082b0819, 0x08081908, 0x082b1908, 0x08081908, 0x082b192b, 0x08081908,
        0x082b2b19, 0x08081908, 0x19080808, 0x08081908, 0x1908082b, 0x08081908, 0x19081919, 0x08081908,
        0x19082b08, 0x08081908, 0x19082b2b, 0x08081908, 0x19190819, 0x08081908, 0x19191908, 0x08081908,
        0x1919192b, 0x08081908, 0x19192b19, 0x08081908, 0x192b0808, 0x08081908, 0x192b082b, 0x08081908,
        0x192b1919, 0x08081908, 0x2b080819, 0x08081908, 0x2b081908, 0x08081908, 0x2b08192b, 0x08081908,
        0x2b082b19, 0x08081908, 0x2b190808, 0x08081908, 0x2b191919, 0x08081908, 0x2b192b08, 0x08081908,
        0x2b2b0819, 0x08081908, 0x2b2b1908, 0x08081908, 0x08080808, 0x08081919, 0x0808082b, 0x08081919,
        0x08081919, 0x08081919, 0x08082b08, 0x08081919, 0x08082b2b, 0x08081919, 0x08190819, 0x08081919,
        0x08191908, 0x08081919, 0x0819192b, 0x08081919, 0x08192b19, 0x08081919, 0x082b0808, 0x08081919,
        0x082b1919, 0x08081919, 0x082b2b08, 0x08081919, 0x19080819, 0x08081919, 0x19081908, 0x08081919,
        0x1908192b, 0x08081919, 0x19082b19, 0x08081919, 0x19190808, 0x08081919, 0x1919082b, 0x08081919,
        0x19191919, 0x08081919, 0x19192b08, 0x08081919, 0x192b0819, 0x08081919, 0x192b1908, 0x08081919,
        0x2b080808, 0x08081919, 0x2b08082b, 0x08081919, 0x2b081919, 0x08081919, 0x2b082b08, 0x08081919,
        0x2b190819, 0x08081919, 0x2b191908, 0x08081919, 0x2b2b0808, 0x08081919, 0x08080819, 0x0808192b,
        0x08081908, 0x0808192b, 0x0808192b, 0x0808192b, 0x08082b19, 0x0808192b, 0x08190808, 0x0808192b,
        0x08191919, 0x0808192b, 0x19080808, 0x0808192b, 0x19081919, 0x0808192b, 0x19082b08, 0x0808192b,
        0x19190819, 0x0808192b, 0x19191908, 0x0808192b, 0x192b0808, 0x0808192b, 0x2b080819, 0x0808192b,
        0x2b081908, 0x0808192b, 0x2b190808, 0x0808192b, 0x08080808, 0x08082b08, 0x0808082b, 0x08082b08,
        0x08081919, 0x08082b08, 0x08082b08, 0x08082b08, 0x08190819, 0x08082b08, 0x08191908, 0x08082b08,
        0x0819192b, 0x08082b08, 0x08192b19, 0x08082b08, 0x082b0808, 0x08082b08, 0x082b1919, 0x08082b08,
        0x082b2b2b, 0x08082b08, 0x19080819, 0x08082b08, 0x19081908, 0x08082b08, 0x1908192b, 0x08082b08,
        0x19082b19, 0x08082b08, 0x19190808, 0x08082b08, 0x1919082b, 0x08082b08, 0x19191919, 0x08082b08,
        0x19192b08, 0x08082b08, 0x192b0819, 0x08082b08, 0x192b1908, 0x08082b08, 0x2b080808, 0x08082b08,
        0x2b081919, 0x08082b08, 0x2b191908, 0x08082b08, 0x2b2b2b2b, 0x08082b08, 0x08080819, 0x08082b19,
        0x08081908, 0x08082b19, 0x08190808, 0x08082b19, 0x0819082b, 0x08082b19, 0x08191919, 0x08082b19,
        0x08192b08, 0x08082b19, 0x082b0819, 0x08082b19, 0x19080808, 0x08082b19, 0x19081919, 0x08082b19,
        0x19082b08, 0x08082b19, 0x19190819, 0x08082b19, 0x19191908, 0x08082b19, 0x192b0808, 0x08082b19,
        0x2b080819, 0x08082b19, 0x2b190808, 0x08082b19, 0x08080808, 0x08082b2b, 0x08190819, 0x08082b2b,
        0x08191908, 0x08082b2b, 0x082b082b, 0x08082b2b, 0x082b2b08, 0x08082b2b, 0x082b2b2b, 0x08082b2b,
        0x19190808, 0x08082b2b, 0x2b192b19, 0x08082b2b, 0x08080819, 0x08190808, 0x08081908, 0x08190808,
        0x0808192b, 0x08190808, 0x08082b19, 0x08190808, 0x08190808, 0x08190808, 0x0819082b, 0x08190808,
        0x08191919, 0x08190808, 0x08192b08, 0x08190808, 0x082b0819, 0x08190808, 0x082b1908, 0x08190808,
        0x082b192b, 0x08190808, 0x19080808, 0x08190808, 0x1908082b, 0x08190808, 0x19081919, 0x08190808,
        0x19082b08, 0x08190808, 0x19190819, 0x08190808, 0x19191908, 0x08190808, 0x1919192b, 0x08190808,
        0x19192b19, 0x08190808, 0x192b0808, 0x08190808, 0x192b082b, 0x08190808, 0x192b1919, 0x08190808,
        0x192b2b08, 0x08190808, 0x2b080819, 0x08190808, 0x2b081908, 0x08190808, 0x2b08192b, 0x08190808,
        0x2b190808, 0x08190808, 0x2b191919, 0x08190808, 0x2b192b08, 0x08190808, 0x2b2b0819, 0x08190808,
        0x2b2b1908, 0x08190808, 0x08080808, 0x08190819, 0x0808082b, 0x08190819, 0x08081919, 0x08190819,
        0x08082b08, 0x08190819, 0x08082b2b, 0x08190819, 0x08190819, 0x08190819, 0x08191908, 0x08190819,
        0x0819192b, 0x08190819, 0x08192b19, 0x08190819, 0x082b0808, 0x08190819, 0x082b082b, 0x08190819,
        0x082b1919, 0x08190819, 0x082b2b08, 0x08190819, 0x19080819, 0x08190819, 0x19081908, 0x08190819,
        0x1908192b, 0x08190819, 0x19082b19, 0x08190819, 0x19190808, 0x08190819, 0x1919082b, 0x08190819,
        0x19191919, 0x08190819, 0x19192b08, 0x08190819, 0x192b0819, 0x08190819, 0x192b1908, 0x08190819,
        0x2b080808, 0x08190819, 0x2b08082b, 0x08190819, 0x2b081919, 0x08190819, 0x2b082b08, 0x08190819,
        0x2b190819, 0x08190819, 0x2b191908, 0x08190819, 0x08080819, 0x0819082b, 0x08081908, 0x0819082b,
        0x08082b19, 0x0819082b, 0x08190808, 0x0819082b, 0x08191919, 0x0819082b, 0x082b0819, 0x0819082b,
        0x082b1908, 0x0819082b, 0x19080808, 0x0819082b, 0x19081919, 0x0819082b, 0x19190819, 0x0819082b,
        0x19191908, 0x0819082b, 0x2b080819, 0x0819082b, 0x2b081908, 0x0819082b, 0x2b190808, 0x0819082b,
        0x08080808, 0x08191908, 0x0808082b, 0x08191908, 0x08081919, 0x08191908, 0x08082b08, 0x08191908,
        0x08190819, 0x08191908, 0x08191908, 0x08191908, 0x0819192b, 0x08191908, 0x08192b19, 0x08191908,
        0x082b0808, 0x08191908, 0x082b1919, 0x08191908, 0x082b2b08, 0x08191908, 0x19080819, 0x08191908,
        0x19081908, 0x08191908, 0x1908192b, 0x08191908, 0x19082b19, 0x08191908, 0x19190808, 0x08191908,
        0x1919082b, 0x08191908, 0x19191919, 0x08191908, 0x19192b08, 0x08191908, 0x192b0819, 0x08191908,
        0x192b1908, 0x08191908, 0x2b080808, 0x08191908, 0x2b08082b, 0x08191908, 0x2b081919, 0x08191908,
        0x2b082b08, 0x08191908, 0x2b190819, 0x08191908, 0x2b191908, 0x08191908, 0x2b2b0808, 0x08191908,
        0x08080819, 0x08191919, 0x08081908, 0x08191919, 0x0808192b, 0x08191919, 0x08082b19, 0x08191919,
        0x08190808, 0x08191919, 0x0819082b, 0x08191919, 0x08191919, 0x08191919, 0x08192b08, 0x08191919,
        0x082b0819, 0x08191919, 0x082b1908, 0x08191919, 0x19080808, 0x08191919, 0x1908082b, 0x08191919,
        0x19081919, 0x08191919, 0x19082b08, 0x08191919, 0x19190819, 0x08191919, 0x19191908, 0x08191919,
        0x192b0808, 0x08191919, 0x2b080819, 0x08191919, 0x2b081908, 0x08191919, 0x2b190808, 0x08191919,
        0x08080808, 0x0819192b, 0x08081919, 0x0819192b, 0x08082b08, 0x0819192b, 0x08190819, 0x0819192b,
        0x08191908, 0x0819192b, 0x082b0808, 0x0819192b, 0x19080819, 0x0819192b, 0x19081908, 0x0819192b,
        0x19190808, 0x0819192b, 0x2b080808, 0x0819192b, 0x2b2b2b2b, 0x0819192b, 0x08080819, 0x08192b08,
        0x08081908, 0x08192b08, 0x0808192b, 0x08192b08, 0x08082b19, 0x08192b08, 0x08190808, 0x08192b08,
        0x08191919, 0x08192b08, 0x08192b08, 0x08192b08, 0x082b0819, 0x08192b08, 0x19080808, 0x08192b08,
        0x1908082b, 0x08192b08, 0x19081919, 0x08192b08, 0x19082b08, 0x08192b08, 0x19190819, 0x08192b08,
        0x19191908, 0x08192b08, 0x192b0808, 0x08192b08, 0x2b080819, 0x08192b08, 0x2b081908, 0x08192b08,
        0x08080808, 0x08192b19, 0x0808082b, 0x08192b19, 0x08081919, 0x08192b19, 0x08082b08, 0x08192b19,
        0x08190819, 0x08192b19, 0x08191908, 0x08192b19, 0x082b0808, 0x08192b19, 0x19080819, 0x08192b19,
        0x19081908, 0x08192b19, 0x19190808, 0x08192b19, 0x192b2b19, 0x08192b19, 0x2b2b082b, 0x08192b19,
        0x08081908, 0x08192b2b, 0x08190808, 0x08192b2b, 0x19080808, 0x08192b2b, 0x1919192b, 0x08192b2b,
        0x08080808, 0x082b0808, 0x0808082b, 0x082b0808, 0x08081919, 0x082b0808, 0x08082b08, 0x082b0808,
        0x08190819, 0x082b0808, 0x08191908, 0x082b0808, 0x0819192b, 0x082b0808, 0x08192b19, 0x082b0808,
        0x082b0808, 0x082b0808, 0x082b1919, 0x082b0808, 0x082b2b2b, 0x082b0808, 0x19080819, 0x082b0808,
        0x19081908, 0x082b0808, 0x19190808, 0x082b0808, 0x1919082b, 0x082b0808, 0x19191919, 0x082b0808,
        0x192b1908, 0x082b0808, 0x2b080808, 0x082b0808, 0x2b082b2b, 0x082b0808, 0x2b191908, 0x082b0808,
        0x2b2b2b2b, 0x082b0808, 0x08080819, 0x082b0819, 0x08081908, 0x082b0819, 0x08190808, 0x082b0819,
        0x0819082b, 0x082b0819, 0x08191919, 0x082b0819, 0x082b0819, 0x082b0819, 0x19080808, 0x082b0819,
        0x1908082b, 0x082b0819, 0x19081919, 0x082b0819, 0x19190819, 0x082b0819, 0x19191908, 0x082b0819,
        0x192b0808, 0x082b0819, 0x2b080819, 0x082b0819, 0x2b081908, 0x082b0819, 0x2b190808, 0x082b0819,
        0x08080808, 0x082b082b, 0x08082b2b, 0x082b082b, 0x082b082b, 0x082b082b, 0x082b2b08, 0x082b082b,
        0x082b2b2b, 0x082b082b, 0x19081908, 0x082b082b, 0x19190808, 0x082b082b, 0x2b082b08, 0x082b082b,
        0x2b082b2b, 0x082b082b, 0x2b2b2b08, 0x082b082b, 0x08080819, 0x082b1908, 0x08081908, 0x082b1908,
        0x0808192b, 0x082b1908, 0x08082b19, 0x082b1908, 0x08190808, 0x082b1908, 0x08191919, 0x082b1908,
        0x08192b08, 0x082b1908, 0x082b0819, 0x082b1908, 0x082b1908, 0x082b1908, 0x19080808, 0x082b1908,
        0x1908082b, 0x082b1908, 0x19081919, 0x082b1908, 0x19082b08, 0x082b1908, 0x19190819, 0x082b1908,
        0x19191908, 0x082b1908, 0x192b0808, 0x082b1908, 0x2b080819, 0x082b1908, 0x2b081908, 0x082b1908,
        0x2b190808, 0x082b1908, 0x08080808, 0x082b1919, 0x08081919, 0x082b1919, 0x08082b08, 0x082b1919,
        0x08190819, 0x082b1919, 0x08191908, 0x082b1919, 0x082b0808, 0x082b1919, 0x19080819, 0x082b1919,
        0x19081908, 0x082b1919, 0x19190808, 0x082b1919, 0x192b192b, 0x082b1919, 0x2b080808, 0x082b1919,
        0x08080819, 0x082b192b, 0x08081908, 0x082b192b, 0x08190808, 0x082b192b, 0x19080808, 0x082b192b,
        0x19192b19, 0x082b192b, 0x08080808, 0x082b2b08, 0x08081919, 0x082b2b08, 0x08190819, 0x082b2b08,
        0x08191908, 0x082b2b08, 0x19080819, 0x082b2b08, 0x19081908, 0x082b2b08, 0x19190808, 0x082b2b08,
        0x2b082b2b, 0x082b2b08, 0x2b2b2b2b, 0x082b2b08, 0x08080819, 0x082b2b19, 0x08081908, 0x082b2b19,
        0x08190808, 0x082b2b19, 0x2b191919, 0x082b2b19, 0x08082b2b, 0x082b2b2b, 0x082b082b, 0x082b2b2b,
        0x192b1908, 0x082b2b2b, 0x2b082b08, 0x082b2b2b, 0x2b082b2b, 0x082b2b2b, 0x08080819, 0x19080808,
        0x08081908, 0x19080808, 0x0808192b, 0x19080808, 0x08082b19, 0x19080808, 0x08190808, 0x19080808,
        0x0819082b, 0x19080808, 0x08191919, 0x19080808, 0x08192b08, 0x19080808, 0x08192b2b, 0x19080808,
        0x082b0819, 0x19080808, 0x082b1908, 0x19080808, 0x082b192b, 0x19080808, 0x19080808, 0x19080808,
        0x1908082b, 0x19080808, 0x19081919, 0x19080808, 0x19082b08, 0x19080808, 0x19082b2b, 0x19080808,
        0x19190819, 0x19080808, 0x19191908, 0x19080808, 0x1919192b, 0x19080808, 0x19192b19, 0x19080808,
        0x192b0808, 0x19080808, 0x192b082b, 0x19080808, 0x192b1919, 0x19080808, 0x2b080819, 0x19080808,
        0x2b081908, 0x19080808, 0x2b190808, 0x19080808, 0x2b191919, 0x19080808, 0x2b192b08, 0x19080808,
        0x2b2b0819, 0x19080808, 0x2b2b1908, 0x19080808, 0x08080808, 0x19080819, 0x0808082b, 0x19080819,
        0x08081919, 0x19080819, 0x08082b08, 0x19080819, 0x08190819, 0x19080819, 0x08191908, 0x19080819,
        0x0819192b, 0x19080819, 0x08192b19, 0x19080819, 0x082b0808, 0x19080819, 0x082b082b, 0x19080819,
        0x082b1919, 0x19080819, 0x19080819, 0x19080819, 0x19081908, 0x19080819, 0x1908192b, 0x19080819,
        0x19082b19, 0x19080819, 0x19190808, 0x19080819, 0x1919082b, 0x19080819, 0x19191919, 0x19080819,
        0x19192b08, 0x19080819, 0x192b0819, 0x19080819, 0x192b1908, 0x19080819, 0x2b080808, 0x19080819,
        0x2b08082b, 0x19080819, 0x2b081919, 0x19080819, 0x2b082b08, 0x19080819, 0x2b190819, 0x19080819,
        0x2b191908, 0x19080819, 0x2b2b0808, 0x19080819, 0x08080819, 0x1908082b, 0x08081908, 0x1908082b,
        0x08190808, 0x1908082b, 0x0819082b, 0x1908082b, 0x08191919, 0x1908082b, 0x08192b08, 0x1908082b,
        0x082b1908, 0x1908082b, 0x19080808, 0x1908082b, 0x19081919, 0x1908082b, 0x19082b08, 0x1908082b,
        0x19190819, 0x1908082b, 0x19191908, 0x1908082b, 0x192b0808, 0x1908082b, 0x2b080819, 0x1908082b,
        0x2b081908, 0x1908082b, 0x08080808, 0x19081908, 0x0808082b, 0x19081908, 0x08081919, 0x19081908,
        0x08082b08, 0x19081908, 0x08082b2b, 0x19081908, 0x08190819, 0x19081908, 0x08191908, 0x19081908,
        0x0819192b, 0x19081908, 0x08192b19, 0x19081908, 0x082b0808, 0x19081908, 0x082b082b, 0x19081908,
        0x082b1919, 0x19081908, 0x082b2b08, 0x19081908, 0x19080819, 0x19081908, 0x19081908, 0x19081908,
        0x1908192b, 0x19081908, 0x19082b19, 0x19081908, 0x19190808, 0x19081908, 0x1919082b, 0x19081908,
        0x19191919, 0x19081908, 0x19192b08, 0x19081908, 0x192b0819, 0x19081908, 0x192b1908, 0x19081908,
        0x2b080808, 0x19081908, 0x2b08082b, 0x19081908, 0x2b081919, 0x19081908, 0x2b082b08, 0x19081908,
        0x2b190819, 0x19081908, 0x2b191908, 0x19081908, 0x2b2b0808, 0x19081908, 0x08080819, 0x19081919,
        0x08081908, 0x19081919, 0x0808192b, 0x19081919, 0x08082b19, 0x19081919, 0x08190808, 0x19081919,
        0x0819082b, 0x19081919, 0x08191919, 0x19081919, 0x08192b08, 0x19081919, 0x082b0819, 0x19081919,
        0x082b1908, 0x19081919, 0x19080808, 0x19081919, 0x1908082b, 0x19081919, 0x19081919, 0x19081919,
        0x19082b08, 0x19081919, 0x19190819, 0x19081919, 0x19191908, 0x19081919, 0x192b0808, 0x19081919,
        0x192b2b2b, 0x19081919, 0x2b080819, 0x19081919, 0x2b081908, 0x19081919, 0x2b190808, 0x19081919,
        0x08080808, 0x1908192b, 0x0808082b, 0x1908192b, 0x08081919, 0x1908192b, 0x08082b08, 0x1908192b,
        0x08190819, 0x1908192b, 0x08191908, 0x1908192b, 0x082b0808, 0x1908192b, 0x19080819, 0x1908192b,
        0x19081908, 0x1908192b, 0x19190808, 0x1908192b, 0x2b080808, 0x1908192b, 0x2b2b1919, 0x1908192b,
        0x08080819, 0x19082b08, 0x08081908, 0x19082b08, 0x08082b19, 0x19082b08, 0x08190808, 0x19082b08,
        0x0819082b, 0x19082b08, 0x08191919, 0x19082b08, 0x08192b08, 0x19082b08, 0x082b0819, 0x19082b08,
        0x082b1908, 0x19082b08, 0x19080808, 0x19082b08, 0x1908082b, 0x19082b08, 0x19081919, 0x19082b08,
        0x19082b08, 0x19082b08, 0x19190819, 0x19082b08, 0x19191908, 0x19082b08, 0x192b0808, 0x19082b08,
        0x2b081908, 0x19082b08, 0x2b190808, 0x19082b08, 0x08080808, 0x19082b19, 0x0808082b, 0x19082b19,
        0x08081919, 0x19082b19, 0x08082b08, 0x19082b19, 0x08190819, 0x19082b19, 0x08191908, 0x19082b19,
        0x082b0808, 0x19082b19, 0x19080819, 0x19082b19, 0x19081908, 0x19082b19, 0x19190808, 0x19082b19,
        0x2b080808, 0x19082b19, 0x2b19192b, 0x19082b19, 0x08080819, 0x19082b2b, 0x08081908, 0x19082b2b,
        0x08190808, 0x19082b2b, 0x19080808, 0x19082b2b, 0x08080808, 0x19190808, 0x0808082b, 0x19190808,
        0x08081919, 0x19190808, 0x08082b08, 0x19190808, 0x08190819, 0x19190808, 0x08191908, 0x19190808,
        0x0819192b, 0x19190808, 0x08192b19, 0x19190808, 0x082b0808, 0x19190808, 0x082b082b, 0x19190808,
        0x082b1919, 0x19190808, 0x082b2b08, 0x19190808, 0x19080819, 0x19190808, 0x19081908, 0x19190808,
        0x1908192b, 0x19190808, 0x19082b19, 0x19190808, 0x19190808, 0x19190808, 0x1919082b, 0x19190808,
        0x19191919, 0x19190808, 0x19192b08, 0x19190808, 0x192b0819, 0x19190808, 0x192b1908, 0x19190808,
        0x2b080808, 0x19190808, 0x2b08082b, 0x19190808, 0x2b081919, 0x19190808, 0x2b082b08, 0x19190808,
        0x2b190819, 0x19190808, 0x2b191908, 0x19190808, 0x08080819, 0x19190819, 0x08081908, 0x19190819,
        0x0808192b, 0x19190819, 0x08082b19, 0x19190819, 0x08190808, 0x19190819, 0x0819082b, 0x19190819,
        0x08191919, 0x19190819, 0x08192b08, 0x19190819, 0x082b0819, 0x19190819, 0x082b1908, 0x19190819,
        0x19080808, 0x19190819, 0x1908082b, 0x19190819, 0x19081919, 0x19190819, 0x19082b08, 0x19190819,
        0x19190819, 0x19190819, 0x19191908, 0x19190819, 0x192b0808, 0x19190819, 0x2b080819, 0x19190819,
        0x2b081908, 0x19190819, 0x2b190808, 0x19190819, 0x08080808, 0x1919082b, 0x08081919, 0x1919082b,
        0x08082b08, 0x1919082b, 0x08190819, 0x1919082b, 0x08191908, 0x1919082b, 0x082b0808, 0x1919082b,
        0x19080819, 0x1919082b, 0x19081908, 0x1919082b, 0x19190808, 0x1919082b, 0x192b2b19, 0x1919082b,
        0x2b080808, 0x1919082b, 0x08080819, 0x19191908, 0x08081908, 0x19191908, 0x0808192b, 0x19191908,
        0x08082b19, 0x19191908, 0x08190808, 0x19191908, 0x0819082b, 0x19191908, 0x08191919, 0x19191908,
        0x08192b08, 0x19191908, 0x082b0819, 0x19191908, 0x082b1908, 0x19191908, 0x19080808, 0x19191908,
        0x1908082b, 0x19191908, 0x19081919, 0x19191908, 0x19082b08, 0x19191908, 0x19190819, 0x19191908,
        0x19191908, 0x19191908, 0x192b0808, 0x19191908, 0x2b080819, 0x19191908, 0x2b081908, 0x19191908,
        0x2b190808, 0x19191908, 0x08080808, 0x19191919, 0x0808082b, 0x19191919, 0x08081919, 0x19191919,
        0x08082b08, 0x19191919, 0x08190819, 0x19191919, 0x08191908, 0x19191919, 0x082b0808, 0x19191919,
        0x19080819, 0x19191919, 0x19081908, 0x19191919, 0x19190808, 0x19191919, 0x2b080808, 0x19191919,
        0x08080819, 0x1919192b, 0x08081908, 0x1919192b, 0x08190808, 0x1919192b, 0x082b192b, 0x1919192b,
        0x19080808, 0x1919192b, 0x08080808, 0x19192b08, 0x0808082b, 0x19192b08, 0x08081919, 0x19192b08,
        0x08082b08, 0x19192b08, 0x08190819, 0x19192b08, 0x08191908, 0x19192b08, 0x082b0808, 0x19192b08,
        0x19080819, 0x19192b08, 0x19081908, 0x19192b08, 0x19190808, 0x19192b08, 0x19192b2b, 0x19192b08,
        0x2b080808, 0x19192b08, 0x08080819, 0x19192b19, 0x08081908, 0x19192b19, 0x08190808, 0x19192b19,
        0x19080808, 0x19192b19, 0x08080808, 0x19192b2b, 0x08192b19, 0x19192b2b, 0x2b081919, 0x19192b2b,
        0x2b2b2b08, 0x19192b2b, 0x08080819, 0x192b0808, 0x08081908, 0x192b0808, 0x0808192b, 0x192b0808,
        0x08190808, 0x192b0808, 0x0819082b, 0x192b0808, 0x08191919, 0x192b0808, 0x08192b08, 0x192b0808,
        0x082b0819, 0x192b0808, 0x082b1908, 0x192b0808, 0x19080808, 0x192b0808, 0x19081919, 0x192b0808,
        0x19082b08, 0x192b0808, 0x19190819, 0x192b0808, 0x19191908, 0x192b0808, 0x192b0808, 0x192b0808,
        0x2b081908, 0x192b0808, 0x2b190808, 0x192b0808, 0x08080808, 0x192b0819, 0x0808082b, 0x192b0819,
        0x08081919, 0x192b0819, 0x08082b08, 0x192b0819, 0x08190819, 0x192b0819, 0x08191908, 0x192b0819,
        0x082b0808, 0x192b0819, 0x19080819, 0x192b0819, 0x19081908, 0x192b0819, 0x19190808, 0x192b0819,
        0x2b080808, 0x192b0819, 0x2b192b19, 0x192b0819, 0x08081908, 0x192b082b, 0x08190808, 0x192b082b,
        0x19080808, 0x192b082b, 0x1919192b, 0x192b082b, 0x2b2b0819, 0x192b082b, 0x08080808, 0x192b1908,
        0x08081919, 0x192b1908, 0x08082b08, 0x192b1908, 0x08190819, 0x192b1908, 0x08191908, 0x192b1908,
        0x082b0808, 0x192b1908, 0x19080819, 0x192b1908, 0x19081908, 0x192b1908, 0x19190808, 0x192b1908,
        0x2b080808, 0x192b1908, 0x08080819, 0x192b1919, 0x08081908, 0x192b1919, 0x08190808, 0x192b1919,
        0x19080808, 0x192b1919, 0x19082b2b, 0x192b1919, 0x192b2b08, 0x192b1919, 0x2b19082b, 0x192b1919,
        0x08080808, 0x192b192b, 0x2b191908, 0x192b192b, 0x08080819, 0x192b2b08, 0x08081908, 0x192b2b08,
        0x08190808, 0x192b2b08, 0x192b1919, 0x192b2b08, 0x2b192b08, 0x192b2b08, 0x08080808, 0x192b2b19,
        0x082b2b2b, 0x192b2b19, 0x1908082b, 0x192b2b2b, 0x2b2b0819, 0x192b2b2b, 0x08080808, 0x2b080808,
        0x0808082b, 0x2b080808, 0x08081919, 0x2b080808, 0x08082b08, 0x2b080808, 0x08190819, 0x2b080808,
        0x08191908, 0x2b080808, 0x08192b19, 0x2b080808, 0x082b0808, 0x2b080808, 0x082b1919, 0x2b080808,
        0x19080819, 0x2b080808, 0x19081908, 0x2b080808, 0x19190808, 0x2b080808, 0x1919082b, 0x2b080808,
        0x19191919, 0x2b080808, 0x19192b08, 0x2b080808, 0x192b0819, 0x2b080808, 0x2b080808, 0x2b080808,
        0x2b081919, 0x2b080808, 0x2b190819, 0x2b080808, 0x2b191908, 0x2b080808, 0x08080819, 0x2b080819,
        0x08081908, 0x2b080819, 0x08082b19, 0x2b080819, 0x08190808, 0x2b080819, 0x0819082b, 0x2b080819,
        0x08191919, 0x2b080819, 0x08192b08, 0x2b080819, 0x082b0819, 0x2b080819, 0x082b1908, 0x2b080819,
        0x19080808, 0x2b080819, 0x1908082b, 0x2b080819, 0x19081919, 0x2b080819, 0x19082b08, 0x2b080819,
        0x19190819, 0x2b080819, 0x19191908, 0x2b080819, 0x2b080819, 0x2b080819, 0x2b081908, 0x2b080819,
        0x2b190808, 0x2b080819, 0x2b2b2b19, 0x2b080819, 0x08080808, 0x2b08082b, 0x08081919, 0x2b08082b,
        0x08082b2b, 0x2b08082b, 0x08190819, 0x2b08082b, 0x08191908, 0x2b08082b, 0x19080819, 0x2b08082b,
        0x19081908, 0x2b08082b, 0x19190808, 0x2b08082b, 0x08080819, 0x2b081908, 0x08081908, 0x2b081908,
        0x0808192b, 0x2b081908, 0x08082b19, 0x2b081908, 0x08190808, 0x2b081908, 0x0819082b, 0x2b081908,
        0x08191919, 0x2b081908, 0x08192b08, 0x2b081908, 0x082b0819, 0x2b081908, 0x19080808, 0x2b081908,
        0x1908082b, 0x2b081908, 0x19081919, 0x2b081908, 0x19082b08, 0x2b081908, 0x19190819, 0x2b081908,
        0x19191908, 0x2b081908, 0x192b0808, 0x2b081908, 0x2b080819, 0x2b081908, 0x2b081908, 0x2b081908,
        0x2b190808, 0x2b081908, 0x08080808, 0x2b081919, 0x0808082b, 0x2b081919, 0x08081919, 0x2b081919,
        0x08082b08, 0x2b081919, 0x08190819, 0x2b081919, 0x08191908, 0x2b081919, 0x082b0808, 0x2b081919,
        0x19080819, 0x2b081919, 0x19081908, 0x2b081919, 0x19190808, 0x2b081919, 0x2b080808, 0x2b081919,
        0x2b082b2b, 0x2b081919, 0x08080819, 0x2b08192b, 0x08081908, 0x2b08192b, 0x08190808, 0x2b08192b,
        0x082b2b19, 0x2b08192b, 0x19080808, 0x2b08192b, 0x08080808, 0x2b082b08, 0x08081919, 0x2b082b08,
        0x08190819, 0x2b082b08, 0x08191908, 0x2b082b08, 0x19080819, 0x2b082b08, 0x19081908, 0x2b082b08,
        0x19190808, 0x2b082b08, 0x2b2b082b, 0x2b082b08, 0x08080819, 0x2b082b19, 0x08081908, 0x2b082b19,
        0x19080808, 0x2b082b19, 0x192b1919, 0x2b082b19, 0x082b082b, 0x2b082b2b, 0x19192b08, 0x2b082b2b,
        0x19192b2b, 0x2b082b2b, 0x2b08082b, 0x2b082b2b, 0x2b2b082b, 0x2b082b2b, 0x08080819, 0x2b190808,
        0x08081908, 0x2b190808, 0x08082b19, 0x2b190808, 0x08190808, 0x2b190808, 0x0819082b, 0x2b190808,
        0x08191919, 0x2b190808, 0x08192b08, 0x2b190808, 0x082b1908, 0x2b190808, 0x19080808, 0x2b190808,
        0x1908082b, 0x2b190808, 0x19081919, 0x2b190808, 0x19082b08, 0x2b190808, 0x19190819, 0x2b190808,
        0x19191908, 0x2b190808, 0x192b0808, 0x2b190808, 0x2b080819, 0x2b190808, 0x2b081908, 0x2b190808,
        0x2b190808, 0x2b190808, 0x08080808, 0x2b190819, 0x08081919, 0x2b190819, 0x08190819, 0x2b190819,
        0x08191908, 0x2b190819, 0x19080819, 0x2b190819, 0x19081908, 0x2b190819, 0x19190808, 0x2b190819,
        0x19192b2b, 0x2b190819, 0x08080819, 0x2b19082b, 0x08081908, 0x2b19082b, 0x08190808, 0x2b19082b,
        0x19080808, 0x2b19082b, 0x2b2b192b, 0x2b19082b, 0x08080808, 0x2b191908, 0x0808082b, 0x2b191908,
        0x08081919, 0x2b191908, 0x08082b08, 0x2b191908, 0x08190819, 0x2b191908, 0x08191908, 0x2b191908,
        0x082b0808, 0x2b191908, 0x19080819, 0x2b191908, 0x19081908, 0x2b191908, 0x19190808, 0x2b191908,
        0x2b080808, 0x2b191908, 0x2b19192b, 0x2b191908, 0x08080819, 0x2b191919, 0x08081908, 0x2b191919,
        0x08190808, 0x2b191919, 0x19080808, 0x2b191919, 0x2b192b08, 0x2b191919, 0x2b2b0819, 0x2b191919,
        0x08080808, 0x2b19192b, 0x1908192b, 0x2b19192b, 0x192b1908, 0x2b19192b, 0x08080819, 0x2b192b08,
        0x08081908, 0x2b192b08, 0x08190808, 0x2b192b08, 0x082b192b, 0x2b192b08, 0x19080808, 0x2b192b08,
        0x2b2b2b19, 0x2b192b08, 0x08080808, 0x2b192b19, 0x19082b19, 0x2b192b19, 0x1919082b, 0x2b192b19,
        0x2b190808, 0x2b192b2b, 0x08080808, 0x2b2b0808, 0x08081919, 0x2b2b0808, 0x08082b2b, 0x2b2b0808,
        0x08191908, 0x2b2b0808, 0x082b082b, 0x2b2b0808, 0x082b2b2b, 0x2b2b0808, 0x19080819, 0x2b2b0808,
        0x19081908, 0x2b2b0808, 0x19190808, 0x2b2b0808, 0x2b2b082b, 0x2b2b0808, 0x2b2b2b2b, 0x2b2b0808,
        0x19080808, 0x2b2b0819, 0x192b1919, 0x2b2b0819, 0x0808082b, 0x2b2b082b, 0x08082b2b, 0x2b2b082b,
        0x082b082b, 0x2b2b082b, 0x082b2b08, 0x2b2b082b, 0x082b2b2b, 0x2b2b082b, 0x2b08082b, 0x2b2b082b,
        0x2b082b08, 0x2b2b082b, 0x2b082b2b, 0x2b2b082b, 0x2b2b2b08, 0x2b2b082b, 0x08080819, 0x2b2b1908,
        0x08081908, 0x2b2b1908, 0x08190808, 0x2b2b1908, 0x19080808, 0x2b2b1908, 0x2b082b19, 0x2b2b1908,
        0x2b2b1908, 0x2b2b1908, 0x08080808, 0x2b2b1919, 0x08192b19, 0x2b2b1919, 0x19190819, 0x2b2b192b,
        0x08082b2b, 0x2b2b2b08, 0x082b2b08, 0x2b2b2b08, 0x2b2b082b, 0x2b2b2b08, 0x19191908, 0x2b2b2b19,
        0x2b08192b, 0x2b2b2b19, 0x08082b08, 0x2b2b2b2b, 0x08082b2b, 0x2b2b2b2b, 0x082b0808, 0x2b2b2b2b,
        0x082b082b, 0x2b2b2b2b, 0x082b2b08, 0x2b2b2b2b, 0x2b082b08, 0x2b2b2b2b, 0x2b2b2b2b, 0x2b2b2b2b)
}

//! Main-context use only (tests, oracles, the emitter's constant bake) - a forked worker reads
//! a module global as zeros; kernels take `iq2s_grid2()` instead.
let IQ2S_GRID = iq2s_grid2()


//! The IQ2_XS grid (upstream's iq2xs_grid, 512 u64 entries as low/high uint pairs): the u16 word's
//! low 9 bits select EIGHT magnitudes ({8, 25, 43}), its high 7 bits the KSIGNS_IQ2XS sign
//! byte (bit j flips element j). Per-call local (4 KB), worker-safe; IQ2XS_GRID = main-context copy.
def iq2xs_grid2() : uint[1024] {   // nolint:STYLE038 - a 1024-word data table, not splittable
    return fixed_array<uint>(
        0x08080808, 0x08080808, 0x0808082b, 0x08080808, 0x08081919, 0x08080808, 0x08082b08, 0x08080808,
        0x08082b2b, 0x08080808, 0x08190819, 0x08080808, 0x08191908, 0x08080808, 0x0819192b, 0x08080808,
        0x08192b19, 0x08080808, 0x082b0808, 0x08080808, 0x082b082b, 0x08080808, 0x082b1919, 0x08080808,
        0x082b2b08, 0x08080808, 0x19080819, 0x08080808, 0x19081908, 0x08080808, 0x1908192b, 0x08080808,
        0x19082b19, 0x08080808, 0x19190808, 0x08080808, 0x1919082b, 0x08080808, 0x19191919, 0x08080808,
        0x19192b08, 0x08080808, 0x192b0819, 0x08080808, 0x192b1908, 0x08080808, 0x2b080808, 0x08080808,
        0x2b08082b, 0x08080808, 0x2b081919, 0x08080808, 0x2b082b08, 0x08080808, 0x2b190819, 0x08080808,
        0x2b191908, 0x08080808, 0x2b192b19, 0x08080808, 0x2b2b0808, 0x08080808, 0x08080819, 0x08080819,
        0x08081908, 0x08080819, 0x0808192b, 0x08080819, 0x08082b19, 0x08080819, 0x08190808, 0x08080819,
        0x0819082b, 0x08080819, 0x08191919, 0x08080819, 0x08192b08, 0x08080819, 0x08192b2b, 0x08080819,
        0x082b0819, 0x08080819, 0x082b1908, 0x08080819, 0x19080808, 0x08080819, 0x1908082b, 0x08080819,
        0x19081919, 0x08080819, 0x19082b08, 0x08080819, 0x19190819, 0x08080819, 0x19191908, 0x08080819,
        0x192b0808, 0x08080819, 0x192b2b08, 0x08080819, 0x2b080819, 0x08080819, 0x2b081908, 0x08080819,
        0x2b190808, 0x08080819, 0x08080808, 0x0808082b, 0x0808082b, 0x0808082b, 0x08081919, 0x0808082b,
        0x08082b08, 0x0808082b, 0x08190819, 0x0808082b, 0x08191908, 0x0808082b, 0x082b0808, 0x0808082b,
        0x19080819, 0x0808082b, 0x19081908, 0x0808082b, 0x19190808, 0x0808082b, 0x19191919, 0x0808082b,
        0x2b080808, 0x0808082b, 0x2b082b2b, 0x0808082b, 0x08080819, 0x08081908, 0x08081908, 0x08081908,
        0x0808192b, 0x08081908, 0x08082b19, 0x08081908, 0x08190808, 0x08081908, 0x0819082b, 0x08081908,
        0x08191919, 0x08081908, 0x08192b08, 0x08081908, 0x082b0819, 0x08081908, 0x082b1908, 0x08081908,
        0x19080808, 0x08081908, 0x1908082b, 0x08081908, 0x19081919, 0x08081908, 0x19082b08, 0x08081908,
        0x19190819, 0x08081908, 0x19191908, 0x08081908, 0x1919192b, 0x08081908, 0x192b0808, 0x08081908,
        0x2b080819, 0x08081908, 0x2b081908, 0x08081908, 0x2b190808, 0x08081908, 0x08080808, 0x08081919,
        0x0808082b, 0x08081919, 0x08081919, 0x08081919, 0x08082b08, 0x08081919, 0x08190819, 0x08081919,
        0x08191908, 0x08081919, 0x082b0808, 0x08081919, 0x19080819, 0x08081919, 0x19081908, 0x08081919,
        0x19190808, 0x08081919, 0x192b0819, 0x08081919, 0x2b080808, 0x08081919, 0x08080819, 0x0808192b,
        0x08081908, 0x0808192b, 0x08190808, 0x0808192b, 0x082b192b, 0x0808192b, 0x19080808, 0x0808192b,
        0x1908082b, 0x0808192b, 0x2b081908, 0x0808192b, 0x08080808, 0x08082b08, 0x0808082b, 0x08082b08,
        0x08081919, 0x08082b08, 0x08082b08, 0x08082b08, 0x08082b2b, 0x08082b08, 0x08190819, 0x08082b08,
        0x08191908, 0x08082b08, 0x082b0808, 0x08082b08, 0x082b1919, 0x08082b08, 0x19080819, 0x08082b08,
        0x19081908, 0x08082b08, 0x19190808, 0x08082b08, 0x19192b08, 0x08082b08, 0x2b080808, 0x08082b08,
        0x2b2b0808, 0x08082b08, 0x2b2b2b2b, 0x08082b08, 0x08080819, 0x08082b19, 0x08081908, 0x08082b19,
        0x08190808, 0x08082b19, 0x19080808, 0x08082b19, 0x2b080819, 0x08082b19, 0x2b082b19, 0x08082b19,
        0x08080808, 0x08082b2b, 0x082b0808, 0x08082b2b, 0x082b2b08, 0x08082b2b, 0x2b19192b, 0x08082b2b,
        0x2b2b0808, 0x08082b2b, 0x08080819, 0x08190808, 0x08081908, 0x08190808, 0x0808192b, 0x08190808,
        0x08082b19, 0x08190808, 0x08190808, 0x08190808, 0x0819082b, 0x08190808, 0x08191919, 0x08190808,
        0x08192b08, 0x08190808, 0x082b0819, 0x08190808, 0x082b1908, 0x08190808, 0x19080808, 0x08190808,
        0x1908082b, 0x08190808, 0x19081919, 0x08190808, 0x19082b08, 0x08190808, 0x19190819, 0x08190808,
        0x19191908, 0x08190808, 0x192b0808, 0x08190808, 0x192b2b2b, 0x08190808, 0x2b080819, 0x08190808,
        0x2b081908, 0x08190808, 0x2b190808, 0x08190808, 0x08080808, 0x08190819, 0x0808082b, 0x08190819,
        0x08081919, 0x08190819, 0x08082b08, 0x08190819, 0x08190819, 0x08190819, 0x08191908, 0x08190819,
        0x082b0808, 0x08190819, 0x19080819, 0x08190819, 0x19081908, 0x08190819, 0x19190808, 0x08190819,
        0x2b080808, 0x08190819, 0x2b191908, 0x08190819, 0x2b19192b, 0x08190819, 0x08080819, 0x0819082b,
        0x08081908, 0x0819082b, 0x0808192b, 0x0819082b, 0x08190808, 0x0819082b, 0x19080808, 0x0819082b,
        0x192b0808, 0x0819082b, 0x08080808, 0x08191908, 0x0808082b, 0x08191908, 0x08081919, 0x08191908,
        0x08082b08, 0x08191908, 0x08190819, 0x08191908, 0x08191908, 0x08191908, 0x082b0808, 0x08191908,
        0x19080819, 0x08191908, 0x19081908, 0x08191908, 0x19082b19, 0x08191908, 0x19190808, 0x08191908,
        0x192b1908, 0x08191908, 0x2b080808, 0x08191908, 0x08080819, 0x08191919, 0x08081908, 0x08191919,
        0x08190808, 0x08191919, 0x19080808, 0x08191919, 0x08080808, 0x0819192b, 0x08191908, 0x0819192b,
        0x19082b19, 0x0819192b, 0x08080819, 0x08192b08, 0x08081908, 0x08192b08, 0x08190808, 0x08192b08,
        0x0819082b, 0x08192b08, 0x19080808, 0x08192b08, 0x19191908, 0x08192b08, 0x2b08192b, 0x08192b08,
        0x08080808, 0x08192b19, 0x08081919, 0x08192b19, 0x192b192b, 0x08192b19, 0x19190819, 0x08192b2b,
        0x2b2b2b19, 0x08192b2b, 0x08080808, 0x082b0808, 0x0808082b, 0x082b0808, 0x08081919, 0x082b0808,
        0x08082b08, 0x082b0808, 0x08082b2b, 0x082b0808, 0x08190819, 0x082b0808, 0x08191908, 0x082b0808,
        0x082b0808, 0x082b0808, 0x19080819, 0x082b0808, 0x19081908, 0x082b0808, 0x19190808, 0x082b0808,
        0x2b080808, 0x082b0808, 0x2b2b0808, 0x082b0808, 0x08080819, 0x082b0819, 0x08081908, 0x082b0819,
        0x08190808, 0x082b0819, 0x19080808, 0x082b0819, 0x19082b08, 0x082b0819, 0x192b1919, 0x082b0819,
        0x08080808, 0x082b082b, 0x082b082b, 0x082b082b, 0x2b080808, 0x082b082b, 0x2b2b2b08, 0x082b082b,
        0x08080819, 0x082b1908, 0x08081908, 0x082b1908, 0x08190808, 0x082b1908, 0x082b2b19, 0x082b1908,
        0x19080808, 0x082b1908, 0x08080808, 0x082b1919, 0x19080819, 0x082b1919, 0x1919082b, 0x082b1919,
        0x2b192b19, 0x082b1919, 0x08080819, 0x082b192b, 0x08192b2b, 0x082b192b, 0x2b2b192b, 0x082b192b,
        0x08080808, 0x082b2b08, 0x08082b08, 0x082b2b08, 0x08082b2b, 0x082b2b08, 0x082b0808, 0x082b2b08,
        0x19191919, 0x082b2b08, 0x2b082b08, 0x082b2b08, 0x2b2b082b, 0x082b2b08, 0x192b2b08, 0x082b2b19,
        0x2b190808, 0x082b2b19, 0x08082b08, 0x082b2b2b, 0x082b0808, 0x082b2b2b, 0x2b08082b, 0x082b2b2b,
        0x2b082b08, 0x082b2b2b, 0x2b082b2b, 0x082b2b2b, 0x08080819, 0x19080808, 0x08081908, 0x19080808,
        0x0808192b, 0x19080808, 0x08082b19, 0x19080808, 0x08190808, 0x19080808, 0x0819082b, 0x19080808,
        0x08191919, 0x19080808, 0x08192b08, 0x19080808, 0x082b0819, 0x19080808, 0x082b1908, 0x19080808,
        0x19080808, 0x19080808, 0x1908082b, 0x19080808, 0x19081919, 0x19080808, 0x19082b08, 0x19080808,
        0x19082b2b, 0x19080808, 0x19190819, 0x19080808, 0x19191908, 0x19080808, 0x192b0808, 0x19080808,
        0x192b1919, 0x19080808, 0x2b080819, 0x19080808, 0x2b081908, 0x19080808, 0x2b190808, 0x19080808,
        0x08080808, 0x19080819, 0x0808082b, 0x19080819, 0x08081919, 0x19080819, 0x08082b08, 0x19080819,
        0x08190819, 0x19080819, 0x08191908, 0x19080819, 0x082b0808, 0x19080819, 0x19080819, 0x19080819,
        0x19081908, 0x19080819, 0x19190808, 0x19080819, 0x2b080808, 0x19080819, 0x2b081919, 0x19080819,
        0x2b2b082b, 0x19080819, 0x08080819, 0x1908082b, 0x08081908, 0x1908082b, 0x08190808, 0x1908082b,
        0x0819082b, 0x1908082b, 0x082b2b19, 0x1908082b, 0x19080808, 0x1908082b, 0x08080808, 0x19081908,
        0x0808082b, 0x19081908, 0x08081919, 0x19081908, 0x08082b08, 0x19081908, 0x08190819, 0x19081908,
        0x08191908, 0x19081908, 0x08192b19, 0x19081908, 0x082b0808, 0x19081908, 0x19080819, 0x19081908,
        0x19081908, 0x19081908, 0x19190808, 0x19081908, 0x2b080808, 0x19081908, 0x2b191908, 0x19081908,
        0x08080819, 0x19081919, 0x08081908, 0x19081919, 0x08190808, 0x19081919, 0x082b1908, 0x19081919,
        0x19080808, 0x19081919, 0x2b192b2b, 0x19081919, 0x08080808, 0x1908192b, 0x08082b2b, 0x1908192b,
        0x19081908, 0x1908192b, 0x19190808, 0x1908192b, 0x08080819, 0x19082b08, 0x08081908, 0x19082b08,
        0x08190808, 0x19082b08, 0x19080808, 0x19082b08, 0x19081919, 0x19082b08, 0x19191908, 0x19082b08,
        0x192b082b, 0x19082b08, 0x08080808, 0x19082b19, 0x08190819, 0x19082b19, 0x19081908, 0x19082b19,
        0x19190808, 0x19082b19, 0x192b2b19, 0x19082b19, 0x08081908, 0x19082b2b, 0x08080808, 0x19190808,
        0x0808082b, 0x19190808, 0x08081919, 0x19190808, 0x08082b08, 0x19190808, 0x08190819, 0x19190808,
        0x08191908, 0x19190808, 0x082b0808, 0x19190808, 0x082b2b08, 0x19190808, 0x19080819, 0x19190808,
        0x19081908, 0x19190808, 0x19190808, 0x19190808, 0x2b080808, 0x19190808, 0x08080819, 0x19190819,
        0x08081908, 0x19190819, 0x08190808, 0x19190819, 0x08191919, 0x19190819, 0x19080808, 0x19190819,
        0x1908082b, 0x19190819, 0x08080808, 0x1919082b, 0x19081908, 0x1919082b, 0x2b2b2b2b, 0x1919082b,
        0x08080819, 0x19191908, 0x08081908, 0x19191908, 0x08190808, 0x19191908, 0x082b0819, 0x19191908,
        0x19080808, 0x19191908, 0x192b0808, 0x19191908, 0x2b080819, 0x19191908, 0x2b2b0819, 0x19191908,
        0x08080808, 0x19191919, 0x08082b08, 0x19191919, 0x2b080808, 0x19191919, 0x2b082b08, 0x19191919,
        0x082b0819, 0x1919192b, 0x192b2b08, 0x1919192b, 0x2b2b0819, 0x1919192b, 0x08080808, 0x19192b08,
        0x08191908, 0x19192b08, 0x19080819, 0x19192b08, 0x19190808, 0x19192b08, 0x2b192b19, 0x19192b08,
        0x08192b2b, 0x19192b19, 0x19080808, 0x19192b19, 0x1908082b, 0x19192b19, 0x2b081919, 0x19192b2b,
        0x08080819, 0x192b0808, 0x08081908, 0x192b0808, 0x08190808, 0x192b0808, 0x19080808, 0x192b0808,
        0x19191908, 0x192b0808, 0x192b082b, 0x192b0808, 0x2b08192b, 0x192b0808, 0x2b2b2b19, 0x192b0808,
        0x08080808, 0x192b0819, 0x082b1908, 0x192b082b, 0x19082b2b, 0x192b082b, 0x2b19082b, 0x192b082b,
        0x08080808, 0x192b1908, 0x0819192b, 0x192b1908, 0x08190808, 0x192b1919, 0x19080808, 0x192b1919,
        0x19081919, 0x192b1919, 0x2b2b1908, 0x192b1919, 0x08080819, 0x192b2b08, 0x192b2b2b, 0x192b2b08,
        0x082b1919, 0x192b2b19, 0x0808192b, 0x192b2b2b, 0x19191908, 0x192b2b2b, 0x192b082b, 0x192b2b2b,
        0x08080808, 0x2b080808, 0x0808082b, 0x2b080808, 0x08081919, 0x2b080808, 0x08082b08, 0x2b080808,
        0x08190819, 0x2b080808, 0x08191908, 0x2b080808, 0x082b0808, 0x2b080808, 0x082b2b2b, 0x2b080808,
        0x19080819, 0x2b080808, 0x19081908, 0x2b080808, 0x19190808, 0x2b080808, 0x2b080808, 0x2b080808,
        0x2b08082b, 0x2b080808, 0x2b2b2b08, 0x2b080808, 0x2b2b2b2b, 0x2b080808, 0x08080819, 0x2b080819,
        0x08081908, 0x2b080819, 0x0808192b, 0x2b080819, 0x08190808, 0x2b080819, 0x19080808, 0x2b080819,
        0x19190819, 0x2b080819, 0x19192b19, 0x2b080819, 0x08080808, 0x2b08082b, 0x082b0808, 0x2b08082b,
        0x2b080808, 0x2b08082b, 0x2b08082b, 0x2b08082b, 0x2b2b0808, 0x2b08082b, 0x2b2b2b08, 0x2b08082b,
        0x08080819, 0x2b081908, 0x08081908, 0x2b081908, 0x08190808, 0x2b081908, 0x0819082b, 0x2b081908,
        0x08191919, 0x2b081908, 0x19080808, 0x2b081908, 0x192b0808, 0x2b081908, 0x2b082b19, 0x2b081908,
        0x08080808, 0x2b081919, 0x19081908, 0x2b081919, 0x2b2b1919, 0x2b081919, 0x08192b08, 0x2b08192b,
        0x192b2b2b, 0x2b08192b, 0x08080808, 0x2b082b08, 0x08082b08, 0x2b082b08, 0x082b1919, 0x2b082b08,
        0x19192b2b, 0x2b082b08, 0x2b080808, 0x2b082b08, 0x2b08082b, 0x2b082b08, 0x2b2b2b08, 0x2b082b08,
        0x0808192b, 0x2b082b19, 0x082b082b, 0x2b082b2b, 0x2b080808, 0x2b082b2b, 0x2b082b08, 0x2b082b2b,
        0x2b19192b, 0x2b082b2b, 0x2b2b2b08, 0x2b082b2b, 0x08080819, 0x2b190808, 0x08081908, 0x2b190808,
        0x08190808, 0x2b190808, 0x19080808, 0x2b190808, 0x1919192b, 0x2b190808, 0x2b081908, 0x2b190808,
        0x08080808, 0x2b190819, 0x082b082b, 0x2b190819, 0x192b1908, 0x2b190819, 0x1919192b, 0x2b19082b,
        0x2b082b19, 0x2b19082b, 0x08080808, 0x2b191908, 0x08081919, 0x2b191908, 0x19081908, 0x2b191908,
        0x19190808, 0x2b191908, 0x19192b08, 0x2b191908, 0x082b2b19, 0x2b191919, 0x2b190808, 0x2b191919,
        0x2b19082b, 0x2b191919, 0x19080819, 0x2b19192b, 0x19190819, 0x2b192b08, 0x2b2b192b, 0x2b192b08,
        0x19082b19, 0x2b192b19, 0x08191919, 0x2b192b2b, 0x192b0808, 0x2b192b2b, 0x08080808, 0x2b2b0808,
        0x0808082b, 0x2b2b0808, 0x08082b08, 0x2b2b0808, 0x08082b2b, 0x2b2b0808, 0x082b0808, 0x2b2b0808,
        0x082b2b2b, 0x2b2b0808, 0x2b2b0808, 0x2b2b0808, 0x19190819, 0x2b2b0819, 0x19192b19, 0x2b2b0819,
        0x2b2b192b, 0x2b2b0819, 0x08080808, 0x2b2b082b, 0x0808082b, 0x2b2b082b, 0x08082b08, 0x2b2b082b,
        0x082b2b2b, 0x2b2b082b, 0x2b080808, 0x2b2b082b, 0x2b2b0808, 0x2b2b082b, 0x19080808, 0x2b2b1908,
        0x2b191919, 0x2b2b1908, 0x192b1919, 0x2b2b192b, 0x2b192b08, 0x2b2b192b, 0x08082b2b, 0x2b2b2b08,
        0x082b0808, 0x2b2b2b08, 0x082b082b, 0x2b2b2b08, 0x082b2b08, 0x2b2b2b08, 0x2b2b0808, 0x2b2b2b08,
        0x2b2b2b08, 0x2b2b2b08, 0x08081908, 0x2b2b2b19, 0x2b081908, 0x2b2b2b19, 0x2b08192b, 0x2b2b2b19,
        0x082b2b08, 0x2b2b2b2b, 0x082b2b2b, 0x2b2b2b2b, 0x2b190819, 0x2b2b2b2b, 0x2b2b2b2b, 0x2b2b2b2b)
}

//! Main-context use only (tests, oracles, the emitter's constant bake) - a forked worker reads
//! a module global as zeros; kernels take `iq2xs_grid2()` instead.
let IQ2XS_GRID = iq2xs_grid2()


//! The IQ2_XXS grid (upstream's iq2xxs_grid, 256 u64 entries as low/high uint pairs): a block's
//! aux32[0] BYTES each select EIGHT magnitudes ({8, 25, 43}), flipped through the shared
//! ksigns_iq2xs table. Per-call local (2 KB), worker-safe; IQ2XXS_GRID = main-context copy.
def iq2xxs_grid2() : uint[512] {
    return fixed_array<uint>(
        0x08080808, 0x08080808, 0x0808082b, 0x08080808, 0x08081919, 0x08080808, 0x08082b08, 0x08080808,
        0x08082b2b, 0x08080808, 0x08190819, 0x08080808, 0x08191908, 0x08080808, 0x082b0808, 0x08080808,
        0x082b082b, 0x08080808, 0x082b2b08, 0x08080808, 0x082b2b2b, 0x08080808, 0x19080819, 0x08080808,
        0x19081908, 0x08080808, 0x19190808, 0x08080808, 0x19192b08, 0x08080808, 0x192b0819, 0x08080808,
        0x192b1908, 0x08080808, 0x2b080808, 0x08080808, 0x2b08082b, 0x08080808, 0x2b082b2b, 0x08080808,
        0x2b2b082b, 0x08080808, 0x08080819, 0x08080819, 0x08081908, 0x08080819, 0x08190808, 0x08080819,
        0x08191919, 0x08080819, 0x19080808, 0x08080819, 0x2b081908, 0x08080819, 0x2b192b08, 0x08080819,
        0x08080808, 0x0808082b, 0x0808082b, 0x0808082b, 0x082b082b, 0x0808082b, 0x2b08082b, 0x0808082b,
        0x08080819, 0x08081908, 0x08081908, 0x08081908, 0x08190808, 0x08081908, 0x082b0819, 0x08081908,
        0x082b1908, 0x08081908, 0x19080808, 0x08081908, 0x1908082b, 0x08081908, 0x19082b08, 0x08081908,
        0x192b0808, 0x08081908, 0x2b080819, 0x08081908, 0x2b081908, 0x08081908, 0x2b190808, 0x08081908,
        0x2b2b1908, 0x08081908, 0x08080808, 0x08081919, 0x0808082b, 0x08081919, 0x08082b08, 0x08081919,
        0x082b0808, 0x08081919, 0x1908192b, 0x08081919, 0x192b2b19, 0x08081919, 0x2b080808, 0x08081919,
        0x2b190819, 0x08081919, 0x08082b19, 0x0808192b, 0x08190808, 0x0808192b, 0x19080808, 0x0808192b,
        0x2b081908, 0x0808192b, 0x2b2b1908, 0x0808192b, 0x08080808, 0x08082b08, 0x08081919, 0x08082b08,
        0x08082b08, 0x08082b08, 0x08191908, 0x08082b08, 0x082b2b08, 0x08082b08, 0x19080819, 0x08082b08,
        0x19081908, 0x08082b08, 0x19190808, 0x08082b08, 0x1919082b, 0x08082b08, 0x2b082b08, 0x08082b08,
        0x08081908, 0x08082b19, 0x19080808, 0x08082b19, 0x0808082b, 0x08082b2b, 0x08191908, 0x08082b2b,
        0x08080819, 0x08190808, 0x08081908, 0x08190808, 0x08190808, 0x08190808, 0x082b0819, 0x08190808,
        0x19080808, 0x08190808, 0x192b0808, 0x08190808, 0x2b081908, 0x08190808, 0x2b190808, 0x08190808,
        0x2b191919, 0x08190808, 0x08080808, 0x08190819, 0x08082b08, 0x08190819, 0x082b0808, 0x08190819,
        0x19190808, 0x08190819, 0x19192b2b, 0x08190819, 0x2b080808, 0x08190819, 0x082b1908, 0x0819082b,
        0x19081919, 0x0819082b, 0x08080808, 0x08191908, 0x08082b08, 0x08191908, 0x082b0808, 0x08191908,
        0x082b1919, 0x08191908, 0x19082b19, 0x08191908, 0x2b080808, 0x08191908, 0x08192b08, 0x08191919,
        0x192b082b, 0x08191919, 0x08080808, 0x0819192b, 0x0819192b, 0x0819192b, 0x08080819, 0x08192b08,
        0x08081908, 0x08192b08, 0x08190808, 0x08192b08, 0x19080808, 0x08192b08, 0x2b080819, 0x08192b08,
        0x08080808, 0x08192b19, 0x08081919, 0x08192b19, 0x2b2b0808, 0x08192b19, 0x19190819, 0x08192b2b,
        0x08080808, 0x082b0808, 0x0808082b, 0x082b0808, 0x08082b2b, 0x082b0808, 0x19081908, 0x082b0808,
        0x192b0819, 0x082b0808, 0x2b080808, 0x082b0808, 0x2b08082b, 0x082b0808, 0x082b2b19, 0x082b0819,
        0x19082b08, 0x082b0819, 0x08080808, 0x082b082b, 0x0808082b, 0x082b082b, 0x08080819, 0x082b1908,
        0x08081908, 0x082b1908, 0x08190808, 0x082b1908, 0x19080808, 0x082b1908, 0x1919192b, 0x082b1908,
        0x08080808, 0x082b1919, 0x19080819, 0x082b1919, 0x192b1908, 0x082b1919, 0x2b190808, 0x082b192b,
        0x08082b08, 0x082b2b08, 0x082b0808, 0x082b2b08, 0x2b191908, 0x082b2b08, 0x19081908, 0x082b2b2b,
        0x08080819, 0x19080808, 0x08081908, 0x19080808, 0x08190808, 0x19080808, 0x08192b08, 0x19080808,
        0x082b0819, 0x19080808, 0x082b1908, 0x19080808, 0x19080808, 0x19080808, 0x19082b08, 0x19080808,
        0x1919192b, 0x19080808, 0x192b0808, 0x19080808, 0x2b080819, 0x19080808, 0x2b081908, 0x19080808,
        0x2b190808, 0x19080808, 0x08080808, 0x19080819, 0x082b0808, 0x19080819, 0x192b0819, 0x19080819,
        0x2b080808, 0x19080819, 0x2b081919, 0x19080819, 0x08080819, 0x1908082b, 0x08190808, 0x1908082b,
        0x19082b08, 0x1908082b, 0x1919192b, 0x1908082b, 0x192b2b08, 0x1908082b, 0x08080808, 0x19081908,
        0x08082b08, 0x19081908, 0x082b0808, 0x19081908, 0x2b080808, 0x19081908, 0x2b192b19, 0x19081908,
        0x0819082b, 0x19081919, 0x082b1908, 0x19081919, 0x08080808, 0x1908192b, 0x08080819, 0x19082b08,
        0x08081908, 0x19082b08, 0x08190808, 0x19082b08, 0x19080808, 0x19082b08, 0x19081919, 0x19082b08,
        0x08080808, 0x19082b19, 0x19192b08, 0x19082b19, 0x192b0819, 0x19082b19, 0x2b08082b, 0x19082b19,
        0x19081919, 0x19082b2b, 0x2b190808, 0x19082b2b, 0x08080808, 0x19190808, 0x08082b08, 0x19190808,
        0x08190819, 0x19190808, 0x08192b19, 0x19190808, 0x082b0808, 0x19190808, 0x2b080808, 0x19190808,
        0x2b082b08, 0x19190808, 0x08081908, 0x19190819, 0x1908082b, 0x19190819, 0x2b2b1908, 0x19190819,
        0x2b190819, 0x1919082b, 0x2b190808, 0x19191908, 0x2b19082b, 0x19191908, 0x08082b2b, 0x19191919,
        0x08080819, 0x1919192b, 0x19191908, 0x1919192b, 0x08080808, 0x19192b08, 0x08190819, 0x19192b08,
        0x08192b19, 0x19192b08, 0x192b1908, 0x19192b08, 0x19080808, 0x19192b19, 0x08082b08, 0x19192b2b,
        0x08081908, 0x192b0808, 0x08190808, 0x192b0808, 0x19080808, 0x192b0808, 0x192b2b08, 0x192b0808,
        0x08080808, 0x192b0819, 0x19191919, 0x192b0819, 0x08192b08, 0x192b082b, 0x192b0808, 0x192b082b,
        0x08080808, 0x192b1908, 0x08081919, 0x192b1908, 0x08190808, 0x192b1919, 0x0819082b, 0x192b1919,
        0x2b081908, 0x192b1919, 0x1908082b, 0x192b2b08, 0x08080808, 0x2b080808, 0x0808082b, 0x2b080808,
        0x08082b2b, 0x2b080808, 0x19080819, 0x2b080808, 0x2b08082b, 0x2b080808, 0x08081908, 0x2b080819,
        0x08192b08, 0x2b080819, 0x19080808, 0x2b080819, 0x08190819, 0x2b08082b, 0x08080819, 0x2b081908,
        0x08081908, 0x2b081908, 0x08190808, 0x2b081908, 0x08191919, 0x2b081908, 0x19080808, 0x2b081908,
        0x192b0808, 0x2b081908, 0x08080808, 0x2b081919, 0x1908192b, 0x2b081919, 0x2b191908, 0x2b081919,
        0x08082b19, 0x2b08192b, 0x19080808, 0x2b08192b, 0x192b0808, 0x2b08192b, 0x0808082b, 0x2b082b08,
        0x08081908, 0x2b082b19, 0x08190819, 0x2b082b2b, 0x08081908, 0x2b190808, 0x08190808, 0x2b190808,
        0x082b1908, 0x2b190808, 0x19080808, 0x2b190808, 0x2b2b0819, 0x2b190808, 0x0819192b, 0x2b190819,
        0x2b080808, 0x2b190819, 0x19081919, 0x2b19082b, 0x08080808, 0x2b191908, 0x082b082b, 0x2b191908,
        0x19081908, 0x2b191908, 0x19190819, 0x2b191919, 0x2b080819, 0x2b192b08, 0x082b0808, 0x2b192b19,
        0x0808082b, 0x2b2b0808, 0x19190808, 0x2b2b0808, 0x2b081919, 0x2b2b0808, 0x08082b19, 0x2b2b0819,
        0x08080808, 0x2b2b082b, 0x08192b08, 0x2b2b1908, 0x19190808, 0x2b2b2b08, 0x08081908, 0x2b2b2b19)
}

//! Main-context use only (tests, oracles, the emitter's constant bake) - a forked worker reads
//! a module global as zeros; kernels take `iq2xxs_grid2()` instead.
let IQ2XXS_GRID = iq2xxs_grid2()

//! The packed IQ1_S / IQ1_M grid, a per-call local kernels read on any lane: entry e at bits 16 x (e % 2) of word e / 2
def iq1s_grid() : uint[1024] {   // nolint:STYLE038 - a 1024-word data table, not splittable
    return fixed_array<uint>(
        0x00020000u, 0x00080005u, 0x0011000au, 0x00200015u, 0x00280022u, 0x0045002au, 0x00540051u, 0x00650056u,
        0x00820080u, 0x008a0088u, 0x00a00095u, 0x00a800a2u, 0x010400aau, 0x01110105u, 0x01160114u, 0x011a0119u,
        0x01410125u, 0x01490146u, 0x01550152u, 0x0161015au, 0x01660164u, 0x01850168u, 0x01940191u, 0x01a50196u,
        0x02020200u, 0x020a0208u, 0x02200215u, 0x02280222u, 0x0245022au, 0x02590251u, 0x02690264u, 0x02820280u,
        0x028a0288u, 0x02950291u, 0x02a00299u, 0x02a802a2u, 0x041102aau, 0x04160414u, 0x04410425u, 0x04550449u,
        0x0464045au, 0x04910465u, 0x04a50499u, 0x05040501u, 0x05060505u, 0x05180515u, 0x0529051au, 0x05450540u,
        0x0550054au, 0x05540551u, 0x05560555u, 0x05600559u, 0x05650562u, 0x056a0568u, 0x05910581u, 0x05980595u,
        0x05a1059au, 0x05a505a4u, 0x05a905a6u, 0x06190614u, 0x06440641u, 0x06520650u, 0x06580655u, 0x06610660u,
        0x06690666u, 0x06910685u, 0x06990694u, 0x08020800u, 0x080a0808u, 0x08200815u, 0x08280822u, 0x0845082au,
        0x08560851u, 0x08800865u, 0x08880882u, 0x0895088au, 0x08a208a0u, 0x08aa08a8u, 0x09110905u, 0x09190914u,
        0x09250924u, 0x09500941u, 0x09550951u, 0x09640961u, 0x09910969u, 0x09960994u, 0x09a50999u, 0x0a020a00u,
        0x0a0a0a08u, 0x0a200a15u, 0x0a280a22u, 0x0a450a2au, 0x0a590a51u, 0x0a650a61u, 0x0a820a80u, 0x0a880a85u,
        0x0a950a8au, 0x0aa20aa0u, 0x0aaa0aa8u, 0x10111010u, 0x10191014u, 0x10251024u, 0x10441041u, 0x10551050u,
        0x10611058u, 0x10651064u, 0x10911069u, 0x10961094u, 0x10a510a1u, 0x11041101u, 0x11091106u, 0x11121110u,
        0x11181115u, 0x11241121u, 0x11451129u, 0x1150114au, 0x11521151u, 0x11551154u, 0x11591156u, 0x11651160u,
        0x11921184u, 0x11a11195u, 0x121111a4u, 0x12161214u, 0x12401225u, 0x12491246u, 0x12551252u, 0x125a1258u,
        0x12661264u, 0x12911285u, 0x12961294u, 0x140112a5u, 0x14091406u, 0x14151414u, 0x14191418u, 0x14261421u,
        0x14451441u, 0x14481446u, 0x1451144au, 0x14551454u, 0x14591456u, 0x14651462u, 0x14841468u, 0x14901489u,
        0x14951494u, 0x14991498u, 0x14a1149au, 0x14a514a4u, 0x150214a9u, 0x150a1505u, 0x15141511u, 0x15161515u,
        0x15201519u, 0x15251522u, 0x152a1528u, 0x15441541u, 0x15461545u, 0x15521551u, 0x15551554u, 0x15591556u,
        0x1561155au, 0x15651564u, 0x15691566u, 0x15821580u, 0x15851584u, 0x158a1588u, 0x15911590u, 0x15951594u,
        0x15991596u, 0x15a0159au, 0x15a515a2u, 0x16041601u, 0x16061605u, 0x16161615u, 0x161a1618u, 0x16261621u,
        0x16421640u, 0x16451644u, 0x164a1648u, 0x16551651u, 0x16581656u, 0x16611659u, 0x16651664u, 0x16691668u,
        0x1686166au, 0x1692168au, 0x16a41695u, 0x181116a9u, 0x18251816u, 0x18441841u, 0x18491846u, 0x18551850u,
        0x185a1858u, 0x18611860u, 0x18661864u, 0x18851869u, 0x18941891u, 0x191018a5u, 0x19151912u, 0x1921191au,
        0x19421925u, 0x19451944u, 0x19511948u, 0x19551954u, 0x19591956u, 0x1960195au, 0x196a1965u, 0x19911989u,
        0x19951992u, 0x19a11998u, 0x19a919a6u, 0x1a161a09u, 0x1a261a24u, 0x1a461a44u, 0x1a501a49u, 0x1a551a52u,
        0x1a611a58u, 0x1a691a66u, 0x1a911a85u, 0x1a9a1a96u, 0x20022000u, 0x200a2008u, 0x20202015u, 0x20252022u,
        0x202a2028u, 0x20512045u, 0x20612059u, 0x20802065u, 0x20882082u, 0x2095208au, 0x20a220a0u, 0x20a820a5u,
        0x210520aau, 0x21142111u, 0x21252119u, 0x21442142u, 0x21552149u, 0x215a2158u, 0x21642161u, 0x21662165u,
        0x21902185u, 0x21992196u, 0x220121a5u, 0x220a2208u, 0x22152211u, 0x22222220u, 0x222a2228u, 0x22512245u,
        0x22592256u, 0x22812265u, 0x228a2288u, 0x22952291u, 0x22a222a0u, 0x22aa22a8u, 0x24142405u, 0x24192416u,
        0x24442425u, 0x24462445u, 0x24522449u, 0x24582455u, 0x2466245au, 0x24912485u, 0x24992494u, 0x24a524a1u,
        0x25152509u, 0x25292521u, 0x25452540u, 0x25512548u, 0x25552554u, 0x25622559u, 0x25682565u, 0x25902589u,
        0x25952594u, 0x259a2598u, 0x25a425a1u, 0x25a925a6u, 0x26102605u, 0x26192612u, 0x26412625u, 0x26552649u,
        0x26612660u, 0x26842669u, 0x26902686u, 0x2800269au, 0x28082802u, 0x2815280au, 0x28222820u, 0x282a2828u,
        0x28512845u, 0x28652854u, 0x28822880u, 0x288a2888u, 0x28a228a0u, 0x28aa28a8u, 0x29112909u, 0x29192914u,
        0x29462925u, 0x29522949u, 0x29612955u, 0x29662964u, 0x29852969u, 0x29962990u, 0x29a42999u, 0x2a0029a5u,
        0x2a082a02u, 0x2a202a0au, 0x2a282a22u, 0x2a452a2au, 0x2a562a51u, 0x2a652a59u, 0x2a822a80u, 0x2a8a2a88u,
        0x2aa02a95u, 0x2aa82aa2u, 0x40052aaau, 0x40164011u, 0x40494025u, 0x40554052u, 0x405a4058u, 0x40644061u,
        0x40944066u, 0x40a14099u, 0x410040a6u, 0x41044101u, 0x41094106u, 0x41154112u, 0x41184116u, 0x4121411au,
        0x41294126u, 0x41484145u, 0x4151414au, 0x41554154u, 0x41594156u, 0x4165415au, 0x416a4168u, 0x41844181u,
        0x41904186u, 0x41954192u, 0x41a141a0u, 0x420541a2u, 0x42144211u, 0x42254216u, 0x42524241u, 0x425a4255u,
        0x42694264u, 0x42944289u, 0x440142a5u, 0x44194415u, 0x44454429u, 0x444a4448u, 0x44544451u, 0x44564455u,
        0x44624461u, 0x44684465u, 0x4481446au, 0x44894486u, 0x44924490u, 0x44a04495u, 0x44a944a1u, 0x45024501u,
        0x450a4505u, 0x45144511u, 0x45164515u, 0x45204519u, 0x452a4525u, 0x45444541u, 0x45464545u, 0x45504549u,
        0x45544551u, 0x45564555u, 0x45594558u, 0x45644561u, 0x45664565u, 0x45824569u, 0x45854584u, 0x45914588u,
        0x45954594u, 0x45994596u, 0x45a5459au, 0x45aa45a8u, 0x46054601u, 0x46144609u, 0x46184615u, 0x4621461au,
        0x46294624u, 0x46424640u, 0x46484645u, 0x46514650u, 0x46554652u, 0x46594656u, 0x46654662u, 0x46814668u,
        0x468a4685u, 0x46954694u, 0x46a446a1u, 0x480546a6u, 0x48154811u, 0x4825481au, 0x48494842u, 0x48554850u,
        0x48614858u, 0x48664864u, 0x48854869u, 0x48944891u, 0x48994896u, 0x490148a5u, 0x49064905u, 0x4910490au,
        0x49154914u, 0x49214918u, 0x49264924u, 0x49454940u, 0x4951494au, 0x49544952u, 0x49564955u, 0x49604959u,
        0x49654962u, 0x496a4966u, 0x49894986u, 0x49954992u, 0x49984996u, 0x49a449a1u, 0x49a949a6u, 0x4a444a16u,
        0x4a494a46u, 0x4a584a55u, 0x4a644a5au, 0x4a944a69u, 0x50014aa5u, 0x50055004u, 0x50095006u, 0x50155012u,
        0x5021501au, 0x50295024u, 0x50455040u, 0x50515048u, 0x50555054u, 0x50595056u, 0x50685065u, 0x50895086u,
        0x50985095u, 0x50a150a0u, 0x50a950a6u, 0x51085105u, 0x510a5109u, 0x51145111u, 0x51165115u, 0x51195118u,
        0x51255120u, 0x51285126u, 0x5141512au, 0x51455144u, 0x51495146u, 0x51515150u, 0x51545152u, 0x51565155u,
        0x51595158u, 0x5161515au, 0x51655164u, 0x51695166u, 0x51855182u, 0x51945191u, 0x51965195u, 0x51a05199u,
        0x51aa51a5u, 0x52065201u, 0x52155212u, 0x5221521au, 0x52425224u, 0x524a5245u, 0x52545251u, 0x52565255u,
        0x52625259u, 0x52855265u, 0x52925290u, 0x52995295u, 0x52a4529au, 0x54055404u, 0x54145411u, 0x54165415u,
        0x54195418u, 0x54255421u, 0x542a5428u, 0x54445441u, 0x54465445u, 0x544a5449u, 0x54515450u, 0x54555454u,
        0x54585456u, 0x545a5459u, 0x54625461u, 0x54655464u, 0x54695466u, 0x54885480u, 0x5491548au, 0x54955494u,
        0x54995496u, 0x54a454a1u, 0x54aa54a5u, 0x55025501u, 0x55055504u, 0x55095506u, 0x55115510u, 0x55145512u,
        0x55165515u, 0x551a5519u, 0x55245521u, 0x55265525u, 0x55405529u, 0x55425541u, 0x55455544u, 0x55485546u,
        0x55505549u, 0x55525551u, 0x55555554u, 0x55585556u, 0x555a5559u, 0x55615560u, 0x55655564u, 0x55685566u,
        0x556a5569u, 0x55845581u, 0x55895585u, 0x5590558au, 0x55945591u, 0x55965595u, 0x55995598u, 0x55a455a1u,
        0x55a655a5u, 0x560055a9u, 0x56025601u, 0x56065604u, 0x56095608u, 0x56145611u, 0x56185615u, 0x56205619u,
        0x56225621u, 0x56255624u, 0x56285626u, 0x56415629u, 0x56465645u, 0x56495648u, 0x5650564au, 0x56525651u,
        0x56555654u, 0x56585656u, 0x565a5659u, 0x56645661u, 0x56695665u, 0x56855682u, 0x56885686u, 0x568a5689u,
        0x56955691u, 0x56a2569au, 0x56a656a5u, 0x56a956a8u, 0x58055804u, 0x58095806u, 0x58155810u, 0x58215818u,
        0x5845582au, 0x584a5848u, 0x58545851u, 0x58565855u, 0x58595858u, 0x58625860u, 0x58655864u, 0x58895882u,
        0x58925890u, 0x58985895u, 0x58a958a1u, 0x59025901u, 0x590a5905u, 0x59145911u, 0x59165915u, 0x59255919u,
        0x59445941u, 0x59465945u, 0x59505949u, 0x59525951u, 0x59555954u, 0x59585956u, 0x595a5959u, 0x59645961u,
        0x59665965u, 0x59815969u, 0x59895985u, 0x59945991u, 0x59965995u, 0x59995998u, 0x5a0459a5u, 0x5a155a08u,
        0x5a205a1au, 0x5a265a25u, 0x5a455a29u, 0x5a495a48u, 0x5a555a51u, 0x5a585a56u, 0x5a625a59u, 0x5a685a65u,
        0x5a815a6au, 0x5a925a8au, 0x5a965a95u, 0x5a9a5a98u, 0x60055aa1u, 0x60166014u, 0x60256019u, 0x60506044u,
        0x60566055u, 0x605a6058u, 0x60646061u, 0x60696066u, 0x60966081u, 0x610160a5u, 0x61066104u, 0x61126109u,
        0x61216115u, 0x61266122u, 0x61456129u, 0x61516149u, 0x61566155u, 0x61656159u, 0x616a6166u, 0x618a6184u,
        0x61956192u, 0x61a661a1u, 0x621161a9u, 0x62196216u, 0x62416240u, 0x62556246u, 0x62586256u, 0x62856260u,
        0x62966291u, 0x641162a5u, 0x64156412u, 0x641a6416u, 0x64266421u, 0x64406429u, 0x64456442u, 0x644a6448u,
        0x64546451u, 0x64566455u, 0x645a6459u, 0x64626460u, 0x64846465u, 0x64896485u, 0x64926490u, 0x64956494u,
        0x64986496u, 0x64a1649au, 0x64a964a4u, 0x65086505u, 0x6511650au, 0x65166515u, 0x65446519u, 0x65466545u,
        0x65506549u, 0x65546551u, 0x65566555u, 0x65616559u, 0x65656564u, 0x65696566u, 0x65896586u, 0x6591658au,
        0x65966595u, 0x659a6599u, 0x65a565a2u, 0x65a865a6u, 0x66096602u, 0x66206615u, 0x66286626u, 0x66406629u,
        0x66486645u, 0x6651664au, 0x66556654u, 0x66586656u, 0x6660665au, 0x66686665u, 0x66826680u, 0x668a6685u,
        0x66966694u, 0x66996698u, 0x66a466a0u, 0x66aa66a6u, 0x68196816u, 0x68416825u, 0x68556852u, 0x6861685au,
        0x68856869u, 0x68986891u, 0x690168a6u, 0x69106904u, 0x69216915u, 0x69266924u, 0x69406929u, 0x69456941u,
        0x69486946u, 0x69546951u, 0x69566955u, 0x69606959u, 0x696a6965u, 0x69846982u, 0x6995698au, 0x69a469a1u,
        0x69a969a5u, 0x6a166a11u, 0x6a416a18u, 0x6a496a44u, 0x6a556a50u, 0x6a5a6a58u, 0x6a656a64u, 0x6a866a69u,
        0x6a986a94u, 0x6aa66a9au, 0x80028000u, 0x800a8008u, 0x80228020u, 0x802a8028u, 0x80508045u, 0x80548051u,
        0x80598056u, 0x80808065u, 0x80888082u, 0x8095808au, 0x80a280a0u, 0x80aa80a8u, 0x81118105u, 0x81168114u,
        0x81258119u, 0x81448141u, 0x81508149u, 0x81558152u, 0x81588156u, 0x81648159u, 0x81698166u, 0x81898185u,
        0x81968194u, 0x81a58199u, 0x82028200u, 0x820a8208u, 0x82208215u, 0x82288222u, 0x8251822au, 0x82598254u,
        0x82808265u, 0x82888282u, 0x8295828au, 0x82a282a0u, 0x82aa82a8u, 0x84198414u, 0x84448441u, 0x84558451u,
        0x8461845au, 0x84698464u, 0x84998494u, 0x85098501u, 0x85158512u, 0x8526851au, 0x85408529u, 0x85458541u,
        0x85518548u, 0x85558554u, 0x85598556u, 0x8565855au, 0x85688566u, 0x8581856au, 0x85868584u, 0x85908589u,
        0x85958592u, 0x85a68598u, 0x86168611u, 0x86258619u, 0x86448641u, 0x864a8649u, 0x86558650u, 0x865a8659u,
        0x86668661u, 0x8685866au, 0x869a8691u, 0x880086a4u, 0x88088802u, 0x8815880au, 0x88228820u, 0x882a8828u,
        0x88458841u, 0x88548851u, 0x88658859u, 0x88808869u, 0x88888882u, 0x8895888au, 0x88a288a0u, 0x88aa88a8u,
        0x89068905u, 0x89148911u, 0x89258916u, 0x89448941u, 0x89498946u, 0x89528950u, 0x895a8955u, 0x89648961u,
        0x89968985u, 0x89a58999u, 0x8a028a00u, 0x8a0a8a08u, 0x8a208a15u, 0x8a288a22u, 0x8a458a2au, 0x8a548a51u,
        0x8a808a56u, 0x8a888a82u, 0x8a958a8au, 0x8aa28aa0u, 0x8aaa8aa8u, 0x90119005u, 0x90189016u, 0x90259019u,
        0x90469041u, 0x90559049u, 0x905a9058u, 0x906a9069u, 0x90919085u, 0x90969094u, 0x90a59099u, 0x91049101u,
        0x91099106u, 0x91159110u, 0x911a9118u, 0x91249121u, 0x91299126u, 0x91459140u, 0x91519150u, 0x91559154u,
        0x91599156u, 0x91659162u, 0x91869184u, 0x91959192u, 0x91a19198u, 0x91a691a4u, 0x920591a9u, 0x92149211u,
        0x92259219u, 0x92469244u, 0x92509249u, 0x92559252u, 0x92669258u, 0x92859269u, 0x92969294u, 0x940192a9u,
        0x94069404u, 0x94159410u, 0x94269418u, 0x944a9440u, 0x94549451u, 0x94569455u, 0x94599458u, 0x94619460u,
        0x94659462u, 0x94869484u, 0x94949492u, 0x94989495u, 0x94a994a1u, 0x95059500u, 0x950a9508u, 0x95119510u,
        0x95159514u, 0x95199516u, 0x95259521u, 0x952a9529u, 0x95449541u, 0x95469545u, 0x95509549u, 0x95529551u,
        0x95559554u, 0x95589556u, 0x955a9559u, 0x95649561u, 0x95669565u, 0x95819569u, 0x95889585u, 0x95929591u,
        0x95959594u, 0x95999596u, 0x95a0959au, 0x95a595a2u, 0x95aa95a8u, 0x96049601u, 0x96159610u, 0x96209619u,
        0x96299626u, 0x96489645u, 0x96519649u, 0x96559652u, 0x96599656u, 0x96689665u, 0x96849682u, 0x968a9689u,
        0x96949692u, 0x96a49695u, 0x96a996a6u, 0x98169805u, 0x98259819u, 0x98469841u, 0x98529850u, 0x98569855u,
        0x9864985au, 0x98859865u, 0x98969891u, 0x98a59899u, 0x99069904u, 0x99109909u, 0x99159912u, 0x991a9918u,
        0x99219920u, 0x99269924u, 0x99429940u, 0x99489945u, 0x9951994au, 0x99559954u, 0x99599956u, 0x99659962u,
        0x996a9966u, 0x99849981u, 0x99929990u, 0x999a9995u, 0x99a699a1u, 0x9a159a05u, 0x9a449a25u, 0x9a499a46u,
        0x9a559a50u, 0x9a619a58u, 0x9a919a85u, 0x9a959a94u, 0xa0009a96u, 0xa008a002u, 0xa015a00au, 0xa022a020u,
        0xa02aa028u, 0xa051a045u, 0xa056a054u, 0xa080a059u, 0xa088a082u, 0xa095a08au, 0xa0a2a0a0u, 0xa0aaa0a8u,
        0xa109a105u, 0xa114a111u, 0xa119a116u, 0xa146a11au, 0xa151a149u, 0xa158a155u, 0xa161a15au, 0xa185a164u,
        0xa192a190u, 0xa199a196u, 0xa208a202u, 0xa210a20au, 0xa222a219u, 0xa22aa228u, 0xa251a245u, 0xa259a256u,
        0xa280a265u, 0xa288a282u, 0xa295a28au, 0xa2a2a2a0u, 0xa2aaa2a8u, 0xa425a419u, 0xa444a441u, 0xa454a450u,
        0xa458a455u, 0xa461a45au, 0xa466a465u, 0xa469a468u, 0xa506a485u, 0xa510a509u, 0xa515a512u, 0xa526a518u,
        0xa542a529u, 0xa551a545u, 0xa555a554u, 0xa559a556u, 0xa56aa565u, 0xa584a581u, 0xa586a585u, 0xa592a589u,
        0xa598a595u, 0xa611a605u, 0xa61aa616u, 0xa625a621u, 0xa646a644u, 0xa652a64au, 0xa656a655u, 0xa660a658u,
        0xa686a662u, 0xa695a690u, 0xa699a696u, 0xa6a4a6a1u, 0xa800a6a6u, 0xa808a802u, 0xa820a80au, 0xa828a822u,
        0xa851a82au, 0xa856a854u, 0xa880a859u, 0xa888a882u, 0xa895a88au, 0xa8a2a8a0u, 0xa8aaa8a8u, 0xa914a905u,
        0xa921a919u, 0xa941a925u, 0xa955a950u, 0xa961a95au, 0xa969a966u, 0xa996a990u, 0xaa02aa00u, 0xaa0aaa08u,
        0xaa22aa20u, 0xaa2aaa28u, 0xaa54aa51u, 0xaa80aa56u, 0xaa88aa82u, 0xaa95aa8au, 0xaaa2aaa0u, 0xaaaaaaa8u)
}

//! Main-context use only - a forked worker reads a module global as zeros; kernels take `iq1s_grid()` instead.
let IQ1S_GRID = iq1s_grid()

//! the delta both 1-bit grid formats add to a grid value (upstream's IQ1S_DELTA = IQ1M_DELTA): an eighth
let IQ1_DELTA = 0.125

let IQ1_EIGHTHS_BIAS = 9   //! the unsigned 1-bit weight in eighths, 8 x code + 1 + delta (0 .. 18), less this is the signed one

//! entry `idx` of the packed 1-bit grid: eight 2-bit codes, weight j's at bits 2j; generic in the table, so it inlines
def iq1_grid_entry(grid : auto(TG); idx : int) : uint => (grid[idx >> 1] >> uint((idx & 1) * 16)) & 0xFFFFu

//! The exact int dot of a 1-bit octet's weights in eighths, 8 x (code - 1) + `delta` (+-1), with a[ab .. ab + 8)
def grid1_octet_dot(entry : uint; delta : int; a : auto(TA) const?; ab : int64) : int {
    var acc = 0
    unsafe {
        for (j in range64(8l)) {
            let c = int((entry >> uint(2l * j)) & 3u)
            acc += (8 * c - 8 + delta) * int(a[ab + j])
        }
    }
    return acc
}

//! A 1-bit octet's weights in eighths, 8 x (grid value) + `delta`, as signed bytes in two words (0-3, 4-7): the tile panel's form
def grid1_octet_words(entry : uint; delta : int) : uint2 {
    let add = delta < 0 ? 0xF7F7F7F7u : 0xF9F9F9F9u
    return uint2(grid1_code_bytes(entry & 0xFFu, add), grid1_code_bytes(entry >> 8u, add))
}

//! four 2-bit codes (the low byte of `x`) spread a byte each as 8 x code, plus the bytes of `add` without a carry between bytes
def grid1_code_bytes(x, add : uint) : uint {
    let codes = (x | (x << 6u) | (x << 12u) | (x << 18u)) & 0x03030303u
    return ((codes << 3u) + (add & 0x7F7F7F7Fu)) ^ (add & 0x80808080u)
}

//! IQ1_S: octet `l`'s three index high bits off its block's 16 qh bits
def iq1s_hi(qh, l : uint) : uint => (qh >> (3u * l)) & 7u

//! IQ1_S: the block's delta sign off its 16 qh bits, 1 for -1/8
def iq1s_neg(qh : uint) : uint => qh >> 15u

//! IQ1_S: the block's scale 2s + 1 off its 16 qh bits
def iq1s_sc(qh : uint) : float => float(2u * ((qh >> 12u) & 7u) + 1u)

//! IQ1_M: octet `l`'s three index high bits off its block's 16 qh bits, a nibble an octet
def iq1m_hi(qh, l : uint) : uint => (qh >> (4u * l)) & 7u

//! IQ1_M: octet `l`'s delta sign, its nibble's top bit, 1 for -1/8
def iq1m_neg(qh, l : uint) : uint => (qh >> (4u * l + 3u)) & 1u

//! the little-endian u16 at byte `o` of `p`, a byte pointer or array
def u16_le(p : auto(TP); o : int64) : uint {
    unsafe {
        return uint(p[o]) | (uint(p[o + 1l]) << 8u)
    }
}

//! IQ1_M's four scale words at byte `o` of `p` (a byte pointer or array): the half-block scales, the f16 d's nibbles on top
def iq1m_scale_words(p : auto(TP); o : int64) : uint[4] => fixed_array<uint>(u16_le(p, o), u16_le(p, o + 2l), u16_le(p, o + 4l), u16_le(p, o + 6l))

//! The 1-bit octet dequantized in ggml's own float order: dst[at + j] = dl x (grid value + delta)
def store_grid1_octet(var dst : auto(TD); at : int64; dl : float; entry : uint; delta : float) {
    unsafe {
        for (j in range64(8l)) {
            dst[at + j] = dl * (float(int((entry >> uint(2l * j)) & 3u) - 1) + delta)
        }
    }
}

//! IQ1_M's f16 d off its superblock's four scale words: word w's top nibble at bits 4w
def iq1m_d_bits(w0, w1, w2, w3 : uint) : uint => (w0 >> 12u) | ((w1 >> 8u) & 0xF0u) | ((w2 >> 4u) & 0xF00u) | (w3 & 0xF000u)

//! IQ1_M's 3-bit scale of 16-weight half-block `hb` (0..15) off scale word hb / 4: field hb % 4, three bits each
def iq1m_half_scale(w : uint; hb : int) : int => int((w >> uint(3 * (hb % 4))) & 7u)

//! Quant-plane bytes per stride unit — the typed stride surface. The stride unit is one
//! 256-weight superblock for the kq lattice and one 32-weight block for q51 (see kq_elems).
//! q8 panics (its planes are byte-per-elem, not stride-walked here).
def kq_qsb(f : KqFmt) : int64 {
    let qsb = kq_desc(f).qsb
    if (qsb == 0l) {
        panic("kq_qsb: no plane stride for this format")
    }
    return qsb
}

let KQ_STRIP_BYTES = 16l   //! the scale strip a superblock keeps in a device form: the disk block's first sixteen bytes

//! the d bytes a superblock keeps beside its strip in the split scale form
def kq_split_tail_bytes(f : KqFmt) : int64 => f == KqFmt.k2 ? 4l : 2l

//! the bytes the split scale form holds of `nsb` superblocks: a strip and a d tail a superblock
def kq_split_scale_bytes(f : KqFmt; nsb : int64) : int64 => nsb * (KQ_STRIP_BYTES + kq_split_tail_bytes(f))

//! the descriptor row of a format with a scale plane - q8 panics, as kq_ssb does
def private kq_scale_desc(f : KqFmt) : KqDesc {
    let d = kq_desc(f)
    if (d.ssb == 0l) {
        panic("{f} has no scale plane")
    }
    return d
}

//! Scale-plane bytes per stride unit (see kq_qsb for the unit); panics, naming the member, when its scale unit serves a group
def kq_ssb(f : KqFmt) : int64 {
    let d = kq_scale_desc(f)
    if (d.sgroup != 1l) {
        panic("kq_ssb: {f} keeps one scale unit a {d.sgroup}-block group, so it has no per-block stride - read kq_soff / kq_sbytes")
    }
    return d.ssb
}

//! the stride units one scale unit of `f` serves
def kq_sgroup(f : KqFmt) : int64 => kq_desc(f).sgroup

//! the weights one scale unit of `f` serves - its disk block's weights, and the unit a row of the format is whole in
def kq_group_elems(f : KqFmt) : int64 => kq_desc(f).elems * kq_desc(f).sgroup

let KQ_METAL_BIND_ALIGN = 16l   //! the byte grain a plane's Metal bind offset keeps

//! the element granule at which a group-scaled format's scale row lands `align`-byte aligned: `align` over a group's scale bytes, in groups
def kq_group_align_elems(f : KqFmt; align : int64) : int64 => kq_group_elems(f) * (align / kq_sbytes(f, kq_sgroup(f)))

//! the scale-plane byte offset of stride unit `blk`: the scale unit of its group
def kq_soff(f : KqFmt; blk : int64) : int64 {
    let d = kq_scale_desc(f)
    return (blk / d.sgroup) * d.ssb
}

//! the scale-plane bytes of `nblocks` whole stride units; panics, naming the member, when they end inside a group
def kq_sbytes(f : KqFmt; nblocks : int64) : int64 {
    let d = kq_scale_desc(f)
    if (nblocks % d.sgroup != 0l) {
        panic("kq_sbytes: {nblocks} {f} blocks end inside its {d.sgroup}-block scale group")
    }
    return (nblocks / d.sgroup) * d.ssb
}

//! Weights per stride unit: the kq lattice strides per 256-weight superblock; q8/q51 per
//! 32-weight block.
def kq_elems(f : KqFmt) : int64 => kq_desc(f).elems

//! The loader's stream/repack region tag for a format (stream_repack_one / push_repack codes).
//! k2's stream-region code: kernel id 2 IS q51's stream tag, so k2 streams under 20 and the
//! repack dispatchers translate it back to kernel id 2.
let Q8_STREAM_TAG = 0   //! the q8 family's stream / repack region tag - the one format with no `kq_desc` row
let K2_STREAM_TAG = 20

//! mx4's stream tag is the GGML type id (`GGML_TYPE_MXFP4`): the code its streamed jobs and repack regions carry
let MX4_STREAM_TAG = 39

//! iq4nl32's stream tag: a code no kernel id claims (iq4nl's 45 is the superblock rail's)
let IQ4NL32_STREAM_TAG = 46

//! q4032's stream tag: the next code no kernel id claims
let Q4032_STREAM_TAG = 47

//! q5032's stream tag: the next code no kernel id claims
let Q5032_STREAM_TAG = 48

//! the affine members' stream tags: the next codes no kernel id claims (49 is nv4's)
let MLX4_STREAM_TAG = 50
let MLX8_STREAM_TAG = 51
let MLX4B_STREAM_TAG = 52
let MLX8B_STREAM_TAG = 53
let MLX6B_STREAM_TAG = 54

//! q20's stream tag: the next code no kernel id claims
let Q20_STREAM_TAG = 55

//! q10's stream tag: the next code no kernel id claims
let Q10_STREAM_TAG = 56

def kq_stream_code(f : KqFmt) : int => kq_desc(f).stream_code

//! the stream code's format; false for a code no format carries
def kq_fmt_of_stream_code(code : int; var out : KqFmt&) : bool => kq_fmt_where(out) <| $(f : KqFmt) => kq_desc(f).stream_code == code

//! a weight plane's site in the words a decline line and the load report name it by, keyed by its GGUF tensor role
let private KQ_SITE_WORDS <- {
    "token_embd" => "token table", "per_layer_token_embd" => "PLE table", "output" => "classifier",
    "attn_q" => "attention q", "attn_k" => "attention k", "attn_v" => "attention v", "attn_output" => "attention o",
    "attn_qkv" => "attention qkv", "attn_gate" => "attention gate",
    "ffn_gate" => "FFN gate", "ffn_up" => "FFN up", "ffn_down" => "FFN down",
    "ffn_gate_exps" => "expert stack gate", "ffn_up_exps" => "expert stack up", "ffn_down_exps" => "expert stack down",
    "ffn_gate_up_exps" => "expert stack gate-up",
    "ffn_gate_shexp" => "shared expert gate", "ffn_up_shexp" => "shared expert up", "ffn_down_shexp" => "shared expert down",
    "ssm_out" => "deltanet out", "ssm_alpha" => "deltanet alpha", "ssm_beta" => "deltanet beta", "ssm_ba" => "deltanet beta-alpha"
}

//! the site words of a GGUF tensor role (`attn_q`, `ffn_down_exps`, `token_embd`); a role with no words is named as itself
def kq_site_words(role : string) : string => KQ_SITE_WORDS?[role] ?? role

//! a GGUF tensor name's role: the name past a `blk.<n>.` prefix and short of a `.weight` suffix
def gguf_tensor_role(name : string) : string {
    var role = name
    if (starts_with(role, "blk.")) {
        let dot = find(role, ".", 4)
        role = dot >= 0 ? slice(role, dot + 1) : role
    }
    return ends_with(role, ".weight") ? slice(role, 0, length(role) - 7) : role
}

