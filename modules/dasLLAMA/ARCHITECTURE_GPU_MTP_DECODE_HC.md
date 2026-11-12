# dasLLAMA Architecture - the Metal decode driver's multi-row hyper-connection mixer

Companion to `ARCHITECTURE_GPU_MTP_DECODE.md`; a section is cited by its anchor.

### The eight-row mixer is four dispatches on the matrix unit {#metal-hc-mix8}

**At three to eight rows over four streams the mixer site runs as four dispatches, not the eight of the rows chain.** The
rows chain (`hc_mix_rows` at any other row count, or under `DASLLAMA_METAL_HC_MIX8=0`) runs the grouped norm, the inject
router, a rows8 prep and the q8 down, `silu(lo / hc)`, a second prep and the q8 up, and the gated mean; its down is a plane
of 320 rows - 20 rows8 threadgroups on a GPU of 40 cores - and every prep is a dependent link. `pf_enc_hc_mix8`
(`dasllama/dasllama_metal_prefill.das`) runs instead:

- `MetalHcPrep8`: a workgroup a (stream, row) norms the stream in `MetalHcNorm`'s order - so the normed stream is the
  chain's bits - and stores it as the rows8 x table and block sums in `MetalRows8Prep`'s order, the inverse norm, and the
  inject rows' partials over the f32 normed stream.
- `MetalRows8Q8Hc` (`MetalRows8T` under `KSPL`): the q8 down with K split by stream - the grid's y the stream, its
  `kseg` blocks, the normed partials at [stream][token][row] - so the plane runs 80 threadgroups.
- `MetalHcFin8`: the four partials summed, `silu(lo / hc)` as the up's x table and sums, and the scatter logits.
- `MetalRows8Q8HcMix` (`MetalRows8T` under `HCMIX`): the q8 up over a threadgroup's 16 columns of all four streams, two
  simdgroups a stream tile each half of K; the slices meet in the template's threadgroup memory and a thread a (column,
  row) lands the gated stream mean, re-forming the normed stream off the wide row and the stored inverse norm.

The stamps keep the template's one q8 fill and scale law, so `check_rows8_kernel_strides` stays the one gate, and the plain
stamps' generated kernels are unchanged. A site whose planes are mlx6b or mlx8b runs the same four dispatches with the
template's affine arms under the same constants (`MetalRows8Mlx6bHc` / `MetalRows8Mlx6bHcMix`, the mlx8b pair): the prep's
table and the finish's are format-free, the down's segment counts 64-groups (`g_hc8_u_kseg64`), and the affine epilogue's
bias term reads the same tables' sums, so the finish's inverse norm scales the pair's two terms alike. The form needs the rows8 layout probe, four streams, whole 32-blocks and a site with
scatter logits (`metal_hc_mix8_serves`); the head mixer, which has none, keeps the chain. `metal_hc_mix8_sites` counts the
sites. The pad tokens' table rows hold whatever the scratch last held: they feed the products' pad columns alone, and the
landing writes the live rows only.

**The verdict is the raw-MSL race's** (`harness/hc_mix_race.das`, Flash-Next's shapes, twelve site copies past the SLC): the
form reads 0.55 of the chain and 0.63-0.66 of the one-row pair, its rows within 1e-5 of the chain's over the rows' magnitude.
SIMT forms of the same split - a lane's load serving eight f32 rows - lose to the chain (1.1-1.7 of it): issue-bound on the
byte loads and the eight rows' registers. The device cell is `hc_mix8_gate` (`tests/test_metal_misc_kernels.das`): the four
dispatches against the chain's kernels at eight and five rows, over planes scaled off the sigmoid's saturation.
