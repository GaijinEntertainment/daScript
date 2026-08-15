# dasLLAMA vision and media rules

**Routed from `REVIEW.md`: a diff touching the vision rail, a media splice, or a media-carrying
scheduler path applies this list with the master's.** `REVIEW_COMMON.md` (repo root) binds this
file too.

**A weight plane's element type follows its SOURCE tensor, per tensor.** A carrier reads a bf16
tensor as bf16 and an fp32 tensor as fp32 — never rounds one down to match the other, and never
decides the question for a whole file, because a shipped file mixes them.

**A new media kind adds its marker pair to the chat template, never a second renderer.** A
family whose template or vocab lacks the pair has no arm for that media kind — `create_chat_`
panics at create, not at render.

**A scheduler stream carrying media rows neither reads nor writes the prefix cache.** Cache
keys are token ids and the KV past the splice does not follow from them, so a media stream
skips `prefix_attach` at admit and `donate_stream` at reap.

**A media stream's rows eval as ONE prefill quantum.** The non-causal flag is per call, so a
chunk boundary inside the span would change the mask.

**A media splice is expressed as two token spans plus a row block, everywhere it appears** — so
BPE merges never cross the media. The engine, the scheduler, and the server all carry the same
shape; a second representation — a placeholder token, a pre-flattened embedding buffer at the
seam — is a defect, as is a new media kind growing a parallel prefill path.
