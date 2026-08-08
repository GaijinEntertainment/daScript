# msl_emit method flattening + scope splicing

Boris ruled 2026-08-08 ("i think now"): pursue immediately, after the gmm8 lab measured the
stateless index-math Q8 join OUT (+3.4–3.6% vs the carried-pointer walk) and the msl_emit
field gate blocked the pointer-preserving alternative. Branch: `bbatkin/tsi-sqattn`.

## Goal

Erase the method-call cost model from `[metal_kernel]` classes:

1. **Flatten** — method calls splice their bodies inline at MSL-emit time. No MSL struct, no
   member functions, no `self_` fill. Recovers the ~0.5% method-emit residue measured on all
   4 MoeMulMm riders (gmm6 lab).
2. **State members (scope splicing)** — a plain (unannotated) class member becomes a
   kernel-entry thread-local, visible to every spliced method body. A `stage_init` override
   binds a carried pointer, `stage_a` advances it: loop-carried rider state from class-grade
   source — the Q8 pointer-walk join and the Mx4 prologue join.

Struct mode is DELETED, not paralleled — its only consumers are the 4 MoeMulMm riders
(dasllama_metal_prefill.das) and the UfxKid fixture (tests/msl/_msl_common.das). One
mechanism, one revert.

## Design

**Statement splice** (`m(args)` as a statement): emit `{ <hoisted binds>; <body lines> }`.
Braces always — sequential splices may reuse local names. Param binding:
- ref param (`T&`) → substitution; the arg must be an lvalue shape (var/member/index/swizzle)
- writable value param (das `var p : T`) → hoisted mutable copy (matches CPU copy semantics)
- read-only value param → substitution when the arg text is a single identifier/literal,
  hoisted `const` decl otherwise
- buffer/pointer param → substitution; requires a member / pointer-local / addr() shape
Hoisted names take a `_s<N>` suffix (N = per-kernel splice counter) — collision-free by
construction. Body locals that would shadow a substituted single-identifier arg are renamed
the same way.

**Value splice** (`let x = m(...)`, args of other calls): the body must be a single
`return expr` (arrow-form) — result emits as the parenthesized expression under the same
param bindings. An impure arg bound to a param the body names more than once is an error.
Multi-statement value-returning methods: error (call as a statement or restructure).

**Returns in spliced bodies**: legal only when the splice SITE is in tail position — last
statement of its block transitively, if-arms inherit the if's status, loop bodies never.
`{ run() }` entries keep their early-exit guards; anything else errors naming the method.

**State members**: declared after the threadgroup pointer fills, name-sorted. Scalars/vectors
zero-init (`= {}`) or constant init from the field initializer; single-dim fixed arrays
`= {}`; pointer members `= nullptr` with address space + root buffer resolved at scan time
from their assignment sites (`p = addr(member[..])`, `p = p + n`, pointer local, another
pointer member — ONE root buffer per pointer member). A mutable-pointee pointer member marks
its root buffer written (device non-const) at assignment — no deref tracking. Unsupported
member types keep the hard error (the old field gate, now scoped to what state can't hold).

**Census**: − `decl.class_struct`, `decl.method`, `call.method`, `stmt.call_method`,
`entry.self_fill`; + `splice.method`, `splice.method.value`, `splice.param.hoist`,
`decl.state`, `decl.state.ptr`.

## Gates

1. tests/msl suite — UfxKid rewritten flattened + carried-pointer state coverage; census both
   directions.
2. tests/metal GPU behavioral twins (unchanged text, must stay green).
3. dasLLAMA kernels suite (`run.das --arm attn --suite kernels` + moe arms) — the 4 riders
   re-emit flattened, bit-exactness holds.
4. gmm6 lab — K6 must not regress (3 launches, interleaved, best-of).
5. Commit B (stage_init + Q8 join): gmm8 lab — the joined Q8's text must match/beat prod
   2.186 ms/mm. Then Mx4 (prologue hook) rides the same mechanism.

## Ledger

- [x] Commit A: emitter flatten + state members + tests/msl updates — DONE 2026-08-08.
      Gates: tests/msl 81/81, tests/metal GPU 53/53, dasLLAMA kernels suite 7/7 files,
      gmm6 stash-interleaved A/B (3 pairs, M1): flattened K6 −0.44/−0.61/−0.58% vs struct
      (mean −0.54% — the predicted ~0.5% method-call residue recovered; prediction HIT),
      per-arm spread ≤0.11%, gmm8_prod anchor ±0.09%, all 24 correctness cells worst rel 0
      in both modes.
- [x] Commit B: `stage_init` hook + Q8 join — DONE 2026-08-08. The rider keeps the
      carried-pointer walk via plain members; standalone deleted; binding contract moved to
      the family numbers (encoder pf_enc_moe_mm, gemm-test gate, gmm8 lab, q8 race harness
      all rebound — tensor twin keeps its compact layout; kn_moe_mm_family_tail helper now
      spells the family tail once for q8/q51/k-quants). Gates: kernels suite 7/7 (q8 oracle
      gates at the new binds, poisoned-y negative control inherent), gmm8 stash-interleaved
      A/B (3 pairs, M1): joined 2.1796/2.1718/2.1790 vs standalone 2.1950/2.1907/2.1907
      ms/mm — joined −0.70/−0.86/−0.53%, bit-exact every launch; fam-qwen3moe (k4 flavor)
      token-for-token parity through the production encoder covers the refactored family
      tail in real serving (the q8 branch's binds are proven by the oracle gate + lab at
      the same slots; the q8-MoE serving census rides the standing PARITY_FULL cadence).
- [x] Commit C: Mx4 join — DONE 2026-08-08 (two commits: the gmm4 lab repair first — the
      prod arm predated the kargs migration and sat unreachable; kargs@5 + kinds aligned +
      run_gmm4_lab moved ahead of the rot — then the join). The rider: stage_a = the
      stateless index decode (mx4 never carried pointers), stage_init = the vtab staging,
      stage_acc (NEW base hook, takes the accumulator array) = the per-expert bias seed;
      bias member renamed wb → wbias (run()'s `wb` local would shadow it in flattened MSL —
      and local_msl_name now suffixes member-shadowing locals in the emitter as the general
      guard). Gates: kernels suite 7/7 (mx4 bias/gather oracle gates at family binds), gmm4
      stash-interleaved A/B (3 pairs, M1): joined 4.7461/4.7462/4.7462 vs standalone
      4.7453/4.7454/4.7454 ms/mm — +0.02% flat, bit-exact, per-arm spread 0.002% (the
      kernel is DRAM-bound at ~67 wGB/s; no residue win, no regression — the join is free).
- [ ] Tail: MoE-lab per-site repair (followup_general #8) — rebind enc_lab_w13*/w2/pair to
      MoeGemvArgs kargs; drop the dead tail duplicate run_gmm6_lab call in main_apple
- [ ] spirv_emit parity: dasGlsl/dasVulkan lowers no method classes today; if a joined class
      ever crosses backends, the same flatten design applies there (not in this arc's scope)
