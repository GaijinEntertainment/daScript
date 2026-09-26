# dasLLAMA Architecture - the Vulkan GEMM probe

Companion to `ARCHITECTURE_MEASUREMENT.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries section 2.5a: the arms `harness/vk_gemm_probe.das` times, the shapes they run
at and the alternates they are read against. The benchmark rig that produces recorded numbers,
the tune gate and the instrumentation rails stay in `ARCHITECTURE_MEASUREMENT.md`.

### 2.5a The Vulkan GEMM probe attributes prefill GEMM cost on three axes {#vk-gemm-probe}

`harness/vk_gemm_probe.das` times one shape at a time: the serving GEMM against its alternates
on the dense role shapes (gate/up, down, q/wo, k/v - the mm_a kernel against the cm2 l and m
tiles, the sdot4 kq tile against the k4 and k6 cm2 tiles); one decode callback against
spellings of itself with the rest of the tile held fixed (the `cm2x` and `k6x` bisect arms);
and our tile against the upstream coopmat2 GEMM blob, served in place of a probe class's body
through `DASLLAMA_VK_SPV_OVERRIDE` (the `ref` arm). The `cm2:<fmt>` arm generalizes the first
axis to any kq superblock format: it drives the prefill's own (format, column) ladders for the
l and m columns with the kq batch tile as the control row, over random block bytes at that
format's device block size, and it runs the four-wide decode's two arms (the twin served, then
stripped through `vkd_pipes_rebuild`) interleaved in one process, two rounds each, so a format's
`DECVEC` verdict comes from one instrument; every arm of the sweep dispatches over two alternating
output planes under a fresh hazard each, the form the served graph overlaps (the RTX 5080 rows the
ledger carries read the one-plane form, which serialized a chain on the plane's write-after-write
hazard). Three forms hold one output plane and one hazard rail on purpose: the `serial` rows
(`cm2g:<fmt>`) read a single dispatch's latency - the cost a dependent decode chain pays a hop -
not the overlapped throughput, the `lastwg` arm (an f32 row GEMV then `cls_ar` against the
GEMV whose last workgroup runs the epilogue) is the hand-off's retained reference beside the
shipped `Q8GemvAr`, and the `wh` arm reads the serialized cost of the whisper encoder's GEMMs the
way its chain pays them, one dependent dispatch after another. The `khrx` arm is the second axis for the KHR kq
tile. It runs eight arms: a resync copy of the shipped k4 tile with no lever moved
(`khrpx_ship`), five copies each with one lever moved back - the weight stage as a constant fill
or as the four-wide callback on the plane element, f32 accumulators, a 16-row strip tiling, and
the reference exe's 128-thread geometry - and the shipped class and the sdot4 tile as controls.
The resync copy is the row a lever's arm is read against, and its bit-exact reading against the
shipped class is what says the copies still track the shipped body. The sweep runs three whole
windows and one partial window of 300 tokens, the row that takes the edge store; the copies
stage and store whole tiles, so on the partial window only the two controls run.
`khrprof:<arm>` submits one arm alone for a GPU profiler. `wh` times the whisper large-v3-turbo
encoder's role shapes over one 30 s chunk (1500 rows, d 1280, ff 5120: q / k / v / o, fc1, fc2,
fc2 under the split-k ladder and q / k / v / o under split 2), then the same shapes at 1536 rows,
where every l column is whole - the alternate that reads what the 1500 rows' partial last column
costs the l stamp. The l and m columns are its race: their rounds alternate, each figure its best
round, each on its own row. Its feed carries `TILE_READ_SLACK` rows past the last, which the m
column's unclamped partial load reads; it skips the sdot4 mm tile, which stalls at 1500 rows on
the fc1 shape, and it needs a coopmat2 device in cm2 mode, exiting non-zero otherwise. Its
third-party row is ggml's per-op `MUL_MAT` on the same shapes, read off whisper-cli's logger -
`GGML_VK_PERF_LOGGER=1 whisper-cli -m ggml-large-v3-turbo-q8_0.bin -f jfk.wav -t 16 -bs 1 -bo 1
-nf`, the whisper.cpp build sec.2.20 of `ARCHITECTURE_MEASUREMENT.md` pins. The `mmqx` arm is the first axis for
the integer tile: the sdot4 k4 tile against register-block prototypes over the same planes. Both
sweeps time the served graph's shape first: sixteen dispatches per submit over two alternating
outputs with a fresh hazard each, the arms interleaved round by round, an arm's figure its best
round. The comparison follows the timing: each compared arm's last output is read back and
measured against the sweep's reference - the shipped class in `khrx`, the sdot4 kq tile in
`mmqx`. An arm that stages constants, or reads its own activation fixture, is timing-only and is
never read back. A khrx copy passes within a 2e-2 relative difference of the shipped class, and
reads bit-exact where the lever leaves the arithmetic alone. The shipped class itself is read
against the k4 CPU oracle on six corners of the output, at the kernel cell's bar:
`|gpu - cpu| <= 2e-2 |cpu| + 4e-3 max|y|`. The probe's exit code is non-zero on a compared arm
over its bound, a CPU-oracle miss, an unknown `khrprof` arm, or a run that produced no result
row. The `moe:<fmt>` and `moesk:<fmt>` arms are the first axis over the expert schedule: the
format's cm2 s and e stamps over the Qwen3-30B-A3B window's routed buckets (512 tokens, 8 routed
of 128 experts, 4096 bucket rows) at the gate/up shape (d 768, K 2048) and the down shape (d
2048, K 768), dispatched to the bound the device schedule sizes with its sentinel tail, both
decode arms interleaved as `cm2:<fmt>` runs them. `moe:` gives every expert 32 rows - the
reference harness's uniform profile - and `moesk:` takes the 30B window's own skew (69 experts
route, nine holding the large buckets, eight at 96 rows, four at 48, the rest at 18). Each
profile runs two schedule forms and their alternates in one process: the whole buckets at the s
column, and the ladder, where a bucket past 32 rows takes whole 128-row columns on the e stamp
with the last one partial and a remainder of at most 32 rows past them takes the s column; the
ladder again on the dense 64-deep m stamp, and - on the formats that carry one - the whole
buckets on the s stamp's other-depth twin, the probe's own stamp of the format template at the
k step the engine does not run for that format (64 where the engine runs 32, 32 where it runs
64). `cm2:<fmt>` runs the same twin beside the s stamp on a 64-row window (`gate@64`),
the dense chain's s use, and the 35B's shared expert shapes (`shexp`: 512 rows over K 2048;
`shdown`: 2048 over 512), where the m column takes 16 and 64 workgroups on a 512-token window.
`cm2d:<fmt>` runs the same sweep over the 4096-wide dense hybrid's window shapes (the
Qwen3.5-9B: the FFN pair 12288 over K 4096, its down 4096 over K 12288, the fused qkv 8192, a z
or out plane 4096 and an attention k or v plane 1024, at 512 tokens); its alternate is the
reference exe's own rate on that model's window under its concurrent logger
(`GGML_VK_PERF_LOGGER=1 GGML_VK_PERF_LOGGER_CONCURRENT=1 llama-bench -p 512 -n 0`: the q5_K
pair 124 TFLOP/s and its down 105, q6_K 112 and 94, on the RTX 5080). `cm2w:<fmt>` is the wave
sweep: the l stamp over one 256-token column (K 4096) at half-wave steps of the device's SM count
from one wave to four, so the cost of a second wave says how many of the stamp's workgroups an SM
runs at once; its alternate is the one-wave row (the RTX 5080 runs one: k5 at 84 workgroups
0.170 ms, 126 0.333, 168 0.379, 252 0.562, 336 0.746 - a partial wave costs a whole one).
`dec` times the decode head's two small kernels in isolation, each as a chain of dispatches
over its own buffers under one hazard set (the serial form the token command runs them in): the
f16 beta/alpha GEMV at the 9B's, 0.8B's and 27B's shapes (64 rows over 4096, 32 over 1024, 64
over 5120) and the fused deltanet step at 32 and 16 heads of 128; its alternates are the
reference exe's decode logger rows on the same models (its two f32 m=32 GEMVs ~5 us each,
GATED_DELTA_NET 4.1 us + SSM_CONV 5.2 at one token, RTX 5080), and a whole-token profile role
that reads past the isolated figure names the chain around the kernel, not the kernel (the RTX
5080 reads the GEMV at 4.6 / 3.0 / 4.7 us and the step at 8.8 where the token profile bills the
roles 17 and 25). The decode GEMV probe (`harness/vk_gemv_probe.das <n> <d>`) streams a ring of
plane copies past the L2 for the DRAM rate; its `single` arm dispatches one plane copy under the
same hazard chain, so the row reads the chained dispatch's floor past its bytes (4.5 us on every
format at 0.6 MB planes, RTX 5080) and the served plane L2-warm - the two figures a token's GEMV
role sits between. Its `cold` arm dispatches one plane copy a dispatch and takes the ring's copies
in turn - the served launch size in the DRAM regime - with the warm `single` row as its alternate
(the pod's RTX PRO 4500, `-jit`, Llama-3.2-1B's shapes: gate 2048x8192 q8 28 us, 0.30 of the
four-copy streaming row; down 8192x2048 35 us, 0.36; q 2048x2048 13 us, 0.37; cls 344 us, 0.26).
Its `cols` ruler holds the one-column class beside the N-column class at 1, 2, 4 and 8 rows; the
chain alternates two output planes under their own hazard bits, so consecutive dispatches overlap
the way the batched step's GEMVs do and the `vs N single` column compares two overlapped chains;
the q8 rows engage the row-pair form, and `n2off` holds them to a row a subgroup.
The reference row is `test-backend-ops perf MUL_MAT_ID` at `n_mats=128,n_used=8,m=768,n=512,k=2048`.
Two arms read the window chain's own overheads at those shapes rather than a tile: `ts:<fmt>`
dispatches two m stamps back to back into two planes - plain, with the profile's bottom-of-pipe
timestamp between them, with a barrier between them (the second writes the first's plane), and
with the split-k reduce between them on that plane - so a driver that drains the queue at a
timestamp reads the stamped pair at the barrier row (the Linux 580 driver does: 103 us plain
against 210 stamped and 209 with a barrier on the RTX 5080; the Windows 616 driver reads the
plain figure on all but the barrier row), and a pipeline change that costs the device a
reconfiguration reads the reduce row past the barrier row by more than the reduce (neither
driver does). `cold:<fmt>` runs the l, m and s stamps warm (one copy of the planes, L2-resident
across dispatches) and cold (a ring of 96 copies, past a 64 MB L2), the m stamp with its
schedule words in host memory (the prefill's meta) and over the engine's plane forms (the weight
planes at the far end of a 3 GB slab; the feed and output planes sized to an 8192-row window),
and the three stamps taking turns over the ring; the warm row is the alternate every other row
ranks against (the RTX 5080 reads the 64-workgroup m stamp at 26-27 us on every row). The arm's
flush row is the split-k reduce copying a 128 MB plane between two dispatches of one stamp, so
the next dispatch finds its code, descriptors and planes out of every cache - the weight a
window chain streams between two uses of one stamp; the flush writes the feed's region, so the
stamp after it waits on the flush and the next flush waits on the stamp.
