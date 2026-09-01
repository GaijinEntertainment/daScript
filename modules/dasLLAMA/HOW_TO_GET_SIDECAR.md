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

One thread, no model: every format's stamped kernel against the reference exe's `test-backend-ops perf`
at the same shape, decode and prefill rows, ratio = reference / ours. This is the box's table; it goes
into `plans/kernel_parity_pass.md`'s fact base (and the records store once the kernel board exists).

## 6. The mint

(The steps below are being run on the zen4 box as this document is written; they are filled in as
they prove out.)

- The vehicle: a public 1B GGUF pulled straight to the box, no copying from the dev box -
  `curl -L -o Llama-3.2-1B-Instruct-Q4_K_M.gguf https://huggingface.co/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q4_K_M.gguf`
  (808 MB).
- The mint: `bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <gguf> --tune` - the
  full race of every family on this box (about 316 s on a c7a.4xlarge), writing
  `modules/dasLLAMA/benchmarks/lcpp_bench.tune.json`. Read the noise verdict it prints.
- The export: `bin/daslang -jit modules/dasLLAMA/harness/export_tune_profile.das -- --sidecar
  modules/dasLLAMA/benchmarks/lcpp_bench.tune.json` writes
  `modules/dasLLAMA/performance/defaults/<class>.tune-defaults.json` for this box's
  `tune_cpu_class()`; `--class <name>` overrides the name.
- A class that does not exist yet (Intel's `x86-amx`): the chain lives in
  `modules/dasLLVM/daslib/llvm_tune.das` (`tune_cpu_class`, `tune_class_chain`) and every feature a
  `requires=` names sits in `TUNE_KNOWN_FEATURES` there; the new class goes above the class it
  supersedes so a host resolves to the highest class it satisfies that has a shipped profile.

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

- 2026-09-01 `c7a.4xlarge` (EPYC 9R14 zen4), `i-043725feb25086523`: sections 1-5 as written.
