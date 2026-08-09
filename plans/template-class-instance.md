# `[template_struct_instance]` — template reification (stamped instances)

Boris + Claude, 2026-08-07. **Stage 0 (probes) DONE — results below; zero C++ changes
needed. Stage 1 IMPLEMENTED 2026-08-07 on `bbatkin/template-struct-instance` (macro +
tests + tutorial, all green; landed deltas below).** Naming + const-marking ruled (see
Decisions). Stage 2 =
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
[|> template_struct_instance]                 // rides down to every instance, prepended
class template MoeMulMmT {
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @workgroup ta : KT[2048]                  // KT: unresolved alias = type parameter
    @template_constant WIDE : bool = true     // constant parameter, erased at reify
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

## Stage 0 — probe results (2026-08-07, this M1, bin/daslang @ bbatkin/profiling-rail)

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

RESOLVED — what looked like a C++ gap was the reifier's own bug (Boris's push-back on
"small C++ change" forced the trace that found it):

10. **Templates are ALREADY fully inference-inert by existing design** — no C++ change
    needed. `canVisitStructure` skips `isTemplate` structs outright
    (`ast_infer_type.cpp:277`), `canVisitFunction` skips template functions (`:671`),
    parsed template-class methods are auto-flagged (`ds2_parser.ypp:2635`), and
    `makeClassFinalize` marks the template's own finalizer
    `func->isTemplate = baseClass->isTemplate` (`ast_generate.cpp:2258`).
    The real 30805: `makeClassFinalize` gives a DERIVED class's `__finalize` field the
    with-parent shape — `cast<auto>(@@Cls'__finalize)` init + `parentType` flag
    (`ast_generate.cpp:2233`) — which resolves ONLY through a live parent
    (`visitStructureField`'s `decl.parentType && st->parent` path,
    `ast_infer_type.cpp:331`). The reifier cut the parent and left that shape orphaned:
    no resolution path, `cast<auto>` reported at final verify, anchored at whatever
    LineInfo the inherited copy carried (template's or child's — which misled the first
    diagnosis). The fix is IN THE REIFIER, ~8 lines: when clearing `parent`, normalize
    generated fields to the parentless shape `makeClassFinalize` itself produces
    (`ast_generate.cpp:2239`) — strip the `cast<auto>` from `__finalize`'s init to the
    bare `@@fn`, and clear `flags.parentType` on every field. With that, the ENTIRE
    previously-red matrix is green with exact runtime values (`plain=42 gated=142
    doubled=840`): same-module and cross-module templates, 1 and 2 clones, override
    arms, used and unused instances, struct and class rails.
    ⚠ Probe trap worth recording: runs where an UNRELATED error fired earlier looked
    "green" for the class rail — compilation stopped before final verify. Two probe
    variants differing only by the erroring line proved the masking. Never certify a
    macro rail green while any other error is present.

## Stage 1 — daslib implementation (LANDED; no C++ changes)

Home: `daslib/typemacro_boost.das`, beside its named siblings `[template_structure]` /
`[template_tuple]` (the file already requires templates_boost publicly). The apply is
factored into `[macro_function]` helpers (`tsi_bind_aliases` / `tsi_is_const_init` /
`tsi_bind_constants` / `tsi_stamp_methods` / `tsi_normalize_fields`), per the pre-PR
rail's STYLE037/038 pre-scoping. One name serves both rails; the typemacro rail stays
untouched.

Landed deltas vs the stage-0 spec (each probe-verified):
- **`st.parent = tem.parent`, not `null`** — a `class template X : ConcreteBase` keeps
  its base in the stamped instance (upcast + dispatch-through-base green); same move as
  `apply_qmacro_template_class:1355`. The `__finalize` parentless-shape normalization
  works under a live grandparent too.
- **Rules also run over instance-authored method bodies** (in place, no clone) — a
  `@template_constant` read inside a `def override` body substitutes like everywhere
  else; constants behave as class-wide compile-time values.
- **Unbound-alias check counts module-level typedefs as bound** (walks
  `for_each_typedef` across the program library) — a field typed by an ordinary module
  alias no longer false-positives; only a genuinely unbindable parameter gets the named
  error, listing every missing name.
- **`@template_constant` accepts signed literals** (`-1` is `ExprOp1` over the const
  pre-fold; `tsi_is_const_init` peels one unary `+`/`-`).
- **`KT(1)`-style ctor-casts in template bodies need no rule** — infer resolves
  call-position alias names through the instance's structure aliases (probe-verified).
- **`late_bind = true`** (decision 4) — RETIRED with the eager unbound-alias check (see the
  resolution further down): unbound names now flow to infer unconditionally, which reports
  them at first use, so the opt-out flag had nothing left to opt out of.
- **`@template_call`** (decision 5) — harvest mirrors `@template_constant`
  (validate `@@name`/string init, `renameCall` rule, erase field, slot-vs-method-name
  collision check); tests cover default, `@@` rebind, string rebind, and `@@` address
  targets through a function-pointer taker; negative fixture for a non-callee init.

Tests (`tests/typemacro/`, 25/25 green interp AND `-jit`; AOT emits folded bodies —
`ToyNarrow`gated` compiles to `return x + 100`): `test_template_struct_instance.das`
(class rail incl. static_if fold + override-wins + const defaults arm, struct rail with
two instances + const in field init, template-with-base arm, cross-module arm via
`_template_struct_instance_mod.das`); negatives `failed_tsi_unbound_alias` (expect 30826 —
infer reports the unbound name at first use since the eager check retired) /
`_not_template_parent` / `_nonconst` (expect 20800) + `_field_collision` (expect 20503,
parser-level). `tests/aot/CMakeLists.txt` typemacro glob now excludes `failed_*`.

Docs: macro tutorial 20 (`tutorials/macros/20_template_struct_instance.das` +
`template_struct_instance_mod.das`, RST under `doc/source/reference/tutorials/macros/`,
toctree + tutorial-19 next-link updated; Sphinx -W clean). `skills/das_macros.md` carries
the `[|>` inherited-annotation fact.

Known lint interaction (report to Boris): **STYLE029 false-positives on a require whose
only visible use is a reified template parent** — the reifier erases the parent link
before lint runs, so the require looks redundant while being load-bearing. Nolint'd with
reason in the tutorial usage file; a proper fix needs STYLE029 to see parse-time usage
(or a reifier breadcrumb). Also: the formatter canonicalizes `[|>` to `[ |>`.

## Stage-1 pre-PR rail (Boris, 2026-08-08: expect bulk lint, don't be surprised by it)

The macro + tests + tutorial touch enough files that lint fires in bulk. In order,
before the `skills/make_pr.md` checklist:

1. **Fresh branch off latest master** (verify base == master; the stage-0 doc commits
   ride along — they currently sit on the merged `bbatkin/vulkan-on-mac` branch and get
   cherry-picked or land via master first).
2. **Full lint sweep, both flavors**: MCP `lint` per changed file AND the CI
   linux-flavor mirror (MCP lint ≠ CI lint) — put the whole finding set on the table
   before any fix.
3. **Fan out Opus agents over the findings** — one per file or rule-family, each under
   the house discipline: fix the root cause; STYLE037/038 split only along a natural
   seam, else `// nolint` with a tail-comment reason on the `def` line; lint TOOL bugs
   get fixed, not worked around; no blanket suppressions. Merge, re-lint to zero.
4. **Known pressure points from the stage-0 skeleton** (write these right at authoring
   time, so the agents handle only residue): the single ~130-line `apply` trips
   STYLE037/038 — factor into `[macro_function]` helpers from the start (alias
   enumeration, const harvest, method cloning, generated-field normalization); plus
   LINT003 let-vs-var, PERF007 das_string compares, PERF017 `empty()`, STYLE016 guard
   merges — all already observed on the probe.

## Decisions (ruled, Boris 2026-08-07)

1. `@template_constant` — EXPLICIT marking. Constants are fields the template marks;
   unmarked overridden fields stay ordinary fields.
2. ONE annotation, named `[template_struct_instance]` — matches the existing
   `template_structure` family; there is no `template_class` in the family, so no
   class-flavored name yet. When class-flavored template macros show up, we add
   `template_class_instance` alongside.
3. Stage-1 GO given.
4. Unbound-alias check: keep the eager named error by default;
   `[ |> template_struct_instance(late_bind = true)]` on the template waives it for
   macro-supplied parameters (option D). Probe round proved: natural resolution covers
   signatures/bodies/fields on the clones (no rules needed), a `[dirty_infer_macro]`
   can bind an alias mid-infer (NOT `[infer_macro]` — that one runs only on a clean
   tree), and `finish`/`patch` never fire for a failed struct, so a deferred named
   error is impossible — the opt-out knob is the only way to keep the good message.
   **SUPERSEDED (Boris, 2026-08-07, stage-2 vehicle evidence): check REMOVED, and
   `late_bind` with it (its only effect was waiving the check — late-supplied aliases
   are now simply the default behavior).** The check rejected valid programs on
   declaration order (a struct declared below the instance false-errored) in a language
   that is otherwise order-independent, and its bound-set was a blocklist that had to
   enumerate every name category (typedefs, structs, enums, handled types, …) to stay
   correct. A forgotten typedef now falls through to infer: one `30826
   undefined structure field type` for a struct template; a handful of
   `don't know what 'KT' is` errors at template lines for a class template (instance
   named only in the mangled function name) — ugly but honest, on actually-wrong code.
5. Call-parameter axis (`@template_call`, IMPLEMENTED same round): the field name is
   what template bodies call, the init (`@@name` or string) is the default target,
   instances redirect with `override sdot = @@ssdot`. Rides the rules engine's
   existing `renameCall`/call2name, which rewrites calls AND `@@` address targets —
   erased like a constant, direct call in the stamped class. Replaces the closed-set
   `static_if` route for callee selection with an open one (a new instance can route
   to a function the template never names).
6. Rebind scope (Boris, PR round): the BARE spelling only. `_::slot` / `__::slot`
   keep their normal `_`/`__` resolution rules — the pinned escape to the real
   function past any rebind (probe-verified both forms; consistent with constants,
   whose replaceVariable is bare-keyed too). This superseded the Copilot round-1
   "rename the qualified spelling too" fix; the parser-generated `@@_::Tem`m`
   method-pointer twins are machine references and still retarget.

## Stage 2 pointer (not planned here)

dasLLAMA restamp per the cluster table (`plans/vulkan-on-mac.md`): SqAttn first
(typedef-only, 23→~6), KqMv (const stamp), then MulMm/MoeMulMm — where followup_general
#8's constraints apply verbatim: every route rewrites hot inner loops, so interleaved A/B
per format, oracles cover correctness only; Q8/Mx4 joining renumbers cnt/basep/bkt
6/7/8 → 7/8/9 (encoder + oracle churn accepted once). MoeGemv stays inheritance (its
dedup is genuinely is-a). The 96 unlensed classes ride the same wave as lens adoption.

### Stage 2, SqAttn COMPLETE (2026-08-08): 23 classes → 10 templates + 3 combs

Remaining tiers followed the plain-tier pattern (e2f5b026f, −361 LOC net): per tier a
KvT template (typedef KT — `half4`/`float4` on the D tier) + a QuantT template (typedef
QB + fill/vacc `@template_call` slots). The D quant pair needed NO call slots — its
`sqd_*_blk_q` stages overload on the byte-view type — and instead carries
`@template_constant LANE_BYTES` (4 quant bytes vs 2 nibble bytes per lane), the constant
axis's first production use. DQ8/DTq4 were non-adjacent in the file; the pair now sits
together. B/PartB tiers have no `[metal_dispatch]` (hand-encoded batch dispatch, declared
roles) — their instances are 2-4 lines. All 16 kernels: MSL byte-identical
(entry-normalized diff empty), kernels suite 7/7. Combs (3) have no format axis — as-is.
Next per the cluster table: KqMv (batch width as a stamp constant, 9→4).

### KqMv survey (2026-08-08): the family RESISTS the mechanical stamp — census row was optimistic

Three structural facts, all from diffs (not the peek the cluster table used):
1. **B8 trio is a different algorithm**, not a width twin — tgmem X-panel staging
   (`@workgroup txp : float4[512]`, barriers, cooperative load), float4 accumulators,
   register-preloaded quants. Nothing to stamp against B2/B4.
2. **B2 deliberately lacks B4's column tiling.** B4's `colbase = gl_WorkGroupID.y * 4u`
   is LIVE (enc_kq_mvb: nlive 5-8 with kq_b8 off → gcols 2); B2's dispatch pins
   gcols = 1, and its body omits the term outright — a hot-path specialization, not
   drift. Merging means always-zero arithmetic on the B=2 decode GEMV (text change →
   interleaved A/B per followup_general #8) or static_if that cannot reproduce
   byte-identical text (folded branches emit scoped blocks; the term threads through
   3 sites).
3. **K4↔K5's qh overlay modifies the innermost w-decode line** (`| (hb << 4u)` inside
   the e-loop) plus the block stride — a byte-identical static_if merge would have to
   branch-duplicate the inner loops. K6 is its own layout (census already said leaf).

Ruled (Boris, same day): measured cases go to the PROFILING FOLLOWUP; tonight sweeps
everything the byte-identity gate admits. Prediction on record: the specializations are
accidental — they will merge.

### Tonight's sweep COMPLETE (2026-08-08): the free merges are exhausted

**Stamped:** SqAttn 23→10 templates + 3 combs (all five tiers); RopeStore float pairs —
`MetalRopeStore{F16,F32}` + `MetalRopeStoreB{F16,F32}` from 2 templates (typedef KT +
`@template_constant CLAMP16` gating the store block via `static_if`). **The static_if
fold emits byte-identical MSL — zero trace, lets and all** (probe-verified both branch
directions on ca5862d8e); the clamp-vs-raw pattern is now free for any format pair.

**Fenced → the profiling followup** (every one needs interleaved A/B on the M1; oracles
cover correctness only):
- ~~KqMv B2→colbase-form unification~~ **DONE 2026-08-08** — see the measured round below.
  The K4/K5 FORMAT-axis merge stays ruled out for the plain GEMVs: the qh overlay + hq
  staging + block stride would static_if-duplicate most of the body (the 43-49%-different
  measurement stands). BUT the mul_mm and B8 shapes merged on exactly that axis — see below.
- ~~MoeMulMm K6~~ **JOINED 2026-08-08** (021fffd87, Boris ruling "measure and refactor"):
  the scalar cache measured SLOWER than reload-per-kb (gmm6 lab, −2.4% ms/mm ×3 launches) —
  the stateless stage_a is both the join and a −2.0% win vs the old standalone.
- ~~MoeMulMm Q8/Mx4~~ **measured OUT 2026-08-08** (gmm8 lab section): the stateless
  index-math stage_a form is +3.4–3.6% vs Q8's production carried-pointer walk (3/3
  launches, bit-exact both arms, occupancy identical 1024/1024 — in-loop addressing, not
  registers). A pointer-preserving join needs loop-carried rider state, which msl_emit's
  field gate forbids (members must be @ssbo/@uniform/@workgroup); Mx4's prologue hook is
  blocked on the same mechanism. Both stay standalone; the unlock is the followup #8
  msl_emit lead — method-flattening WITH scope splicing (also worth ~0.5% on all riders).
  The MoE-lab per-site rot repair stays ledgered there too.
- ~~The plain GEMV width pairs~~ — Gemv + W13Sw **DONE 2026-08-08 free** (see below);
  Q8Mv B2/B4 CLOSED: 16-vs-8 lanes/row thread geometry IS the specialization (a
  pick-one-geometry unification is an optimization experiment, not a dedup).

### The TILED/WIDE static_if round (2026-08-08, after the KqMv restamp): 4 more free pairs

The KqMv lesson generalized: any STATEMENT-shaped divergence merges free by branch-duplicating
just the divergent statements per `static_if` — the fold emits each instance's production text.
The earlier "not scaled twins / genuinely different" verdicts assumed one shared spelling.
Merged (each gated: entry-normalized MSL diff + tgmem, kernels suite 7/7):
- `MetalKqMulMmK45T` (91a70c3d9): the non-MoE prefill GEMM K4/K5 — QH arm duplicates only the
  dequant k-loop (nsh repeated per arm for line order). Both stamps byte-identical, −72 LOC.
- `MetalKqMvB8K45T` (6fa3ac6d8): the B8 X-panel pair — QH arms run through the acc loop (w4 is
  arm-scoped). K5 byte-identical; K4 + one dead zero-init hq array (never read). −45 LOC.
- `MetalGemvB24T` + `MetalGemvW13SwB24T` (3bdc3331d): the width pairs — WIDE carries the panel
  load split (one slot/thread at B2, two at B4), NR/PANEL the sizes; `float4[PANEL]` field
  substitution reproduces tgmem 0x800/0x1000. All four stamps byte-identical, −80 LOC.
Closed on diffs the same round: KqGemv K4/K5 + MoeGemv K4/K5 (per-format lane geometries — the
census's "MoeGemv base+5 ~200 LOC" was optimistic like the KqMv row; only the ~10-line gather
prologue is shared), Bf16MulMm↔Q8MulMm (field sets differ — a template must declare every
field its body references, the same blocker as MoE Q8/Mx4).

**enc_ twins commit DONE 2026-08-08.** The kernel merges unified binding contracts, so the
per-format encoder wrappers converged into literal twins. Collapsed onto the existing
`KVDtype` axis: single-row `enc_attn`/`enc_attn_part` absorb the `_q8`/`_tq4` twins (the
generated `_c` builders differ per codec, one dispatcher each); batch
`enc_attn_b`/`enc_attn_part_b`/`enc_attn_d` go 9 functions → 3 (quant codecs bind the arena
4× and shift the tail binds by 2 — a `quant` flag + slot shift, the pso/tgmem pick is a
KVDtype ladder). The 5 decode call-site ladders become single calls passing `kdt` — the
tq4-before-blockc ordering hazard at those sites is gone. Out of scope on inspection:
`enc_kq_mvb`/`enc_kq_gemm_mm_b` (already fmt-driven; residue is global NAMES, not structure)
and the rope-store families (distinct args types + grid math — different, not twins).
Gates: kernels suite attn arm 7 files green; decode parity arm1-basic + arm7-q8kv +
arm7b-tq4kv + arm11-depth + batch all PASS.

### KqMv B2/B4 measured round + restamp (2026-08-08): 9→3 templates + B8 trio, −116 LOC

The A/B ran in `bench_metal_gemv_kernels` (new `kq_mvb*` arms under the production
KqMvArgs contract; 3 launches per question, interleaved, cls3b = the DRAM-honest cell):

1. **Naive colbase form** (the text a plain width template stamps at NR=2): k4 +1.9/+2.0/+2.0%
   at cls3b, +1..+5% at w13/w2; k6 +0.5% cls3b; k5 −1.4/−1.5/−1.4% cls3b (BETTER, occupancy
   448→512). Boris's prediction ("accidental, will merge") — k5 hit; k4/k6 MISS: the omitted
   colbase is a load-bearing specialization.
2. **Hoisted form** (colrow/cb4, one add per chunk instead of a mul per b): did NOT recover
   k4 cls3b (+1.8..+2.0% still); both hoisted B4s dead-neutral vs production. So the cost is
   not the per-b mul, and occupancy is unchanged — ISA-level mechanism unresolved; the number
   is the ruling (6 launches, 2 spellings, same +2%).
3. **Dead-decl preview** (production text + an unused colbase line — the static_if stamp's
   only residue): ±0.1% everywhere. Free.

Restamp (46a6252f7): `MetalKqMvK4T/K5T/K6T`, NR/NRU width constants (int + uint spellings —
fixed-array dims and `range()` take the int, colbase math the uint; `float[NR]` substitution
works). K4T/K6T carry `TILED` static_if branch-duplicating ONLY the b-loop + writeback tail:
B4 stamps byte-identical, B2 stamps = production + the dead decl. K5T keeps one colbase
spelling (its B2 text change is the measured improvement). Gate: entry-normalized MSL diff
(b4k4/b4k5/b4k6 empty; b2k4/b2k6 the one dead line; b2k5 the three expected sites), kernels
suite 7/7. The lab's production arms now reference the stamped globals — a standing tripwire
on the stamped text.

**Genuinely different, NOT merge candidates** (surveyed, closed): rope-store quant
stores (Q8/Tq4/BQ8/BTq4 — the tq4 rotate, 168 diff lines), `KqGemvK5C` (a measured
alternative lowering the encoder picks per shape), `Q8Gemm` vs `Q8Gemm64` (90 vs 316
lines), KqMv B8 trio (tgmem X-panel algorithm). The tensor (tmm2d) `*T` twins are
followup_general #12's deletion audit, not merges.

### Stage 2 first vehicle (2026-08-07): plain SqAttn tier, 4→2 templates

`MetalSqAttn{F16,F32}` → `MetalSqAttnKvT` (typedef `KT`); `MetalSqAttn{Q8,Tq4}` →
`MetalSqAttnQuantT` (typedef `QB` + two `@template_call` slots — their binding layouts
already matched, no renumber). Instances keep their own `[metal_dispatch]`; the template
method carries a bare `[metal_kernel]`. **Proof: the four stamped kernels emit MSL
byte-identical to the hand-written bodies (entry-name-normalized diff empty)** — same
text ⇒ same PSO ⇒ perf settled by construction; kernels suite 7/7 green on the M1.

Machinery gaps the vehicle exposed (both fixed in the same round):

1. **Stamped clones never got their function-annotation applies** — the parser runs
   `runFunctionAnnotations` only for functions it parses, so a template method's
   `[metal_kernel]` never created its per-instance globals. Fix in `tsi_stamp_methods`:
   re-add each cloned decl through the applying `add_function_annotation` overload,
   then erase the inert front copies (test arm: `test_tsi_method_annotation_apply`).
2. **The eager unbound-alias check only knew typedefs** — a struct-typed field
   (`ka : SqAttnArgs`) parses as an alias node and false-errored as a missing template
   parameter. Fix: the bound sweep now also counts structure/enum names and handled
   typenames (module annotations) across the program.

dasMetal rode along: `[metal_kernel]` with no `name=` now derives spellable globals
(`Class_method_msl`, backticks flattened) and a qualified MSL entry (`Class_method`) so
stamp twins stay distinguishable in GPU captures; explicit `name=` behavior unchanged
(zero no-name users existed).

**Parse-order false positive → check removal:** the eager check could only treat as
"bound" names already parsed, so `struct SqAttnArgs` declared below the instances
false-errored (the args structs moved above the kernel section as the immediate fix —
now pure layout preference). This plus the blocklist fragility led to the Decision-4
supersede: the eager check and `late_bind` are REMOVED; unbound names fall through to
infer.

**Macro-diagnostics collapse (observed 2026-08-07, fix GO same day):** Rule 2 of
`Program::deduplicateErrors` (cbc2184f0) collapses same-line same-cerr different-text
errors to the first plus "+N more on this line". All macro-apply failures share cerr
20800 `runtime_annotation`, so when two macros on one declaration both error (the
inherited reifier + the instance's own `[metal_dispatch]`), the second macro's named
error — an independent, actionable message — is dropped. Fix (Boris GO: "macro errors
are too important"): `isMacroDiagnostic` exempts the macro cerr family (20800, 31200,
31210, 50501, 50503 — and post-review the lint pair 31208/31209) from Rule 2; Rule 1 (byte-identical dedup) still applies, and macro
errors are bounded by the annotation list so they cannot avalanche like the inference
noise Rule 2 exists for.

## Appendix — the stage-0 reifier skeleton (matrix-green verbatim, minus probe prints)

Hardcodes one alias ("KT") and one const ("WIDE") — stage 1 generalized exactly those
two spots (plus `st.parent = tem.parent` instead of `null`; see the landed deltas). Ran
green: same-module + cross-module class templates, struct templates, override arms,
multi-clone, used/unused instances.

```das
[structure_macro(name = "tci_probe")]
class TciProbe : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        if (st.flags.isTemplate) return true // on the template itself: nothing to do
        if (st.parent == null || !st.parent.flags.isTemplate) {
            errors := "parent is not a class template"
            return false
        }
        var tem = st.parent
        let temName = string(tem.name)
        let instName = string(st.name)
        var mod = compiling_module()
        var instType = new TypeDecl(at = st.at, baseType = Type.tStructure, structType = st)
        var kt = get_structure_alias(st, "KT")               // stage 1: enumerate template aliases
        var wideInit : ExpressionPtr                          // stage 1: fields marked @template_const
        var wideIdx = -1
        for (i in range(length(st.fields))) {
            if (st.fields[i].name == "WIDE") {
                wideInit = st.fields[i].init
                wideIdx = i
            }
        }
        var ownMethods : table<string>                        // instance-authored methods win
        for_each_function(mod, "", $(func) {
            if (func.flags.isClassMethod && func.classParent == st) {
                var parts <- split(string(func.name), "`")
                ownMethods |> insert(parts[length(parts) - 1])
            }
        })
        var rules : Template
        rules |> replaceStructWithTypeDecl(tem) <| clone_type(instType)
        rules |> replaceTypeWithTypeDecl(temName) <| clone_type(instType)
        rules |> replaceTypeWithTypeDecl("KT") <| clone_type(kt)
        rules |> replaceVariable("WIDE") <| clone_expression(wideInit)
        for_each_function(tem._module, "", $(func) {          // both rename spellings: field
            if (func.flags.isClassMethod && func.classParent == tem) {   // inits hold @@_::Tem`m
                let fn = string(func.name)
                var parts <- split(fn, "`")
                let mname = parts[length(parts) - 1]
                rules |> renameVariable(fn, "{instName}`{mname}")
                rules |> renameVariable("_::{fn}", "_::{instName}`{mname}")
            }
        })
        var toClone : array<FunctionPtr>                      // collect THEN add
        for_each_function(tem._module, "", $(func) {
            if (!(func.flags.isClassMethod && func.classParent == tem)) return
            let fn = string(func.name)
            var parts <- split(fn, "`")
            let mname = parts[length(parts) - 1]
            if (mname == temName || fn == "{temName}'__finalize" || key_exists(ownMethods, mname)) {
                return // ctor + 'finalize (quote spelling): instance has its own; overrides win
            }
            toClone |> push(func)
        })
        for (func in toClone) {
            var parts <- split(string(func.name), "`")
            let mname = parts[length(parts) - 1]
            var fc <- clone_function(func)
            fc.name := "{instName}`{mname}"
            fc.moreFlags.isTemplate = false
            fc.classParent = st
            fc.result = apply_template(rules, func.at, clone_type(func.result))
            for (arg in fc.arguments) {
                arg._type = apply_template(rules, arg.at, clone_type(arg._type))
            }
            fc.body = apply_template(rules, func.at, clone_expression(func.body), false)
            if (!(mod |> add_function(fc))) {
                errors := "can't add {instName}`{mname}"
            }
        }
        for (f in st.fields) {                                // repair fields through the rules
            f._type = apply_template(rules, f.at, clone_type(f._type))
            if (f.init != null) {
                f.init = apply_template(rules, f.at, clone_expression(f.init))
            }
            // derived-class generated __finalize has the with-parent shape (cast<auto> init +
            // parentType, ast_generate.cpp:2233) resolvable only through a live parent; we cut
            // the parent, so normalize to the parentless shape (ibid:2239): bare @@fn, no flag
            f.flags.parentType = false
            if (f.name == "__finalize" && f.init != null && f.init is ExprCast) {
                let c = f.init as ExprCast
                if (c.castType != null && c.castType.baseType == Type.autoinfer) {
                    f.init = clone_expression(c.subexpr)
                }
            }
        }
        if (wideIdx >= 0) {
            st.fields |> erase(wideIdx)                       // consts never become fields
        }
        st.parent = null
        return true
    }
}
```
