# dasLLAMA Architecture - engine mechanisms

Companion to `ARCHITECTURE.md` beside `ARCHITECTURE_ENGINE.md`; a section is cited by its
anchor.

### RoPE scaling is folded at the load {#rope-scaling-bake}

The gguf loader folds a file's `rope.scaling.*` keys into two `Model` fields once: `linear`
scales the position (`Config.rope_freq_scale`), `yarn` synthesizes the per-pair `rope_freqs`
divisors over the NTK-by-parts band (`yarn_rope_freqs`) and multiplies the `1 + 0.1 ln(s)`
magnitude into `Config.rope_mscale`. Every rope consumer - the CPU leaves, the cos/sin tables
the GPU drivers read - takes those fields and never the metadata, and both run over the ROTATED
span, so a partial-rotary head (`rope_dim` under `head_size`) takes the band and the factors
like a full one. A Phi-3 file ships two LongRoPE factor sets; the loader reads into `rope_freqs`
the one `rope_factor_tensor` names - the long set when the file's context (`Config.seq_len`, the
context a session is sized to) runs past `rope.scaling.original_context_length`, else the short
set - which is llama.cpp's rule with its context set to the file's length. The reference switches
by each call's sequence length; one Model-level set cannot, so a short session on a 128k file runs
the long set, as llama.cpp does at that context. A serving override (`set_rope_scaling_override`, seeded from
`DASLLAMA_ROPE_SCALING` / `ROPE_SCALE` / `YARN_ORIG_CTX`, the tools' `--rope-scaling` flags and
the server's config keys) replaces the file's keys at the same fold, because the Qwen files ship
none and enable YaRN as a setting. The fold is image BYTES, so `DlimCpuConfig.rope_override`
carries the override in the identity and `image_lane_name` as a lane suffix: an image minted
under it and one without coexist, and a load under the other never reinterprets either. A file
that ships per-pair `rope_freqs` (Llama-3.1) refuses the override - two factor sets on one pair
have no defined product.

### The decode pick tape {#moe-pick-tape}

`dasllama_moe.das`'s `moe_pick_tape` pins one arm's routing on another for a forced-feed parity
compare. Recording, every decode `moe_select` appends its k picks in call order; replaying, the
select masks every expert the tape does not carry to -1e30 before the same top-k, so the picks are
the tape's and the weights are the picked experts' own (exact under a top-k renorm: the masked
experts cancel out of the ratio), and it counts the row's own top-k picks the tape did not carry.
The prefill's grouped selects take the rows form: `moe_pick_tape_rows_begin` hands the replaying lanes
the rows' taped picks (the lanes run in their own contexts and read the tape through that pointer
alone), `moe_pick_tape_mask_row` masks each row to its picks before its own select, and
`moe_pick_tape_rows_done` advances past the rows, or appends their picks when recording; a rows
replay counts no misses. `moe_select`'s own grouped path (`off > 0`) never touches the tape. The
arming is `MoePickTape.record`, the replay `MoePickTape.replay`, `MoePickTape.off` stops. A rows-form
tape is row-major a layer (row 0's select, row 1's, ...), and a one-row chain replays one of its rows
as a lane (`moe_pick_tape_lane`: the replay starts at select `first` and advances `stride` selects a
select), so the verify's device rows compare against the split command's own one-row steps on the
picks the rows made. A tape crosses processes as a list of picks: `moe_pick_tape_picks` copies a
recording out and `moe_pick_tape_replay_from` arms a replay on one, which is how the parity dump
(`harness/_parity_dump.das`) pins a CPU reference run in its own process on the routing a device
run recorded in another. A 512-expert top-10 router over
mixed residual streams flips a near-tie on kernel-order noise alone, and one flipped expert of ten
moves the whole row past any logits bar: on Qwen3.8-Flash-Next the CPU chain on its reference
kernel bodies parts from the tuned chain by 3-7 logits with argmax flips on three rows of seven,
so a compare of two arms reads the seams only with the routing pinned
(`tests/test_gpu_resident_hc.das`; the arms' own picks off the tape are logged, never asserted).
