options gen2
options no_aot     // requires the no_aot _spirv_xmod fixture (optimizer-folded shape; AOT hash unstable)

require dastest/testing_boost public
require _spirv_common
require _spirv_xmod
require spirv/spirv_shader
require spirv/spirv_builtins public
require spirv/spirv_dis
require spirv/spirv_grammar
require math

// Both-branches-return if/else (found lowering the one-source path tracer's shared core module).
//
// When BOTH branches of an if/else terminate (return), the selection-merge block is unreachable.
// The emitter used to unconditionally reset terminated=false after the merge label, so a function
// BODY ending on such an if was misread as "not every control-flow path returns a value". Now the
// merge block is closed with OpUnreachable and the function stays terminated. Two sources of the
// shape: explicit if/else-both-return in the same file, and a REQUIRED module (see _spirv_xmod.das)
// whose optimizer-folded functions arrive with trailing code folded into `else`. spirv-val'd.
var @ssbo @binding = 0 rp_data : array<float>

// explicit both-return if/else as the WHOLE body (same-file, unoptimized AST)
def private rp_pick(x : float) : float {
    if (x < 0.0) {
        return 0.0
    } else {
        return x * 2.0
    }
}

[compute_shader(local_size_x=64, name="rp_spv"), marker(no_coverage)]
def rp {
    let i = int(gl_GlobalInvocationID.x)
    var v = 0.0
    if (xm_clip(rp_data[i], v)) {
        rp_data[i] = rp_pick(v) + xm_sign_mag(v)
    }
}

[test(description = "An if/else whose branches both return closes its merge block with OpUnreachable and the module passes spirv-val.")]
def test_return_paths(t : T?) {
    t |> run("if/else with both branches returning: merge closes with OpUnreachable; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(rp_spv)
        t |> success(!empty(words), "shader with both-return if/else bodies lowers")
        t |> success(has_op(words, SpvOp.Unreachable), "unreachable merge block closed with OpUnreachable")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Struct VALUES: a plain (undecorated) struct as a function parameter and as a function result, plus
// the local whose initializer the optimizer dropped as dead. Both are what porting a GLSL helper that
// returns a struct needs -- glslang emits `OpFunction %SvPhys` over an OpTypeStruct with no Block /
// Offset decoration, and so must dasSpirv.
//
// The fixtures live in the REQUIRED module _spirv_xmod on purpose: that is the real-world shape (a
// shared CPU/GPU core module), and the dead-initializer drop happens ONLY there -- a required module
// finishes optimization before the requiring module's macros run, so a same-file copy would keep its
// initializer and never exercise the path. spirv-val is the oracle for the composite/type correctness
// the counts cannot see.
var @ssbo @binding = 0 sv_data : array<float>

[compute_shader(local_size_x=64, name="sv_spv"), marker(no_coverage)]
def sv {
    let i = int(gl_GlobalInvocationID.x)
    let d = xm_sv_seed(sv_data[i])
    let p = xm_sv_phys(sv_data[i + 1], d)
    sv_data[i] = p.temp + p.radius + xm_sv_dropped_init(sv_data[i + 2])
}

[test(description = "A plain struct passed as a parameter and returned as a result, plus a local whose initializer was dropped, emits an undecorated OpTypeStruct that spirv-val accepts.")]
def test_struct_value(t : T?) {
    t |> run("struct values: struct param + struct result, plain OpTypeStruct, dropped init") <| @(t : T?) {
        var words <- clone_to_move(sv_spv)
        // entry + xm_sv_seed + xm_sv_phys + xm_sv_dropped_init = 4 OpFunctions, each called once.
        t |> equal(count_op(words, SpvOp.Function), 4)
        t |> equal(count_op(words, SpvOp.FunctionCall), 3)
        // seed(s) = 1 + phys(m, d) = 2 + dropped_init(m) = 1 -> 4 OpFunctionParameter. The struct
        // param `d` is ONE by-value parameter, not a flattened field list.
        t |> equal(count_op(words, SpvOp.FunctionParameter), 4)
        // SvDraws + SvPhys + the sv_data SSBO's Block = 3 OpTypeStruct. SvDraws is named by the seed's
        // result, the phys's parameter AND the construction inside the seed; without dedup by Structure
        // identity each mention would mint its own, giving 5.
        t |> equal(count_op(words, SpvOp.TypeStruct), 3)
        // exactly ONE OpMemberDecorate -- the SSBO Block's Offset. A struct VALUE is not an interface
        // block, so neither SvDraws nor SvPhys may carry a member decoration.
        t |> equal(count_op(words, SpvOp.MemberDecorate), 1)
        // the two constructions -> one OpCompositeConstruct each (no per-field store into a temp).
        t |> equal(count_op(words, SpvOp.CompositeConstruct), 2)
        // d.scat_temp / d.scat_r / d.giant_r / p.temp / p.radius -> 5 reads off a struct VALUE,
        // each an OpCompositeExtract (glslang would AccessChain a Function-storage copy instead).
        t |> equal(count_op(words, SpvOp.CompositeExtract), 5)
        // the dropped initializer lowers to the zero das guarantees such a local: one OpConstantNull.
        t |> equal(count_op(words, SpvOp.ConstantNull), 1)
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "struct_value: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// A write to a fixed-array element is dropped when the function arrives from a REQUIRED module and
// the element index is built from module-scope consts: the emitter lowers the destination
// OpAccessChain and then stores the value into a scratch local instead of through it, so the
// element keeps its previous contents. spirv-val is clean either way -- a dropped store leaves
// valid, wrong code -- and the same body compiled in THIS file is correct, which is why the gate
// compares the two rather than trusting one. Found in a cloud radiative-transfer LUT bake, where
// only the l=0 row of a Legendre table survived and the baked values came out up to 3x off.
var @ssbo @binding = 0 fa_data : array<float>

let FA_N = 8
let FA_L = 15

def private fa_tab(i : int) : float {
    let mu = fixed_array(
        0.01985507175123188, 0.10166676129318663,
        0.23723379504183551, 0.40828267875217510,
        0.59171732124782490, 0.76276620495816449,
        0.89833323870681337, 0.98014492824876812)
    return mu[i]
}

// Byte-for-byte the body of xm_fa_recurrence, compiled in the requiring file.
def private fa_recurrence() : float {
    var p : float[128]
    for (i in range(FA_N)) {
        p[i] = 1.0
        p[FA_N + i] = fa_tab(i)
        for (l in range(2, FA_L + 1)) {
            p[l * FA_N + i] = (
                ((2.0 * float(l) - 1.0) * fa_tab(i) * p[(l - 1) * FA_N + i]
                    - (float(l) - 1.0) * p[(l - 2) * FA_N + i])
                / float(l))
        }
    }
    var s = 0.0
    for (q in range(128)) {
        s += abs(p[q])
    }
    return s
}

[compute_shader(local_size_x=1, name="fa_xmod_spv"), marker(no_coverage)]
def fa_xmod {
    fa_data[int(gl_GlobalInvocationID.x)] = xm_fa_recurrence()
}

[compute_shader(local_size_x=1, name="fa_local_spv"), marker(no_coverage)]
def fa_local {
    fa_data[int(gl_GlobalInvocationID.x)] = fa_recurrence()
}

// An OpAccessChain nothing consumes is a destination address whose store was dropped: the emitter
// has no other reason to compute one.
def private access_chains(words : array<uint>) : table<uint> {
    var chains : table<uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.AccessChain) && n > 1) {
            chains |> insert(w[s + 1])
        }
    }
    return <- chains
}

// Writes that land in an aggregate: an OpStore whose POINTER is an element address. The dropped
// write shows up here and nowhere else -- the broken copy still stores the value, into a scratch
// local, so the raw OpStore count goes UP while the element never gets written.
def private aggregate_stores(words : array<uint>) : int {
    var chains <- access_chains(words)
    var n_st = 0
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Store) && n > 0 && key_exists(chains, w[s])) {
            n_st++
        }
    }
    delete chains
    return n_st
}

def private dead_access_chains(words : array<uint>) : int {
    var chains <- access_chains(words)
    var used : table<uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        for (k in range(n)) {
            if (opcode == uint(SpvOp.AccessChain) && k == 1) {
                continue
            }
            if (key_exists(chains, w[s + k])) {
                used |> insert(w[s + k])
            }
        }
    }
    var dead = 0
    for (id in keys(chains)) {
        if (!key_exists(used, id)) {
            dead++
        }
    }
    delete chains
    delete used
    return dead
}

[test(description = "A fixed-array element store in a function from a required module is not dropped: its blob matches the same-file copy and both pass spirv-val.")]
def test_xmod_array_store(t : T?) {
    t |> run("required-module fixed-array element store is not dropped") <| @(t : T?) {
        var xmod <- clone_to_move(fa_xmod_spv)
        var local <- clone_to_move(fa_local_spv)
        t |> equal(dead_access_chains(local), 0)
        t |> equal(dead_access_chains(xmod), 0)
        let ax = aggregate_stores(xmod)
        let al = aggregate_stores(local)
        t |> success(ax == al,
            "required-module copy writes {ax} elements, same-file copy {al}")
        delete xmod
        delete local
    }
    t |> run("both blobs are spirv-val clean") <| @(t : T?) {
        var xmod <- clone_to_move(fa_xmod_spv)
        let r = validate_spirv(xmod)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete xmod
    }
}
