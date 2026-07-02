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
