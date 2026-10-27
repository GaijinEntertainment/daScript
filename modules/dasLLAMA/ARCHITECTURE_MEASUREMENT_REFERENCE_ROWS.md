# dasLLAMA Architecture - the llama.cpp reference rows

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries the llama.cpp rows a board's records read against on a GPU home: the Vulkan board's
(`#vulkan-reference-rows`) and the CUDA build's, with its kernel-by-kernel trace
(`#cuda-reference-rows`). The CPU rows' `--ref` recipe, the host-experts row and the rest of the
rig stay in `ARCHITECTURE_MEASUREMENT.md`.

### The Vulkan board's reference rows {#vulkan-reference-rows}

A Vulkan board's reference row (`engine` `llama.cpp`, `backend` `vulkan`) times the `vulkan` flavor
`benchmarks/setup_lcpp_ref.das --flavor vulkan` builds at `DEFAULT_REF_SHA` (the row's `sha`): the
Release configure with `-DLLAMA_CURL=OFF -DGGML_VULKAN=ON`, its glslc from a Vulkan SDK of 1.4.357
or later (`BRINGUP.md`). `gen_bench_records --ref-vulkan <build-vulkan/bin/llama-bench>` (or
`LLAMA_BENCH_VULKAN`) runs, per model, `llama-bench -m <gguf> -ngl 99 -t <threads> -p 512 -n 128 -r
5 -o json` for the flat rows and the sibling `llama-batched-bench -m <gguf> -ngl 99 -t <threads> -c
4096 -b 2048 -ub 512 -npp 512 -ntg 128 -npl 4 -fa on` for `tg128@4`, with no environment setting
beyond the command line; the row's `cmd` carries both.

### The CUDA reference rows {#cuda-reference-rows}

A llama.cpp row read against `benchmarks/lcpp_bench.das` under `DASLLAMA_CUDA=1` times a Release source build of tag
b10660 (`6c84c7d5d`) configured `-DGGML_CUDA=ON`: on the Windows reference box under Visual Studio 2022 against the CUDA
13.4 toolkit (`-T cuda=13.4`, `CMAKE_CUDA_ARCHITECTURES` 120), on the Linux pod the same tag's `build-cuda`. It runs, per
model, `llama-bench -m <gguf> -ngl 99 -fa 1 -p 512 -n 128 -r 3`, with no environment setting beyond the command line.
A kernel-by-kernel reference row traces the same binary under Nsight Systems (`nsys profile --cuda-graph-trace=node`,
2026.3.2 on the pod, 2026.4.1 on the Windows box) over `llama-bench -m <gguf> -ngl 99 -fa 1 -p 512 -n 64 -r 1`, the
same pass count as ours under the tracer, and reads per-kernel wall from the trace; the traced rates are not compared
(the per-launch cost lands on the many small kernels), the per-kernel times are.
