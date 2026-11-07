options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's audio seats: under the CUDA ask, each audio family's hooks and flavor come from the home its row of
//! `CUDA_AUDIO_HOMES` names - this home's own for every family on the rail; a family not on it would read the Vulkan home's -
//! so a family's home is its one row (`ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs`).
module dasllama_cuda_tower_audio shared public

require dasllama/dasllama_lint public
require daslib/enum_trait
require dasllama/dasllama_tower   // TowerDev
require dasllama/dasllama_gpu_rail   // TowerAudioFamily
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, register_gpu_home
require ?vulkan dasllama/dasllama_vulkan_tower   // vulkan_tower_install_audio_of: a family the Vulkan home still seats
require ?vulkan dasllama/dasllama_vulkan_asr_dec   // vulkan_asr_dec_install_of: ... its decoder seats
require dasllama/dasllama_cuda_tower_whisper   // the whisper class's seats on this home
require dasllama/dasllama_cuda_asr_dec   // the whisper decoder's seats on this home
require dasllama/dasllama_cuda_tower_g4a   // gemma4a's, qwen3a's own, the VAD's seats and the device mel on this home
require dasllama/dasllama_cuda_tower_fc   // canary's, parakeet's and GigaAM's seats on this home
require dasllama/dasllama_cuda_asr_dec_fc   // the transducer's and GigaAM's CTC head's seats on this home

//! the home whose seats serve each audio family under the CUDA ask: `cuda` for every family on the rail
let CUDA_AUDIO_HOMES <- {
    TowerAudioFamily.whisper => TowerDev.cuda,
    TowerAudioFamily.qwen3a => TowerDev.cuda,
    TowerAudioFamily.gemma4a => TowerDev.cuda,
    TowerAudioFamily.canary => TowerDev.cuda,
    TowerAudioFamily.parakeet => TowerDev.cuda,
    TowerAudioFamily.gigaam => TowerDev.cuda,
    TowerAudioFamily.transducer => TowerDev.cuda,
    TowerAudioFamily.vad => TowerDev.cuda
}

//! the home whose seats serve audio family `fam` under the CUDA ask
def cuda_audio_home(fam : TowerAudioFamily) : TowerDev => CUDA_AUDIO_HOMES?[fam] ?? TowerDev.vulkan

//! family `fam`'s seats on this home: a family's CUDA seats land as its arm here with its row's flip
def private cuda_tower_install_audio_family(fam : TowerAudioFamily) {
    if (fam == TowerAudioFamily.whisper) {
        cuda_whisper_install_tower()
        cuda_whisper_install_decoder()
        cuda_device_mel_install()
    } elif (fam == TowerAudioFamily.gemma4a) {
        cuda_gemma4a_install()
    } elif (fam == TowerAudioFamily.qwen3a) {
        cuda_qwen3a_install()
    } elif (fam == TowerAudioFamily.vad) {
        cuda_vad_install()
    } elif (fam == TowerAudioFamily.canary || fam == TowerAudioFamily.parakeet || fam == TowerAudioFamily.gigaam || fam == TowerAudioFamily.transducer) {
        cuda_tower_install_fc(fam)
        cuda_asr_dec_install_fc(fam)
    } else {
        panic("dasLLAMA cuda tower: CUDA_AUDIO_HOMES moves {fam} to the CUDA home, which seats no {fam} chain yet")
    }
}

//! the audio seats under the CUDA ask: every family's from the home its row of `CUDA_AUDIO_HOMES` names
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_tower_install_audio {
    for (fam in each(type<TowerAudioFamily>)) {
        if (cuda_audio_home(fam) == TowerDev.cuda) {
            cuda_tower_install_audio_family(fam)
        } else {
            static_if (typeinfo builtin_module_exists(vulkan) && !typeinfo builtin_module_exists(das_metal)) {
                vulkan_tower_install_audio_of(fam)
                vulkan_asr_dec_install_of(fam)
            }
        }
    }
}

[init]
def dasllama_cuda_tower_audio_register {
    register_gpu_home("cuda", @@cuda_tower_install_audio)
    return if (!gpu_want_cuda())
    cuda_tower_install_audio()
}
