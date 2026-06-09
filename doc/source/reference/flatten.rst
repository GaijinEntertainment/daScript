.. _flatten:

==========================================================================
Function flattening — ``[flatten]`` for branchless / callless backends
==========================================================================

``daslib/flatten`` rewrites a function into a semantically equivalent
**branchless, call-free twin** ``<name>_flat``. Every user call is inlined
away, all control flow is lowered to predicated ``cond ? a : b`` selects, and
early ``return`` becomes a runtime *live mask*. The output contains no user
calls, no branches, and no loops.

This exists for backends that cannot represent calls or dynamic control flow —
the canonical case is a **GPU shader-graph compiler** whose IR is pure
dataflow (leaves are primitive nodes, accumulators are rebindable locals, the
only conditional is a *select* node). On such a backend a user function or an
``if`` is not slow — it is *unrepresentable*. Flattening is the front-half that
makes those constructs expressible: the backend only has to add a *select*
opcode (and a bool-mask local) to consume the flattened form.

Flattening is **not** an optimization for the interpreter / AOT / JIT tiers —
those already inline downstream via clang / LLVM. It is a correctness /
expressibility prerequisite for the branchless target.

.. note::

   The original function is left intact. ``[flatten]`` declares the twin early
   (so its symbol exists for inference and callers) and transforms the twin's
   body across the compiler's re-infer passes. Call ``foo_flat`` exactly like
   ``foo``.

Overview
========

.. code-block:: das

    options gen2
    require daslib/flatten

    [flatten]
    def shade(x : float) : float {
        if (x > 0.5) {
            return x * 2.0
        }
        return x
    }

    // generated twin (conceptually):
    //   def shade_flat(x : float) : float {
    //       return (x > 0.5) ? (x * 2.0) : x
    //   }

The ``if`` became a select; the two ``return`` statements collapsed into the
twin's single tail return. A real backend then walks ``shade_flat`` and emits a
*compare* node feeding a *select* node — no branch required.

The transform
=============

Flattening runs as a small set of compiler-driven phases. Each phase is a
single shallow pass over the (uninferred) twin body; the compiler re-infers
between phases, so types stay clean at every boundary.

**Predicated lowering** threads two pieces of state down the body:

- a **structural predicate** — the AND of the enclosing ``if`` conditions on
  the current path (``null`` means statically ``true``);
- a **live mask** — a ``bool`` local that tracks "are we still executing this
  frame", narrowed by ``return``.

The effective write predicate at any point is ``liveMask && structPred``. The
core rules:

- ``if (c) A else B`` hoists ``let c = …`` once, then lowers ``A`` under
  ``structPred && c`` and ``B`` under ``structPred && !c`` — both straight-line.
- ``lhs = rhs`` becomes ``lhs = P ? rhs : lhs`` (the select is elided when ``P``
  is statically ``true``, so straight-line code stays clean).
- ``return e`` becomes ``__ret = P ? e : __ret`` plus a narrow of the live mask
  on the taken path, so later statements see the mask go false.
- A **user call** is inlined: its parameters bind to value temps, it gets its
  own frame live mask seeded from the caller's predicate, and its body is
  lowered recursively. Inlining is transitive.
- A **whitelisted leaf primitive** (see below) is kept, its arguments lowered.

**Loop unrolling** turns a fixed-count ``for`` (over a constant ``range`` /
``urange`` or an array literal) into straight-line copies — the loop variable is
substituted by each iteration's constant and each copy is lowered under the same
predicate. Per-iteration locals are renamed so the copies don't collide. Parallel
multi-source loops (``for (a, b in xs, ys)``) unroll in lockstep — every source
must have the same constant length — substituting each loop variable per copy.

**Generated locals are reserved-namespaced.** Every flatten-introduced local takes a
``__``-prefixed name — ``__flat_*`` for the lowering scaffold (live masks, value temps,
unrolled-loop locals) and ``__ssa_*`` for the single-assignment versions of reassigned
user locals. The language reserves the ``__`` prefix (a user cannot declare such a name),
so the generated names never collide with anything in the source.

**break / continue** lower to predication, not jumps:

- ``break`` narrows a **loop-scoped mask** that is declared once and *persists
  across the unrolled copies* — once a copy breaks, the rest of it and every
  later copy mask off.
- ``continue`` narrows a **per-iteration mask** that is re-declared ``= true``
  at the head of each copy — the rest of *that* copy masks off, the next copy
  resets.

Both compose with the function live mask, so a ``return`` inside a loop and an
inlined callee's own early ``return`` interact correctly (a callee return never
kills the caller's masks).

**Output cleanup** — a dead-store / constant-propagation / copy-propagation
pipeline collapses the mask scaffolding. On a function with no surviving runtime
mask (e.g. one with no early return), the live mask folds to constant ``true``
and disappears entirely, so a branchless input flattens *byte-identical* to its
source. A post-inference fold collapses ``const ? a : b`` selects to the live
arm and drops the pure self-assigns a folded false-select leaves behind.

**Constant folding.** Because the branchless target has no downstream optimizer,
the twin is reduced as far as possible *before* the backend sees it. The
post-inference fold applies algebraic identities — ``x*1``, ``x+0``, ``x-0`` and
``x*-1`` — over the full **scalar + vector (float / int / uint)** family, with the
constant operand either a scalar or an all-lanes-equal vector literal (``x * float3(1)``,
``v + int3(0)``, ``uint3(0) - w``). Each returns the non-constant operand, gated on a
matching result type so a scalar-broadcast ``s * float3(1)`` (which is ``float3(s,s,s)``,
not ``s``) is left intact rather than collapsing to the scalar. ``x*0`` returns a zero of
the **result type**, so both ``v * 0`` and ``v * float3(0)`` fold to a width-matched vector
zero (``*-1`` is signed-only — an unsigned "-1" is not a negation factor). It also folds
the boolean ``true && x``, ``c ? true : false``, ``!const``; collapses constant vector
constructors (``float3(1, 2, 3)``, ``uint3(1, 2, 3)``) and const-argument pure builtins
(``float(7)``, ``min(2, 3)``) to literals; and constant-propagates single-definition
locals — so a fully-constant accumulator loop reduces to its final constant. It also
**reassociates** scattered constants in commutative ``+``/``-`` and ``*`` chains —
``0.5 + x + 0.6 → x + 1.1``, ``2 * x * 3 → x * 6`` — gathering the constant operands the
general compiler leaves non-adjacent (it folds only *adjacent* constant arithmetic and never
reassociates, for float rounding) into one adjacent group the all-const fold then collapses.
The same pass sorts the chain's *variable* terms into a canonical (structural) order, so
commutative chains converge to one form (``b + a`` and ``a + b`` both become ``a + b``) —
gated on every term being side-effect-free, and intended to feed a later common-subexpression
pass. Integer ``+``/``*`` reassociate exactly; integer ``/`` is non-associative and excluded.
These are exactly the folds the general compiler leaves for the downstream tiers (it folds
constant *arithmetic* but not constant *constructors*, and never a runtime-operand identity
or reassociation), done here under a shader's fast-math assumption (``x*0 → 0`` always fires).

The fold and the typer's const-fold are *mutually-enabling*, so the fold phase
**iterates to a fixpoint**. Flattening folds the runtime-operand identities the
typer will not (``0*b → 0``); the re-infer between passes then const-folds the
freshly-constant operands the fold does not touch (``24 >> (24 & 31) → 0``),
which can expose a fresh identity (``x - 0``) for the next pass. A single pass is
therefore not enough — the fold re-runs until nothing changes before the twin is
handed to the backend.

**Preshader extraction and CSE.** Once the fold fixpoint converges, a final
optimize pass runs **once** on the canonical body — preshader extraction then
common-subexpression elimination (``flatten_optimize``):

* **Preshader extraction** colours each subtree *uniform* (it reads only material
  props / shader globals + literals) or *varying* (it transitively reads a function
  parameter). Every maximal uniform subtree is hoisted to a top-of-body
  ``_preshader_N`` ``let``; a backend recognises the name prefix and routes the node
  to the **per-draw preshader**, so uniform work the general compiler would re-run
  per pixel runs once per draw. Sampler / procedural intrinsics (``tex2d``, ``noise``)
  are *barriers* — they cannot run in a preshader, so a subtree containing one stays
  per-pixel even when its inputs are uniform.
* **CSE** value-numbers the (now-canonical) body by structural key and shares any
  pure subtree computed twice or more into one ``let`` before its first use, so the
  backend emits one graph node and *N* links instead of *N* recomputations. The
  reassociation pass's canonical operand order is what makes the key match across
  ``a + b`` and ``b + a``; a uniform repeat routes to the preshader
  (``_preshader_cse_``), a varying one stays in the body (``_cse_``).

Both passes are **value-exact** — they only hoist and share existing subtrees, never
regroup or round — and both exclude any subtree that reads a **reassigned** variable
(a live/loop mask or a written global), whose value is not stable across the body.

Supported subset
================

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Construct
     - Lowering
   * - ``if`` / ``if … else`` (any nesting)
     - dual-arm predicated ``?:`` selects under the path predicate
   * - early / multiple ``return``
     - function live mask + a single tail return of the result temp
   * - user function call
     - inlined transitively; parameters bound to value temps
   * - fixed-count ``for``
     - unrolled — ``range(CONST)``, ``range(A, B)``, ``urange(…)``, or an array literal ``[a, b, c]``
   * - parallel multi-source ``for (a, b in xs, ys)``
     - unrolled in lockstep; every source must share one constant length
   * - ``break`` / ``continue``
     - loop-scoped (persistent) / per-iteration (per-copy) bool masks
   * - ``+=`` ``-=`` ``*=`` … and ``++`` / ``--``
     - rewritten to a predicated select on the base operation
   * - bare block ``{ … }`` / ``with (x) { … }``
     - flattened in place / unwrapped to its body
   * - copyable value assignment
     - selects support scalars, vectors (``float4`` …), structs and tuples
   * - ``void`` functions
     - predicated writes to output globals (the writes-to-globals shader model)

Boundaries (loud errors)
========================

Everything outside the supported subset is rejected with a specific
``flatten:`` error rather than emitting wrong output:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Rejected
     - Why
   * - ``while`` loops
     - no compile-time iteration count — not unrollable
   * - ``for`` over a non-constant range
     - same — the bound must be a constant ``range`` / ``urange`` or an array literal
   * - parallel ``for`` sources of unequal length
     - lockstep unroll needs one shared count; mismatched lengths are rejected
   * - move ``<-`` / clone ``:=``
     - predication is copy-only; a move/clone cannot be predicated
   * - recursion
     - cannot inline a self-referential call
   * - a non-whitelisted leaf builtin
     - not provably pure / GPU-expressible (e.g. ``print``, allocation, I/O)
   * - a non-copyable type at an inline boundary
     - ``array<T>``, ``table``, ``lambda`` — not copy-init-able into a select

The leaf-primitive whitelist
============================

Inlining stops at **leaf primitives** — functions the backend implements
natively and flatten must *keep* (not inline). These must be pure,
branch-safe, and backend-expressible, since under predication both arms of a
select are conceptually evaluated. The whitelist is **passed in by the backend**
so it names exactly its own primitive set; the thin ``[flatten]`` annotation
passes a standard pure-math default.

A whitelisted name is matched by its *simple* (un-mangled) name, so an
OR-typed primitive (e.g. ``saturate(x : float | int)``) is kept regardless of
which monomorphization the call resolved to.

Public API
==========

``[flatten]``
    The thin annotation. Generates ``<name>_flat`` with the standard whitelist.
    Use it when you just want a flattened twin to call.

``flatten_function(var func, whitelist : table<string>) : FlatCtx?``
    Flattens ``func``'s body in place using the caller-supplied primitive set.
    Backends call this directly with their own ``[hint]`` primitives, before
    their own macro walks the result. Inspect ``.ok`` / ``.err``. The body is
    uninferred afterward — re-infer before reading types.

``flatten_fold(var func) : bool``
    The post-inference fold pass: collapse ``const ? a : b`` selects, drop the
    pure ``lhs = lhs`` self-assigns, strip redundant zero initializers. Run it
    *after* re-inference (the constant conditions must already be folded to
    ``ExprConstBool``). ``[flatten]`` runs it automatically; a backend calling
    ``flatten_function`` runs it after its own re-infer, before consuming the
    twin. A single call is **one shallow pass — it is not self-converging**.
    A multi-step *reveal* cascade (a folded ``0*s → 0`` makes a local constant
    that the next re-infer settles for const-prop, exposing a fresh ``v + 0``
    identity) only fully collapses if the backend **re-enters the fold across
    re-inference until it reports no change** — exactly as ``[flatten]``'s own
    patch does (and as the ``[pixel_shader]`` backend now does).

``flatten_fold_residuals(var func) : array<string>``
    Test-framework / fuzzer introspection: walks a compiled twin's final body
    and returns a description for each const-foldable residual a complete fold
    should have collapsed — an unconditional algebraic identity (``x*1``,
    ``x+0``, …), an all-const foldable call, a const-condition select, or
    ``!const``. Empty means clean. It runs over the *compiled* output (not at
    transform time), so it covers backend paths such as ``[pixel_shader]`` and
    is the fold-completeness oracle for both ``tests/flatten/test_flatten_fold.das``
    (the ``[flatten]`` + const-identity corpus and every example shader) and the
    ``flatten-fuzz`` ``--strict-fold`` mode. This is the *only* fold-completeness
    check — there is no compile-time flag; a missed fold is suboptimal, not an
    error, so it is validated by tests rather than gated in the macro.

``flatten_optimize(var func, barriers : table<string>) : bool``
    The post-fold optimize pass: hoist maximal uniform subtrees to per-draw
    ``_preshader_`` lets, then CSE-dedup repeated subtrees. Run it **once** after
    the ``flatten_fold`` fixpoint converges (and *not* before — reassociation runs
    in the fold and would reorder a hoisted reference back into a fresh uniform
    subtree). ``barriers`` is the backend's sampler / intrinsic call-name set
    (``{ "tex2d", "noise" }``) — those stay per-pixel. ``[flatten]`` runs it
    automatically; the ``[pixel_shader]`` backend runs it after its fold fixpoint.

``flatten_opt_residuals(var func) : array<string>``
    The optimize-completeness oracle (sibling of ``flatten_fold_residuals``):
    walks a compiled twin and returns a description for each missed optimization —
    a maximal uniform subtree still inline in the varying body, or a pure subtree
    computed twice or more left un-shared. Empty means complete. Drives the same
    ``tests/flatten/test_flatten_fold.das`` corpus and the ``flatten-fuzz``
    strict mode.

A backend's typical pipeline is therefore: ``flatten_function`` → re-infer →
``flatten_fold`` (to a fixpoint) → re-infer → ``flatten_optimize`` → re-infer →
walk the now-branchless, call-free twin and emit its dataflow graph (mapping
``?:`` to a *select* node and the bool masks to a 0/1 selector).

.. seealso::

    Worked end-to-end examples — real shaders flattened through a stand-in
    shader-graph backend, plus a capability tier (if/else, helpers, loops,
    ``break`` / ``continue``) — live under ``examples/flatten/`` in the source
    tree.
