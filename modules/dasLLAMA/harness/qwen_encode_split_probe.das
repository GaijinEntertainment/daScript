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

// Splits one vision encode per FAMILY: wall, the preproc half (letterbox + normalize, timed
// standalone), and the asr_prof buckets (stem/tail where the family carries them).
// Direction-grade attribution for das-vs-clip encode gaps; best-of over REPS walls.

let REPS = 5

def probe_one(mmproj, picture : string) {
    print("===== {mmproj} =====\n")
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
    delete pic
}

[export]
def main {
    let pic = "/Users/borisbatkin/Work/llama.cpp/models/coco-val2017-000000039769.jpg"
    with_job_que() {
        setup_dasllama_jobque_()
        probe_one("/Users/borisbatkin/Work/llama.cpp/models/mmproj-Qwen3VL-8B-Instruct-F16.gguf", pic)
        probe_one("/Users/borisbatkin/Work/llama.cpp/models/mmproj-gemma-3-4b-it-F16.gguf", pic)
        probe_one("/Users/borisbatkin/Work/llama.cpp/models/mmproj-Qwen2.5-Omni-3B-f16.gguf", pic)
    }
}
