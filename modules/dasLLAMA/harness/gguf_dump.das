options gen2

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs
require dasllama/dasllama_gguf
require strings
require math

// GGUF parser validator: parse a .gguf and print header / metadata / tensor info, so the
// output can be diffed against llama.cpp's `llama-gguf <file> r`. The position-0 detector for
// the loader — prove the parse is right before trusting any weights.
//
// Usage: bin/daslang modules/dasLLAMA/dasllama_gguf_dump.das -- <path-to.gguf>

def gtype_name(t : int) : string {
    let names = ["u8", "i8", "u16", "i16", "u32", "i32", "f32", "bool", "string", "array", "u64", "i64", "f64"]
    return t >= 0 && t < length(names) ? names[t] : "?{t}"
}

def dump_kv(m : GGUFMeta; bytes : array<uint8>#; key : string) {
    if (!gguf_has(m, key)) {
        return
    }
    print("  {key} = ")
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    let t = loc.gtype
    if (t == 8) {  // string
        print("\"{gguf_str(m, bytes, key)}\"\n")
    } elif (t == 6) {  // f32
        print("{gguf_f32(m, bytes, key)}\n")
    } elif (t == 9) {  // array
        print("array<{gtype_name(loc.arr_type)}>[{loc.count}]\n")
    } else {
        print("{gguf_int(m, bytes, key)}\n")
    }
}

[export]
def main {
    var path = "/Users/borisbatkin/Work/llama.cpp/models/ggml-vocab-llama-spm.gguf"
    let args = get_command_line_arguments()
    if (!empty(args)) {
        path = args[length(args) - 1]
    }
    print("=== dasllama_gguf parse: {path} ===\n")
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var m <- parse_gguf_meta(bytes)
        print("version:     {int(m.version)}\n")
        print("alignment:   {m.alignment}\n")
        print("data offset: {m.data_offset}\n")
        print("n_kv:        {length(m.kv)}\n")
        print("n_tensors:   {length(m.tensors)}\n")
        print("--- selected metadata ---\n")
        for (k in ["general.architecture", "general.name", "llama.context_length", "llama.embedding_length",
                   "llama.block_count", "llama.feed_forward_length", "llama.attention.head_count",
                   "llama.attention.head_count_kv", "llama.attention.layer_norm_rms_epsilon",
                   "llama.rope.dimension_count", "llama.rope.freq_base", "general.file_type",
                   "llama.vocab_size", "tokenizer.ggml.model", "tokenizer.ggml.tokens",
                   "tokenizer.ggml.scores", "tokenizer.ggml.token_type", "tokenizer.ggml.bos_token_id",
                   "tokenizer.ggml.eos_token_id"]) {
            dump_kv(m, bytes, k)
        }
        let nt = length(m.tensors)
        if (nt > 0) {
            print("--- first 6 tensors ---\n")
            for (i in range(min(6, nt))) {
                var dimstr = ""
                for (d in m.tensors[i].dims) {
                    dimstr += "{d} "
                }
                print("  [{i}] {m.tensors[i].name}  type={m.tensors[i].ggml_type} dims=[{dimstr}] off={m.tensors[i].offset} nelem={m.tensors[i].n_elem}\n")
            }
        }
    }
    fclose(f)
}
