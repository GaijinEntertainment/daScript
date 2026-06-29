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

Frozen fixtures (prompt IDs + reference GEN_IDS) live with the suite in `tests/dasLLAMA/` so CI checks
parity without needing llama.cpp at test time.
