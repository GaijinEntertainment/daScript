# How to get a sidecar: a fresh box to a committed CPU-class profile

You have ssh into a machine nobody has tuned on. This is the walk from there to a checked-in
`performance/defaults/<class>.tune-defaults.json` that every box of that CPU class adopts at start,
with the kernel ladder proving the kernels along the way. Every command below was run as written on
the boxes named in the last section; a step that differs per box says so.

What a sidecar and a profile are: `skills/tune.md` (the `[tune]` framework, the per-app
`<app>.tune.json`), `../dasLLVM/ARCHITECTURE.md` sec.3 (the knob inventory), `performance/REVIEW.md`
(what a committed profile must satisfy). Short form: the tuner races every kernel family's
`[tune_perm]` seats on the box and writes the winners into the app's sidecar; `export_tune_profile.das`
strips the box-specific rows and saves the kernel winners as the class profile; a box whose
`tune_cpu_class()` resolves to that class adopts the profile and races nothing unless its ISA unlocks
a seat the profile never saw.

## 1. The box

CPU classes and the AWS instance that carries each (`us-west-2`; the CLI on the zen2 box is configured):

| class (`tune_cpu_class()`) | what gates it | instance |
|---|---|---|
| `x86-avx2` | avx2, no VNNI | any zen2/zen3 box (the dev zen2) |
| `x86-vnni512` | avx512vnni + avx512bw | `c7a.4xlarge` (EPYC zen4) |
| `x86-amx` | amx-int8 + amx-tile (+ avx512vnni) | `c7i.4xlarge` (Sapphire Rapids), `c8i` (Granite Rapids) |
| `arm-neon` / `arm-i8mm` | dotprod / i8mm | M1 / M2+, `c8g.2xlarge` (Graviton4) |

Launch (Ubuntu 24.04, 16 vCPU, 60 GB gp3; the key pair `dasbox` and the group `dasbench-ssh` exist in
the account - the group admits the dev box's IP on port 22, `aws ec2 describe-security-groups
--group-ids sg-0dcc81f768b888767` shows the rule to update when that IP moves):

```
aws ec2 run-instances --image-id ami-0ba3fae0cca9442ee --instance-type c7a.4xlarge --key-name dasbox \
  --security-group-ids sg-0dcc81f768b888767 \
  --block-device-mappings '[{"DeviceName":"/dev/sda1","Ebs":{"VolumeSize":60,"VolumeType":"gp3"}}]' \
  --user-data file://aws_bootstrap.sh --tag-specifications 'ResourceType=instance,Tags=[{Key=Name,Value=dasbench-zen4}]'
aws ec2 describe-instances --instance-ids <id> --query 'Reservations[0].Instances[0].[State.Name,PublicIpAddress]' --output text
ssh -i ~/.ssh/dasbox_ed25519 ubuntu@<ip>
```

The AMI id is the current Canonical Ubuntu 24.04 amd64 image (`aws ec2 describe-images --owners
099720109477 --filters "Name=name,Values=ubuntu/images/hvm-ssd-gp3/ubuntu-noble-24.04-amd64-server-*"`,
newest by `CreationDate`). On-demand `c7a.4xlarge` is about $0.82 an hour; terminate when done (last section).

## 2. Dependencies and the build

The user-data script (`aws_bootstrap.sh`, reproduced in the last section) does 2-4 unattended; the
same lines by hand:

```
sudo apt-get update -y
sudo apt-get install -y --no-install-recommends build-essential clang cmake ninja-build git python3 pkg-config \
    libssl-dev curl ca-certificates libatomic-ops-dev libglu1-mesa-dev freeglut3-dev mesa-common-dev \
    libglfw3-dev libfreetype6-dev libudev-dev libopenal-dev libvorbis-dev libflac-dev libx11-dev \
    libxrandr-dev libxcursor-dev libxinerama-dev libxi-dev
git clone --depth 1 --recursive -b <branch> https://github.com/GaijinEntertainment/daScript.git daScript
cd daScript
CC=clang CXX=clang++ cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_GLFW_DISABLED=ON \
    -DDAS_HV_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF
cmake --build build --target daslang -j 16
ls -la bin/daslang lib/LLVM.dll
```

`-DDAS_LLVM_DISABLED=OFF` is the JIT (default off). The dasLLVM module is das-only; the configure step
downloads the prebuilt `lib/LLVM.dll` for the platform (that name on every OS) - if `ls` does not show it,
nothing below runs. Only `daslang` needs building: dasLLAMA is das, its native needs are builtins.

### A macOS box you do not own (the M4 Pro walk)

Homebrew there may belong to another user (`/opt/homebrew` not writable) - do not chown it. CMake and
Ninja come as user-local binaries instead: the Kitware `cmake-<v>-macos-universal.tar.gz` and the
ninja-build `ninja-mac.zip` unpacked under `~/tools`, prepended to `PATH`. Apple's `/usr/bin/bison` is
2.3 and cannot read `ds2_parser.ypp`, so configure with `-DDAS_FLEX_BISON_DISABLED=ON` (the committed
generated parser is current); no OpenSSL means `-DDAS_HV_DISABLED=ON`. The reference builds with
`-DGGML_METAL=OFF -DGGML_BLAS=OFF` (CPU only). On a P+E chip give the ladder the performance cores
(`TEAM=10` on the M4 Pro), not `nproc`.

## 3. The reference exe

The kernel ladder's other side. Pin the same tip the dev box's tables use (`6c84c7d5d` for the
2026-09 tables); `test-backend-ops` needs the thread define (a three-line edit of `N_THREADS` in
`tests/test-backend-ops.cpp` - `harness/backend_ops_shapes.patch` is the older, wider form and no
longer applies at this tip):

```
git clone https://github.com/ggml-org/llama.cpp.git && cd llama.cpp && git checkout 6c84c7d5d
python3 - <<'EOP'
import io
p="tests/test-backend-ops.cpp"; s=io.open(p,encoding="utf-8").read()
old="#   define N_THREADS std::thread::hardware_concurrency()"
assert s.count(old)==1
s=s.replace(old,'#   define N_THREADS (getenv("GGML_BENCH_THREADS") && atoi(getenv("GGML_BENCH_THREADS")) > 0 ? (size_t) atoi(getenv("GGML_BENCH_THREADS")) : std::thread::hardware_concurrency())')
io.open(p,"w",encoding="utf-8").write(s)
EOP
cmake -B build-clean-cpu -DCMAKE_BUILD_TYPE=Release -DGGML_NATIVE=ON -DLLAMA_CURL=OFF -DLLAMA_BUILD_TESTS=ON
cmake --build build-clean-cpu --target test-backend-ops llama-bench -j 16
```

`build-clean-cpu` is the flavor the tables name: no GPU backend, no BLAS - a GPU build's
`llama-bench -ngl 0` is not CPU-only.

The perf list has no decode row above 4096 x 14336, and on a box with a large L3 that op stays cached
between the repeated runs (the 3990X reads it at 295 GB/s with 32 threads, three times its DRAM) - a
many-lane ratio taken there flatters the reference. For the DRAM-bound row (`BIG=1` in the ladder,
d=32768) add, inside the same `for (int bs ...)` loop in `make_test_cases_perf`, after the 4096 line:
`if (bs == 1) test_cases.emplace_back(new test_mul_mat(type_a, type_b, 32768, bs, 14336, {1, 1}, {1, 1}));`

Confirm the define is in the binary the ladder gets: `grep -c GGML_BENCH_THREADS
build-clean-cpu/bin/test-backend-ops` prints 1. A box with several llama.cpp builds is where this
bites - a binary that prints 0 runs every core and the ratio column lies; `kernel_ladder.sh` refuses it.

## 4. Correctness before any number

```
cd ~/daScript
DAS_TUNE_MODE=test DAS_JOBQUE_THREADS=8 bin/daslang -jit modules/dasLLAMA/harness/gen_tune_probe.das
```

Every `[tune_perm]` seat of every family is stamped and checked bit-exact against its reference body;
the box's ISA is what makes seats that never ran before run here (the 512-bit VNNI seats on zen4, the
AMX leg on Intel). `ok` on every line, or the box has found a defect and the walk stops until it is fixed.

## 5. The kernel ladder

```
LCPP_TBO=~/llama.cpp/build-clean-cpu/bin/test-backend-ops NTOK=512 ROUNDS=5 \
    bash modules/dasLLAMA/harness/kernel_ladder.sh all > ladder.tsv 2> ladder.err
```

Then the engine's shape - every lane, a weight above the box's L3 (needs the m=32768 perf row, section 3):

```
LCPP_TBO=~/llama.cpp/build-clean-cpu/bin/test-backend-ops NTOK=0 ROUNDS=5 TEAM=16 BIG=1 bash modules/dasLLAMA/harness/kernel_ladder.sh all > ladder_big16.tsv 2> ladder_big16.err
```

One thread, no model: every format's stamped kernel against the reference exe's `test-backend-ops perf`
at the same shape, decode and prefill rows, ratio = reference / ours. This is the box's table; it goes
into `plans/kernel_parity_pass.md`'s fact base (and the records store once the kernel board exists).

## 6. The mint

The vehicle is a public 1B GGUF pulled straight to the box - nothing copies from the dev box:

```
mkdir -p ~/models && cd ~/models
curl -L -o Llama-3.2-1B-Instruct-Q4_K_M.gguf https://huggingface.co/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q4_K_M.gguf
cd ~/daScript
DAS_JOBQUE_THREADS=16 bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m ~/models/Llama-3.2-1B-Instruct-Q4_K_M.gguf --tune > ~/mint.log 2>&1
```

808 MB, about a minute to fetch. `--tune` is the full race: every generator family
(`@tune begin name=<fmt>q8_tile_gen ... @tune end ... winner=<seat> verdict=beats`), then the
`[tuned]` loop-hint kernels (`axpy`, `dot`, `rope_*`, `quantize_*`...), then
`confirm_e2e_prefill`; on the c7a.4xlarge the whole walk took 5-6 minutes. It writes
`modules/dasLLAMA/benchmarks/lcpp_bench.tune.json` beside the app and then re-launches the app to
apply it. The bench rows the re-launch would print are refused without `--for-debug-purposes` (a `-jit`
script run is not record-grade) - that refusal is expected here; the mint is done.

Read the sidecar's provenance before anything else:

```
python3 -c 'import json;d=json.load(open("modules/dasLLAMA/benchmarks/lcpp_bench.tune.json"));p=d["provenance"];print(p["noise"],p["validation"],p["features"],len(d["kernels"]))'
```

`ok ok <features> 49` is the pass (a few more when the harness gave a kq gemv its own seat - those entries are
named `<fmt>q8_gemv_gen` and ride into the profile like any other): `noise` is the tuner's own drift verdict (a busy or thermally
unstable box says otherwise - re-mint, never edit), `validation` is every winner checked against its
fallback, `features` is the box's fingerprint of `TUNE_KNOWN_FEATURES` (zen4:
`avx2;f16c;fma;sse4.2;avx512f;avx512bw;avx512vl;avx512vnni`), 49 the kernel count the scope demands.

The export:

```
bin/daslang -jit modules/dasLLAMA/harness/export_tune_profile.das -- --sidecar modules/dasLLAMA/benchmarks/lcpp_bench.tune.json
git status --short modules/dasLLAMA/performance/defaults/
```

It writes `performance/defaults/<class>.tune-defaults.json` for this box's `tune_cpu_class()`
(`--class <name>` overrides, `--out <dir>` relocates) with the box, engine sha and timing rows
stripped - a profile is kernel winners plus the provenance the adopt path checks.

Compare before committing. A class that already ships a profile will differ in a few winners every
time - two mints of one class on two boxes disagree on the tie-class seats (the zen4 re-mint of
2026-09-01 flipped 4 of 49: `axpy` vec8_u2 -> vec8, `axpy_f16` -> plain, `rope_scaled_neox_tab` ->
plain, `q51q8_tile_gen` 512 -> 256-bit; the format tiles agreed). Commit a re-mint only when a seat
that carries a kernel family changed or new families exist; a tie flip is not a reason.

```
python3 - <<'EOP'
import json
n=json.load(open("modules/dasLLAMA/performance/defaults/x86-vnni512.tune-defaults.json"))
import subprocess; o=json.loads(subprocess.check_output(["git","show","HEAD:modules/dasLLAMA/performance/defaults/x86-vnni512.tune-defaults.json"]))
for k in sorted(set(n["kernels"])|set(o["kernels"])):
    if n["kernels"].get(k)!=o["kernels"].get(k): print(k, o["kernels"].get(k), "->", n["kernels"].get(k))
EOP
```

A class with no shipped profile yet (Intel's `x86-amx`, born 2026-09-01) needs the class first, on the
branch the box clones: `tune_cpu_class()` and the ladder in `tune_class_chain()`
(`modules/dasLLVM/daslib/llvm_tune.das`), the new class above the one it supersedes, and every feature
a `requires=` names in `TUNE_KNOWN_FEATURES` there (`amx-int8`, `amx-tile` were already listed). Until
its profile ships, a box of the new class adopts the class below it (the chain) and races only the
seats that class could not answer. Then the same mint and export produce `<class>.tune-defaults.json`,
and that one IS committed - the Intel one differed from `x86-vnni512` in 15 of 49 winners, all
`[tuned]` loop-hint kernels preferring `vec16`, while every generator tile kept the 512-bit VNNI seat
(the AMX tiles raced and lost the q8q8 family).

Three things the boxes taught that the walk now carries:

- **Alignment.** Intel splits a 64-byte vector load that crosses a cache line and pays for it; AMD
  barely does. The engine's image planes are page-aligned, but a bench that hands kernels 16-byte-aligned
  arrays reads 2x slow on Intel and true on zen4 - `kq_kernel_bench` aligns its planes to 64 bytes.
- **Denormals.** Random bytes in a scale plane are denormals or infinities often enough to bend a
  table on Intel (~100 cycles a denormal op) and not on AMD: the q8/mx4 tile rows read 6x slow until the
  bench filled scale planes with a byte that is a normal number in every scale form.
- **AMX permission.** A tile instruction before the per-process `arch_prctl` grant is SIGILL. The
  family's witness performs the grant; in tune mode call the witness *variants* (the AMX row does it),
  as `gen_tune_probe` does - the unstamped base does not.

## 7. What the commit must satisfy

`modules/dasLLAMA/performance/REVIEW.md` and its `REVIEW.das` gate: a `defaults/` profile carries no
`engine_sha`, its `provenance.dasllama_version` equals `DASLLAMA_RELEASE`, its `noise` reads `ok`,
and it was minted on a quiet, session-free box - never hand-edited. Run the gate before pushing:
`bin/daslang utils/internal/preflight/main.das -- --only review-md`.

## 8. Home, and the bill

```
scp -i ~/.ssh/dasbox_ed25519 ubuntu@<ip>:daScript/modules/dasLLAMA/performance/defaults/<class>.tune-defaults.json .
scp -i ~/.ssh/dasbox_ed25519 ubuntu@<ip>:ladder.tsv ladder_<box>.tsv
aws ec2 terminate-instances --instance-ids <id>
```

## Boxes this walk ran on

- 2026-09-01 `c7a.4xlarge` (EPYC 9R14 zen4, class `x86-vnni512`), `i-043725feb25086523`: sections 1-6
  as written; the TEST gate 65/65 ok; the re-mint agreed with the shipped profile on every format tile
  and flipped four tie seats, so nothing was committed from it. Model-level sanity on the vehicle
  (`lcpp_bench -m <gguf> -p 512 -n 128 -r 3 --for-debug-purposes --ref ~/llama.cpp/build-clean-cpu/bin/llama-bench
  --ref-flavor clean-cpu --ref-no-affinity`, 16 threads): pp512 1172 vs 927, tg128 88.7 vs 86.0.
  Wall clock from launch to terminate: about 90 minutes, of which the build is 12 and the mint 6.
- 2026-09-01 `c8i.4xlarge` (Xeon 6975P-C Granite Rapids, class `x86-amx`), `i-0fb77cb72129e36a2`:
  sections 1-7; the TEST gate 65/65 ok with the AMX leg; the class was added on the branch before
  launch; the minted `x86-amx.tune-defaults.json` is the shipped one.
