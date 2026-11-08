options gen2

require dastest/testing_boost public
require daslib/flatten
require daslib/flatten_opt_common
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require strings
require daslib/templates_boost

//! Regression for the helper-inline swizzle bug. flatten inlines a
//! helper that reads a by-value VECTOR parameter through a
//! single-component swizzle (`p.x`). copy_prop then substitutes the
//! call argument into the swizzle base. That argument is a VALUE —
//! arithmetic, or the value-copy of a ref — so the swizzle becomes
//! non-ref, while the `p.x`-of-a-ref-local read it replaced still
//! carried an ExprRef2Value. Before the typer folded
//! ref2value-of-a-value (ast_infer_type.cpp), the generated `_flat`
//! twin failed to compile with error[30921] "can only dereference a
//! reference". Reported as the shaders heal_3d / force_field
//! (examples/flatten); these cover the same shapes in plain flatten.
//! The file COMPILING proves the twins build; the equal() checks
//! prove equivalence.

// helper whose by-value vector param is read via single-component
// swizzles
def axis_mix(p : float3) : float => p.x * 0.5 + p.y - p.z * 0.25

// value source: the call argument is arithmetic (force_field's
// `(scrolledPos * gridScale).x`)
[flatten]
def inl_swizzle_val(s : float) : float => axis_mix(float3(s, s * 2.0, s - 1.0) * 1.5)

// ref source: the argument is a (by-value) vector param; the `var
// __flat_arg = v` value-copy still feeds a non-ref swizzle base after
// copy_prop (heal_3d's `inp.worldPos`)
[flatten]
def inl_swizzle_ref(v : float3) : float => axis_mix(v)

// loop-unroll + helper inline, accumulating swizzled reads (heal_3d's
// per-seed loop)
def hatch(p : float3; k : float) : float => p.y * k + p.x

[flatten]
def inl_swizzle_loop(base : float3) : float {
    var acc = 0.0
    for (i in range(3)) {
        acc += hatch(base * float(i + 1), float(i) * 0.5)
    }
    return acc
}

var GRID <- [-7.0, -1.0, 0.0, 1.0, 2.5, 7.0]

[test(description = "a helper reading a vector parameter through a swizzle inlines into a twin that compiles and matches its original")]
def test_flatten_inline_swizzle(t : T?) {
    t |> run("helper swizzle of a value-source vector arg") @(t : T?) {
        for (v in GRID) {
            t |> equal(inl_swizzle_val_flat(v), inl_swizzle_val(v))
        }
    }
    t |> run("helper swizzle of a ref-source vector arg") @(t : T?) {
        for (v in GRID) {
            let p = float3(v, -v, v * 0.5)
            t |> equal(inl_swizzle_ref_flat(p), inl_swizzle_ref(p))
        }
    }
    t |> run("helper swizzle inlined across an unrolled loop") @(t : T?) {
        for (v in GRID) {
            let p = float3(v, -v, v * 0.5)
            t |> equal(inl_swizzle_loop_flat(p), inl_swizzle_loop(p))
        }
    }
}

// `collect_mutable` is what tells CSE and the preshader which names
// are value-stable, so every spelling a store can take has to reach
// it. A store form it cannot see is a name CSE believes constant: it
// shares a subexpression straight across the mutation and the twin
// computes the pre-store value twice. `_mut_fixtures.das` carries one
// local per spelling.
//
// Lives in tests/flatten/no_aot/ (excluded from test_aot via
// tests/.das_test): it compiles the fixture at runtime, which pulls
// macro_boost generic instantiations that don't AOT-link.

def private fixture_mutables(t : T?; fnName : string) : table<string> {
    var res : table<string>
    let path = "{get_das_root()}/tests/flatten/no_aot/_mut_fixtures.das"
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.version_2_syntax = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; iss) {
                if (!ok) {
                    t |> failure("compile failed: {path}\n{iss}")
                    return
                }
                program_for_each_module(program) $(mod) {
                    for_each_function(mod, "") $(func) {
                        if (func.body == null || func.flags.builtIn || "{func.name}" != fnName) {
                            return
                        }
                        var m <- collect_mutable(func.body as ExprBlock)
                        for (k in keys(m)) {
                            res |> insert(k)
                        }
                        delete m
                    }
                }
            }
        }
    }
    return <- res
}

def private assert_arg_guard(t : T?; fnName : string) {
    let path = "{get_das_root()}/tests/flatten/no_aot/_mut_fixtures.das"
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.version_2_syntax = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; iss) {
                if (!ok) {
                    t |> failure("compile failed: {path}\n{iss}")
                    return
                }
                var seen = false
                program_for_each_module(program) $(mod) {
                    for_each_function(mod, "") $(func) {
                        if (func.body == null || func.flags.builtIn || "{func.name}" != fnName) {
                            return
                        }
                        seen = true
                        t |> equal(length(func.arguments), 2, "mut_out takes a value and a var ref")
                        t |> success(!writes_through_argument(func, 0),
                                     "a by-value parameter is not a write target")
                        t |> success(writes_through_argument(func, 1),
                                     "a 'var T&' parameter is a write target")
                        t |> success(!writes_through_argument(func, -1),
                                     "a negative index answers false, never throws")
                        t |> success(!writes_through_argument(func, 2),
                                     "an index past the last argument answers false")
                    }
                }
                t |> success(seen, "{fnName} must be found")
            }
        }
    }
}

[test(description = "collect_mutable sees every spelling of a store, and writes_through_argument is false off both ends of the argument list")]
def test_flatten_mutcollect(t : T?) {
    t |> run("every store spelling reaches collect_mutable") @(t : T?) {
        var m <- fixture_mutables(t, "mut_forms")
        t |> success(!empty(m), "mut_forms must be found and walked")
        for (name in ["cp", "mv", "src", "cl", "inc", "acc", "lane", "sn", "cs"]) {
            t |> success(key_exists(m, name),
                         "'{name}' is written in mut_forms but is not in the mutable set")
        }
        t |> success(!key_exists(m, "ro"),
                     "'ro' is only read; a read must not enter the mutable set")
        delete m
    }
    t |> run("writes_through_argument answers false off both ends of the argument list") @(t : T?) {
        assert_arg_guard(t, "mut_out")
    }
}

// Targeted test for the scalar-packing pass (`pack_map_reduce`,
// exposed as `flatten_pack`). It compiles _pack_fixtures.das — whose
// `[flat_pack]` annotation flattens a twin, runs the PUBLIC
// `flatten_pack` entry, then folds — and asserts the post-pack body
// structurally: • a real min reduction re-rolls into `_pk*` vector
// vars (the pass FIRED); • a const-threshold select and an
// accumulator-dependent lane do NOT (the fail-closed bailout).
// Structural (not value) so it is tier-independent; the `_pk*` marker
// also regression-guards the classify_link fail-closed gate — if it
// ever re-accepts a threshold, `_pk` reappears and this reds.
//
// Lives in no_aot/ (compiles daslib/flatten at runtime, pulling
// macro_boost instantiations that don't AOT-link in the full-tree
// batch — same reason as test_flatten_fold.das).

// describe() of each `*_packflat` twin produced by compiling `path`,
// keyed by twin name.
def private pack_twins(t : T?; path : string) : table<string; string> {
    var bodies : table<string; string>
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.version_2_syntax = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; iss) {
                if (!ok) {
                    t |> failure("compile failed: {path}\n{iss}")
                    return
                }
                program_for_each_module(program) $(mod) {
                    for_each_function(mod, "") $(func) {
                        if (func.body != null && !func.flags.builtIn
                            && func.name |> ends_with("_packflat")) {
                            bodies["{func.name}"] = "{describe(func.body)}"
                        }
                    }
                }
            }
        }
    }
    return <- bodies
}

[test(description = "flatten_pack re-rolls a min reduction into vector variables and bails out on a const-threshold select and an accumulator-dependent lane")]
def test_flatten_pack(t : T?) {
    let root = get_das_root()
    t |> run("min reduction packs; fail-closed shapes bail clean") @(t : T?) {
        var bodies <- pack_twins(t, "{root}/tests/flatten/no_aot/_pack_fixtures.das")
        // FIRED: the unrolled min spine re-rolls into width-4 vector vars
        // (`_pk*`).
        let mn = bodies?["pk_min4_packflat"] ?? ""
        t |> success(!empty(mn), "pk_min4_packflat twin must be produced")
        t |> success(mn |> find("_pk") >= 0,
                     "min reduction must re-roll into _pk* vector vars: {mn}")
        // BAILED (classification): a const-threshold select is not a min/max
        // reduction.
        let th = bodies?["pk_thresh_packflat"] ?? ""
        t |> success(!empty(th), "pk_thresh_packflat twin must be produced")
        t |> success(th |> find("_pk") < 0,
                     "const-threshold select must NOT pack (fail-closed): {th}")
        // BAILED (independence): a lane reading the accumulator is
        // sequential, not data-parallel.
        let dp = bodies?["pk_dep_packflat"] ?? ""
        t |> success(!empty(dp), "pk_dep_packflat twin must be produced")
        t |> success(dp |> find("_pk") < 0,
                     "accumulator-dependent lane must NOT pack (fail-closed): {dp}")
        delete bodies
    }
}

// Re-entry contract for `flatten_optimize`: a backend may rewrite an
// already-optimized body and call the pass again. The pass seeds its
// `_preshader_N` / `_cse_N` counters from the names already present,
// so a second call must hoist NEW work under FRESH names instead of
// re-minting `_preshader_0` / `_cse_0` (a duplicate-name compile
// error once the body re-infers). The test compiles
// _reentry_fixture.das (its twin keeps one of each name), injects a
// new uniform let and a new duplicated varying pair — the way a
// backend rewrite would — re-runs the pass, and asserts every
// preshader/cse name in the block is still unique.

def private let_name(s : ExpressionPtr) : string {
    if (s is ExprLet && length((s as ExprLet).variables) == 1) {
        return "{(s as ExprLet).variables[0].name}"
    }
    return ""
}

// Partition the block's single-var let names into the two counter
// classes: `_preshader_cse_*`/`_cse_*` belong to the cse counter,
// remaining `_preshader_*` to extraction.
def private collect_class_names(blk : ExprBlock?; var pres : array<string>;
                                var cses : array<string>) {
    for (s in blk.list) {
        let nm = let_name(s)
        if (empty(nm)) {
            continue
        }
        if (nm |> starts_with("_preshader_cse_") || nm |> starts_with("_cse_")) {
            cses |> push(nm)
        } elif (nm |> starts_with("_preshader_")) {
            pres |> push(nm)
        }
    }
}

def private has_dup(names : array<string>) : bool {
    var seen : table<string>
    for (n in names) {
        if (key_exists(seen, n)) return true
        seen |> insert(n)
    }
    return false
}

[test(description = "re-running flatten_optimize on an already optimized body hoists new work under fresh preshader and cse names without colliding with existing ones")]
def test_flatten_reentry(t : T?) {
    let root = get_das_root()
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.version_2_syntax = true
            compile_file("{root}/tests/flatten/no_aot/_reentry_fixture.das", access,
                         unsafe(addr(mg)), cop) $(ok; program; iss) {
                if (!ok) {
                    t |> failure("compile failed: _reentry_fixture.das\n{iss}")
                    return
                }
                var checked = 0
                program_for_each_module(program) $(mod) {
                    for_each_function(mod, "") $(func) {
                        if ("{func.name}" != "reentry_flat" || func.body == null) {
                            return
                        }
                        checked ++
                        ast_gc_guard() {
                            var blk = func.body as ExprBlock
                            var pres1, cses1 : array<string>
                            collect_class_names(blk, pres1, cses1)
                            // fixture sanity: the pipeline run must have left one name of each
                            // class
                            t |> success(!empty(pres1),
                                         "fixture twin must keep a _preshader_ let, got {pres1}")
                            t |> success(!empty(cses1),
                                         "fixture twin must keep a _cse_ let, got {cses1}")
                            // a backend-style rewrite: fresh uniform compute + a fresh duplicated
                            // varying pair
                            var injU = qmacro_expr(${ let _reentry_u = G_REENTRY * G_REENTRY - 3.0; })
                            var injD1 = qmacro_expr(${ let _reentry_d1 = (x - 2.0) * x; })
                            var injD2 = qmacro_expr(${ let _reentry_d2 = (x - 2.0) * x; })
                            blk.list |> emplace(injU)
                            blk.list |> emplace(injD1)
                            blk.list |> emplace(injD2)
                            let nobarriers : table<string>
                            t |> success(flatten_optimize(func, nobarriers),
                                         "re-entered optimize must hoist the injected work")
                            var pres2, cses2 : array<string>
                            collect_class_names(blk, pres2, cses2)
                            t |> success(length(pres2) > length(pres1), "new uniform compute must hoist a fresh _preshader_ let, got {pres2}")
                            t |> success(length(cses2) > length(cses1), "new varying dup must share into a fresh _cse_ let, got {cses2}")
                            t |> success(!has_dup(pres2),
                                         "re-entry minted a colliding _preshader_ name: {pres2}")
                            t |> success(!has_dup(cses2),
                                         "re-entry minted a colliding _cse_ name: {cses2}")
                        }
                    }
                }
                t |> equal(checked, 1, "expected exactly one reentry_flat twin")
            }
        }
    }
}
