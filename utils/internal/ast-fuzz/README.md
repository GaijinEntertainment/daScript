# ast-fuzz — random AST generator + verifier

Two tools for hardening the daslang compiler (and your own macros) against
malformed-but-plausible ASTs:

- **generator** — builds random AST straight from the node definitions and asks
  *does the compiler crash?*
- **verifier** (`daslib/ast_verify`) — turns a crash-class malformation into a
  clean diagnostic instead of a segfault several passes later

## Generator

```
daslang utils/internal/ast-fuzz/main.das -- --bin bin/daslang --seeds 50
```

Each seed writes a victim that is nothing but `def main { }` plus the options
that turn the generator on. An `[infer_macro]` (`_ast_synth.das`) then adds whole
functions with random signatures, the structures / enumerations / globals their
types point at, and a generated body for each — so nothing about the program is
hardcoded. It compiles in a **subprocess**, the only way to attribute a crash to
the seed that caused it, since a crash kills the process. A normal compiler error
is fine; an abnormal exit (signal / abort) is a compiler bug.

It **generates** rather than mutating parsed trees on purpose. The parser reaches
only a subset of legal shapes, and a mutation set mostly produces programs the
type checker is designed to reject — measured: 800 mutation runs, 645 valid
samples, zero crashes. Generation found crashes on the first probe.

### What it covers

`_ast_synth_all.das` is **generated from the AST headers** by
`gen_invariants.das`, so node coverage cannot drift from the definitions: every
constructible `Expression` subclass (the abstract bases are skipped), with every
child slot filled recursively.

`TypeDecl` coverage includes every payload-bearing family — `tStructure`,
`tEnumeration`/`8`/`16`/`64`, `tHandle`, `tDistinct`, `tFixedArray` (with
`fixedDim`), `tBitfield`/`8`/`16`/`64` (with `argNames`), the 16/8-bit lattice
vectors, and `fakeContext` / `anyArgument` / `option`.

Four axes are opt-in, because each leaves parser-shaped AST behind:

| Flag | What it adds |
|---|---|
| `--synth-funcs N` | N whole functions with random signatures and bodies |
| `--synth-decls N` | N structures, enumerations and globals, so a named `TypeDecl` carries a real payload instead of a null one |
| `--synth-bind` | binds resolved pointers directly (`ExprVar.variable`, `ExprCall.func`, `ExprAddr.func`, `ExprConstEnumeration.enumType`, `ExprField.fieldIndex`) the way a macro does, rather than leaving the name for inference |
| `--synth-pretype` | pre-sets `Expression._type`; infer treats a typed node as already inferred, so a wrong type reaches codegen |

Handled-type annotations come from a fixed list of real types (`HANDLED_TYPES` in
`_ast_synth.das`) rather than being enumerated: `module_find_type_annotation`
`static_cast`s whatever `findAnnotation` returns, so asking it for `export` would
hand back a *function* annotation reinterpreted as a type annotation — a pointer
no source can produce, whose crashes would not count.

Extending the type space this way found two crash classes and both are now
checked: a `tHandle` with a null `annotation`, and a `tDistinct` without a
`DistinctTypeAnnotation`.

Not generated: annotation declarations on functions/structures, shared nodes (a
DAG) or cycles, and multi-module programs.

`_ast_synth_all.das` is a **build product, not a source** — gitignored (see the
root `.gitignore`), and written on first driver run. Its static half lives in
`gen_prelude.das.in`; only the per-kind dispatch is generated. Regenerate with:

```
daslang utils/internal/ast-fuzz/gen_invariants.das
daslang utils/internal/ast-fuzz/gen_invariants.das -- --verify
```

A saved crash repro run directly needs it present, so run the driver once in a
fresh checkout before replaying one.

### Shrinking a crash

The seed reproduces the whole run, so reduce from there:

- `--synth-keep N` truncates each body to its first N statements **after**
  generation, so the kept statements stay byte-identical.
- `--synth-prune N` replaces the Nth generated node with a constant; bisect N to
  find the smallest subtree the crash needs.
- `--synth-skip Kind1,Kind2` drops node kinds from the pool; bisect to identify
  which kind the crash requires.
- `--synth-only <fn>` replaces one body while still counting every match, so the
  per-function seeds — and therefore every other body — are unchanged.
- `--synth-dump` prints each synthesized body as source.

### CRASH vs TIMEOUT vs resource

A run is `CRASH` only on a definitive abnormal exit (a `CRASH:` banner, signal,
or abort). A **timeout** is reported separately, because it is ambiguous: a real
hang *or* just a slow compile. Re-run with a larger `--timeout` to tell them
apart. Hitting the `--memcap` ceiling is reported as `resource`: unbounded
allocation on a pathological input is not a compiler crash.

## Verifier (`daslib/ast_verify`)

Reports a clean diagnostic when an AST violates a structural invariant, instead
of letting the compiler crash on it several passes later:

```
$ daslang t.das
CRASH: SIGSEGV (Segmentation fault) (signal 11) at address 0x30

$ daslang --ast-verify t.das
AST verify: let variable 'i' has no type (Variable._type is null) at t.das:6:9
```

Three ways in:

- **CLI:** `daslang --ast-verify foo.das` force-includes the module (the way
  `-jit` pulls in its daslib support). `--ast-verify-batch` is the CI gate form:
  no pre-infer checks, post-infer over the module being compiled (plus each
  other module once per process) — the finished tree is checked, at a fraction
  of the cost; a tree a macro breaks mid-inference then crashes the compiler
  instead of being reported, so the CI/preflight gates count a crash as red.
- **Source:** `require daslib/ast_verify`.
- **Inline:** call `verify_module` / `verify_program` at the end of your own
  macro's `apply()`, right after building AST. `verify_expression` is for macros
  that build and *return* a subtree (for-loop / call / annotation / reader),
  which run inside inference where the module-level form cannot see the result
  yet.

The check set is a denylist of shapes that **crash** the compiler, not everything
that looks malformed. A shape the compiler tolerates (a null statement in a
block) or legitimately produces (a bare `return` in a void function, an `if` with
no `else`, `with (module x)` having no subject) is deliberately not flagged.

### Node uniqueness

An `Expression` or a `TypeDecl` reachable from two parents is a missing
`clone_expression` / `clone_type` — a compiler or macro bug that surfaces later
as a double edit or a double free, not as a segfault. Each such node is reported
once per walk, and an aliased type is unshared by cloning it at the slot the
visitor can replace (the outermost one — nested `firstType` / `argTypes` are
reported only). `options _ast_verify_unique = false` turns the check off for a
module that legitimately shares nodes.

Expression types (`expr.type`) are not walked by the visitor either, so the pass
records them as whole trees from `preVisitExpression`, along with the type slots
the expression visits skip (`ascType`, `recordType`, `bitfieldType`,
`assumeType`, a non-closure block's `returnType`) and the `typeMacroExpr` /
`fixedDimExpr` payloads. A slot the walk itself enters is left alone —
`ExprMakeLocal::visit` walks `makeType`, so recording it here would read as an
alias of the node the walk is about to reach.

Quote bodies count too: `ExprQuote::visit` descends only when a visitor answers
`canVisitQuoteSubexpression`, which this pass does.

### gc-root reachability (off by default)

`verify_module_gc_root` reports every gc-tracked node a module owns that a full
walk of its AST never reaches — an orphan, or a node in a slot no visitor walks.
It runs from a `[post_compile_macro]`, the first hook after the compiler collects
the module root; before that collect the root still holds the garbage inference
left behind, so every node there reads as unreachable.

Enable it with `options _ast_verify_gc_root = true`. It stays off because
`gc_collect` deliberately keeps nodes no `Visitor` walks — `daslib/quote.das`
carries a field blacklist naming three (`Function.inferStack`,
`ExprReturn._block`, `ExprVar.pBlock`) — so gc-reachable and visitor-reachable
are not yet the same set, and real files still report a handful of nodes.

### Why every check repairs

`Expr*::visit` in `src/ast/ast.cpp` dereferences children without null checks:

```cpp
vis.preVisit(this);
subexpr = subexpr->visit(vis);   // <- no guard
```

The verifier walks with that same visitor, so an unrepaired null crashes the
*verify pass* and reports nothing. Every check therefore repairs in `preVisit`,
which runs before the descent.

Coverage of that is mechanical, not assumed: `gen_invariants.das` parses
`src/ast/ast.cpp`, collects every unguarded `child->visit(vis)` (skipping loop
variables and names guarded earlier in the body), and **fails** if any lacks a
repair. Currently 56 derefs, 56 repaired. A new deref upstream is a hard error,
not a silent blind spot.

This matters most for *inherited* slots, because the visitor dispatches on the
**concrete** kind: `preVisitExprOp2` never fires for an `ExprCopy`, and
`preVisitExprField` never fires for an `ExprSafeField`, so each derived kind
needs its own override.

### When the verifier runs

Two compiler-side hooks, both no-ops unless the module is loaded:

- **once per inference pass** (`ast_infer_type.cpp`). Macros that run *inside*
  inference (for-loop / call / variant / reader) substitute a subtree that is
  consumed in the same pass, and at the substitution site it is not yet installed
  in the module — so checking there sees the old tree. Checking per pass, with
  the tree in place, is what catches them.
- **once inference is done** (`ast_parse.cpp`), as a `[post_infer_macro]`, where a
  function's inferred result type is final. That is the only point at which a
  bare `return` in a non-void function is decidable: the infer-time twin has to
  skip an unresolved result type, because a function's result keeps moving until
  the last pass and firing there reports valid code.

  It deliberately does NOT check "every expression carries a type". Walking the
  AST cannot tell what codegen will actually emit — a generic instance can hold
  an untyped constant that `SimulateVisitor` never touches (`daslib/archive`'s
  per-field `apply` blocks are one) — so that check only ever produced false
  positives.

A macro that re-breaks the same node every pass never converges: the verifier
repairs, the macro breaks it again, and the compile ends in `error[30507]: type
inference exceeded maximum allowed number of passes`. That is the macro's bug —
break the node once.

## Self-test

```
daslang dastest/dastest.das -- --test ./utils/internal/ast-fuzz
```

It also runs in CI: `run_utils_tests` covers this directory, and an
`extended_checks` step compiles every file under `tests/` with `--ast-verify-batch`
and fails on any report, crash, or timeout.

1. the verifier reports a deliberately-nulled `ExprOp2.right` without crashing
2. `--ast-verify` is active and silent on valid code (false-positive guard)
3. every unguarded child deref in the visitor has a repair (the gate above)
4. the driver generates, compiles and classifies
5. `--ast-verify-batch` still reports a tree broken after inference, is silent
   on valid code, and runs no pre-infer walk (`_mid_pass.das`: plain mode
   reports the mid-pass null and repairs it; batch mode dies or reports it
   post-infer, never silently)
