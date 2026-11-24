// The audio carrier's cells that run under test_audio.das's blocks: the gemma4a arm on the E2B mmproj's `image` form and
// its direct-image routes, the E4B width probe, the pre-encoded rows seam on a plain chat - the E2B Q4_K_M decoder's
// block nested in a form of the mmproj minted in memory - and the whisper-class tower arm on the ultravox and Qwen2.5-Omni
// mmprojs' `image_emb` forms.

options gen2
options _dasllama_internal = true

module _audio_embedder_cells shared public

require dastest/testing_boost public
require dasllama/dasllama_audio_embedder public
require dasllama/dasllama_image   // image_family_tag - the per-arm dlim selection
require dasllama/dasllama         // add_user_audio_rows: the carrier rows on a plain chat
require dasllama/dasllama_common public   // Model
require dasllama/dasllama_gemma4a // stage_ + mint_gemma4a_encoder: the box's own image, minted into scratch
require daslib/jobque_boost
require daslib/fio
require strings
require math   // sin, abs, is_finite: the tower arm's clips and rows
require _model_tier   // models_dir() + model_available() gate
require _model_form public   // ModelForm: the decoder form a chat cell runs on; own_load
require _scratch_dir  // scratch_path: the box's own image, minted for the direct route's control

let private E2B_MM = "mmproj-gemma-4-E2B-it-bf16.gguf"

// The gemma4a arm, on the E2B mmproj's `image` form - the carrier's own load: the probe and the carrier report the family
// and the width, a sub-frame clip pads to one soft token, and the image that load baked beside the mmproj routes on its
// baked tag.
[cold_path]
def test_audio_embedder_gemma4a_arm(t : T?; e : AudioEmbedder) {
    own_load("reloads the model: the image the form's load baked beside the mmproj, mapped by the direct route")
    let mm = path_join(models_dir(), E2B_MM)
    t |> equal(audio_probe_proj_dim(mm), 1536l, "the probe reports the E2B projection width")
    t |> equal(audio_family(e), "gemma4a", "the carrier reports the gemma4a family")
    t |> equal(audio_proj_dim(e), 1536l, "the carrier reports the same width")
    var st = AudioState()
    var samples : array<float>
    samples |> resize(320)
    var rows : array<float>
    t |> equal(encode_audio_(e, st, samples, rows), 1l, "a 20ms sub-frame clip pads to exactly one soft token - a non-empty clip can never encode to zero rows")
    delete rows
    delete samples
    delete st
    return if (!image_rail_on(t))
    var image_path = ""
    var foreign = ""
    var current <- dlim_current_identities()
    // the lane the form's load took: on a box whose GPU tower serves the lane, the device flavor's image
    let lane = tower_dev_tag(gemma4a_lane_tag(e.gemma4a.q8, e.gemma4a.gemm_f16),
        gemma4a_dev_flavor(e.gemma4a.q8))
    dir(models_dir()) $(name) {
        // the mmproj bakes one dlim PER ARM (gemma4a audio, gemma4v vision) — select by the
        // baked family tag, or directory order hands this audio cell a vision image. The
        // load above minted the gemma4a arm (the image rail is on), so the scan finds it
        if (empty(image_path) && starts_with(name, "{base_name(mm)}.") && ends_with(name, ".dlim")) {
            let cand = path_join(models_dir(), name)
            let tag = image_family_tag(cand)
            //! of the lanes only the one this box serves: the direct route reads under the lane the loader picked above
            if (tag == lane && cand == image_path_for(mm, tag)) {
                //! the lane's file name carries no box identity: a models directory another box shares holds that box's bake under it
                let verdict = dlim_image_verdict(image_peek(cand), current)
                if (verdict == "CURRENT") {
                    image_path = cand
                } else {
                    foreign = "{name} ({verdict})"
                }
            }
        }
    }
    delete current
    if (empty(image_path)) {
        t |> success(!empty(foreign), "the form's load with the image rail on baked a gemma4a-tagged {base_name(mm)}.*.dlim beside the mmproj")
        if (!empty(foreign)) {
            t |> skip("the gemma4a-tagged image beside the mmproj, {foreign}, was baked under another box's identity - the direct route maps only this box's")
        }
    } else {
        t |> equal(audio_probe_proj_dim(image_path), 0l, "the gguf probe answers 0 on a .dlim by pinned contract")
        var inscope ei <- load_audio_embedder(image_path)
        t |> equal(audio_family(ei), "gemma4a", "a prepared image routes on its baked family tag, not the probe")
        t |> equal(audio_proj_dim(ei), 1536l, "the image-loaded carrier reports the same width")
    }
}

// An E2B gemma4a image minted for this box into the scratch directory reads CURRENT and maps by the direct route.
[cold_path]
def embedder_scratch_image(t : T?) {
    own_load("reloads the model: an image minted into the scratch directory, loaded back by the direct route")
    let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
    if (!model_available(t, mm)) {
        return
    }
    var inscope st <- stage_gemma4a_encoder(mm)
    let tag = gemma4a_lane_tag(st.e.q8, st.e.gemm_f16)
    let img = scratch_path("{base_name(mm)}.{tag}.dlim")
    var inscope minted <- Gemma4aEncoder()
    mint_gemma4a_encoder(st, minted, img)
    t |> success(stat(img).is_valid, "the staged mint wrote {img}")
    var current <- dlim_current_identities()
    t |> equal(dlim_image_verdict(image_peek(img), current), "CURRENT", "an image baked on this box reads CURRENT, the verdict the scan above admits")
    delete current
    var inscope ei <- load_audio_embedder(img)
    t |> equal(audio_family(ei), "gemma4a", "the box's own image routes on its baked family tag")
    t |> equal(audio_proj_dim(ei), 1536l, "the box's own image reports the E2B width")
}

// The E4B mmproj probes its own decoder's width, read off the embedder's rows: a header read, no load.
[cold_path]
def embedder_e4b_probe(t : T?) {
    let mm = path_join(models_dir(), "mmproj-gemma-4-E4B-it-BF16.gguf")
    if (!model_available(t, mm)) {
        return
    }
    t |> equal(audio_probe_proj_dim(mm), 2560l, "the probe reports the E4B decoder width (E2B's is 1536, the encoder's 1024)")
}

// The pre-encoded audio rows on a PLAIN chat (no tower): the E2B family hears through this seam,
// and every refusal at it is a panic the caller must never reach by accident.
// The rows turn with thinking OFF under the chat sampler preset: the E-series opens a media turn
// with a channel marker even so, and the instruct-mode guard lets the reply through.
def private instruct_audio_turn(t : T?; m : Model; rows : array<float>; n : int64) {
    var chat <- create_chat(m, "", 96l)
    set_thinking(chat, false)
    set_seed(chat.session, 1234)
    var rows_d := rows
    add_user_audio_rows(m, chat, rows_d, n)
    add_user(chat, "What did you hear?")
    var pieces = 0l
    let reply = respond(m, chat, SamplingParams(temp = 0.7, top_k = 20l, top_p = 0.95)) $(_piece) {
        pieces++
        return true
    }
    t |> success(pieces > 0l && !empty(reply), "the instruct-mode audio turn answers: {length(reply)} chars over {pieces} pieces")
    delete chat
}

//! a cell of the rows seam: `cell` on the decoder's form `m` and the carrier `e`, as the subtest `name` under the
//! CPU-prefill intent, where the form is the CPU load asked alone (`form.alone`)
def audio_chat_cell(t : T?; name : string; var m : Model; form : ModelForm; var e : AudioEmbedder;
                    cell : function<(t : T?; var m : Model; e : AudioEmbedder) : void>) {
    return if (!form.alone)
    with_cpu_prefill(true) {
        tower_cell(t, name, m, e, cell)
    }
}

// Rows land, the turn answers, a second clip appends, and the refusals panic: on the E2B Q4_K_M decoder's CPU form (the
// planar rail, its context cut to 2048) and the mmproj's encoder minted in memory.
[cold_path]
def test_audio_rows_on_a_plain_chat(t : T?; var m : Model; e : AudioEmbedder) {
    let dim = m.config.dim
    with_dasllama_jobque() {
        var st = AudioState()
        var samples : array<float>
        samples |> resize(16000)   // one second of silence: real rows, a real span
        var rows : array<float>
        let n = encode_audio(e, st, samples, rows)
        t |> equal(n, 25l, "a second of audio is 25 soft tokens on this family (4 s of the JFK clip is 100)")
        // a second clip in the same turn appends its rows
        var chat <- create_chat(m, "", 8l)
        var rows2 : array<float>
        let n2 = encode_audio(e, st, samples, rows2)
        var rows_a := rows
        add_user_audio_rows(m, chat, rows_a, n)
        add_user_audio_rows(m, chat, rows2, n2)
        t |> equal(chat.n_audio_rows, n + n2, "two clips append")
        t |> equal(long_length(chat.audio_rows), (n + n2) * dim, "rows are dim-wide")
        // the length check
        var short : array<float>
        short |> resize(int(dim) - 1)
        var tripped = false
        try {
            add_user_audio_rows(m, chat, short, 1l)
        } recover {
            tripped = true
        }
        t |> success(tripped, "rows that are not n x dim floats panic")
        delete chat
        // the turn answers through the plain chat
        var chat2 <- create_chat(m, "", 8l)
        var rows_b := rows
        add_user_audio_rows(m, chat2, rows_b, n)
        add_user(chat2, "What did you hear?")
        var head, tail : array<int64>
        render_turn_audio(m, chat2, head, tail)   // render-only: the spans the prefill will carry around the rows
        var pieces = 0l
        respond(m, chat2, SamplingParams()) $(_piece) {
            pieces++
            return true
        }
        t |> equal(stats(chat2.session).n_gen, pieces, "every generated token reached the block")
        t |> equal(stats(chat2.session).n_prompt, long_length(head) + n + long_length(tail), "the prefill is head + rows + tail")
        t |> equal(chat2.n_audio_rows, 0l, "the rows are consumed by the turn")
        delete chat2
        instruct_audio_turn(t, m, rows, n)
        // one media kind per turn: an image already queued refuses the audio rows
        var chat3 <- create_chat(m, "", 8l)
        var img : array<float>
        img |> resize(dim * (1l + m.config.n_deepstack))
        add_user_image_rows(m, chat3, img, 1l, int2(0))
        var rows_c := rows
        var mixed = false
        try {
            add_user_audio_rows(m, chat3, rows_c, n)
        } recover {
            mixed = true
        }
        t |> success(mixed, "audio rows beside a queued image panic instead of dropping either")
        delete chat3
        delete rows
        delete samples
        delete st
    }
}

// A whisper-class mmproj's tower arm, on the carrier its block's `image_emb` form loaded: the probed width and family, a
// chunk's finite rows, a reused state bit for bit, another clip's rows apart, a 35 s clip two chunks, a flat heap.
[cold_path]
def private tower_arm_cell(t : T?; e : AudioEmbedder; file, family : string; width, rows_per_chunk : int64) {
    let mm = path_join(models_dir(), file)
    t |> equal(audio_probe_proj_dim(mm), width, "the probe reports the projector's output width off its tensors")
    t |> success(e.kind == AudioKind.audio_tower, "the carrier holds the whisper-class tower")
    t |> equal(audio_family(e), family, "the carrier reports the projector kind")
    t |> equal(audio_proj_dim(e), width, "the carrier reports the probed width")
    var st = AudioState()
    var samples : array<float>
    samples |> resize(16000 * 5)
    for (i, s in count(), samples) {
        s = 0.3 * sin(float(i) * 0.05)
    }
    var rows : array<float>
    t |> equal(encode_audio_(e, st, samples, rows), rows_per_chunk, "a 5 s clip is one mel chunk")
    t |> equal(long_length(rows), rows_per_chunk * width, "one row of the decoder's width a soft token")
    var peak = 0.
    var finite = true
    for (v in rows) {
        finite &&= is_finite(v)
        peak = max(peak, abs(v))
    }
    t |> success(finite && peak > 0., "the rows are finite and not all zero (peak {peak})")
    var again : array<float>
    t |> equal(encode_audio_(e, st, samples, again), rows_per_chunk, "the reused state encodes the clip again")
    var moved = 0
    for (a, b in rows, again) {
        moved += a != b ? 1 : 0
    }
    t |> equal(moved, 0, "the reused state reads the same rows bit for bit")
    for (i, s in count(), samples) {
        s = 0.3 * sin(float(i) * 0.11)
    }
    delete again
    encode_audio_(e, st, samples, again)
    moved = 0
    for (a, b in rows, again) {
        moved += a != b ? 1 : 0
    }
    t |> success(moved > length(rows) / 2, "another clip reads other rows ({moved} of {length(rows)} differ) - the control")
    samples |> resize(16000 * 35)
    delete again
    t |> equal(encode_audio_(e, st, samples, again), 2l * rows_per_chunk, "a 35 s clip is two chunks, their rows appended")
    let h0 = heap_bytes_allocated()
    for (_i in range(2)) {
        delete again
        encode_audio_(e, st, samples, again)
    }
    let h1 = heap_bytes_allocated()
    t |> success(h1 <= h0 + 262144ul, "two more encodes leave the heap where it stood ({h0} -> {h1} bytes) - an encode frees its mel and frames")
    delete again
    delete rows
    delete samples
    delete st
}

//! the ultravox v0_5 f16 mmproj's tower arm (`tower_arm_cell`)
[cold_path]
def embedder_ultravox_arm(t : T?; e : AudioEmbedder) {
    with_dasllama_jobque() {
        tower_arm_cell(t, e, "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", "ultravox", 2048l, 187l)
    }
}

//! the Qwen2.5-Omni 3B f16 mmproj's tower arm (`tower_arm_cell`): its audio side beside the vision tensors
[cold_path]
def embedder_omni_arm(t : T?; e : AudioEmbedder) {
    with_dasllama_jobque() {
        tower_arm_cell(t, e, "mmproj-Qwen2.5-Omni-3B-f16.gguf", "qwen2a", 2048l, 750l)
    }
}
