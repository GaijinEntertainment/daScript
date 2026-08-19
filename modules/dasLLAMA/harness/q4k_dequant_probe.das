options gen2
options _dasllama_internal = true

require daslib/fio
require dasllama/dasllama_gguf
require strings

// K-quant dequant oracle probe: dump the fp32 expansion of Q4_K / Q6_K tensor prefixes as raw
// bit patterns (one uint per line), for bit-exact comparison against gguf-py's dequantize.
//
// Usage: bin/daslang modules/dasLLAMA/harness/q4k_dequant_probe.das -- <path-to.gguf> > dump.txt
// Compare: python modules/dasLLAMA/harness/q4k_dequant_compare.py <path-to.gguf> dump.txt

def dump_tensor(m : GGUFMeta; bytes : array<uint8>#; name : string; n : int64) {
    var dst : array<float>
    dst |> resize(n)
    gguf_read_tensor_f32(m, bytes, name, dst, 0l, n)
    print("TENSOR {name} {n} type {gguf_tensor_type(m, name)}\n")
    for (v in dst) {
        print("{unsafe(reinterpret<uint>(v))}\n")
    }
    delete dst
}

[export]
def main {
    var path = "D:/Work/llama.cpp/models/gemma-4-12B-it-Q4_K_M.gguf"
    let args = get_command_line_arguments()
    if (!empty(args)) {
        path = args[length(args) - 1]
    }
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var m <- parse_gguf_meta(bytes)
        dump_tensor(m, bytes, "blk.0.attn_q.weight", 256l * 256l)   // Q4_K
        dump_tensor(m, bytes, "blk.0.attn_v.weight", 256l * 256l)   // Q6_K
        dump_tensor(m, bytes, "token_embd.weight", 256l * 256l)     // Q6_K (tied cls)
        delete m
    }
    fclose(f)
}
