# dasLLAMA Architecture - the Metal prefill driver's routed block

Companion to `ARCHITECTURE_GPU_PREFILL.md`; a section is cited by its anchor. This
document carries the routed block of the Metal prefill driver: the atomics-free
bucket rail, the tensor-twin scaffold the gathered expert sites ride, and the split-format
expert twins. The GEMM form ladder those sites pick from (`ARCHITECTURE_GPU_PREFILL.md#prefill-gemm-ladder`), the dev-W panel knee map
(`ARCHITECTURE_GPU_PREFILL.md#devw-panel-knees`) and the dense-KQ tensor mul_mm scaffold the split-format twins derive from (`ARCHITECTURE_GPU_PREFILL.md#prefill-kq-tensor-scaffold`)
stay in `ARCHITECTURE_GPU_PREFILL.md`.

### The prefill MoE bucket rail {#prefill-moe-buckets}

Routing is atomics-free: a router GEMV and a select pass, then a per-expert count kernel, then
one bucket kernel that computes the padded prefix and fills the buckets. Each expert's bucket
PADS to a whole 32-row tile, every threadgroup computes the same padded prefix, and threadgroup
`e` publishes `basep[e]` for the mm and activation consumers. The bucket fill splits the entry
range into contiguous ascending per-lane chunks and scans the chunk counts, which reproduces the
serial entry order exactly, so the ordered weighted reduce is bit-stable against the CPU path
that parks each routed expert's rows and reduces them in entry order. The selection is read
GPU-side by the kernels; nothing reads back to the CPU, so encode-ahead and speculation stay
compatible.

The router's logits are one half x half GEMM (`pf_enc_router_mm`): the routed rows' half twin
against a half panel of the layer's router rows, a megabyte a layer kept for the weights' life. At
2048 positions and 256 experts it runs thirteen times the batched slab GEMV's rate
(`harness/router_race.das`). The GEMV serves a router with a bias, an expert count off the 64-wide
tile, and a window with no twin. The GEMM's operands are halves, so its logits sit 3e-4 of their
largest from the GEMV's, and a near-tie between two experts can fall either way.

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
and gate sites ride the contiguous tensor twins instead of the in-kernel gather form at every window
size - the in-kernel gather is the slower form from a 17-token window up; the panel is
minted once per layer and shared by both sites. An X read through the bucket index can never
form a tensor view, which is why every tensor twin of the MoE family serves contiguous rows
only.

A routed site rides one of two ladders over an expert's rows. Below a mean of 32 rows an expert
(`set_metal_moe_tall_avg`) the 32-row stamp serves every tile. From that mean up the tall pair
serves: the 128-row stamp takes each expert's whole 128-row tiles and the remainder stamp its last
rows in 32-row tiles past them. Both ladders compute a row the same way, so the logits are the
same bit for bit; the pair is the faster one from 1024 tokens up on a 256-expert model. A 64-row
rung between the pair's two stamps serves the same rows in the same time, so the ladder carries none.

A 32-row tile runs its tensor op at the rows its expert leaves it (`ADAPT`, on both scaffolds): an op
of eight rows for one to eight live rows, sixteen for nine to sixteen, else the thirty-two - three
walks of one op each, each in its own scope, because the emitter's cooperative-tensor declarations
are block-scoped and an op's begin, steps and store must share one. The tile's cost is the op's, not
the live rows': over 128 experts of a 1024 x 2048 plane the 32-row stamp reads the same time whether
an expert holds 4, 12 or 32 rows, the staging alone a third of it and the op alone three quarters,
and the adaptive op reads 0.57 / 0.69 / 1.00 of the fixed tile at those counts, bit-equal on every
live element (`benchmarks/matmul/bench_metal_moe_tile_lab.das`: the shipped form, the fixed 32-row
form it replaced, and the two knockouts). The mx4 twin keeps the 32-row op, since its per-expert
bias store is MT-only.

The split-format expert twins (k3, q40 and the iquants) do not derive from that scaffold: they
derive from the format's DENSE split class (`ARCHITECTURE_GPU_PREFILL.md#prefill-kq-tensor-scaffold`) with the base's `MOE` axis set and run its
`stage16` under the dense base's `moe_kernel` entry, whose expert plane rides `nBase` -
`(e*ndim + n)*nsb + sb` is the plane's superblock, so the decode is one source for both the dense
and the routed site, and a table format's threadgroup prologue is one override serving both.
Their bindings are the dense layout's (`xf` 3, `y` 4, the kdim/ndim uniforms at 5 and 6) plus the
axis-gated `cnt` at 7 and `basep` at 8; `pf_moe_split_stamps` is the one place a format maps
to its psos and builders per form;
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

The CPU sizes the bucket panel and the padded-row passes at `pf_moe_padded_rows`: the routed rows
plus 31 pad rows for every expert that can hold a row, and at most `min(ne, mtot)` experts can, so a
2-token window on 256 experts is sized at 512 rows where the all-experts worst case is 7968 - the
gather and the activation over the panel scale with the tokens, not the expert count, until every
expert holds a row. The bucket kernel runs one threadgroup past the experts: threadgroup `ne` publishes the padded
total at `basep[ne]`. The routed block's activation guards on that total (`MetalEw2T`'s TOTB
stamps read `totb[0] * tmul` off the basep buffer at its bind offset) rather than the CPU's
padded bound, and writes the down site's f16 X twin alongside its f32 rows (the dense HX form), so
a panel that is mostly padding - ten rows per expert at 512 experts - costs neither a convert pass
nor an activation pass over its tail.

### The short window's gathered route {#prefill-moe-gemv-route}

A window of `pf_moe_gemv_knee` tokens or fewer - the sidecar's `metal_moe_gemv_max` where it names
one, 0 for never, else 16 - whose expert planes all have a gathered decode kernel
(`moe_fmt_metal_served`, or the mx4 planes) serves its routed block through the decode step's
kernels instead of the bucket rail's tiles: the gate and up sites as the gathered GEMV over one
dispatch slice per (token, slot) pair off the selection the route wrote, the activation over the
live `npos x k x n_ff_exp` rows alone, the down site the same way over the hidden rows, and the
decode's combine summing the k slot rows under their weights. No gather panel, no padded rows, no
f16 convert; the selection buffer, the gate, up and down panels and the output rows are the ones
the tile path binds. The block runs at the reduce's seat, after the shared expert's GEMMs read the
residual rows it overwrites.

Why a tile loses there: the tile path dispatches one 32-row tile per expert on a z-grid of every
expert, each tile walking K serially under barriers, so a 2-token window on 256 experts runs 128
working threadgroups - one per core, the GPU idle between barriers - at 16 times the time its
expert bytes take to stream; at 64 tokens it is still 4.5 times. The gathered GEMV reads an expert's
weights once per routed token, so its cost grows with the tokens - with the layers, the slots and the
expert bytes a token reads: 2.2 to 4.5 ms a token on the M1 Max, a third of that on the M5 Max -
while the tiles' cost grows with the M-tile count and the experts a window populates. The two cross
between 16 and 128 tokens by model and by box: near 128 for the 256-expert Qwen3.6 hybrid on the M1
Max's base-form tiles, under 32 for the 48-layer Qwen3-30B and for gpt-oss on either box, near 56 for
the hybrid on the M5 Max's tensor twins. The default knee is the window every swept carrier won at,
16; a deployment that serves one model sets the sidecar's knob at that model's crossing
(`benchmarks/prefill_window_probe.das` reads it, the readings sit in `PERF_LEDGER.md`), and the form
that serves the band above it is `followup_metal.md`'s. llama.cpp's Metal backend takes the same route below 32 tokens, MLX below 128; neither
has a form that reads an expert's weights once for its few rows, which is the form that beats both
on the band between (`followup_metal.md`).

**The staging form that wins inside the gathered mul_mm kernels is per format, not universal.**
The gathered q8 form carries its scale and quant pointers across k-blocks; the stateless index
form measures slower (`benchmarks/matmul/bench_metal_moe_lab.das`, gmm8 section). The gathered
Q6_K is the opposite: a superblock-scalar cache measures slower per mm than reloading per
k-block (same lab, gmm6 section), so its stage is stateless.
