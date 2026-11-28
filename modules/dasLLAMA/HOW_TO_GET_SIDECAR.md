# How to get a sidecar: a fresh box to a committed CPU-class profile

You have ssh into a machine nobody has tuned on. This is the walk from there to a checked-in
`performance/defaults/<class>.tune-defaults.json` that every box of that CPU class stamps at its first compile,
with the kernel ladder proving the kernels along the way. A step that differs per box says so.

What a sidecar and a profile are: `skills/tune.md` (the `[tune]` framework, the per-app
`<app>.tune.json`), `../dasLLVM/ARCHITECTURE.md#override-knobs` (the knob inventory),
`performance/REVIEW.md` (what a committed profile must satisfy). Short form: the tuner races every
kernel family's `[tune_perm]` seats on the box and writes the winners into the app's sidecar;
`export_tune_profile.das` strips the box-specific rows and saves the kernel winners as the class
profile; a box whose `tune_native_target().klass` resolves to that class is covered by the profile at its
first compile - no race, no sidecar, no restart - unless its ISA unlocks a seat the profile never
saw, which it adopts and races.

## 1. The box

A box of a class stamps that class's profile at its first compile
(`ARCHITECTURE_ENGINE_FORMATS.md#cpu-kernel-tiers`, the `dasllama_tune_scope.das` charter). CPU
classes and the AWS instance that carries each (`us-west-2`):

| class (`tune_native_target().klass`) | what gates it | instance |
|---|---|---|
| `x86-avx` | avx + f16c, no avx2 | none: Jaguar, Ivy Bridge, Piledriver - minted under a pin, section 10 |
| `x86-avx2` | avx2, no VNNI | `c5a.4xlarge` (EPYC zen2), or any zen2/zen3 box |
| `x86-vnni512` | avx512vnni + avx512bw | `c7a.4xlarge` (EPYC zen4) |
| `x86-amx` | amx-int8 + amx-bf16 + amx-tile (+ avx512vnni, avx512bf16) | `c7i.4xlarge` (Sapphire Rapids), `c8i` (Granite Rapids) |
| `arm-neon` / `arm-i8mm` | dotprod / i8mm, no SVE | M1 / M2+ (Apple) |
| `arm-sve` | i8mm + sve | `c8g.4xlarge` (Graviton4) |

Launch (Ubuntu 24.04, 16 vCPU, 60 GB gp3; the key pair `dasbox` and the group `dasbench-ssh` exist in
the account - the group admits the dev box's IP on port 22, `aws ec2 describe-security-groups
--group-ids sg-0dcc81f768b888767` shows the rule to update when that IP moves):

```
aws ec2 run-instances --image-id <ami> --instance-type c7a.4xlarge --key-name dasbox \
  --security-group-ids sg-0dcc81f768b888767 \
  --block-device-mappings '[{"DeviceName":"/dev/sda1","Ebs":{"VolumeSize":60,"VolumeType":"gp3"}}]' \
  --user-data file://aws_bootstrap.sh --tag-specifications 'ResourceType=instance,Tags=[{Key=Name,Value=dasbench-zen4}]'
aws ec2 describe-instances --instance-ids <id> --query 'Reservations[0].Instances[0].[State.Name,PublicIpAddress]' --output text
ssh -i ~/.ssh/dasbox_ed25519 ubuntu@<ip>
```

`<ami>` is the newest Canonical Ubuntu 24.04 amd64 image (`aws ec2 describe-images --owners
099720109477 --filters "Name=name,Values=ubuntu/images/hvm-ssd-gp3/ubuntu-noble-24.04-amd64-server-*"`,
newest by `CreationDate`). Terminate the box when done (the close of section 8).

## 2. Dependencies and the build

The user-data script (`aws_bootstrap.sh`, section 8) does 2, 4 and 6 unattended; the
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

`-DDAS_LLVM_DISABLED=OFF` is the JIT (default off), and the tune framework exists only in a build
with it (`ARCHITECTURE_ENGINE_FORMATS.md#cpu-kernel-tiers`, the `dasllama_tune.das` charter). The
dasLLVM module is das-only; the configure step downloads the prebuilt `lib/LLVM.dll` for the
platform (that name on every OS) - if `ls` does not show it, nothing below runs. Only `daslang`
needs building: dasLLAMA is das, its native needs are builtins. On a box whose image already
carries a daScript tree (and the reference build's), the user-data clones skip: check both out by
hand and verify `git log -1` before the first gate.

### A macOS box you do not own

Homebrew there may belong to another user (`/opt/homebrew` not writable) - do not chown it. CMake and
Ninja come as user-local binaries instead: the Kitware `cmake-<v>-macos-universal.tar.gz` and the
ninja-build `ninja-mac.zip` unpacked under `~/tools`, prepended to `PATH`. Apple's `/usr/bin/bison` is
2.3 and cannot read `ds2_parser.ypp`, so configure with `-DDAS_FLEX_BISON_DISABLED=ON` (the committed
generated parser is current); no OpenSSL means `-DDAS_HV_DISABLED=ON`. The reference builds with
`-DGGML_METAL=OFF -DGGML_BLAS=OFF` (CPU only). On a P+E chip give the ladder the performance cores
(`TEAM=<performance cores>`), not `nproc`.

## 3. The reference exe

The kernel ladder's other side, timed at the same shape as ours
(`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity`). Pin the tip the dev box's tables
use (`6c84c7d5d`); `test-backend-ops` needs the thread define (a three-line edit of `N_THREADS` in
`tests/test-backend-ops.cpp` - `harness/backend_ops_shapes.patch` is a wider form that does not
apply at this tip):

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

The perf list has no decode row above 4096 x 14336, and on a box whose L3 holds that op it stays
cached between the repeated runs - a many-lane ratio taken there flatters the reference. For the
DRAM-bound row (`BIG=1` in the ladder, d=32768) add, inside the same `for (int bs ...)` loop in
`make_test_cases_perf`, after the 4096 line:
`if (bs == 1) test_cases.emplace_back(new test_mul_mat(type_a, type_b, 32768, bs, 14336, {1, 1}, {1, 1}));`

Confirm the define is in the binary the ladder gets: `grep -c GGML_BENCH_THREADS
build-clean-cpu/bin/test-backend-ops` prints 1. A box with several llama.cpp builds is where this
bites - a binary that prints 0 runs every core and the ratio column lies; `kernel_ladder.sh` refuses it.

## 4. Correctness before any number

```
cd ~/daScript
DAS_TUNE_MODE=test DAS_JOBQUE_THREADS=8 bin/daslang -jit modules/dasLLAMA/harness/gen_tune_probe.das
```

Every `[tune_perm]` seat of every family is stamped and held to its reference body within the family's
bar (`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#rm-family-race` names the row-major families') - the q8
tile, the superblock and per-32 tiles, the bf16-weight tile and the twenty-six row-major GEMVs
(`<fmt>q8_rm_gemv_gen`, the streamed expert rail); the box's ISA is what makes seats that never ran
before run here (the 512-bit VNNI seats on zen4, the AMX leg on Intel). `ok` on every line, or the box
has found a defect and the walk stops until it is fixed.
The gate is the tune framework's TEST mode over the perm grids
(`ARCHITECTURE_ENGINE_FORMATS.md#cpu-kernel-tiers`); it does not reach the cells that call a
stamped tile directly, so run the module's dastest files on the box too.

## 5. The kernel ladder

```
LCPP_TBO=~/llama.cpp/build-clean-cpu/bin/test-backend-ops NTOK=512 ROUNDS=5 \
    bash modules/dasLLAMA/harness/kernel_ladder.sh all > ladder.tsv 2> ladder.err
```

Then the engine's shape - every lane, a weight above the box's L3 (needs the m=32768 perf row, section 3):

```
LCPP_TBO=~/llama.cpp/build-clean-cpu/bin/test-backend-ops NTOK=0 ROUNDS=5 TEAM=16 BIG=1 bash modules/dasLLAMA/harness/kernel_ladder.sh all > ladder_big16.tsv 2> ladder_big16.err
```

One thread, no model: every format's stamped kernel against the reference exe's `test-backend-ops
perf` at the same shape, decode and prefill rows, ratio = reference / ours. This is the box's
table. The bench's fixture conditions - one staggered arena, normal-number scale bytes, warmed
rows - are `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#cpu-kernel-bench-fixture`.

## 6. The mint

The vehicle is a public 1B GGUF pulled straight to the box - nothing copies from the dev box:

```
mkdir -p ~/models && cd ~/models
curl -L -o Llama-3.2-1B-Instruct-Q4_K_M.gguf https://huggingface.co/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q4_K_M.gguf
curl -L -o Llama-3.2-1B-Instruct-Q8_0.gguf https://huggingface.co/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q8_0.gguf
cd ~/daScript
unset DAS_TUNE_MANIFEST
DASLLAMA_MODELS_DIR=~/models DASLLAMA_CONFIRM_MODEL=$HOME/models/Llama-3.2-1B-Instruct-Q8_0.gguf bin/daslang utils/daspkg/main.das -- release --root modules/dasLLAMA/benchmarks --out modules/dasLLAMA/performance/_rig > ~/mint.log 2>&1
```

The tuner's own transcript - every family's rows, a rejected family's miss - is
`modules/dasLLAMA/benchmarks/modules/.daspkg.log`, not the release's stdout. A box that minted
before keeps a previous `lcpp_bench.tune.json` beside the app, and a seat whose perm emits on
this box stays crowned inside the win margin (the sticky seat; the transcript names a seat that
moved because its perm runs the reference body here); delete it first for a mint that answers on
its own.
`DASLLAMA_CONFIRM_MODEL` names the q8 vehicle the confirm runs the q8 leg on; without one the
confirm pins the q8 fallback for want of a q8 model. A kernel family that wins the race and then
loses `confirm_e2e_prefill` is a defect to fix before the profile is committed, not evidence that
the kernel is slower. The mint is the release: `daspkg release` builds the standalone bench exe,
races every generator family (`@tune begin name=<fmt>q8_tile_gen ... @tune end ...
winner=<seat> verdict=beats`), then the `[tuned]` loop-hint kernels (`axpy`, `dot`, `rope_*`,
`quantize_*`...), then `confirm_e2e_prefill`, and rebuilds the exe with the winners baked in. It
writes
`modules/dasLLAMA/benchmarks/lcpp_bench.tune.json` beside the app and ships a copy beside the
exe as `dasllama-bench.tune.json`, which the exe reads on its own - a pinned `DAS_TUNE_MANIFEST`
in the shell overrides it, so the variable stays unset. A `-jit` script run of `lcpp_bench.das`
cannot mint: the record-grade gate refuses the script before `--tune` runs, and
`--for-debug-purposes` keys the winners to the script binary, not the exe.

Read the sidecar's provenance before anything else
(`ARCHITECTURE_MEASUREMENT.md#mint-wall-provenance`):

```
python3 -c 'import json;d=json.load(open("modules/dasLLAMA/benchmarks/lcpp_bench.tune.json"));p=d["provenance"];print(p["noise"],p["validation"],p["features"],len(d["kernels"]))'
```

`ok ok <features> <n>` is the pass: `noise` is the tuner's own drift verdict (a busy or thermally
unstable box says otherwise - re-mint, never edit), `validation` is every winner checked against
its fallback, `features` is the box's fingerprint of `CpuFeatures` (zen4:
`avx2;f16c;fma;sse4.2;avx512f;avx512bw;avx512vl;avx512vnni`), `<n>` the number of kernels the
tune scope declares, a kq gemv's own `<fmt>q8_gemv_gen` seat included.

The export:

```
bin/daslang -jit modules/dasLLAMA/harness/export_tune_profile.das -- --sidecar modules/dasLLAMA/benchmarks/lcpp_bench.tune.json
git status --short modules/dasLLAMA/performance/defaults/
```

It writes `performance/defaults/<class>.tune-defaults.json` for the class the process is pinned to, else this box's (`tune_mint_target`)
(`--class <name>` overrides, `--out <dir>` relocates) with the box, engine sha and timing rows
stripped - a profile is kernel winners plus the provenance the adopt path checks.

Compare before committing. A class that already ships a profile differs in a few winners every
time - two mints of one class on two boxes disagree on the tie-class seats (loop-hint kernels such
as `axpy`, `axpy_f16` and `rope_scaled_neox_tab`, a tile's vector width). Commit a re-mint only when
a seat that carries a kernel family changed or new families exist; a tie flip is not a reason.

```
python3 - <<'EOP'
import json
n=json.load(open("modules/dasLLAMA/performance/defaults/x86-vnni512.tune-defaults.json"))
import subprocess; o=json.loads(subprocess.check_output(["git","show","HEAD:modules/dasLLAMA/performance/defaults/x86-vnni512.tune-defaults.json"]))
for k in sorted(set(n["kernels"])|set(o["kernels"])):
    if n["kernels"].get(k)!=o["kernels"].get(k): print(k, o["kernels"].get(k), "->", n["kernels"].get(k))
EOP
```

Then run a model-level sanity row on the vehicle against the reference build:
`lcpp_bench -m <gguf> -p 512 -n 128 -r 3 --for-debug-purposes --ref
~/llama.cpp/build-clean-cpu/bin/llama-bench --ref-flavor clean-cpu --ref-no-affinity`.

A class with no shipped profile needs the class first, on the branch the box clones:
its row in `JIT_CPU_CLASSES` and its place in `jit_cpu_class_ladder`
(`modules/dasLLVM/daslib/llvm_cpu_class.das`), the new class above the one it supersedes, its arm
in `tune_native_target().klass` (`modules/dasLLVM/daslib/llvm_tune.das`) and every feature a `requires=` names
in `CpuFeatures`. Until its profile ships, a box of the new class adopts the class below
it (the chain) and races only the seats that class could not answer. Then the same mint and export
produce `<class>.tune-defaults.json`, and that one IS committed. Box differences the walk carries:

- **Alignment.** Intel splits a 64-byte vector load that crosses a cache line and pays for it; AMD
  barely does. The engine's image planes are page-aligned, but a bench that hands kernels 16-byte-aligned
  arrays reads 2x slow on Intel and true on zen4 - `kq_kernel_bench` aligns its planes to 64 bytes.
- **Denormals.** The bench fills scale planes with a byte that is a normal number in every scale
  form: random scale bytes are denormals or infinities often enough to bend a table on Intel.
- **AMX permission.** A tile instruction before the per-process `arch_prctl` grant is SIGILL. The
  family's witness performs the grant; in tune mode call the witness *variants* (the AMX row does it),
  as `gen_tune_probe` does - the unstamped base does not.

## 7. What the commit must satisfy

`modules/dasLLAMA/performance/REVIEW.md` and its `REVIEW.das` gate: a `defaults/` profile carries no
`engine_sha`, its `provenance.dasllama_version` equals `DASLLAMA_RELEASE`, its `noise` reads `ok`,
it carries an entry for every `[tune(gen = ...)]` family of the scope its class has a seat in (a fat
release ships a clone per such family, `ARCHITECTURE_MEASUREMENT.md#committed-records-gate`), and it is minted on a
quiet, session-free box - never hand-edited. The shipped profile is the scope's defaults every
kernel reads at its own compile (`ARCHITECTURE_ENGINE_FORMATS.md#cpu-kernel-tiers`). Run the gate
before pushing: `bin/daslang utils/internal/preflight/main.das -- --only review-md`. The push
itself runs the dasllama-server release workflow on a profile change (its path filter names
`performance/defaults/`), so the fat build proves the profile before the PR merges.

## 8. The unattended form

`aws_bootstrap.sh`, the user-data of section 1: sections 2, 4 and 6 and the export in one pass - the
box clones master, builds `daslang`, fetches the 1B Q4_K_M vehicle and the 1B Q8_0 the confirm pins the
q8 leg on, runs the TEST gate, mints and exports, and writes each step's verdict to `~/mint/mint.out`
(`MINT-DONE` last). Read that file and its provenance line
(`ARCHITECTURE_MEASUREMENT.md#mint-wall-provenance`), then section 6's compare, then bring the
profile home and terminate the box (the commands close this section).

```bash
#!/bin/bash
# sidecar refresh: dependencies, the tree on master, the daslang build, the vehicle models, the TEST gate, the mint and the export - unattended
set -u
export DEBIAN_FRONTEND=noninteractive
apt-get update -y
apt-get install -y --no-install-recommends build-essential clang cmake ninja-build git python3 pkg-config \
    libssl-dev curl ca-certificates libatomic-ops-dev libglu1-mesa-dev freeglut3-dev mesa-common-dev \
    libglfw3-dev libfreetype6-dev libudev-dev libopenal-dev libvorbis-dev libflac-dev libx11-dev \
    libxrandr-dev libxcursor-dev libxinerama-dev libxi-dev
sudo -u ubuntu -i bash <<'EOU'
set -u
mkdir -p ~/mint ~/models
: > ~/mint/mint.out
echo "boot $(date -u +%FT%TZ) $(nproc) cores $(lscpu | grep -m1 'Model name' | cut -c1-80)" >> ~/mint/mint.out
git clone --depth 1 --recursive -b master https://github.com/GaijinEntertainment/daScript.git daScript > ~/mint/clone.log 2>&1
cd ~/daScript
echo "tip $(git log --oneline -1 | cut -c1-60)" >> ~/mint/mint.out
CC=clang CXX=clang++ cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_GLFW_DISABLED=ON \
    -DDAS_HV_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF > ~/mint/cmake.log 2>&1
cmake --build build --target daslang -j $(nproc) > ~/mint/build.log 2>&1
echo "build exit=$? $(ls -la bin/daslang 2>&1 | cut -c1-60) llvm=$(ls lib/LLVM.dll 2>/dev/null | wc -l)" >> ~/mint/mint.out
cd ~/models
curl -sL -o Llama-3.2-1B-Instruct-Q4_K_M.gguf https://huggingface.co/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q4_K_M.gguf
curl -sL -o Llama-3.2-1B-Instruct-Q8_0.gguf https://huggingface.co/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q8_0.gguf
echo "models $(ls -la ~/models | grep gguf | awk '{print $5, $9}' | tr '\n' ' ')" >> ~/mint/mint.out
cd ~/daScript
DAS_TUNE_MODE=test DAS_JOBQUE_THREADS=8 bin/daslang -jit modules/dasLLAMA/harness/gen_tune_probe.das > ~/mint/probe.log 2>&1
echo "probe exit=$? ok=$(grep -c ' ok' ~/mint/probe.log) notok=$(grep -ci 'fail\|mismatch\|not ok' ~/mint/probe.log)" >> ~/mint/mint.out
unset DAS_TUNE_MANIFEST
DASLLAMA_MODELS_DIR=$HOME/models DASLLAMA_CONFIRM_MODEL=$HOME/models/Llama-3.2-1B-Instruct-Q8_0.gguf bin/daslang utils/daspkg/main.das -- release --root modules/dasLLAMA/benchmarks --out modules/dasLLAMA/performance/_rig > ~/mint/mint.log 2>&1
echo "mint exit=$?" >> ~/mint/mint.out
python3 -c 'import json;d=json.load(open("modules/dasLLAMA/benchmarks/lcpp_bench.tune.json"));p=d["provenance"];print("provenance", p["noise"],p["validation"],p["features"],len(d["kernels"]))' >> ~/mint/mint.out 2>&1
bin/daslang -jit modules/dasLLAMA/harness/export_tune_profile.das -- --sidecar modules/dasLLAMA/benchmarks/lcpp_bench.tune.json >> ~/mint/mint.out 2>&1
git status --short modules/dasLLAMA/performance/defaults/ >> ~/mint/mint.out
echo MINT-DONE >> ~/mint/mint.out
EOU
```

Home, and the bill:

```
scp -i ~/.ssh/dasbox_ed25519 ubuntu@<ip>:daScript/modules/dasLLAMA/performance/defaults/<class>.tune-defaults.json .
scp -i ~/.ssh/dasbox_ed25519 ubuntu@<ip>:ladder.tsv ladder_<box>.tsv
aws ec2 terminate-instances --instance-ids <id>
```

## 9. The wasm profile

A wasm64 artifact is its own class, `wasm-simd128`, whatever box built it
(`../dasLLVM/ARCHITECTURE_CPU_CLASSES.md#cross-class`), so its profile is minted inside the
artifact: both tuners and the export are built as wasm releases and run under node. Sections 1
to 6 do not apply - no reference exe, no kernel ladder, no vehicle model. The files are the three
packages under `harness/wasm_mint/` - `tuner/` (the generator families, `gen_tune_probe.das`),
`kernels/` (the loop-hint kernels, `tune_kernels.das`) and `export/` - and
`harness/wasm_mint/run.cjs`.

The box needs an activated emsdk (the version `.github/workflows/pages.yml` pins), its node on
`PATH`, the wasm64 archives (`daspkg build --wasm`, `skills/daspkg.md`), and a daslang built as
the wasm cross-compile host in a worktree of its own: `sh web/build_wasm_host.sh`, whose header
says why the box's native build cannot stand in for it.

```
H=modules/dasLLAMA/harness/wasm_mint
mkdir -p ~/mint_wasm/io
DAS_TUNE_MODE=tune bin/daslang utils/daspkg/main.das -- release wasm --root $H/tuner --out ~/mint_wasm/tuner
DAS_TUNE_MODE=tune bin/daslang utils/daspkg/main.das -- release wasm --root $H/kernels --out ~/mint_wasm/kernels
bin/daslang utils/daspkg/main.das -- release wasm --root $H/export --out ~/mint_wasm/export
DAS_TUNE_MODE=test node $H/run.cjs ~/mint_wasm/tuner/wasm_tuner/wasm_tuner.js ~/mint_wasm/io
DAS_TUNE_MODE=tune DAS_JOBQUE_THREADS=8 node $H/run.cjs ~/mint_wasm/tuner/wasm_tuner/wasm_tuner.js ~/mint_wasm/io
DAS_TUNE_MODE=tune DAS_JOBQUE_THREADS=8 node $H/run.cjs ~/mint_wasm/kernels/wasm_kernels/wasm_kernels.js ~/mint_wasm/io
node $H/run.cjs ~/mint_wasm/export/wasm_export/wasm_export.js ~/mint_wasm/io --out-dir /out
cp ~/mint_wasm/io/wasm-simd128.tune-defaults.json modules/dasLLAMA/performance/defaults/
```

Trap: the export runs in wasm too - a native `export_tune_profile.das` reads this sidecar as another box's and refuses it with "no kernels".

`DAS_TUNE_MODE=tune` at a tuner's build stamps its grid; a cross build's generator grid is the
rows the class can run (`../dasLLVM/ARCHITECTURE_CPU_CLASSES.md#cross-grid`), and the
generator tuner races the families that have one
(`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#seated-families`). `run.cjs` mounts the host directory
at `/out` and puts the sidecar at `/out/wasm.tune.json`: the two tuners write into the one file,
the generator tuner first, and the export reads it there. The TEST line ends `GEN TUNE TEST OK`
and the generator mint `GEN TUNE RUN OK`; the loop-hint tuner holds every hint variant of every
kernel to its oracle before it times one and ends on its summary. Each mint sits behind four
quiet noise probes.

The class has one profile and any device runs the artifact, so mint on the box the release is
measured on and re-mint when a family gains a seat. A release built with no variable set stamps
from the committed profile: time the model the family serves against a build made before the
profile landed (`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#x16-served-race`).

Section 7's gate binds this profile as it binds every other.

## 10. A class minted under a pin

A class no box at hand belongs to is minted on a box above it, under `DAS_JIT_BASELINE=<class>`:
the build targets the class, the tune grid holds the rows the class can run
(`../dasLLVM/ARCHITECTURE_CPU_CLASSES.md#cross-grid`), and the tuner races those. Sections 3 and
5 do not apply, and of section 6 only the provenance line and the export - no reference exe, no
ladder, no vehicle model. Run it in a tree that
already carries the class row (`JIT_CPU_CLASSES`, `../dasLLVM/daslib/llvm_cpu_class.das`):

```
export DAS_JIT_BASELINE=x86-avx DAS_JOBQUE_THREADS=8
M=~/mint_class/class.tune.json
mkdir -p ~/mint_class && rm -f $M
DAS_TUNE_MODE=test bin/daslang -jit modules/dasLLAMA/harness/gen_tune_probe.das
DAS_TUNE_MODE=tune DAS_TUNE_MANIFEST=$M bin/daslang -jit modules/dasLLAMA/harness/dasllama_tuner.das
bin/daslang -jit modules/dasLLAMA/harness/export_tune_profile.das -- --sidecar $M
```

The TEST line ends `GEN TUNE TEST OK`; the export writes
`performance/defaults/<class>.tune-defaults.json`, and section 6's provenance line reads
`ok ok <the class's features> <n>`.

Trap: a suite run under the pin before the class's profile is committed mints at its first
child, since a class with no profile is an untuned box - commit the profile first.

The pin makes the code the class's, never the timings: the winners are the minting box's, so the
profile is a stand-in until a box of the class runs these same commands.

A target with no compiler, no tuner process and its own file IO - a console - takes the mint as
one library call, `class_mint()` of `harness/class_mint.das`, built with the pin and the
target's triple (`../dasLLVM/ARCHITECTURE_CPU_CLASSES.md#cross-class`):

```
DAS_TUNE_MODE=tune DAS_JIT_BASELINE=x86-avx bin/daslang -lib -output out/classmint \
    --disable-module dasvulkan --disable-module dascuda --disable-module dasmetal \
    --disable-module dasaccelerate --disable-module dashv \
    modules/dasLLAMA/harness/class_mint.das -- --jit-target=<triple>
```

It writes `out/classmint.o` and `out/classmint.h`; the target's link brings the runtime archive.
The host calls `classmint_create()`, then `classmint_class_mint(ctx)`: the test gate, both
tuners, the sidecar kept in memory, no file touched
(`../dasLLVM/ARCHITECTURE_CPU_CLASSES.md#tune-memory-sidecar`). The text it returns is one JSON object -
`profile`, the file to commit as `performance/defaults/<class>.tune-defaults.json`, and
`sidecar`, the whole mint with its race tables - or `error` alone. Send the whole text home:
the race tables are the evidence a winner is read against.

Trap: without the `--disable-module` flags the object registers the GPU modules its host
compiler carries, and the target's link fails on archives the target does not have.
Trap: with no `--jit-target` the same command builds a library for the build box, which is how
the call is tried before the target sees it; run its host under `DAS_JIT_BASELINE=<class>`, or
the profile is labelled with the box's own class.

Section 7's gate binds this profile too.
