# LAWS - append-only intent provenance for this folder's rule documents

- **2026-08-24** (`REVIEW.md`): Boris ruled that prose describes each mechanism in its own
  terms - state what the code does and why its shape wins; legal attribution lives in
  `THIRD_PARTY_NOTICES.md` and the `LICENSE.*` files, which the ported reference
  implementations get like every other third party, installed with the SDK.

- **2026-08-24** (`ARCHITECTURE.md`): Boris ruled completed-arc `*_plan.md` docs retire to
  `history/dasLLAMA/` (the standing `doc_archiving` process); a plan doc in the module
  root is an ACTIVE arc's working plan.

- **2026-08-25** (`REVIEW_GPU.md`): Boris ruled no unguarded range-check invariants inside
  kernel main loops - all kernels, not just new kernels; guard them with `static_if` around
  a template argument, and see the gains.

- **2026-08-25** (`REVIEW_GPU.md`): Boris ruled the arc's measured kernel findings become
  standing rules so slower kernels are not attempted in the first place - the half-operand,
  stage-only-to-transform, consecutive-staging, walk-bound-pick, double-buffered-scratch,
  and size-gate rules land; the EXISTING kernel census gets an end-of-arc audit against
  them; and "a kernel split into multiple instances instead of using static_if for a
  template constant is a defect" (folded into the twins rule's defect list).

- **2026-08-25** (`REVIEW_GPU.md`): Boris ruled fastmath is the kernel default - a
  `fastmath = false` opt-out ships only with a failing test provided (the kernel
  demonstrably "goes bonkers" under fastmath); the fastmath audit joins the end-of-arc
  kernel sweep. (Census at ruling time: zero engine kernels opt out.)

- **2026-08-26** (`REVIEW_GPU.md`): Boris approved the bucketed-dispatch stamped-table rule
  ("good rule", on the MoE gather find: an O(experts) per-row basep/cnt search on all 256
  threads of every row threadgroup cost 24% of the qwen3moe pp512 forward) - per-row
  validity/ownership in a bucketed dispatch is answered by a table the bucket builder
  stamps, never by a per-row search over the bucket directory.
