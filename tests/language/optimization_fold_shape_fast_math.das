options gen2
options rtti
options fast_math = true

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require dastest/testing_boost public
require strings
require daslib/strings_boost
require _optimized_body_helper

// structural FIRED proof for the fast_math-gated folds (x*0 / x-x on FP, reciprocal
// division, NaN-compare flips, reassociation). the behavioral half lives in
// optimization_fast_math.das.

[export]
def target_mul_zero_f(f : float) : float => f * 0.0

[export]
def target_sub_self_f(f : float) : float =>
    f - f  // nolint:LINT007 -- deliberate: the fold under test

[export]
def target_sub_self_swizzle_f(v : float2) : float =>
    v.x - v.x  // nolint:LINT007 -- deliberate: sameAs-shape fold under fast_math

[export]
def target_rcp(f : float) : float => f / 4.0

[export]
def target_flip_f(f : float) : bool => !(f < 4.0)

[export]
def target_reassoc_f(f : float) : float => (f + 1.0) + 2.0

def has(s, sub : string) : bool => find(s, sub, 0) >= 0

[test]
def test_fast_math_folds_fired(t : T?) {
    ast_gc_guard() {
        t |> run("f*0.0 becomes the zero constant") @(t : T?) {
            let b = body_of(t, "target_mul_zero_f")
            t |> success(!has(b, "*"), "no `*` survives: {b}")
            t |> success(has(b, "return 0f"), "constant zero returned: {b}")
        }
        t |> run("f-f becomes the zero constant") @(t : T?) {
            let b = body_of(t, "target_sub_self_f")
            t |> success(!has(b, "-"), "no `-` survives: {b}")
        }
        t |> run("v.x-v.x becomes the zero constant") @(t : T?) {
            let b = body_of(t, "target_sub_self_swizzle_f")
            t |> success(!has(b, "-"), "no `-` survives: {b}")
        }
        t |> run("f/4.0 becomes multiply by reciprocal") @(t : T?) {
            let b = body_of(t, "target_rcp")
            t |> success(!has(b, "/"), "no `/` survives: {b}")
            t |> success(has(b, "*"), "reciprocal multiply present: {b}")
        }
        t |> run("!(f<4.0) becomes the flipped compare") @(t : T?) {
            let b = body_of(t, "target_flip_f")
            t |> success(!has(b, "!"), "no `!` survives: {b}")
            t |> success(has(b, ">="), "flipped compare present: {b}")
        }
        t |> run("(f+1)+2 regroups to f+3") @(t : T?) {
            let b = body_of(t, "target_reassoc_f")
            t |> success(has(b, "3f"), "merged constant present: {b}")
            t |> success(!has(b, "2f"), "original tail constant gone: {b}")
        }
    }
}
