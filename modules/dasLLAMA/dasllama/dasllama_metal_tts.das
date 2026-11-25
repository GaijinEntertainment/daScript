options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_tts shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_tower   // TowerDev
require dasllama/dasllama_kqformat   // KqFmt
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks   // SineSourceCfg, TtsNoise
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket
require dasllama/dasllama_gpu_tier   // the decline counter
require dasllama/dasllama_gpu_rail   // TowerDecline
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_tower   // the tower's knob and arm
require dasllama/dasllama_tower_tts

//! The Metal home's TTS seats: the shared TTS driver's bodies (`dasllama_tower_tts`) over this home's rail, registered as
//! the kitten and kokoro families' synthesis seats and the Pocket family's seats, as the discrete homes register theirs
//! (`ARCHITECTURE_GPU_TOWER_TTS.md`).

var private g_mts_rail : MtRail
let private MTS_WORDS_KNOB = "DASLLAMA_METAL_TOWER=0 keeps every tower seat on the CPU chain"
let private MTS_WORDS_DEVICE = "the Metal device did not come up"
let private MTS_WORDS_MEMORY = "the device had no room for the seat's buffers"

[cold_path]
def private metal_tts_words(why : TowerDecline) : string => tower_decline_words(why, "Metal", MTS_WORDS_KNOB, MTS_WORDS_DEVICE, MTS_WORDS_MEMORY)

[unused_argument(fmt, d, rows)]
def private metal_tts_cm2(fmt : KqFmt; d, rows : int64) : bool => false

def private metal_tts_arms : bool => metal_tower_init()

[hot_path]
def private mts_styletts2_albert(a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool
    => tts_st2_albert(g_mts_rail, a, ids, out, sc)

[hot_path]
def private mts_styletts2_text(te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool
    => tts_st2_text(g_mts_rail, te, ids, out, sc)

[hot_path]
def private mts_styletts2_dur_enc(p : St2Predictor; bert : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>;
                                  var sc : St2Scratch) : bool
    => tts_st2_dur_enc(g_mts_rail, p, bert, t, s, out, sc)

[hot_path]
def private mts_styletts2_durations(p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var raw : array<float>;
                                    var sc : St2Scratch) : bool
    => tts_st2_durations(g_mts_rail, p, d, t, speed, raw, sc)

[hot_path]
def private mts_styletts2_prosody(p : St2Predictor; en : array<float>; cc, frames : int64; s : array<float>; @scratch @exact_size var f0 : array<float>;
                                  @scratch @exact_size var n : array<float>; var f2 : int64&; var sc : St2Scratch) : bool
    => tts_st2_prosody(g_mts_rail, p, en, cc, frames, s, f0, n, f2, sc)

[hot_path]
def private mts_styletts2_decode(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64; f0, nrg : array<float>;
                                 f2 : int64; s_ref : array<float>; noise : TtsNoise; seed : uint64; @scratch @exact_size var wave : array<float>;
                                 var sc : St2Scratch) : bool
    => tts_st2_decode(g_mts_rail, d, text_c, source, asr, frames, f0, nrg, f2, s_ref, noise, seed, wave, sc)

[hot_path]
def private mts_pocket_codec(m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) : bool
    => tts_pk_codec(g_mts_rail, m, latents, frames, sc, wave)

[hot_path]
def private mts_pocket_frames(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; std : float; frames_after_eos : int;
                              captured, forced : array<float>; max_frames : int64; var sc : PocketScratch) : tuple<frames : int64; eos_at : int64>
    => tts_pk_frames(g_mts_rail, m, vs, n_txt, std, frames_after_eos, captured, forced, max_frames, sc)

[hot_path]
def private mts_pocket_prompt(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; var sc : PocketScratch) : bool
    => tts_pk_prompt(g_mts_rail, m, vs, n_txt, sc)

[cold_path]
def private mts_pocket_encode(m : PocketModel; pcm : array<float>; var sc : PocketScratch; @scratch @exact_size var latents : array<float>) : int64
    => tts_pk_encode(g_mts_rail, m, pcm, sc, latents)

[cold_path, unused_argument(sc)]
def private mts_pocket_voice(m : PocketModel; lat : array<float>; frames : int64; var vs : PocketVoiceState; var sc : PocketScratch) : bool
    => tts_pk_voice(g_mts_rail, m, lat, frames, vs)

[cold_path]
def private metal_tts_forget {
    tts_forget(g_mts_rail)
}

[cold_path]
def private metal_tts_release {
    tts_release_activations(g_mts_rail)
}

[cold_path]
def private metal_tts_held : tuple<weights : int64; activations : int64> => tts_held(g_mts_rail)

[unused_argument(kq), arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private mts_styletts2_serves(kq : bool) : bool => metal_tower_knob() && metal_tower_init() && tts_st2_ready(g_mts_rail)

[unused_argument(native), arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private mts_pocket_serves(native : bool) : bool => metal_tower_knob() && metal_tower_init() && tts_pk_ready(g_mts_rail)

def private mts_styletts2_bake(m : StyleTts2Model; var b : TowerDevBuild) : bool => tts_st2_bake(m, b, TowerDev.metal)

def private mts_pocket_bake(m : PocketModel; var b : TowerDevBuild) : bool => tts_pk_bake(m, b, TowerDev.metal)

def private metal_tts_install {
    register_styletts2_flavor(TowerDev.metal, @@mts_styletts2_serves, @@mts_styletts2_bake)
    set_styletts2_block_layout(tts_st2_block_layout(TowerDev.metal))
    set_pocket_block_layout(tts_pk_block_layout(TowerDev.metal))
    register_pocket_flavor(TowerDev.metal, @@mts_pocket_serves, @@mts_pocket_bake)
    register_styletts2_gpu(St2GpuDriver(albert = @@mts_styletts2_albert, text = @@mts_styletts2_text, dur_enc = @@mts_styletts2_dur_enc,
        durations = @@mts_styletts2_durations, prosody = @@mts_styletts2_prosody, decode = @@mts_styletts2_decode))
    register_pocket_gpu(PocketGpuDriver(encode = @@mts_pocket_encode, voice = @@mts_pocket_voice, codec = @@mts_pocket_codec, frames = @@mts_pocket_frames, prompt = @@mts_pocket_prompt))
}

[init, arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def dasllama_metal_tts_register {
    tts_home_register(TowerDev.metal, TtsHome(name = "metal", on = @@metal_tower_knob, arms = @@metal_tts_arms, words = @@metal_tts_words,
        forget = @@metal_tts_forget, cm2 = @@metal_tts_cm2,
        release = @@metal_tts_release, held = @@metal_tts_held))
    metal_tts_install()
    register_reload_prep(@@metal_tts_forget)
}

[finalize, arch(at="../ARCHITECTURE_GPU_METAL_LIFECYCLE.md#metal-family-lifecycle")]
def private metal_tts_context_shutdown {
    metal_tts_forget()
}
