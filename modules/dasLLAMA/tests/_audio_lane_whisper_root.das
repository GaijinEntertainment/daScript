options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_transformer   // the umbrella's [init] registrations: the GPU home's tower and decoder seats
require dasllama/dasllama_asr
require dasllama/dasllama_audio_io
require dasllama/dasllama_tower_image   // tower_dev_halfword: a discrete home's exact encoder is the f16 lane
require dasllama/dasllama_math   // with_dasllama_jobque_

// The child test_whisper.das spawns under each DASLLAMA_AUDIO_LANE value (the knob loads once at context init): one facade
// load of the whisper bin it is handed (`-- <bin> <clip>`) on the run's GPU home, its encoder lane on one `WLANE` record,
// then the clip's transcript on a `TEXT` record.

[export, cold_path]
def main {
    let cmd <- get_command_line_arguments()
    let n = length(cmd)
    if (n < 2) {
        print("WLANE_SKIP no model\n")
        return
    }
    let samples <- load_audio_16k_mono(cmd[n - 1])
    set_asr_fp32(false)
    set_asr_tower_fp32(false)
    with_dasllama_jobque_() {
        var inscope m <- load_asr_model(cmd[n - 2])
        let dev = m.whisper.enc.dev_backend
        print("WLANE q8={m.whisper.enc.q8} f16={m.whisper.enc.gemm_f16} halfword={tower_dev_halfword(dev)} dec_q8={m.whisper.dq8} dev={dev}\n")
        var inscope s <- create_session(m)
        print("TEXT {transcribe(m, s, samples)}\n")
    }
    print("ROOT_DONE\n")
}
