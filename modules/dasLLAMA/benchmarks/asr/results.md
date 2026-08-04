# ASR CPU scoreboard (living doc — CURRENT numbers only; history in git + Changelog)

Method: best-of-reps per side, CPU-only everywhere (`-ng`), greedy, transcribe time excludes
model load. Matched threads per box. **Precision is apples-to-apples 8-bit**: das = q8 default
(word-exact vs fp32, see Correctness); cli = whisper.cpp `whisper-cli`/`parakeet-cli` on
**q8_0** models (the upstream quantize tools; measured their best config on-box too — the
old f32/f16 cli rows conflated precision with kernels); onnx = onnx-asr + ONNX-Runtime int8
(engine-level `onnx_bench.py`, no HTTP). Each table's sub-line carries the das precision
(`exec_fmt`), rev and date it was measured; tables render from `performance/records/<box>.json`
via `gen_results.das` (noise policy: >2-3% run-to-run swings mean other software, not thermals).

## Parakeet-TDT 0.6B v2 — das vs parakeet-cli vs ONNX-Runtime int8

### Apple M1 Max, 8 threads

<!-- GEN:asr Parakeet-TDT v2 m1 -->
_Apple M1 Max, 8 threads — das q8 — daslang 0.6.4 db50a7da2, 2026-08-03._

| file      | audio s | das ms | parakeet-cli q8_0 ms | onnx int8 ms | das/parakeet-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | -------------------: | -----------: | ---------------: | --------: | ------: |
| jfk.wav   |      11 |    183 |                  239 |          506 |        **0.77x** | **0.36x** |    60.2 |
| jfk3.wav  |      33 |    455 |                  681 |          905 |        **0.67x** | **0.50x** |    72.6 |
| gb1.wav   |     199 |   4370 |                 7296 |         7111 |        **0.60x** | **0.61x** |    45.5 |
| hp0.wav   |     273 |   7292 |                12250 |        11458 |        **0.60x** | **0.64x** |    37.5 |
| hp0x2.wav |     547 |  23478 |                43842 |            - |        **0.54x** |         - |    23.3 |
<!-- /GEN:asr Parakeet-TDT v2 m1 -->

Source: `performance/records/m1.json` (the record store).

### Apple M4 Pro, 10 threads

<!-- GEN:asr Parakeet-TDT v2 m4 -->
_Apple M4 Pro, 10 threads — das q8 — daslang 0.6.4 c00de5e5b, 2026-08-03._

| file      | audio s | das ms | parakeet-cli q8_0 ms | onnx int8 ms | das/parakeet-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | -------------------: | -----------: | ---------------: | --------: | ------: |
| jfk.wav   |      11 |    114 |                  155 |          412 |        **0.73x** | **0.28x** |    96.8 |
| jfk3.wav  |      33 |    286 |                  437 |          691 |        **0.65x** | **0.41x** |   115.6 |
| gb1.wav   |     199 |   2716 |                 4455 |         4954 |        **0.61x** | **0.55x** |    73.2 |
| hp0.wav   |     273 |   4796 |                 7493 |         8168 |        **0.64x** | **0.59x** |    57.0 |
| hp0x2.wav |     547 |  15724 |                26572 |            - |        **0.59x** |         - |    34.8 |
_&#9888; This row's das run carries no tune stamp: it was measured 2026-08-03 through a released exe whose bundle shipped a stale July sidecar (the daspkg ship-manifest mis-ship fixed in the canary-apples PR), so the winners it ran under cannot be named. The adjacent same-session reference pairing keeps the RATIO honest; absolutes are suspect until the scheduled M4 re-sweep under the fixed release re-stamps them._

<!-- /GEN:asr Parakeet-TDT v2 m4 -->

Source: `performance/records/m4.json` (the record store).

### AMD Ryzen Threadripper 3990X, 16 threads

<!-- GEN:asr Parakeet-TDT v2 zen2 -->
_AMD Ryzen Threadripper 3990X 64-Core Processor, 16 threads — daslang 0.6.4 b1a3af2da, 2026-07-29._

| file      | audio s | das ms | parakeet-cli ms | onnx ms | das/parakeet-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | --------------: | ------: | ---------------: | --------: | ------: |
| jfk.wav   |      11 |    260 |             643 |     625 |        **0.40x** | **0.42x** |    42.3 |
| jfk3.wav  |      33 |    756 |            1714 |    2009 |        **0.44x** | **0.38x** |    43.7 |
| gb1.wav   |     199 |   6656 |           15825 |   15153 |        **0.42x** | **0.44x** |    29.9 |
| hp0.wav   |     273 |  10499 |           25678 |   25263 |        **0.41x** | **0.42x** |    26.0 |
| hp0x2.wav |     547 |  33572 |           83889 |       - |        **0.40x** |         - |    16.3 |
<!-- /GEN:asr Parakeet-TDT v2 zen2 -->

Source: `performance/records/zen2.json` (the record store).

## Parakeet-TDT 0.6B v3 — das vs parakeet-cli vs ONNX-Runtime int8

onnx's exported graph bakes a max input length: hp0x2 overflows its rel-pos table → skipped
(das/cli handle it).

### Apple M1 Max, 8 threads

<!-- GEN:asr Parakeet-TDT v3 m1 -->
_Apple M1 Max, 8 threads — das q8 — daslang 0.6.4 db50a7da2, 2026-08-03._

| file      | audio s | das ms | parakeet-cli q8_0 ms | onnx int8 ms | das/parakeet-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | -------------------: | -----------: | ---------------: | --------: | ------: |
| jfk.wav   |      11 |    199 |                  244 |          459 |        **0.81x** | **0.43x** |    55.3 |
| jfk3.wav  |      33 |    490 |                  706 |          910 |        **0.69x** | **0.54x** |    67.4 |
| gb1.wav   |     199 |   4485 |                 7279 |         7057 |        **0.62x** | **0.64x** |    44.3 |
| hp0.wav   |     273 |   7512 |                12273 |        11420 |        **0.61x** | **0.66x** |    36.4 |
| hp0x2.wav |     547 |  24034 |                44887 |            - |        **0.54x** |         - |    22.7 |
<!-- /GEN:asr Parakeet-TDT v3 m1 -->

Source: `performance/records/m1.json` (the record store).

### Apple M4 Pro, 10 threads

<!-- GEN:asr Parakeet-TDT v3 m4 -->
_Apple M4 Pro, 10 threads — das q8 — daslang 0.6.4 c00de5e5b, 2026-08-03._

| file      | audio s | das ms | parakeet-cli q8_0 ms | onnx int8 ms | das/parakeet-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | -------------------: | -----------: | ---------------: | --------: | ------: |
| jfk.wav   |      11 |    120 |                  162 |          405 |        **0.74x** | **0.30x** |    91.9 |
| jfk3.wav  |      33 |    300 |                  441 |          712 |        **0.68x** | **0.42x** |   110.0 |
| gb1.wav   |     199 |   2820 |                 4490 |         4802 |        **0.63x** | **0.59x** |    70.5 |
| hp0.wav   |     273 |   4980 |                 7472 |         8064 |        **0.67x** | **0.62x** |    54.9 |
| hp0x2.wav |     547 |  16453 |                26559 |            - |        **0.62x** |         - |    33.2 |
_&#9888; This row's das run carries no tune stamp: it was measured 2026-08-03 through a released exe whose bundle shipped a stale July sidecar (the daspkg ship-manifest mis-ship fixed in the canary-apples PR), so the winners it ran under cannot be named. The adjacent same-session reference pairing keeps the RATIO honest; absolutes are suspect until the scheduled M4 re-sweep under the fixed release re-stamps them._

<!-- /GEN:asr Parakeet-TDT v3 m4 -->

Source: `performance/records/m4.json` (the record store).

### AMD Ryzen Threadripper 3990X, 16 threads

<!-- GEN:asr Parakeet-TDT v3 zen2 -->
_AMD Ryzen Threadripper 3990X 64-Core Processor, 16 threads — daslang 0.6.4 b1a3af2da, 2026-07-29._

| file      | audio s | das ms | parakeet-cli ms | onnx ms | das/parakeet-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | --------------: | ------: | ---------------: | --------: | ------: |
| jfk.wav   |      11 |    275 |             643 |     484 |        **0.43x** | **0.57x** |    40.0 |
| jfk3.wav  |      33 |    806 |            1744 |    1586 |        **0.46x** | **0.51x** |    40.9 |
| gb1.wav   |     199 |   6912 |           16041 |   14780 |        **0.43x** | **0.47x** |    28.8 |
| hp0.wav   |     273 |  10992 |           26517 |   24405 |        **0.41x** | **0.45x** |    24.9 |
| hp0x2.wav |     547 |  34151 |           83758 |       - |        **0.41x** |         - |    16.0 |
<!-- /GEN:asr Parakeet-TDT v3 zen2 -->

Source: `performance/records/zen2.json` (the record store).

## LibriSpeech test-clean, 25 short clips — per-clip latency (dictation case)

### Apple M1 Max, 8 threads

<!-- GEN:asr librispeech m1 -->
| side              | total ms |      xRT |
| :---------------- | -------: | -------: |
| **das**           | **4060** | **53.2** |
| parakeet-cli q8_0 |     5154 |     41.9 |
| onnx int8         |     9862 |     21.9 |
<!-- /GEN:asr librispeech m1 -->

Source: `performance/records/m1.json` (das rows bold). das leads AMX cli on every side —

### Apple M4 Pro, 10 threads

<!-- GEN:asr librispeech m4 -->
| side              | total ms |      xRT |
| :---------------- | -------: | -------: |
| **das**           | **2701** | **79.9** |
| parakeet-cli q8_0 |     3414 |     63.2 |
| onnx int8         |     9425 |     22.9 |
<!-- /GEN:asr librispeech m4 -->

Source: `performance/records/m4.json` (the record store).
parakeet v2/v3 and whisper tiny — and runs 2–3.6× faster than onnx-int8. whisper-tiny is the
quicker dictation model (p50 ~115 ms), parakeet v2/v3 the stronger.

## Whisper — das vs whisper-cli (`-bs 1 -bo 1 -nf -ng`)

das = tower q8 + threaded gelu table (`d25a46542`) + decoder q8 (`cb20e2954`) + the
parakeet-parity opt pass (`8c10b930e`: per-frame threaded mel, threaded f4 cross_kv
scatters, zero-alloc sessions, hmax+exp4 decode passes) + threaded bias/residual row
passes (`cb26a05d0`) + flattened tower attention (`cab95ee9c`: (head × query-block)
units over the slot-indexed team dispatch — killed the head-unit raggedness that
dominated encode; bit-exact, fingerprints byte-identical).

### Apple M1 Max, 8 threads

_tiny:_

<!-- GEN:asr Whisper tiny m1 -->
_Apple M1 Max, 8 threads — das q8/q8 — daslang 0.6.4 3ec014ab6, 2026-08-03._

| file      | audio s | das ms | whisper-cli q8_0 ms | onnx int8 ms | das/whisper-cli |  das/onnx | das xRT | das enc ms | whisper-cli enc ms | onnx enc ms |
| :-------- | ------: | -----: | ------------------: | -----------: | --------------: | --------: | ------: | ---------: | -----------------: | ----------: |
| jfk.wav   |      11 |     86 |                 109 |          404 |       **0.79x** | **0.21x** |   127.6 |         54 |                  - |           - |
| jfk3.wav  |      33 |    198 |                 261 |            - |       **0.76x** |         - |   166.9 |        110 |                  - |           - |
| gb1.wav   |     199 |   1002 |                1497 |            - |       **0.67x** |         - |   198.3 |        435 |                  - |           - |
| hp0.wav   |     273 |   1252 |                2009 |            - |       **0.62x** |         - |   218.4 |        601 |                  - |           - |
| hp0x2.wav |     547 |   2515 |                4009 |            - |       **0.63x** |         - |   217.4 |       1197 |                  - |           - |
<!-- /GEN:asr Whisper tiny m1 -->

_large-v3-turbo:_

<!-- GEN:asr Whisper large-v3-turbo m1 -->
_Apple M1 Max, 8 threads — das q8/q8 — daslang 0.6.4 3ec014ab6, 2026-08-03._

| file      | audio s | das ms | whisper-cli q8_0 ms | onnx int8 ms | das/whisper-cli |  das/onnx | das xRT | das enc ms | whisper-cli enc ms | onnx enc ms |
| :-------- | ------: | -----: | ------------------: | -----------: | --------------: | --------: | ------: | ---------: | -----------------: | ----------: |
| jfk.wav   |      11 |   1969 |                2161 |         2260 |       **0.91x** | **0.87x** |     5.6 |       1856 |                  - |           - |
| jfk3.wav  |      33 |   4030 |                4446 |            - |       **0.91x** |         - |     8.2 |       3715 |                  - |           - |
| gb1.wav   |     199 |  16794 |               23196 |            - |       **0.72x** |         - |    11.8 |      14925 |                  - |           - |
| hp0.wav   |     273 |  24719 |               34918 |            - |       **0.71x** |         - |    11.1 |      22559 |                  - |           - |
| hp0x2.wav |     547 |  47244 |               61384 |            - |       **0.77x** |         - |    11.6 |      42995 |                  - |           - |
<!-- /GEN:asr Whisper large-v3-turbo m1 -->

### Apple M4 Pro, 10 threads

_tiny:_

<!-- GEN:asr Whisper tiny m4 -->
_Apple M4 Pro, 10 threads — das q8/q8 — daslang 0.6.4 c00de5e5b, 2026-08-03._

| file      | audio s | das ms | whisper-cli q8_0 ms | onnx int8 ms | das/whisper-cli |  das/onnx | das xRT | das enc ms | whisper-cli enc ms | onnx enc ms |
| :-------- | ------: | -----: | ------------------: | -----------: | --------------: | --------: | ------: | ---------: | -----------------: | ----------: |
| jfk.wav   |      11 |     54 |                  69 |          418 |       **0.77x** | **0.13x** |   205.1 |         33 |                  - |           - |
| jfk3.wav  |      33 |    124 |                 162 |            - |       **0.76x** |         - |   267.2 |         67 |                  - |           - |
| gb1.wav   |     199 |    632 |                 906 |            - |       **0.70x** |         - |   314.6 |        269 |                  - |           - |
| hp0.wav   |     273 |    798 |                1184 |            - |       **0.67x** |         - |   342.5 |        375 |                  - |           - |
| hp0x2.wav |     547 |   1612 |                2428 |            - |       **0.66x** |         - |   339.0 |        757 |                  - |           - |
_&#9888; This row's das run carries no tune stamp: it was measured 2026-08-03 through a released exe whose bundle shipped a stale July sidecar (the daspkg ship-manifest mis-ship fixed in the canary-apples PR), so the winners it ran under cannot be named. The adjacent same-session reference pairing keeps the RATIO honest; absolutes are suspect until the scheduled M4 re-sweep under the fixed release re-stamps them._

<!-- /GEN:asr Whisper tiny m4 -->

_large-v3-turbo:_

<!-- GEN:asr Whisper large-v3-turbo m4 -->
_Apple M4 Pro, 10 threads — das q8/q8 — daslang 0.6.4 c00de5e5b, 2026-08-03._

| file      | audio s | das ms | whisper-cli q8_0 ms | onnx int8 ms | das/whisper-cli |  das/onnx | das xRT | das enc ms | whisper-cli enc ms | onnx enc ms |
| :-------- | ------: | -----: | ------------------: | -----------: | --------------: | --------: | ------: | ---------: | -----------------: | ----------: |
| jfk.wav   |      11 |   1218 |                1359 |         1678 |       **0.90x** | **0.73x** |     9.0 |       1149 |                  - |           - |
| jfk3.wav  |      33 |   2507 |                2823 |            - |       **0.89x** |         - |    13.2 |       2320 |                  - |           - |
| gb1.wav   |     199 |  10594 |               16279 |            - |       **0.65x** |         - |    18.8 |       9460 |                  - |           - |
| hp0.wav   |     273 |  17082 |               21515 |            - |       **0.79x** |         - |    16.0 |      15661 |                  - |           - |
| hp0x2.wav |     547 |  32622 |               43007 |            - |       **0.76x** |         - |    16.8 |      29817 |                  - |           - |
_&#9888; This row's das run carries no tune stamp: it was measured 2026-08-03 through a released exe whose bundle shipped a stale July sidecar (the daspkg ship-manifest mis-ship fixed in the canary-apples PR), so the winners it ran under cannot be named. The adjacent same-session reference pairing keeps the RATIO honest; absolutes are suspect until the scheduled M4 re-sweep under the fixed release re-stamps them._

<!-- /GEN:asr Whisper large-v3-turbo m4 -->

Source: `performance/records/m1.json` (the record store);
onnx is jfk-only (single-window adapter). The flattened tower attention (`cab95ee9c`) had das
**beat AMX cli on all five tiny rows** and win every turbo row past jfk3 (hp0 0.99x, hp0x2 0.96x —
both were cli's). Stage breakdown (per rep, jfk): tiny encode 81.7 / decode 23.3 / cross_kv 5.6;
turbo encode 2540 / decode 107 / cross_kv 41.

### AMD Ryzen Threadripper 3990X, 16 threads

_tiny:_

<!-- GEN:asr Whisper tiny zen2 -->
_AMD Ryzen Threadripper 3990X 64-Core Processor, 16 threads — daslang 0.6.4 2356e63c6, 2026-07-29._

| file      | audio s | das ms | whisper-cli ms | onnx ms | das/whisper-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | -------------: | ------: | --------------: | --------: | ------: |
| jfk.wav   |      11 |    140 |            221 |     573 |       **0.63x** | **0.24x** |    78.7 |
| jfk3.wav  |      33 |    326 |            500 |       - |       **0.65x** |         - |   101.1 |
| gb1.wav   |     199 |   1801 |           3062 |       - |       **0.59x** |         - |   110.3 |
| hp0.wav   |     273 |   2460 |           3583 |       - |       **0.69x** |         - |   111.1 |
| hp0x2.wav |     547 |   4261 |           7218 |       - |       **0.59x** |         - |   128.3 |
<!-- /GEN:asr Whisper tiny zen2 -->

_large-v3-turbo:_

<!-- GEN:asr Whisper large-v3-turbo zen2 -->
_AMD Ryzen Threadripper 3990X 64-Core Processor, 16 threads — daslang 0.6.4 2356e63c6, 2026-07-29._

| file      | audio s | das ms | whisper-cli ms | onnx ms | das/whisper-cli |  das/onnx | das xRT |
| :-------- | ------: | -----: | -------------: | ------: | --------------: | --------: | ------: |
| jfk.wav   |      11 |   2922 |           6578 |    3273 |       **0.44x** | **0.89x** |     3.8 |
| jfk3.wav  |      33 |   5949 |          13360 |       - |       **0.45x** |         - |     5.5 |
| gb1.wav   |     199 |  24935 |          74264 |       - |       **0.34x** |         - |     8.0 |
| hp0.wav   |     273 |  35919 |          96386 |       - |       **0.37x** |         - |     7.6 |
| hp0x2.wav |     547 |  69225 |         184229 |       - |       **0.38x** |         - |     7.9 |
<!-- /GEN:asr Whisper large-v3-turbo zen2 -->

Source: `performance/records/zen2.json` (the record store);
onnx is jfk-only (single-window adapter, >30 s clips skipped). Same flattened tower attention
(`cab95ee9c`) as on M1: das leads whisper-cli on every turbo row and sits at cli parity / leads long
on tiny; das beats onnx-int8 ~3x on the comparable tiny row. No VNNI on Zen 2 (plain AVX2).

## Canary-Qwen 2.5B — das vs the NeMo SALM reference engine

llama.cpp has **zero** canary/SALM/FastConformer support and onnx-asr ships no canary export, so the
only reference is **NeMo SALM greedy** (`canary_qwen_bench.py`, engine-level python, **CPU** — torch
threads, no MPS; NVIDIA ships the checkpoint bf16 and nobody quantizes canary — a das q8 canary is
the first). das runs **q8** end to end (decoder on the tuned q8 rail, FastConformer encoder
transcoded Q8_0 at load) and the greedy ids DON'T MOVE: `test_canary_qwen_oracle` holds
token-for-token against the frozen NeMo ids on the q8 rail, transcripts byte-identical. The fp32
arm remains the off-facade parity lane (`set_asr_fp32`).

The ladder caps at gb1 — the model's limitation, not a budget choice: canary's trained envelope is
40 s of audio / 1024 total tokens (model card), and past it single-window greedy decode
runaway-loops on repetitive audio in every engine. das decode is budget-capped at 256 new tokens
(`CN_MAX_NEW`, mirroring the reference's `--max-new-tokens 256`), so both engines time the same
bounded decode. 40 s chunked long-form — the NVIDIA-sanctioned long-audio usage — is ledgered.

### Apple M1 Max, 8 threads

<!-- GEN:asr Canary-Qwen 2.5B m1 -->
_Apple M1 Max, 8 threads — das q8/q8 — daslang 0.6.4 db50a7da2, 2026-08-03._

| file     | audio s | das ms | nemo bf16 ms |  das/nemo | das xRT | das enc ms | nemo enc ms |
| :------- | ------: | -----: | -----------: | --------: | ------: | ---------: | ----------: |
| jfk.wav  |      11 |   1206 |         8097 | **0.15x** |     9.1 |        395 |           - |
| jfk3.wav |      33 |   3717 |        22050 | **0.17x** |     8.9 |       1068 |           - |
| gb1.wav  |     199 |  16734 |        41201 | **0.41x** |    11.9 |       8313 |           - |
<!-- /GEN:asr Canary-Qwen 2.5B m1 -->

Source: `performance/records/m1.json` (the record store).

### Apple M4 Pro, 10 threads

<!-- GEN:asr Canary-Qwen 2.5B m4 -->
_Apple M4 Pro, 10 threads — das q8/q8 — daslang 0.6.4 c00de5e5b, 2026-08-03._

| file     | audio s | das ms | nemo bf16 ms |  das/nemo | das xRT | das enc ms | nemo enc ms |
| :------- | ------: | -----: | -----------: | --------: | ------: | ---------: | ----------: |
| jfk.wav  |      11 |    709 |         3336 | **0.21x** |    15.5 |        266 |           - |
| jfk3.wav |      33 |   2166 |         7705 | **0.28x** |    15.2 |        729 |           - |
| gb1.wav  |     199 |  14532 |        18620 | **0.78x** |    13.7 |       8416 |           - |
_&#9888; This row's das run carries no tune stamp: it was measured 2026-08-03 through a released exe whose bundle shipped a stale July sidecar (the daspkg ship-manifest mis-ship fixed in the canary-apples PR), so the winners it ran under cannot be named. The adjacent same-session reference pairing keeps the RATIO honest; absolutes are suspect until the scheduled M4 re-sweep under the fixed release re-stamps them._

<!-- /GEN:asr Canary-Qwen 2.5B m4 -->

Source: `performance/records/m4.json` (the record store).
The table above predates the q8 cutover (the next sweep refreshes it). On the q8 rail (M1,
interleaved best-of-3, 2026-08-03): jfk 1304 ms = **0.16x**, jfk3 3851 = **0.18x**, gb1 17762 =
**0.50x** — das **leads every cell**, gb1 went 125.4 s → 17.8 s across the arc (q8 decoder
prefill 10.2x, q8 encoder GEMMs 1.7x, threaded slab attention 4x on encode), xRT 11.2. NeMo's
heavy fixed per-`generate` overhead still dominates its short clips.

## Gemma-4 E2B audio — das vs llama-mtmd-cli (CPU-only)

das = gemma4a Conformer encoder **fp32** (correctness-first — encode is not the perf gate) + Gemma-4
decoder q8 (matching mtmd). Reference = `llama-mtmd-cli --temp 0 --jinja --reasoning off` (no-think),
**CPU-only** (Accelerate/AMX, Metal off). Transcribe excludes model load; both greedy, one 17.4 s clip.

### Apple M1 Max, 8 threads

<!-- GEN:asr Gemma-4 E2B audio m1 -->
_Apple M1 Max, 8 threads — das f32/q8 — daslang 0.6.4 db50a7da2, 2026-08-03._

| file     | audio s | das ms | llama-mtmd-cli bf16/q8_0 ms | das/llama-mtmd-cli | das xRT |
| :------- | ------: | -----: | --------------------------: | -----------------: | ------: |
| jfk.wav  |      11 |   2379 |                       10812 |          **0.22x** |     4.6 |
| jfk3.wav |      33 |   6750 |                       32393 |          **0.21x** |     4.9 |
| gb1.wav  |     199 |  45445 |                      198190 |          **0.23x** |     4.4 |
| hp0.wav  |     273 |  62016 |                           - |                  - |     4.4 |
<!-- /GEN:asr Gemma-4 E2B audio m1 -->

Source: `performance/records/m1.json` (the record store).

### Apple M4 Pro, 10 threads

<!-- GEN:asr Gemma-4 E2B audio m4 -->
_Apple M4 Pro, 10 threads — das f32/q8 — daslang 0.6.4 20c53ec26, 2026-08-03._

| file     | audio s | das ms | llama-mtmd-cli bf16/q8_0 ms | das/llama-mtmd-cli | das xRT |
| :------- | ------: | -----: | --------------------------: | -----------------: | ------: |
| jfk.wav  |      11 |   1366 |                        5150 |          **0.27x** |     8.1 |
| jfk3.wav |      33 |   3913 |                       15528 |          **0.25x** |     8.4 |
| gb1.wav  |     199 |  27864 |                       97510 |          **0.29x** |     7.1 |
| hp0.wav  |     273 |  40496 |                           - |                  - |     6.7 |
_&#9888; This row's das run carries no tune stamp: it was measured 2026-08-03 through a released exe whose bundle shipped a stale July sidecar (the daspkg ship-manifest mis-ship fixed in the canary-apples PR), so the winners it ran under cannot be named. The adjacent same-session reference pairing keeps the RATIO honest; absolutes are suspect until the scheduled M4 re-sweep under the fixed release re-stamps them._

<!-- /GEN:asr Gemma-4 E2B audio m4 -->

Source: `performance/records/m4.json` (the record store).
**das leads ~3x.** On a genuine CPU-only basis, ggml's Conformer audio ops don't route through
BLAS/AMX, so mtmd's encode is ~16 s (≈99% of its total) — das's optimized fp32 scalar Conformer
(~1.9 s) beats it ~9x on encode. The prior "das trails 3.9x" compared das-CPU against an mtmd whose
audio encode was silently **Metal/GPU**-offloaded (mislabeled CPU-only) — a CPU-vs-GPU mismatch.

### AMD Ryzen Threadripper 3990X, 16 threads

<!-- GEN:asr Gemma-4 E2B audio zen2 -->
_AMD Ryzen Threadripper 3990X 64-Core Processor, 16 threads — daslang 0.6.4 2356e63c6, 2026-07-29._

| file     | audio s | das ms | llama-mtmd-cli ms | das/llama-mtmd-cli | das xRT |
| :------- | ------: | -----: | ----------------: | -----------------: | ------: |
| jfk3.wav |      33 |   8494 |              8957 |          **0.95x** |     3.9 |
<!-- /GEN:asr Gemma-4 E2B audio zen2 -->

Source: `performance/records/zen2.json` (the record store).

## Qwen3-Omni-30B audio — das vs llama-mtmd-cli (CPU-only)

Audio-in/text-out (Talker out of scope). das = SHARED qwen3a AuT tower **fp32** (d_model 1280 × ff
5120, 32 blocks — correctness-first, not the perf gate) + qwen3vlmoe (30B-A3B) thinker q8 (matching
mtmd). Reference = `llama-mtmd-cli --temp 0 --jinja --reasoning off` (no-think), **CPU-only** (Metal
off). Transcribe excludes model load; both greedy.

### Apple M1 Max, 8 threads

<!-- GEN:asr Qwen3-Omni-30B audio m1 -->
_Apple M1 Max, 8 threads — das f32/q8 — daslang 0.6.4 b1db4b7f6, 2026-08-03._

| file      | audio s | das ms | llama-mtmd-cli bf16/q8_0 ms | das/llama-mtmd-cli | das xRT |
| :-------- | ------: | -----: | --------------------------: | -----------------: | ------: |
| jfk.wav   |      11 |   3281 |                       21035 |          **0.16x** |     3.4 |
| jfk3.wav  |      33 |   7410 |                       44210 |          **0.17x** |     4.5 |
| gb1.wav   |     199 |  64240 |                      246790 |          **0.26x** |     3.1 |
| hp0.wav   |     273 |  89098 |                           - |                  - |     3.1 |
| hp0x2.wav |     547 | 218319 |                           - |                  - |     2.5 |
<!-- /GEN:asr Qwen3-Omni-30B audio m1 -->

Source: `performance/records/m1.json` (the record store).

### Apple M4 Pro, 10 threads

<!-- GEN:asr Qwen3-Omni-30B audio m4 -->
_Apple M4 Pro, 10 threads — das f32/q8 — daslang 0.6.4 20c53ec26, 2026-08-03._

| file      | audio s | das ms | llama-mtmd-cli bf16/q8_0 ms | das/llama-mtmd-cli | das xRT |
| :-------- | ------: | -----: | --------------------------: | -----------------: | ------: |
| jfk.wav   |      11 |   1893 |                       52718 |          **0.04x** |     5.8 |
| jfk3.wav  |      33 |   4393 |                       22343 |          **0.20x** |     7.5 |
| gb1.wav   |     199 |  42863 |                      126040 |          **0.34x** |     4.6 |
| hp0.wav   |     273 |  60046 |                           - |                  - |     4.6 |
| hp0x2.wav |     547 | 153586 |                           - |                  - |     3.6 |
_&#9888; This row's das run carries no tune stamp: it was measured 2026-08-03 through a released exe whose bundle shipped a stale July sidecar (the daspkg ship-manifest mis-ship fixed in the canary-apples PR), so the winners it ran under cannot be named. The adjacent same-session reference pairing keeps the RATIO honest; absolutes are suspect until the scheduled M4 re-sweep under the fixed release re-stamps them._

<!-- /GEN:asr Qwen3-Omni-30B audio m4 -->

Source: `performance/records/m4.json` (the record store).
**das leads ~5x.** CPU-only, the AuT tower is entirely encode-bound in ggml (~21 s jfk / ~45 s jfk3,
≈99% of mtmd's total); das's tower does it in ~1.7 / 4.8 s. Same CPU-vs-GPU correction as gemma4a —
the prior "das trails 3-4x" measured mtmd's Metal-offloaded encode, mislabeled CPU-only.

### AMD Ryzen Threadripper 3990X, 16 threads

<!-- GEN:asr Qwen3-Omni-30B audio zen2 -->
_AMD Ryzen Threadripper 3990X 64-Core Processor, 16 threads — daslang 0.6.4 2356e63c6, 2026-07-29._

| file     | audio s | das ms | llama-mtmd-cli ms | das/llama-mtmd-cli | das xRT |
| :------- | ------: | -----: | ----------------: | -----------------: | ------: |
| jfk.wav  |      11 |   4553 |            296392 |          **0.02x** |     2.4 |
| jfk3.wav |      33 |   8290 |             11220 |          **0.74x** |     4.0 |
<!-- /GEN:asr Qwen3-Omni-30B audio zen2 -->

Source: `performance/records/zen2.json` (the record store).

## Correctness

- das fp32 is token-for-token with parakeet-cli (v2 jfk 33 + gb1 786; v3 jfk 38 + gb1 655)
  and with whisper-cli (tiny jfk, suite oracle), and Canary-Qwen with NeMo SALM greedy
  (jfk + 2 LibriSpeech clips, incl. trailing EOS).
- Gemma-4 E2B audio (gemma4a Conformer): the fp32 encoder soft tokens match mtmd's audio
  embeddings at rel ~0.0027 (float-reduction-order noise, not a bug); the no-think greedy
  transcription is token-for-token with mtmd-cli through the 48-token confident prefix and
  diverges only at the Gemma-4 q8 decoder's OWN ~0.6-logit near-tie (llama.cpp's raw logits at
  that step: three tokens within 0.6). `test_gemma4a_audio_oracle` asserts the confident head.
- Qwen3-Omni-30B (audio-in/text-out; SHARED qwen3a AuT tower + qwen3vlmoe MoE thinker): the greedy
  transcription is **FULL token-for-token** with mtmd-cli on jfk + 2 LibriSpeech clips incl. the
  trailing `<|im_end|>` (151645), with NO near-tie divergence. `test_qwen3omni_audio_oracle` (large-tier).
- das q8 (the path benched here): parakeet ids/text exact vs fp32, gb1 shows a handful of
  duration-pick flips (≤4-frame timestamp drift); whisper (tower + decoder q8) word-exact
  on tiny/small/large-v3-turbo, base drops one comma on a 0.018-logit fp32 top-2 near-tie
  (teacher-forced adjudication; `wh_q8_probe`).
- onnx-int8 changes TEXT vs fp32 (e.g. jfk "for you, ask" → "for you. Ask").

## Changelog

- 2026-08-03: APPLES CUTOVER — cli references switch to q8_0 models (upstream quantize tools,
  provisioned by `setup_asr_rig --refs`; also their measured-best config), tables regenerate from
  `performance/records/<box>.json` (the orphaned `profile_asr_*.json` + baseline TSVs retired),
  sub-lines carry das `exec_fmt` + rev + date, LibriSpeech becomes totals-only (the per-clip
  p50/p95 died with the old profile shape). Canary goes q8 end-to-end with the oracle ids
  UNCHANGED and gb1 125.4 s → 17.8 s (leads NeMo 2x); parakeet + canary quadratic attention
  scratch replaced by per-job slabs. Full three-box re-sweep pending at arc close.
- 2026-07-08: M1 Class-2 (Canary, Gemma-4 E2B, Qwen3-Omni-30B) now GENERATED by the suite too — all
  M1 model sections are suite-driven; only zen2 rows stay hand-authored. **Methodology correction:**
  the mtmd (gemma4a/qwen3omni) reference is now genuine **CPU-only** (Accelerate/AMX, Metal off). The
  prior "das trails 3–4x" was a CPU-vs-GPU mismatch — the fast mtmd numbers rode Metal (GPU), since
  ggml's Conformer/AuT audio ops don't hit BLAS/AMX; on CPU-only the encode is 16–45 s and **das
  leads 3–5x** (gemma4a 0.32x, q3o 0.17x/0.19x). Reference no-think via a fresh llama.cpp patch
  (`harness/mtmd_bench.patch`: expose `--reasoning` to mtmd-cli + thread `enable_thinking`; run
  `--reasoning off`), CPU-only build `llama.cpp-clean/build-amx`. Canary das now loads its encoder
  (2-file); NeMo ref is CPU (torch, no MPS); das leads short (0.69x/0.81x), trails gb1 (3.74x).
- 2026-07-08: the M1 Parakeet v2/v3, Whisper tiny/turbo, and LibriSpeech dictation tables are now
  GENERATED by the profiling suite (`performance/gen_asr_profile.das` → `profile_asr_m1.json` →
  `gen_results.das`, spliced between `<!-- GEN:asr … -->` markers; prose hand-maintained). All
  references (parakeet-cli/whisper-cli + onnx) re-established fresh this round (Parsec off) into
  `baseline_asr_m1.tsv` — within noise of the prior stored rows. Parakeet v2 gains onnx columns.
  Class-2 (Canary/Gemma-4/Qwen3-Omni) + zen2 rows remain hand-authored pending their suite run.
- 2026-07-08: NEW Qwen3-Omni-30B audio section (Wave A3) — das (fp32 SHARED qwen3a AuT tower + q8
  qwen3vlmoe MoE thinker) vs `llama-mtmd-cli`. FULL token-for-token (jfk + 2 LibriSpeech, incl.
  trailing `<|im_end|>`). das trails 3.1x→4.0x: fp32 scalar audio tower (~4.8x/4.0x on encode) + the
  MoE thinker path; the A3 gate is fp32 encoder + parity. TSVs `results_q3o_m1_t8.tsv` /
  `results_q3o_m1_mtmd.tsv`.
- 2026-07-08: NEW Gemma-4 E2B audio section — das (fp32 gemma4a Conformer + q8 Gemma-4 decoder)
  vs the CLI oracle `llama-mtmd-cli` (no-think). das trails 3.9x: fp32 scalar encoder (16x on
  encode) + long-context decode gap; the A2 gate is fp32 encoder correctness (rel ~0.0027 vs
  mtmd soft tokens). TSVs `results_g4a_m1_t8.tsv` / `results_g4a_m1_mtmd.tsv`.
- 2026-07-08 `daf12e7b7`: NEW Canary-Qwen 2.5B section — das (fp32) vs the NeMo SALM reference
  engine (`canary_qwen_bench.py`; no llama.cpp/onnx canary support). das leads every short clip
  1.4–3x, trails 3.7x on the fp32-decoder-bound 3-min gb1. TSVs `results_cq_m1_t8.tsv` /
  `results_cq_m1_nemo.tsv`.
- 2026-07-08 `cab95ee9c`: flattened tower attention over (head × query-block) units via the
  new slot-indexed team dispatch (jobque `team_parallel_for_indexed` `9f7b10288` +
  `maybe_parallel_for_indexed` `3800b2aa4`) — bit-exact (pre/post fingerprints
  byte-identical, 6 model×wav combos, q8+fp32). zen2 das re-sweep: tiny -14-24%
  (cli parity short, leads long), turbo -12-14% (0.45-0.58x). M1 re-sweep (Parsec off):
  das -9-10% every row — whisper tiny beats AMX cli on all five corpus rows + LibriSpeech
  (p50 117 vs 129), turbo wins everything past jfk3.
- 2026-07-08 (Parsec-off window): M1 whisper das re-sweep @ `cb26a05d0` (-35-45% vs the
  07-07 rows — tiny beats AMX cli on 4 of 5 rows) + NEW baselines: M1 whisper onnx-int8
  jfk columns, LibriSpeech whisper-tiny 3-way. Parakeet das re-check: within noise of the
  standing 07-07 rows (no table change).
- 2026-07-08 `cb26a05d0`: threaded bias/residual row passes (the "fc1/fc2 q8 rate gap" —
  sub-buckets showed single-threaded bias bandwidth, not requant); zen2 turbo -3-5%.
- 2026-07-08 `8c10b930e`: whisper parakeet-parity opt pass (per-frame threaded mel `a19a9d5ec`,
  threaded f4 cross_kv scatters `dec9f8656`, phase-0 zero-alloc `3c31be3a0`, decode
  hmax+exp4 `8c10b930e`); zen2 das re-sweep — tiny -22-25%, turbo -6-8%. M1 whisper table
  still @ `a97881dfb` pending a Parsec window.
- 2026-07-08 `cb20e2954`: whisper decoder q8; zen2 das re-sweep — end-to-end neutral there
  (logits/cross_kv wins vs cache-hot GEMV losses), M1 re-sweep pending Parsec window.
- 2026-07-08 `63e2ac191`: zen2 onnx whisper columns (jfk-only — the onnx-asr whisper
  adapter is single-window; >30 s rows skipped by the harness).
- 2026-07-07 `c5ad73980`+: doc restructured — current tables only, per-platform sections.
- 2026-07-07 `97e7e7cf6`: zen2 v3 three-side round (first v3/onnx numbers on that box).
- 2026-07-07 `0ee3807ec`: zen2 das re-sweep post dispatch-fix + TUNE — das leads every row.
- 2026-07-07 `17abcd32d`: M1 das re-sweep post dispatch-fix — das leads every row; the
  pre-fix das rows (2026-07-06 tables) measured a bare `with_job_que()` fifo dispatch
  (2.2x handicap) and are superseded; cli/onnx TSVs from that round remain the baselines.
- 2026-07-06 `ee83e59b5`: Parsec-off cli/onnx standing baselines (M1); zen2 v2 cli baseline.
