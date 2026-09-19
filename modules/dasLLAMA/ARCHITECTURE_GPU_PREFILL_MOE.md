# dasLLAMA Architecture - the Metal prefill driver's routed block

Companion to `ARCHITECTURE_GPU_PREFILL.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries section 2.2g, the routed block of the Metal prefill driver: the atomics-free
bucket rail, the tensor-twin scaffold the gathered expert sites ride, and the split-format
expert twins. The GEMM form ladder those sites pick from (sec.2.2c), the dev-W panel knee map
(sec.2.2d) and the dense-KQ tensor mul_mm scaffold the split-format twins derive from (sec.2.2aa)
stay in `ARCHITECTURE_GPU_PREFILL.md`.

### 2.2g The prefill MoE bucket rail {#prefill-moe-buckets}

Routing is atomics-free: a router GEMV and a select pass, then a per-expert count kernel, then
one bucket kernel that computes the padded prefix and fills the buckets. Each expert's bucket
PADS to a whole 32-row tile, every threadgroup computes the same padded prefix, and threadgroup
`e` publishes `basep[e]` for the mm and activation consumers. The bucket fill splits the entry
range into contiguous ascending per-lane chunks and scans the chunk counts, which reproduces the
serial entry order exactly, so the ordered weighted reduce is bit-stable against the CPU path
that parks each routed expert's rows and reduces them in entry order. The selection is read
GPU-side by the kernels; nothing reads back to the CPU, so encode-ahead and speculation stay
compatible.

Pad rows inside each expert's padded bucket carry a stamped sentinel and the reduce never
references them; rows past the last expert's stamped tail are unstamped stale pool bytes, which
is why validity tests compare the per-row entry against the live count, never the sentinel.

The MoE tensor twins ride one scaffold: `MetalMoeMulMmKqTensorBase` carries the expert
prologue, the staged K walk with its barrier pair, and the store; a weight format derives,
owns its weight-view bindings, and overrides the staged decode (`stage_block`) - mx4 also the
store (its per-expert bias) and the chunk shape (32-deep, 128-item quota). The q8 twin is not
a copy of this scaffold: its whole body is the tuned `tmm2d_q8u_f32` staged helper, a
different staging mechanism, so it stays its own template. The scaffold fixes its own binding numbers - `xf` at 3, `y` at 4, kargs at 5, `cnt` at 6,
`basep` at 7 - and every derived twin inherits them. Those are NOT the family tail's numbers
(`kn_moe_mm_family_tail` binds `cnt` at 7, `basep` at 8, `bkt` at 9), so a race harness
hand-binding a tensor twin follows the scaffold's declaration and its base arm follows the
tail's; binding a twin at the base's numbers hands the kernel the OUTPUT buffer as X, and the
race then crowns whichever arm computed nothing. The gather-X pass
copies the bucket's token rows into a CONTIGUOUS f16 panel with pad rows zeroed, which lets the up
and gate sites ride the contiguous tensor twins instead of the in-kernel gather form; the panel is
minted once per layer and shared by both sites. An X read through the bucket index can never
form a tensor view, which is why every tensor twin of the MoE family serves contiguous rows
only.

The split-format expert twins (k3, q40 and the iquants) do not derive from that scaffold: they
derive from the format's DENSE split class (sec.2.2aa) with the base's `MOE` axis set and run its
`stage16` under the dense base's `moe_kernel` entry, whose expert plane rides `nBase` -
`(e*ndim + n)*nsb + sb` is the plane's superblock, so the decode is one source for both the dense
and the routed site, and a table format's threadgroup prologue is one override serving both.
Their bindings are the dense layout's (`xf` 3, `y` 4, the kdim/ndim uniforms at 5 and 6) plus the
axis-gated `cnt` at 7 and `basep` at 8; `pf_moe_split_pso` and `pf_moe_split_enc` are the one place
a (format, form) maps to its pso and builder;
the dispatcher passes the site's dim uniforms, the padded panel's row count for the y span,
and the tile count as its own parameter - the K-quant twins' `npos/32` (an expert holds at most
npos rows), never the padded row count, which would launch one tile per padded row and exit all
but one. These formats have no gathered base kernel, so their twins compile behind the toolchain
probe alone (`g_pf_tensor_ok`), not behind the dense race's crown - on a GPU where the tensor
stamp loses the dense race by a few percent (M4-class) the twin is still the only Metal path,
and a few percent of a GEMM beats the CPU. Their model builds the gather panel at every prompt
length and declines `moe_twin` only where the probe failed. q40 owns no split class of its own:
its four stamps derive from the iq4xs MoE template with `IQ4NL` and `Q40` set, the same pair the
dense q40 twins carry, and because its scale plane is compact - the eight per-32-block f16 d
verbatim, no strip tail - both of its scale bindings take `soff`, where every other split format
binds the d plane at `doff`.

The select kernel is one template at two per-lane depths: 8 logits per lane covers 256 experts,
the 16-deep stamp 512 (Qwen3-Coder-Next), and the serve gate admits 512.

The bucket kernel runs one threadgroup past the experts: threadgroup `ne` publishes the padded
total at `basep[ne]`. The routed block's activation guards on that total (`MetalEw2T`'s TOTB
stamps read `totb[0] * tmul` off the basep buffer at its bind offset) rather than the CPU's
padded bound, and writes the down site's f16 X twin alongside its f32 rows (the dense HX form), so
a panel that is mostly padding - ten rows per expert at 512 experts - costs neither a convert pass
nor an activation pass over its tail.

**The staging form that wins inside the gathered mul_mm kernels is per format, not universal.**
The gathered q8 form carries its scale and quant pointers across k-blocks; the stateless index
form measures slower (`benchmarks/matmul/bench_metal_moe_lab.das`, gmm8 section). The gathered
Q6_K is the opposite: a superblock-scalar cache measures slower per mm than reloading per
k-block (same lab, gmm6 section), so its stage is stateless.
