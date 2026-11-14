# dasLLAMA Architecture - the llama.cpp reference rows

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries the llama.cpp rows a board's records read against on a GPU home: the Vulkan board's
(`#vulkan-reference-rows`) and the CUDA build's, with its kernel-by-kernel trace and the discrete homes'
batched and speculative rows (`#cuda-reference-rows`). The CPU rows' `--ref` recipe, the host-experts row and the rest of the
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

A pod board's NVFP4 and Q4_0 rows (the 35B-A3B split cells) read against a Release source build of `9febe63` on the
pod, `build-vulkan` (`-DGGML_VULKAN=ON`) and `build-cuda` (`-DGGML_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=120`), each run as
`llama-bench -m <gguf> -ngl 999 -t 48 -p 512 -n 128 -r 5` with no `-fa` and, for the forced-split rows, the `-ot` regex
that keeps the same layers' expert stacks on the host, with no environment setting beyond the command line.

The discrete homes' token rows on the pod read against master `5676dfb`, cloned at depth 1 and built Release twice
beside each other: `cmake -B build -DGGML_CUDA=ON -DLLAMA_CURL=OFF -DCMAKE_BUILD_TYPE=Release -DLLAMA_BUILD_TESTS=OFF`
against the pod's `/usr/local/cuda`, and `cmake -B build-vk -DGGML_VULKAN=ON -DGGML_CUDA=OFF -DCMAKE_BUILD_TYPE=Release
-DLLAMA_CURL=OFF` with the distribution's `libvulkan-dev` and `glslc`, each `--target llama-server llama-bench
llama-batched-bench llama-cli`. Against `lcpp_bench`'s pp512 and tg128 a model runs `llama-bench -m <gguf> -ngl 99 -fa 1
-p 512 -n 128 -r 3 -o md`; against its `tg128@N`, three runs of `llama-batched-bench -m <gguf> -ngl 99 -t 16 -c <ctx> -b
2048 -ub 512 -npp 512 -ntg 128 -npl <N> -fa on`, `ctx` 4096 at two and four streams, 5120 at eight, 10240 at sixteen, the
row their `S_TG` median. Against `harness/dflash_probe.das`'s pooled rows the reference is `llama-server -m <gguf> --host
127.0.0.1 --port 8090 -c 8192 -ngl 99 -fa on --jinja --chat-template-kwargs '{"enable_thinking":false}'
--reasoning-budget 0`, plus `--spec-type draft-dflash -md <drafter> --spec-draft-n-max <N>` for a block drafter,
`--spec-type draft-mtp --spec-draft-n-max 3` for a NextN head and `--spec-type draft-mtp -md <assistant drafter>
--spec-draft-n-max 3` for gemma's assistant drafter. Each of the four prompts - "Count from one to forty in words.",
"Explain how a binary search works, step by step.", "Write a short story about a lighthouse keeper.", "List the planets
of the solar system with one fact each." - goes to `/v1/chat/completions` twice as `{"messages":[{"role":"user",
"content":<prompt>}],"temperature":0,"max_tokens":128,"cache_prompt":false}`, and the row is the second turns' summed
`predicted_n` over their summed `predicted_ms`. The bench runs and the 35B-A3B's server sweep run under the shell's
`GLIBC_TUNABLES=glibc.rtld.optional_static_tls=16384`, a loader setting llama.cpp does not read; no run sets anything else.

On the records rig the CUDA home is the `cuda` leg: `gen_bench_records --legs cuda` under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1`
on a dasCUDA build, alone in its run as the vulkan leg is, paired with `--ref-cuda <build-cuda/bin/llama-bench>` (or
`LLAMA_BENCH_CUDA`) under the row's `-ngl 99 -fa 1` arms and the sibling `llama-batched-bench` for `tg128@4`. A das row
stamps `backend` with the armed home's name (`lcpp_bench` reads `moe_gpu_backend_name`), so a cuda cell whose row came back
`vulkan` or `cpu` fails as a vulkan cell's would, and the oracle treats the home's rows as it treats the vulkan home's -
no rig leg re-measures them.
