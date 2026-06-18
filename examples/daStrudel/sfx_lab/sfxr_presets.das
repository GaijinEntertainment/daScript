// sfxr_presets — render and audition the classic sfxr preset family.
// Renders each generator to a WAV under ./sfxr_renders/ and plays them in sequence.
// Run: daslang examples/daStrudel/sfx_lab/sfxr_presets.das
options gen2
options indenting = 4

require strudel/strudel_sfxr
require audio/audio_boost
require audio/audio_wav
require daslib/fio

def build_preset(idx : int; var seed : uint&) : SfxrSound {
    if (idx == 0) return sfxr_pickup_coin(seed)
    if (idx == 1) return sfxr_laser_shoot(seed)
    if (idx == 2) return sfxr_explosion(seed)
    if (idx == 3) return sfxr_powerup(seed)
    if (idx == 4) return sfxr_hit_hurt(seed)
    if (idx == 5) return sfxr_jump(seed)
    return sfxr_blip_select(seed)
}

[export]
def main() {
    let names = fixed_array("pickup_coin", "laser_shoot", "explosion", "powerup", "hit_hurt", "jump", "blip_select")
    let seeds = fixed_array(12345u, 22222u, 33333u, 44444u, 55555u, 66666u, 77777u)
    let outdir = "sfxr_renders"
    mkdir_rec(outdir)

    print("sfxr presets -> ./{outdir}/ (playing each as it renders)\n")
    with_audio_system() {
        for (idx in range(length(names))) {
            var seed = seeds[idx]
            let snd = build_preset(idx, seed)
            var buf <- render_sfxr(snd)
            let secs = float(length(buf)) / float(SFXR_SAMPLE_RATE)
            let path = "{outdir}/{names[idx]}.wav"
            write_wav(path, buf, uint(SFXR_SAMPLE_RATE), 1)
            print("  {names[idx]}  ({secs}s)  -> {path}\n")
            play_sound_from_pcm(SFXR_SAMPLE_RATE, 1, buf)
            sleep(uint((secs + 0.4) * 1000.0))
        }
    }
    print("done.\n")
}
