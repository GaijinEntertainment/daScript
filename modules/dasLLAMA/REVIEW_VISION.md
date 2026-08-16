# dasLLAMA vision and media rules

**Routed from `REVIEW.md`: a diff touching the vision rail, a media splice, or a media-carrying
scheduler path applies this list with the master's.** `REVIEW_COMMON.md` (repo root) binds this
file too. Architecture doc: `ARCHITECTURE.md`.

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
