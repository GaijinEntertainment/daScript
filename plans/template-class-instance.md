# `[template_class_instance]` — class-template reification (stamped instances)

Boris + Claude, 2026-08-08. **Stage 0 (probes) DONE — results below.** Stage 1 = the
daslib + C++ implementation, tests, tutorial (daslib-only, nothing metal/llama). Stage 2 =
the dasLLAMA kernel restamp, planned separately once stage 1 lands; its worklist is the
reification cluster table in `plans/vulkan-on-mac.md` (SqAttn 23→~6 first, then KqMv,
MulMm/MoeMulMm with followup_general #8's A/B measurement discipline, RopeStore).

## The idea

A `class template` (existing grammar) is the shared body; an instance is an ordinary class
that inherits from it. The reifier — a structure macro — flattens the template into the
instance at parse time with substitutions, so every instance gets its OWN specialized copy
of the methods (types baked, constants folded, no virtual hops), and every downstream
structure annotation (`[metal_dispatch]` and friends) sees a finished concrete class.

```das
[|> template_class_instance]                  // rides down to every instance, prepended
class template MoeMulMmT {
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @workgroup ta : KT[2048]                  // KT: unresolved alias = type parameter
    @template_const WIDE : bool = true        // constant parameter, erased at reify
    def run {
        static_if (WIDE) { ... } else { ... } // folds per instance, in infer, for free
        stage_a(wb, kb, il0, tA0)
    }
}

[metal_dispatch(name = "enc_moe_mulmm_k4", pso = "g_moe_mulmm_k4_pso", tg = 64, ...)]
class MetalMoeMulMmK4 : MoeMulMmT {
    typedef KT = float16                      // binds the type parameter (full type grammar)
    override WIDE = false                     // binds the constant (init replacement)
    @ssbo @binding = 0 @role = "weight" ksh : array<float16>
    def override stage_a(wb, kb, il0, tA0 : uint) { ... }   // instance body wins
    [metal_kernel(name = "metal_moe_mulmm_k4_msl")]
    def metal_moe_mulmm_k4 { run() }
}
```

Covers the four dedup axes from the vulkan-on-mac peek with one mechanism: type axis →
typedef; small format delta → `static_if` on a const; big delta → instance method override;
pass axis → the already-landed `kernel=` multi-kernel. Dissolves followup_general #8's
blockers by construction (the whole stamped body is the instance's — K6's cross-iteration
scalars and Q8's advancing pointers live inline; Mx4's prologue is its own `run`).

## Stage 0 — probe results (2026-08-08, this M1, bin/daslang @ bbatkin/profiling-rail)

Everything below is witnessed, not inferred. Probe files lived in the job scratch dir
(gone with the job); the mini-reifier probe is the stage-1 skeleton and is reproduced by
the stage-1 implementation itself.

GREEN — proven and load-bearing for the design:

1. **Structure-annotation `apply` runs at parse time, in written order, sequentially**
   (`parser_impl.cpp:492`); a later macro sees an earlier macro's mutations (added fields
   visible). The "one macro only" restriction is for handled-type annotations, not
   structure macros.
2. **`[|> name]` (RPIPE prefix, `ds2_parser.ypp:1331`) marks an annotation inherited**:
   copied to every child down the parent chain, PREPENDED before the child's own
   annotations (`parser_impl.cpp:473-490`) — so the reifier always runs before the
   instance's dispatch/lens annotations, and instances write zero reifier boilerplate.
   It also fires on the template itself; `st.flags.isTemplate` is the discriminator.
3. **`typedef NAME = <type>` inside a struct/class body is existing grammar**
   (`ds2_parser.ypp:2601` → structure alias). Visible via `get_structure_alias` at apply
   time; infer resolves inherited alias-typed fields against the owning struct's aliases
   (the qmacro_template_class mechanism) — the type axis needs no field rewriting.
4. **`override WIDE = false` parses bare** (no type respell) and replaces the inherited
   field's init (`parser_impl.cpp:446-464` — the error message even says "use override to
   replace initial value instead"). Constants ride the class body, per Boris's call.
5. **Parent fields are already merged into the child at parse** (marked `inherited`,
   `parser_impl.cpp:374`), including fields a macro added to the template earlier.
6. **The Template rules engine does everything the reifier needs pre-infer**: bare field
   reads inside class-method bodies are `ExprVar`s under the parser's `with(self)` wrap,
   so `replaceVariable("WIDE", <init-clone>)` hits them; `replaceStructWithTypeDecl` +
   `replaceTypeWithTypeDecl` retype signatures; `renameVariable` must be added in BOTH
   spellings (`Tem`m` and `_::Tem`m`) because inherited method-pointer field inits hold
   module-qualified `@@_::Tem`m` targets — same as `apply_qmacro_template_class:1401`.
   Witnessed: a cloned body carrying `static_if(false)` with the substituted literal.
7. **Method clone + rename + classParent retarget + field-init repair all work** with the
   existing machinery (`clone_function`, `apply_template`, `add_function`); the child's
   own `def override` methods win (skip-if-owned); the ctor and the `'`-spelled
   `Tem'__finalize` are skipped (the child parser already generated its own).
8. **Struct rail is FULLY GREEN end-to-end**: reify + const-field erase
   (`st.fields |> erase(i)` works) + `parent = null` + correct runtime values. Clearing
   parent also sidesteps the debug-info verify `!st.isTemplate`
   (`ast_debug_info_helper.cpp:188`) that a kept template-parent trips.
9. **`static_if` needs NO reifier folding** (Boris's position, now with evidence): MSL
   emission runs at the `fixup` annotation stage (`msl_shader.das` → `generate_msl`) —
   post-infer, after `visit(ExprIfThenElse)` has already folded static branches
   (`ast_infer_type.cpp:4793`). The emitter only ever sees the taken branch. Residual
   stage-2 note only: the metal lens derives roles pre-infer and would see BOTH branches
   (union roles — conservative); revisit only if a real family over-declares.

RED — the one genuine gap, C++-side, small and well-bounded:

10. **Deriving from a `class template` always leaves the TEMPLATE's own generated
    machinery unresolved.** `makeClassRtti`/`makeClassFinalize` run unconditionally on
    `isClass` (`parser_impl.cpp:366`) — including templates — and infer's
    `visitStructureField` processes template structs, so the template's
    `__finalize : auto = cast<auto> @@Tem'__finalize` init can never resolve (the
    finalizer body references unbound aliases) → `error[30805]` anchored at the template,
    reported at final verify. A standalone template is inert; a derived-from one is not.
    ⚠ Probe trap worth recording: runs where an UNRELATED error fired earlier looked
    "green" for the class rail — compilation stopped before final verify. Two probe
    variants differing only by the erroring line proved the masking. Never certify a
    macro rail green while any other error is present.

## Stage 1 — daslib + C++ implementation

C++ (contained):
- Make template classes inference-inert, mirroring template functions: skip
  `visitStructureField` (and the struct's init/layout verification) for
  `st->isTemplate`, or mark the generated finalizer/rtti template-flagged when the
  structure is a template. Acceptance: the class-rail probe shape compiles green with the
  das-side reifier; a standalone `class template` stays green; existing
  `tests/typemacro/_template_structure_class_mod.das` (typemacro rail) unaffected.
- Keep the debug-info `!st.isTemplate` verify — unreachable post-reify (parent cleared).

daslib (`[template_class_instance]`, typemacro_boost family, knows nothing of metal):
- The probe macro is the skeleton (~120 lines). Generalizations over the probe:
  enumerate the template's unresolved alias names by walking its field/method types
  (probe hardcoded "KT"); each must be bound by an instance typedef or `errors :=` names
  the missing one. Constants = template fields marked `@template_const`: take the
  instance's (possibly overridden) init, require it literal/const-foldable, add the
  `replaceVariable` rule, erase the field. Clone loop: collect-then-add, skip ctor +
  `'__finalize` + instance-owned names, both rename spellings, repair every field type +
  init through the rules, then `parent = null`.
- Struct twin: same implementation; `isClass` discriminates the (absent) rtti/finalize
  handling. Whether it registers as a second name `[template_struct_instance]` or one
  name serves both = Boris's naming call.
- The typemacro rail (`[template_structure]`, `[template_tuple]`) stays untouched — it
  serves the type-position/container use case; this serves the annotation-driven one.

Tests (`tests/typemacro/`): struct rail green today — land those with the macro; class
rail lands with the C++ fix. Negative fixtures: unbound alias → named error; parent not a
template → error; `@template_const` init not a literal → error; instance field-name
collision with a template field → error. Plus an override-wins arm, a two-instance arm
(same template stamped twice in one module), and a cross-module arm (template in a
required module).

Docs: tutorial chapter under `tutorials/macros/` (the concept deserves one — Boris);
`skills/das_macros.md` gains the `[|>` inherited-annotation fact (done, this session);
doc-comment surface per `skills/daslib_modules.md` when the macro lands.

## Decisions needed (Boris)

1. `@template_const` explicit marking (recommended: greppable, and "force it's a
   constant" has a place to hang the diagnostic) vs implicit "any overridden literal
   field is a constant".
2. One annotation name for both class and struct rails, or the
   `template_class_instance` / `template_struct_instance` pair.
3. Stage-1 go.

## Stage 2 pointer (not planned here)

dasLLAMA restamp per the cluster table (`plans/vulkan-on-mac.md`): SqAttn first
(typedef-only, 23→~6), KqMv (const stamp), then MulMm/MoeMulMm — where followup_general
#8's constraints apply verbatim: every route rewrites hot inner loops, so interleaved A/B
per format, oracles cover correctness only; Q8/Mx4 joining renumbers cnt/basep/bkt
6/7/8 → 7/8/9 (encoder + oracle churn accepted once). MoeGemv stays inheritance (its
dedup is genuinely is-a). The 96 unlensed classes ride the same wave as lens adoption.
