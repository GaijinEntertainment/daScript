options gen2
options indenting = 4
options no_aot     // requires the no_aot _spirv_xmod fixture (optimizer-folded shape)

// A write to a fixed-array element is dropped when the function arrives from a REQUIRED module and
// the element index is built from module-scope consts: the emitter lowers the destination
// OpAccessChain and then stores the value into a scratch local instead of through it, so the
// element keeps its previous contents. spirv-val is clean either way -- a dropped store leaves
// valid, wrong code -- and the same body compiled in THIS file is correct, which is why the gate
// compares the two rather than trusting one. Found in a cloud radiative-transfer LUT bake, where
// only the l=0 row of a Legendre table survived and the baked values came out up to 3x off.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require _spirv_xmod                 // xm_fa_recurrence -- the required-module copy
require spirv/spirv_shader
require spirv/spirv_builtins public
require spirv/spirv_dis
require spirv/spirv_grammar
require math

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

[test]
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
