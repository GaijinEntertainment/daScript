options gen2

require dasllama_transformer

// Step-1 checkpoint runner: load stories15M.bin and print the parsed Config.
// Model path is local (the file is 60MB, gitignored).
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"

[export]
def main {
    let t <- load_checkpoint(MODEL_PATH)
    let c = t.config
    print("=== dasLLAMA loaded {MODEL_PATH} ===\n")
    print("dim            = {c.dim}\n")
    print("hidden_dim     = {c.hidden_dim}\n")
    print("n_layers       = {c.n_layers}\n")
    print("n_heads        = {c.n_heads}\n")
    print("n_kv_heads     = {c.n_kv_heads}\n")
    print("vocab_size     = {c.vocab_size}\n")
    print("seq_len        = {c.seq_len}\n")
    print("head_size      = {c.head_size}\n")
    print("kv_dim         = {c.kv_dim}\n")
    print("shared_weights = {c.shared_weights}\n")
    print("blob floats    = {length(t.blob)}\n")
    print("--- offsets (float elements) ---\n")
    print("tok_emb={t.tok_emb_off} wq={t.wq_off} w1={t.w1_off} rms_final={t.rms_final_off} wcls={t.wcls_off}\n")
    print("emb[0..2]      = {t.blob[0]} {t.blob[1]} {t.blob[2]}\n")
}
