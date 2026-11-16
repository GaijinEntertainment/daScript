options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// One model through the cached load, a flat session, a served walk of `tokens` (32) and the deletes; at every stage the
// process's resident memory by kind (anonymous = heap, file = mapped file pages), its high-water mark and the das heap.
// The kinds read /proc/self/status and print -1 where the system has none; the das heap prints everywhere.
//
//   DASLLAMA_GPU=1 [DASLLAMA_CUDA=1] [DASLLAMA_IMAGE=0] [DASLLAMA_HOST_PLANES=1] [DASLLAMA_SOURCE_RELEASE=0]
//       bin/daslang -jit modules/dasLLAMA/harness/load_probe.das -- <model.gguf> [tokens]

require dasllama/dasllama
require dasllama/dasllama_common
require dasllama/dasllama_image
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_gpu_tier
require daslib/fio
require daslib/strings_boost
require daslib/strings_convert   // try_to_int: the token count off the command line

def private gb(b : int64) : double => double(b) / 1.0e9lf

//! the kB figure after `key` in /proc/self/status's text, in GB; -1 where the key is absent
def private status_gb(text, key : string) : double {
    let at = find(text, key)
    return -1.0lf if (at < 0)
    let eol = find(text, "\n", at)
    let kb = to_int64(strip(slice(text, at + length(key), eol < 0 ? length(text) : eol)), false)
    return double(kb) * 1024.0lf / 1.0e9lf
}

def private stage(name : string) {
    let text = build_string() $(var w) {
        fopen("/proc/self/status", "rb") $(f) {
            if (f != null) {
                while (!feof(f)) {
                    w |> write(fgets(f))
                }
            }
        }
    }
    print("STAGE {name} | rss {fmt(":.2f", status_gb(text, "VmRSS:"))} anon {fmt(":.2f", status_gb(text, "RssAnon:"))} file {fmt(":.2f", status_gb(text, "RssFile:"))}"
        + " | hwm {fmt(":.2f", status_gb(text, "VmHWM:"))} | das heap {fmt(":.2f", gb(int64(heap_bytes_allocated())))}\n")
}

def private model_bytes(t : Model) {
    let f = footprint(t)
    print("  planes: total {fmt(":.2f", gb(f.total))} = f32 {fmt(":.2f", gb(f.aux_fp32_bytes + f.weight_fp32_bytes))} q8 {fmt(":.2f", gb(f.q8_bytes + f.q8_scale_bytes))}"
        + " kq {fmt(":.2f", gb(f.kq_bytes + f.kq_scale_bytes))} mx4 {fmt(":.2f", gb(f.mx4_bytes + f.mx4_scale_bytes))}"
        + " | vkblob {fmt(":.2f", gb(long_length(t.vkblob)))} embq {fmt(":.2f", gb(long_length(t.embq)))} | image {fmt(":.2f", gb(int64(t.image_bytes)))} trimmed {t.planes_trimmed}\n")
}

[export, cold_path]
def main {
    let args <- get_command_line_arguments()
    let asked = length(args) >= 2 ? try_to_int(args[length(args) - 1]) ?? -1 : -1   //! a last argument that is a whole number is the token count: a model path never is
    let tokens = asked >= 0 ? asked : 32
    let path = args[length(args) - (asked >= 0 ? 2 : 1)]
    stage("start")
    with_dasllama_jobque() {
        let ts = ref_time_ticks()
        var m <- load_model_cached(path, QuantMode.q8)
        print("  ctx {m.config.seq_len} | load {get_time_usec(ts) / 1000} ms | live loads {gguf_live_loads()} | driver decline '{gpu_resident_decline_()}'\n")
        model_bytes(m)
        stage("loaded")
        let prompt <- encode(m, "Count from one to forty, in words: one, two, three, four, five,", true)
        var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        stage("session made")
        var ids <- generate(m, s, prompt, int(long_length(prompt)) - 1 + tokens)
        var passes <- gpu_cpu_passes_()
        print("  walk: {length(ids)} ids | cpu passes {passes}\n")
        print("  text: {slice(decode(m, ids), 0, 200)}\n")
        stage("{tokens} tokens")
        delete passes
        delete s
        delete ids
        stage("session deleted")
        delete m
        stage("model deleted")
    }
    stage("end")
}
