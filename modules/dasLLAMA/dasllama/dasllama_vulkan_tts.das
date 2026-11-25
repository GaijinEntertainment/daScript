options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tts shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_tower   // TowerDev
require dasllama/dasllama_common   // register_weights_epoch_listener
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks   // SineSourceCfg, TtsNoise
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, register_gpu_home, the decline counter's note
require dasllama/dasllama_gpu_rail   // TowerDecline
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_tower   // the tower's knob and arm
require dasllama/dasllama_tower_tts

//! The Vulkan home's TTS seats: the TTS driver's bodies (`dasllama_tower_tts`) over this home's rail, registered
//! as the kitten and kokoro families' synthesis seats and the Pocket family's seats
//! (`ARCHITECTURE_GPU_TOWER_TTS.md`).

var private g_vts_rail : VtRail
[cold_path]
def private vulkan_tts_words(why : TowerDecline) : string
    => tower_decline_words(why, "Vulkan", VK_DECLINE_WORDS_TOWER_KNOB, VK_DECLINE_WORDS_DEVICE,
        "{VK_DECLINE_WORDS_MEMORY}, or a buffer would bind past the device's storage range")

var private g_vts_cm2_said : table<string>   //! the cm2 fallback's reasons said, cleared with the model

//! the tile's column is held to 128: the 256 column rounds a record's rows up, and a seat's rows are the call's own
def private vulkan_tts_cm2(fmt : KqFmt; d, rows : int64) : bool {
    if (!vt_cm2_feed()) {
        gpu_said_once(g_vts_cm2_said, LOG_INFO, "the TTS seats' stored conv leaves take the exact tile - ", "the device serves no cooperative-matrix feed")
        return false
    }
    var t = vt_tile(true, d, rows, int(fmt))
    t.tc = min(t.tc, 128l)
    g_vts_rail.sites[int(RailSite.conv2)] = t
    return true if (vt_tile_ensure(t))
    gpu_said_once(g_vts_cm2_said, LOG_INFO, "the TTS seats' stored conv leaves take the exact tile - ", "the {fmt} tile's class did not build")
    return false
}

[hot_path]
def private vulkan_styletts2_albert(a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool
    => tts_st2_albert(g_vts_rail, a, ids, out, sc)

[hot_path]
def private vulkan_styletts2_text(te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool
    => tts_st2_text(g_vts_rail, te, ids, out, sc)

[hot_path]
def private vulkan_styletts2_dur_enc(p : St2Predictor; bert : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>;
                                     var sc : St2Scratch) : bool
    => tts_st2_dur_enc(g_vts_rail, p, bert, t, s, out, sc)

[hot_path]
def private vulkan_styletts2_durations(p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var raw : array<float>;
                                       var sc : St2Scratch) : bool
    => tts_st2_durations(g_vts_rail, p, d, t, speed, raw, sc)

[hot_path]
def private vulkan_styletts2_prosody(p : St2Predictor; en : array<float>; cc, frames : int64; s : array<float>; @scratch @exact_size var f0 : array<float>;
                                     @scratch @exact_size var n : array<float>; var f2 : int64&; var sc : St2Scratch) : bool
    => tts_st2_prosody(g_vts_rail, p, en, cc, frames, s, f0, n, f2, sc)

[hot_path]
def private vulkan_styletts2_decode(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64; f0, nrg : array<float>;
                                    f2 : int64; s_ref : array<float>; noise : TtsNoise; seed : uint64; @scratch @exact_size var wave : array<float>;
                                    var sc : St2Scratch) : bool
    => tts_st2_decode(g_vts_rail, d, text_c, source, asr, frames, f0, nrg, f2, s_ref, noise, seed, wave, sc)

[hot_path]
def private vulkan_pocket_codec(m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) : bool
    => tts_pk_codec(g_vts_rail, m, latents, frames, sc, wave)

[hot_path]
def private vulkan_pocket_frames(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; std : float; frames_after_eos : int;
                                 captured, forced : array<float>; max_frames : int64; var sc : PocketScratch) : tuple<frames : int64; eos_at : int64>
    => tts_pk_frames(g_vts_rail, m, vs, n_txt, std, frames_after_eos, captured, forced, max_frames, sc)

[hot_path]
def private vulkan_pocket_prompt(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; var sc : PocketScratch) : bool
    => tts_pk_prompt(g_vts_rail, m, vs, n_txt, sc)

[cold_path]
def private vulkan_pocket_encode(m : PocketModel; pcm : array<float>; var sc : PocketScratch; @scratch @exact_size var latents : array<float>) : int64
    => tts_pk_encode(g_vts_rail, m, pcm, sc, latents)

[cold_path, unused_argument(sc)]
def private vulkan_pocket_voice(m : PocketModel; lat : array<float>; frames : int64; var vs : PocketVoiceState; var sc : PocketScratch) : bool
    => tts_pk_voice(g_vts_rail, m, lat, frames, vs)

//! the model-drop hook and the weights-epoch listener: every slab, scratch and voice slot is released here, ahead of the sweep, and rebuilt on the next call
[cold_path]
def private vulkan_tts_forget {
    tts_forget(g_vts_rail)
    clear(g_vts_cm2_said)
}

[cold_path]
def private vulkan_tts_release {
    tts_release_activations(g_vts_rail)
}

[cold_path]
def private vulkan_tts_held : tuple<weights : int64; activations : int64> => tts_held(g_vts_rail)

[unused_argument(kq), arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private vulkan_styletts2_serves(kq : bool) : bool => vulkan_tower_on() && vulkan_tower_arms() && tts_st2_ready(g_vts_rail)

[unused_argument(native), arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private vulkan_pocket_serves(native : bool) : bool => vulkan_tower_on() && vulkan_tower_arms() && tts_pk_ready(g_vts_rail)

def private vulkan_styletts2_bake(m : StyleTts2Model; var b : TowerDevBuild) : bool => tts_st2_bake(m, b, TowerDev.vulkan)

def private vulkan_pocket_bake(m : PocketModel; var b : TowerDevBuild) : bool => tts_pk_bake(m, b, TowerDev.vulkan)

def private vulkan_tts_install {
    register_styletts2_flavor(TowerDev.vulkan, @@vulkan_styletts2_serves, @@vulkan_styletts2_bake)
    set_styletts2_block_layout(tts_st2_block_layout(TowerDev.vulkan))
    set_pocket_block_layout(tts_pk_block_layout(TowerDev.vulkan))
    register_pocket_flavor(TowerDev.vulkan, @@vulkan_pocket_serves, @@vulkan_pocket_bake)
    register_styletts2_gpu(St2GpuDriver(albert = @@vulkan_styletts2_albert, text = @@vulkan_styletts2_text, dur_enc = @@vulkan_styletts2_dur_enc,
        durations = @@vulkan_styletts2_durations, prosody = @@vulkan_styletts2_prosody, decode = @@vulkan_styletts2_decode))
    register_pocket_gpu(PocketGpuDriver(encode = @@vulkan_pocket_encode, voice = @@vulkan_pocket_voice, codec = @@vulkan_pocket_codec, frames = @@vulkan_pocket_frames, prompt = @@vulkan_pocket_prompt))
}

[init, arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def dasllama_vulkan_tts_register {
    tts_home_register(TowerDev.vulkan, TtsHome(on = @@vulkan_tower_on, arms = @@vulkan_tower_arms, words = @@vulkan_tts_words,
        forget = @@vulkan_tts_forget, cm2 = @@vulkan_tts_cm2,
        release = @@vulkan_tts_release, held = @@vulkan_tts_held))
    register_vk_drop_hook(@@vulkan_tts_forget)
    register_weights_epoch_listener(@@vulkan_tts_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gpu_home("vulkan", @@vulkan_tts_install)
        if (!gpu_want_cuda()) {
            vulkan_tts_install()
        }
    }
}
