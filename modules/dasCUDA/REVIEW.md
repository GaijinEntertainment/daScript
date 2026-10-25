# dasCUDA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_EMIT.md`. Planned work: `LEDGER.md`.

**Every change under this folder applies `modules/REVIEW_SHADER_EMITTERS.md` too** - the rules
binding a kernel body, a launch of an emitted kernel and an emitter's lowering live there.

**A `[test]` file with a `require cuda/...` line of its own, wherever the diff puts it, answers to
`tests/cuda/REVIEW.md` (repo root).**

**A new or changed extern in `cuda/cuda_driver.das`, or a new or changed public function in
`cuda/das_cuda_boost.das`, ships a host-side test under `tests/cuda/` (repo root) in the same
change.**

**A diff that adds an extern to `cuda/cuda_driver.das` sets its `name=` to the `_v2` export where
the driver API header's `#define` maps the API to one; the das name stays unsuffixed.** The
unsuffixed export is an older entry point the library may stop exporting.

**A diff that adds an extern to `cuda/cuda_driver.das` gives it the same `windows_library` and
`linux_library` candidate lists as the other externs of the same library.**

**Never add a C++ source under this module beyond the `cuda` witness `src/dasCUDA.cpp`, and
never give the witness a binding - a driver or toolkit entry point binds through a dasbind
`[extern(late)]` instead.**

**Never bind managed memory (`cuMemAllocManaged`) and never return a device address as a host
pointer - bytes move through `cuda_memcpy_*` and `cuda_memset_d8`, and a host-visible block is
the mapped pair `cuda_mem_alloc_host_mapped` hands back.**

**A diff that changes how `cuda_launch` hands the parameter space to the driver keeps the bytes
of the struct passed as `params` - its fields in the kernel's parameter order at natural
alignment - as the kernel's whole parameter space.** The emitter writes the kernel signature to
that layout.

**A diff to `cuda/cuda_emit.das` never makes an emitted text include a header - spell a
conversion or tensor-core form as inline PTX instead.** NVRTC finds no header without the
toolkit's include dir, and a deployed box does not have it.

**A diff that adds, drops or changes an input of `cubin_cache_key` names in the PR body that
input and why a cubin compiled before the change cannot be served after it.**

**A diff that makes `cuda/cuda_emit.das` write a new text form - a construct, an operand type, or
a new spelling of an existing construct - records a census kind of its own for that form through
`census_kind` and adds the kind to `declared_cuda_census` (`tests/cuda/_cuda_common.das`, repo
root) in the same change.** Two forms sharing one kind let either go untested.

**A diff that adds an emitter rejection a `[cuda_kernel]` body can reach, or changes a
rejection's text, adds a case to `tests/cuda/test_cuda_reject.das` (repo root) asserting a
substring of that text, in the same change.**

**A diff that changes the arithmetic, indexing or synchronization `cuda/cuda_emit.das` writes, or
makes it write such a form it did not write before, adds a case to
`tests/cuda/test_cuda_oracle.das` (repo root) that runs a kernel using the form on the CPU and on
the device and compares the results.**

**Never hand-write a kernel text under this module, in a string or a `.cu` file - emit it from a
`[cuda_kernel]` body instead.**

**A diff that adds a name a `[cuda_kernel]` body can call puts it in
`modules/dasSpirv/spirv/spirv_builtins.das` when SPIR-V has an op or a GLSL builtin that computes
it, and otherwise in `cuda/cuda_intrinsics.das` with a row in the table at
`ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers`; the PR body names the file it took.** A body
that calls a `cuda_intrinsics` name compiles only for CUDA, so a name SPIR-V can express never
lands there.

**A diff that changes the order of a device function's hidden parameters - the builtins and
members it takes after its own parameters - changes `hidden_params` and `hidden_args` in
`cuda/cuda_emit.das` together.** A change to only one of them still compiles on the das side and
fails only in NVRTC, which CI does not run.

**Weakening the descriptor census in `REVIEW.das` - the check that every `cuda/*.das` is
registered in `.das_module` and every name registered there exists on disk - is a defect.**
