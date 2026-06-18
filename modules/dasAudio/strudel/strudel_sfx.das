options gen2
options indenting = 4

module strudel_sfx shared public
//! Multi-layer sound model — a sound is a stack of voices (sfxr or modal), each with its own
//! gain + onset delay, summed. This is the "several waves with delays" model: e.g. a bouncing
//! ball = one click layer duplicated with increasing delay + falling gain. Voices come from
//! strudel_sfxr / strudel_modal; render_sound mixes a layer stack into one mono buffer.

require strudel/strudel_sfxr public
require strudel/strudel_modal public
require math

let SFX_SAMPLE_RATE = 44100

variant VoiceParams {
    sfxr : SfxrSound
    modal : ModalVoice
}

struct Layer {
    //! One voice in a sound. gain/delay/enabled drive the mix; locked/link_source are authoring
    //! metadata (a linked layer mirrors its source's voice params, keeping its own gain/delay).
    name : string = "layer"
    voice : VoiceParams = VoiceParams(sfxr = SfxrSound())
    gain : float = 1.0
    delay : float = 0.0          //!< onset delay in seconds
    enabled : bool = true        //!< mute when false
    locked : bool = false        //!< UI: collapsed + read-only
    link_source : int = -1       //!< index of the layer this mirrors; -1 = independent
}

def sfxr_layer(name : string; s : SfxrSound) : Layer {
    //! Make an sfxr voice layer.
    return Layer(name = name, voice = VoiceParams(sfxr = s))
}

def modal_layer(name : string; v : ModalVoice) : Layer {
    //! Make a modal voice layer.
    return Layer(name = name, voice = VoiceParams(modal = v))
}

def render_voice(v : VoiceParams) : array<float> {
    //! Render a single voice to a mono buffer.
    if (v is sfxr) {
        return <- render_sfxr(v as sfxr)
    }
    return <- render_modal_voice(v as modal)
}

// Copy each linked layer's voice params from its source (keeps own gain/delay/name/enabled).
def propagate_links(var layers : array<Layer>) {
    for (i in range(length(layers))) {
        let src = layers[i].link_source
        if (src >= 0 && src < length(layers) && src != i) {
            layers[i].voice = layers[src].voice
        }
    }
}

def render_sound(layers : array<Layer>) : array<float> {
    //! Mix a layer stack into one mono buffer: each enabled voice, offset by its delay and scaled
    //! by its gain, summed; the result is peak-normalized to -1 dBFS.
    var out : array<float>
    for (lyr in layers) {
        if (!lyr.enabled) {
            continue
        }
        var buf <- render_voice(lyr.voice)
        let off = max(int(lyr.delay * float(SFX_SAMPLE_RATE)), 0)
        let need = off + length(buf)
        if (length(out) < need) {
            out |> resize(need)
        }
        for (i in range(length(buf))) {
            out[off + i] += buf[i] * lyr.gain
        }
        delete buf
    }
    var pk = 0.0
    for (x in out) {
        pk = max(pk, abs(x))
    }
    if (pk > 0.0) {
        let g = 0.89 / pk
        for (x in out) {
            x *= g
        }
    }
    return <- out
}
