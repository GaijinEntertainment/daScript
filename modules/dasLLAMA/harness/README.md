# dasLLAMA harness

Verification / eval test beds and GGUF inspection tools (not shipped with the module).

| File | What it does |
|---|---|
| `parity.das` | dasLLAMA side of the token-for-token oracle check: prompt IDs in → greedy generated IDs out (`GEN_IDS: ...`). Arch-agnostic — routes through `load_gguf` + `generate`. |
| `parity.sh` | Runs the reference oracle and `parity.das` on the same prompt and diffs the two ID streams. |
| `oracle/simple_ids.cpp` | The reference oracle — a trimmed llama.cpp `examples/simple` that prints greedy token IDs (CPU, no EOG break) instead of text. |
| `quant_eval.das` / `quant_eval_q4.das` | Q8 / Q4 reconstruction-error stats vs the fp32 weights. |
| `bpe_test.das` | BPE tokenizer corpus gate against the in-repo `ggml-vocab-llama-bpe` fixtures (no model needed). |
| `gguf_dump.das` | Dump a GGUF's metadata + tensor table. |
| `mem.das` | Resident memory footprint of a loaded model, by region. |

## Token-for-token parity (the correctness detector)

Correctness is proven by reproducing a reference **token-for-token** under greedy decoding, with the
tokenizer kept out of the loop — only the loader + forward pass are under test. The reference IDs come
from `simple_ids` (the same method the arc used); the prompt is tokenized once by llama.cpp and the
exact IDs are fed to both sides.

### One-time: build the oracle

`simple_ids` links against an existing llama.cpp build. From the llama.cpp checkout:

```sh
cp <daScript>/modules/dasLLAMA/harness/oracle/simple_ids.cpp .
clang++ -std=c++17 -O2 -I include -I ggml/include simple_ids.cpp \
  -L build/bin -lllama -lggml -Wl,-rpath,"$PWD/build/bin" -o build/bin/simple_ids
```

### Run a parity check

```sh
# fp32 storage for an F16/F32 file; q8 for a Q8_0 file; q4 for Q4_0
modules/dasLLAMA/harness/parity.sh ~/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf 40 fp32
modules/dasLLAMA/harness/parity.sh ~/Work/llama.cpp/models/Qwen2.5-0.5B-Instruct-Q8_0.gguf 40 q8
```

Prints both ID streams and `PASS: N/N token-for-token` or the first mismatch index.

### Just the dasLLAMA side

```sh
bin/daslang -jit modules/dasLLAMA/harness/parity.das -- \
  -m <model.gguf> -n 40 --quant q8 --ids 1,9038,2501,263,931
```

Frozen fixtures (prompt IDs + reference GEN_IDS) live with the suite in `tests/dasLLAMA/test_parity.das`
so the parity gate runs without llama.cpp/the oracle at test time. The GGUF models themselves are
gitignored, so each case skips cleanly when its model is absent (a real local regression, a no-op in
CI). **Run model tests with the JIT** — interpreted is far too slow for any real model:

```sh
bin/daslang -jit dastest/dastest.das -- --test tests/dasLLAMA/test_parity.das
```

## Sliding-window attention (Gemma2) — long-context check

Gemma2 uses sliding-window (local) attention on its even layers (window 4096, `swa_period=2`) and
global attention on the odd layers. It's a **no-op for contexts ≤ 4096**, so the short parity prompts
above never exercise the window. To test it, feed a prompt that tokenizes past 4096 and diff the
continuation against the oracle (which applies SWA correctly):

```sh
# build a >4096-token prompt (distinctive early fact + filler + a recall query), then:
modules/dasLLAMA/harness/parity.sh ~/Work/llama.cpp/models/gemma-2-2b-it-Q8_0.gguf 12 q8 "$(cat long_prompt.txt)"
```

Verified: a 4168-token context is **11/11 token-for-token** vs the oracle — the window path is correct.

Two caveats worth knowing:
- **It's slow.** The prefill attention loop is single-threaded (only the matmuls thread), so a
  ~4k-token prefill is ~9 minutes. That's why this stays a manual harness check, not a CI fixture.
- **Token-level parity is a weak SWA discriminator on Gemma.** Its *global* (odd) layers carry the
  long-range signal, so toggling the sliding layers off often yields the *same* tokens. The window is
  still load-bearing: at the last position of a 4168-token context (where the sliding layers exclude
  the early tokens) the last-position logits shift by up to **~2.06** (on the ~30 soft-capped scale)
  between SWA-on and SWA-off — i.e. a near-tie prompt *would* flip, so the oracle gate can catch a
  broken window given a sensitive enough prompt.
