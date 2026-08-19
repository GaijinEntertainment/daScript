# dasLLAMA vision and media rules

**Routed from `REVIEW.md`: a diff touching the vision rail, a media splice, a media-carrying
scheduler path, `dasllama/dasllama_vision_embedder.das`, or a vision family file — one
`dasllama/dasllama_<family>.das` holding a single vision projector family
(`dasllama/dasllama_gemma4uv.das`, `dasllama/dasllama_gemma4v.das`) — applies this list with the
master's.** `REVIEW_COMMON.md` (repo root) binds this file too. Architecture doc:
`ARCHITECTURE.md`.

**A type or loader a single vision family owns (its embedder or tower, its state, its staging)
is named outside its own file only in `dasllama/dasllama_vision_embedder.das`, in
`dasllama/dasllama_metal_tower.das`'s family hooks, and in that family's own tests.** The `VisionEmbedder` union carries it
through every other seam (chat, server, bench, facade, tutorials); a family name at a seam is a
defect.

**A vision tower's clamp bounds come from the file's sidecar scalars (`read_clamp`), never a
literal** — the mtmd ±FLT_MAX default applies only where the scalars are absent.

**A shape shared by two tower families lives in `dasllama/dasllama_audio.das`** (the
encoder-tower home — its list is `ARCHITECTURE.md` §1.7); a family file that re-implements one is
a defect.

**A test that reads a vision encode oracle dump names the minting arm in its header — the
backend, the flash-attention setting, and the mmproj precision the dump came from.** The mint
doctrine itself (CPU, `-fa off`, the f32-widened mmproj twin — the only true-f32 reference arm)
is `ARCHITECTURE.md` §1.7b's oracle-provenance paragraph.

**A new media kind adds its marker pair to the chat template, never a second renderer.** A
family whose template or vocab lacks the pair has no arm for that media kind — `create_chat_`
panics at create, not at render.

**A scheduler stream carrying media rows neither reads nor writes the prefix cache.** Cache
keys are token ids and the KV past the splice does not follow from them, so a media stream
skips `prefix_attach` at admit and `donate_stream` at reap.

**A media splice's rows reach `forward_prefill_embd` in ONE call** (`eval_embd_span_`'s middle
slice) — splitting them across calls, or letting a driver chunk them by row, is a defect: the
non-causal flag is per call, so a boundary inside the span would change the mask. A driver
DECLINING the whole call to the CPU loop, or splitting command buffers per layer, is not a
split.

**A media splice is expressed as two token spans plus a row block, everywhere it appears** — so
BPE merges never cross the media. The engine, the scheduler, and the server all carry the same
shape; any other representation at the seam is a defect.

**A new media kind rides the existing splice prefill path** — growing a parallel prefill path
for it is a defect.
