options gen2
options indenting = 4
options no_aot     // the fixture exists to pin the optimizer-folded REQUIRED-module shape; the same
                   // root-vs-required fold asymmetry makes its AOT semantic hash unstable (50101)

// Cross-module shader-helper fixture for test_return_paths. Functions in a REQUIRED module reach
// the emitter OPTIMIZED (required modules finish optimization before the requiring module's macros
// run), and the optimizer folds trailing code after a returning `if` into an `else` — producing
// if/else bodies where BOTH branches return. Keep these shapes verbatim: they pin the real-world
// path (a shared CPU/GPU core module) that same-file fixtures can't reach.

module _spirv_xmod public

require math


def xm_clip(x : float; var out_v : float&) : bool {
    if (x < 0.0) {
        return false
    }
    out_v = x * 2.0
    return true
}

def xm_sign_mag(x : float) : float {
    if (x < 0.0) {
        return -x
    }
    return x + 1.0
}

struct SvDraws {
    scat_r : float
    scat_temp : float
    giant_r : float
}

struct SvPhys {
    temp : float
    radius : float
}

def xm_sv_seed(s : float) : SvDraws {
    return SvDraws(scat_r = s * 2.0, scat_temp = s + 1.0, giant_r = s * s)
}

// A struct-typed PARAMETER. das passes a struct by reference and `let`/no-`var` appends const, so `d`
// arrives as a const ref -- a read-only view, which lowers to a by-value OpFunctionParameter.
def xm_sv_phys(m : float; d : SvDraws) : SvPhys {
    return SvPhys(temp = m * d.scat_temp, radius = d.scat_r + d.giant_r)
}

// The optimizer DROPS an initializer it proves dead -- every path assigns `l` before it is read, so
// `= 0.0` never reaches the emitter and the local arrives with init == null. Same required-module
// asymmetry as the folded returns above: a same-file copy of this keeps its initializer.
def xm_sv_dropped_init(m : float) : float {
    var l = 0.0
    if (m <= 1.0) {
        l = m * 2.0
    } else {
        l = m * 3.0
    }
    return l
}

// A fixed-array element assigned at an index built from module-scope consts, where the same array
// is read back at neighbouring indices. Optimized-arrival reshapes the assignment enough that the
// emitter computed the destination OpAccessChain and never stored through it -- the write vanished
// with no diagnostic. Spelling the bounds as literals instead of consts hides it, so keep the
// consts. Companion same-file copy lives in test_xmod_array_store.das.
let XM_FA_N = 8
let XM_FA_L = 15

def xm_fa_tab(i : int) : float {
    let mu = fixed_array(
        0.01985507175123188, 0.10166676129318663,
        0.23723379504183551, 0.40828267875217510,
        0.59171732124782490, 0.76276620495816449,
        0.89833323870681337, 0.98014492824876812)
    return mu[i]
}

def xm_fa_recurrence() : float {
    var p : float[128]
    for (i in range(XM_FA_N)) {
        p[i] = 1.0
        p[XM_FA_N + i] = xm_fa_tab(i)
        for (l in range(2, XM_FA_L + 1)) {
            p[l * XM_FA_N + i] = (
                ((2.0 * float(l) - 1.0) * xm_fa_tab(i) * p[(l - 1) * XM_FA_N + i]
                    - (float(l) - 1.0) * p[(l - 2) * XM_FA_N + i])
                / float(l))
        }
    }
    var s = 0.0
    for (q in range(128)) {
        s += abs(p[q])
    }
    return s
}
