# dasLLAMA Vulkan GEMM Tile Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_SHARED_GEMM.md`, `ARCHITECTURE_GPU_VULKAN_GEMM.md`. Planned work:
`followup_vulkan.md`, `followup_general.md`.

**Never take a quant byte out of an `unpack8` select in a superblock format's (a format whose
block holds 256 weights - `kq_sb` true) cm2 (NV_cooperative_matrix2 GEMM tile) decode body - the
`decode` method of its `<Fmt>Cm2T` class in `dasllama/dasllama_gpu_kernels_discrete.das`, or its
four-wide twin (`decode_v4`) - load the 16-bit lane from the `int16[N]` block member and shift the
byte out, and assemble a field that straddles two lanes from those lanes, never from selected
bytes.** Indexing `unpack8` of a 32-bit word with a runtime value drops the whole kernel off the
driver's block-load path (`ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes`).

**The coordinate a k loop's counter (the loop that steps along the dot product's length, k)
feeds to `coopmatLoadTensor` / `coopmatLoadTensorDecode` starts at a literal or at a value
rounded down to the loop's step - never at a bare runtime product or a buffer-read value.** That
coordinate is the call's `c0` argument where k runs along columns, its `r0` argument where k runs
along rows. The shader compiler vectorizes the decode-load only where it can prove the
coordinate's alignment, and an unproven start runs the same loop at half the rate
(`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`).

**A cm2 tile's fast path loads weights through an unclamped `tensorLayout2D`, never a clamped
`tensorLayout2DPad`.** A cm2 tile is `cm2_tile` of `KqCm2BatchT` in
`dasllama/dasllama_gpu_kernels_discrete.das`, the NV_cooperative_matrix2 GEMM body stamped per
weight format. Its fast path is the branch taken when the tile's 128 weight rows fit the plane,
the plane's width is a whole number of k steps (`BK`), and the token column (`BN` wide) is whole
or the stamp sets `STILE`, which loads partial columns unclamped. A clamped decode-load runs every
tile at a third the speed (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`).

**On a plane of 128 rows or more, a cm2 tile's (`cm2_tile` of `KqCm2BatchT`) partial last weight
tile starts at the plane's last whole 128 rows (`d - 128`), never past the plane's end.** A load
past the end reads memory the plane does not own. Sending the partial tile down the clamped edge
path would hold the whole dispatch to that tile's speed
(`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`).

**A scale cache a cm2 tile stages (`sc_cache`, `sc6_cache` - a `@workgroup` array of the tile's
per-row sub-block scales) indexes a row by its offset from the tile's first row (`wg_m0`), never
by `bc.x & 127`.** `bc.x` is the decode callback's row coordinate, which is the plane's absolute
row, and a partial last weight tile starts at the plane's last whole 128 rows, off the 128 grid.

**Never split k (cut a GEMM's dot-product length into chunks whose partial planes `SplitKReduce`
sums) on a prefill GEMM whose output rows start above row 0 - encode it whole, whichever splitter
would split it (`cm2_gemm_pick`, or `F16GemmCm2`'s `F16G_SPLIT` chunks).** The reduce sums
partial planes counted from row 0, so a split dispatch starting above row 0 reduces the wrong
rows.

**A diff that changes the SPIR-V words a cm2 tile stamp emits - its `DASLLAMA_VK_SPV_DUMP` output
against the base branch's - puts that stamp's probe rows (runs of `harness/vk_gemm_probe.das`) in
the PR body, with `DASLLAMA_VK_DECVEC=1` and with `=0` (the four-wide decode callback on and
off).** A cm2 tile stamp is a class stamped from `KqCm2BatchT` through a format's `<Fmt>Cm2T` with
`KHR` false (`<Fmt>Cm2{L,M,S,E}Batch`).

**A diff that owes a cm2 stamp's probe rows takes them from that stamp's `harness/vk_gemm_probe.das`
arm - `cm2:<fmt>` or `cm2g:<fmt>` for the l and m stamps (`<Fmt>Cm2LBatch`, `<Fmt>Cm2MBatch`),
`cm2:<fmt>` and `moe:<fmt>` or `moesk:<fmt>` for the s stamp (`<Fmt>Cm2SBatch`), `moe:<fmt>` or
`moesk:<fmt>` for the e stamp (`<Fmt>Cm2EBatch`) - never
from another arm and never from a whole-model sweep; a stamp the probe admits no arm for answers
with a run of its kernel cell (its test block in `tests/test_vulkan_kernels.das`) instead.**

**A diff that changes the SPIR-V words a KHR tile emits - its `DASLLAMA_VK_SPV_DUMP` output
against the base branch's - runs that format's kernel cell (its test block in
`tests/test_vulkan_kernels.das`) on a device where the cell's KHR arm passes rather than skips
(`DASLLAMA_COOPMAT=mm` on NVIDIA, or a device without NV_cooperative_matrix2 running at 32 lanes)
and puts the run in the PR body.** A KHR tile is a `<Fmt>KhrBatch` class in
`dasllama/dasllama_vulkan_classes.das`, stamped from `KqCm2BatchT` through a format template
(`<Fmt>Cm2T`, or `F32RowsKhrT` for the f32 rows), or an `MmBatchT` stamp (the mm l and m tiles).

**A diff that owes a KHR tile's kernel-cell run puts in the PR body, beside that run, either the
`khrx` probe rows (`harness/vk_gemm_probe.das`) or a `tests/test_gpu_resident_hybrid.das` run on a
model in that format.**

**A diff that adds a `<Fmt>KhrBatch` stamp adds that format's KHR arm to its kernel cell in
`tests/test_vulkan_kernels.das`, in the same change.**

**A kernel body that calls a `[spirv_decode]` method directly passes the plane element itself
(`decode(wq[i], ...)`), never a local copy of it (`let blk = wq[i]` then `decode(blk, ...)`).**
Both compile: on the element the emitter passes the block's index and the callee reads the
block's fields from the plane by it; on a copy it copies the whole block into local memory on
every call (`ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes`).

**A diff whose probe rows (`harness/vk_gemm_probe.das`) show a format's `DASLLAMA_VK_DECVEC=1` row
(the four-wide decode callback on) slower than its `=0` row ships one of two fixes in the same
change: a hand-written `decode_v4` under `override DECV4 = true` on that format's class
(`dasllama/dasllama_gpu_kernels_discrete.das`), re-measured so its `=1` row now beats its `=0`
row; or `override DECV4 = false` and `override DECVEC = false` together, which puts the format
back on the scalar callback.** With `DECV4 = true` the class never reads `DECVEC`, so
`override DECVEC = false` alone leaves the hand-written twin running.
