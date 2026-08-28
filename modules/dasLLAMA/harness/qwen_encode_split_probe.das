options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require llvm/daslib/llvm_tune   // the tune-manifest seat the image identity reads
require dasllama/dasllama_vision_embedder
require dasllama/dasllama_vision
require dasllama/dasllama_vision_io
require dasllama/dasllama_tower   // set_asr_prof / asr_prof_report — the encode-stage rail
require dasllama/dasllama_math   // setup_dasllama_jobque_
require ?das_metal dasllama/dasllama_metal_tower   // [init] registers the GPU block serve — without it the CPU f32 path runs
require daslib/jobque_boost

// Splits one vision encode: wall vs the q3v.* prof buckets (stem = CPU patch conv,
// tail = the whole GPU blocks+taps+tail serve). preproc = wall - sum(buckets).
// Direction-grade attribution for the das-vs-clip encode gap; best-of over REPS walls,
// prof printed for the LAST rep alone.

let REPS = 5

[export]
def main {
    let mmproj = "/Users/borisbatkin/Work/llama.cpp/models/mmproj-Qwen3VL-8B-Instruct-F16.gguf"
    let picture = "/Users/borisbatkin/Work/llama.cpp/models/coco-val2017-000000039769.jpg"
    with_job_que() {
        setup_dasllama_jobque_()
        let emb <- load_vision_embedder(mmproj)
        var pic <- load_image_rgb(picture)
        if (pic.width == 0) {
            panic("probe: could not decode '{picture}'")
        }
        var st = VisionState()
        var rows : array<float>
        var n = encode_image_(emb, st, pic, "probe-warm", rows)   // warm: JIT + first touch
        print("warm: {n} rows\n")
        var best = 1.0e30lf
        for (r in range(REPS)) {
            set_asr_prof(true)
            asr_prof_reset()
            let t0 = ref_time_ticks()
            n = encode_image_(emb, st, pic, "probe", rows)
            let ms = double(get_time_usec(t0)) / 1000.0lf
            if (ms < best) {
                best = ms
            }
            print("rep {r}: wall {ms} ms ({n} rows)\n")
        }
        print("best wall: {best} ms\n")
        asr_prof_report()   // the LAST rep's buckets
        delete rows
        delete st
    }
}
