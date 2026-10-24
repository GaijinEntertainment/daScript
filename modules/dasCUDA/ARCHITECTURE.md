# dasCUDA - native CUDA compute: the driver API and NVRTC as dasbind externs

Contract: `ARCHITECTURE_COMMON.md` (repo root). Rules binding a diff: `REVIEW.md`. What the emitter
writes - the emitted kernel, the decoded tile's pipeline, the kernel vocabulary's two tiers - is the
companion `ARCHITECTURE_EMIT.md`.

## Why

Vulkan runs the whole dasLLAMA engine on NVIDIA cards and stays the cross-vendor GPU tier. CUDA
is the vendor-native path on the same cards: the tensor-core forms Vulkan compute never
exposes (fp8 and block-scaled FP4 `mma.sync`, `ldmatrix`, `cp.async`), the datacenter cards
where nobody tunes a Vulkan driver, and the rigs a customer already runs CUDA on. The module is
the third kernel home beside dasMetal and dasVulkan, and it takes the hybrid shape: the Vulkan
kernel bodies, which are written in the shared `gl_*` / `@ssbo` / `barrier()` vocabulary the
MSL emitter already reads, over Metal's host machinery, since CUDA's kernel model is Metal's -
plain pointers, a by-value argument struct, streams in place of command buffers.

## Settled decisions

- **Pure daslang: the driver API and NVRTC are dasbind late externs, the OpenGL model.** Both
  libraries export plain C functions over handles (`void?`), device addresses (`uint64`) and
  `int` results, and the surface this module needs carries no structure, so `cuda/cuda_driver.das`
  declares each entry point with `[extern(cdecl, late, name=..., windows_library=...,
  linux_library=...)]` and nothing is compiled: no `cuda.h`, no `nvrtc.h`, no toolkit at build
  time, no C++ binding, and an installed SDK runs it without a rebuild. The structs the wider
  driver API carries - strided 2D and 3D copies, texture descriptors, explicit graph node
  parameters, the virtual-memory allocation properties - sit outside this surface; one that
  becomes needed is a das struct of scalars passed by address, still an extern, never a binder.
  `cuLaunchKernelEx` is the one so far: `CuLaunchConfig` and `CuLaunchAttribute` in
  `das_cuda_boost.das` mirror the driver's structs field for field (the attribute's 64-byte
  value union as eight words), and `cuda_launch` takes that path whenever the programmatic
  dependent launch flag is on (see "Programmatic dependent launch" below).
- **The driver binding and its boost serve the interpreter and the JIT, never AOT.** A late
  extern is a dasbind proxy, and the AOT emitter spells a proxy by its mangled name, which no C++
  compiler accepts; `cuda_driver.das`, `das_cuda_boost.das` and the plumbing test under
  `tests/cuda` carry `options no_aot`, the module's AOT file list is empty, and a program that
  AOTs its own functions still calls the binding through the interpreter, as dasOpenGL's externs
  are called.
- **The das name drops the `_v2` suffix the library exports under.** `cuMemAlloc` names
  `cuMemAlloc_v2`, `cuMemcpyHtoD` names `cuMemcpyHtoD_v2`, `cuStreamDestroy` names
  `cuStreamDestroy_v2`; `cuda.h` hides the suffix behind macros and the export table does not.
- **A library is named per toolkit major and the declaration lists every major it knows.**
  NVRTC is `nvrtc64_130_0.dll` under toolkit 13 and `libnvrtc.so.12` under 12, so the extern's
  library is a `;`-separated candidate list, newest first, and dasbind binds through the first
  name that loads. A toolkit outside `PATH` is reached through `CUDA_PATH` by
  `cuda_nvrtc_library_loads`, which loads the file by its full path first so the later
  bare-name resolve finds the module already in the process. A box without NVRTC still runs
  cubins; `cuda_nvrtc_available()` says which box this is.
- **"No CUDA" is the build configuration's word, never the filesystem's.** The `cuda` witness
  module (`src/dasCUDA.cpp`, the module's one C++ source) binds nothing: it is compiled in
  exactly when the package is configured, which `CMakeLists.txt` admits on 64-bit hosts with
  `DAS_CUDA_DISABLED` off, and the descriptor registers it through the two-argument
  `register_dynamic_module`, which skips silently where no `.shared_module` was built. The
  `cuda/` files sit on disk in every checkout, so a path guard (`require ?cuda/cuda_driver`)
  would read a disabled build as present; every consumer guards on the witness instead -
  `require ?cuda <target>` in dasLLAMA's engine, `has_module("cuda")` in `tests/.das_test`,
  `builtin_module_exists(cuda)` under a `static_if` - and a disabled build loads no CUDA home
  and runs no `tests/cuda` cell.
- **64-bit hosts only; never Apple, never wasm.** The descriptor registers the `cuda/` paths
  only where a pointer is 8 bytes: no CUDA driver exists for a 32-bit host, and `[extern]`
  refuses to apply there (`DAS_BIND_EXTERNAL` is off), so on a 32-bit host `require cuda/...`
  is a missing module. On a 64-bit host the files compile everywhere; `cuda_available()`
  answers false where no driver library loads, and a consumer feints on it.
- **Device memory is a 64-bit device address, never a host pointer.** `cuda_mem_alloc` returns
  a `uint64`; the host reaches it only through explicit copies (`cuda_memcpy_*`), with pinned,
  device-mapped host memory (`cuda_mem_alloc_host_mapped`) where a kernel's inputs or its answer
  cross with no copy-engine work. Managed memory is not bound: its semantics differ between WDDM
  and Linux, and explicit copies are one model on both.
- **A launch takes the kernel's whole parameter space as one byte blob.** `cuda_launch` hands
  the driver `params` + `params_len` through the launch's buffer-pointer extra, so a das struct
  laid out as the kernel's by-value parameters in order - a struct of buffer addresses first, the
  args struct second - is the launch argument. A CUDA kernel has no binding slots: a buffer is a
  pointer parameter and its "binding" is its position, so the emitter writes the `@ssbo` members
  as one `struct KBufs` of pointers in `@binding` order, a gap in the numbers costing nothing, and
  the filled struct is the cached descriptor set the Vulkan driver keeps by its buffer tuple.
  Parameters live in the `.param` constant bank either way; a struct member read is the same
  `ld.param` as a bare pointer parameter.
- **NVRTC ships no builtin headers.** `cuda_fp16.h`, `cuda_bf16.h` and `mma.h` resolve only with
  the toolkit's include dir on the command line, which a deployed box does not have. A kernel
  text is therefore self-contained: the emitter's prelude spells half and bf16 conversion as
  inline PTX and the tensor-core forms as inline `mma.sync`, and no kernel includes a header.
- **Cubins, cached once per process-wide key.** NVRTC takes 35 to 50 ms a kernel and the driver's
  PTX JIT another 17 at module load, while a cubin loads in 0.2 ms; a few hundred kernels per
  model make the compile path a 20 s start. `cubin_cached` keys a text by NVRTC version, driver
  version, options and source under `.cache/daslang/cuda/` in the working directory - the JIT
  object cache's home - shared by every model and every run; a caller that must not share it
  (a test, into its run's temp dir) names its own `cache_dir`. An arch-specific target
  (`sm_120a`, the FP4 forms) has no forward PTX JIT, so a kernel that asks for one bakes per
  arch and the key carries the suffix through the options.
- **The driver's current context is per thread.** `cuda_create_context` retains the device's
  primary context and binds it to the calling thread, and every later call runs on that thread.
- **Every handle and every device allocation is counted.** Driver objects and device memory are
  invisible to all six das leak detectors; the boost counts each live kind and
  `cuda_live_object_report` lists them, so every test file closes on
  `cuda_log_leaks_if_any() == 0`.

### Programmatic dependent launch {#cuda-programmatic-dependent-launch}

Every kernel launches under programmatic dependent launch where the home asks it
(`cuda_set_launch_pdl`). The launch carries the programmatic stream serialization attribute,
so the driver may launch a kernel while the one before it in the stream still runs, and the
emitter puts the pair `griddepcontrol.wait; griddepcontrol.launch_dependents` at the top of
every kernel on sm_90 and up: the wait holds the kernel's reads until its predecessor completes
and its memory is visible, the trigger lets the kernel after it launch at once. The dependent's
blocks sit resident at their wait while the predecessor finishes, so the launch latency and
the block scheduling a kernel paid at the stream's head ride under its predecessor's tail;
inside a captured graph the same attribute becomes a programmatic edge. Without the attribute,
or on an older arch, both instructions are no-ops, so a kernel text is one text for every
launch mode. Only a stream whose every kernel came through the emitter may turn the flag on: a
kernel without the wait would read its predecessor's output early.

## Files

| File | Purpose |
|---|---|
| `cuda/cuda_driver.das` | The externs: the driver API in seven families - init and device, context, module and function, memory and copies, streams and events, graphs (stream capture, instantiate, launch, destroy), launch and error text - and NVRTC's program lifecycle; the attribute and flag constants the calls take; the two library probes. |
| `cuda/das_cuda_boost.das` | Host sugar over the externs: `cuda_available` / `cuda_nvrtc_available`, `with_cuda_context`, the error text and the last-error slot, the live-object registry and its gate (`cuda_log_leaks_if_any`), `cuda_compile`, the shared cubin cache (`cubin_cached`, `module_from_source`, `function_from_source` - the entry point with its dynamic-shared opt-in), device buffer helpers, `cuda_launch` / `run_compute_1d`, the graph verbs (`cuda_graph_capture` captures a block's launches on a stream into one executable graph, `cuda_graph_launch` replays it as one launch with the parameters as captured, so a step's inputs ride buffers the captured launches read; `cuda_graph_destroy`). |
| `cuda/cuda_types.das` | The CUDA side of the type rail over `daslib/shader_emit_common` (the lattice spellings) and `daslib/shader_block_layout` (the classes): the half-family predicate and a shared element's byte size. |
| `cuda/cuda_builtins.das` | The kernel vocabulary: a public re-export of `spirv/spirv_builtins` (the compute IDs, barriers, atomics and the subgroup set), so a class the Vulkan home stamps stamps here unchanged. |
| `cuda/cuda_emit.das` | The text emitter: `generate_cuda(fn, var errors, cfg, var census, var smem) : string`. Manual recursion (`emit_value` / `emit_stmt`), the member and body scans, the prelude generators, the census at every emit site. What it writes: `ARCHITECTURE_EMIT.md`. |
| `cuda/cuda_intrinsics.das` | The CUDA-only tier of the kernel vocabulary (the table under "The kernel vocabulary's two tiers" in `ARCHITECTURE_EMIT.md`): AST markers the emitter lowers by name, spelled as PTX/CUDA spells them, with CPU bodies that emulate the pure lane functions and stand in for the rest. A body that calls one is a CUDA-only kernel by construction. |
| `cuda/cuda_kernel.das` | `[cuda_kernel]` function-macro on a class method (`name=`, `entry=`, `local_size_x/y/z=`, `min_blocks=`, `precise=`). `apply` declares the companion globals - the text, `_entry`, `_census`, `_smem`, `_block` - and `fixup` fills them with constant nodes; `patch` pins the module's reachable functions against the CPU inliner. The companion-global scaffolding is `daslib/shader_emit_common`'s, shared with `[metal_kernel]`. |
| `CMakeLists.txt` | `ADD_MODULE_DAS_FROM_DESCRIPTOR(cuda cuda)` and the install rule; nothing is compiled. |

## Test architecture

`tests/cuda/` (repo root) holds every gate, and each `[test]` compiles on every lane:

1. **The host gate** (`test_cuda_plumbing.das`): driver, NVRTC, the cubin cache, a hand-written
   kernel launched with the struct-of-pointers parameter blob. It tests the driver binding without
   the emitter, so `tests/cuda/test_cuda_plumbing.das` holds the one hand-written kernel text.
2. **The text layer** (`test_cuda_emit.das`): the emitted shape of every construct family on the
   `_cuda_common.das` fixtures, plus the compile gate - every fixture's text through NVRTC for
   the card present.
3. **The census** (`test_cuda_census.das`): the union of construct kinds the emitter records
   across every fixture equals `declared_cuda_census`, both directions - text has no
   disassembler, so the census is the emitter's coverage.
4. **The behavioral gate** (`test_cuda_oracle.das`): the same kernel method runs on the CPU - a
   driver loop sets the builtin IDs per thread and calls it on a class instance whose members
   are plain arrays - and on the GPU through the emitted text; ints compare bit-exact, floats
   under the fastmath tolerance, a `precise` kernel's floats bit-exact; kernels whose meaning
   lives in the warp (reductions, shared hand-offs, atomics) compare against hand-computed
   expectations.
5. **The rejection gate** (`test_cuda_reject.das`): a kernel class the emitter must refuse is
   compiled at run time from source text the test holds (`set_file_source` + `compile_file`; a
   file on disk would fail the lint lane by design), and the refusal's text is the assertion - a
   rejection that stopped firing would emit a kernel whose device reads differ from the Vulkan
   home's.

The fixtures carry `[marker(no_coverage)]`, which keeps the coverage instrumentation off the kernel
bodies the emitter reads. The coverage measure over the real kernels is `utils/internal/cuda-survey/main.das`: every
`[spirv_kernel]` method of a Vulkan module file through `generate_cuda` unchanged, each emitted
text through NVRTC, the refusals bucketed by message. Its number on
`dasllama_vulkan_classes.das` is the arc's progress toward one shared kernel source.

CI has no NVIDIA runner, so the GPU arms feint where no driver, NVRTC or device is; execution
is a local box and the pod. Every file closes on `cuda_log_leaks_if_any() == 0`.
