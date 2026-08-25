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
