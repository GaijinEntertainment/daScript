options gen2
options indenting = 4

// Author best-guess .sfx drum sounds (sfxr/modal layers approximating the Stolen
// soundfont references), seeded from what the spectrum/peaks of each reference show.
// Run: daslang examples/daStrudel/sfx_lab/gen_drum_guess.das
// Then open the .sfx in the lab and tweak by ear against the reference overlay + fit %.

require strudel/strudel_sfx
require strudel/strudel_modal
require daslib/fio

def lab_dir : string {
    return "{get_das_root()}/examples/daStrudel/sfx_lab"
}

def save_guess(name : string; doc : Sound) {
    let path = "{lab_dir()}/sounds/{name}.sfx"
    if (save_sound(path, doc)) {
        to_log(LOG_INFO, "wrote {path}\n")
    } else {
        to_log(LOG_ERROR, "failed to write {path}\n")
    }
}

// best-guess kick: a modal body at the measured ~86 Hz fundamental (1 mode, dark,
// short ~150 ms ring) + a light transient for the beater click. No high content.
def guess_kick : Sound {
    var doc = Sound()
    doc.mimic_target = "{lab_dir()}/drum_refs/bd.wav"
    doc.layers |> push(modal_layer("kick body", ModalVoice(
        pitch = 86.0, spread = 1.1, n_modes = 1, ring = 0.18,
        brightness = 0.2, transient = 0.15, duration = 0.4)))
    return <- doc
}

[export]
def main {
    mkdir("{lab_dir()}/sounds")
    save_guess("kick", guess_kick())
}
