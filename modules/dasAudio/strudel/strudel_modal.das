options gen2
options indenting = 4

module strudel_modal shared public
//! Modal impact synthesizer — the "struck hard object" voice (clicks, knocks, bells, hits).
//! A modal voice is a small bank of inharmonic resonant modes (decaying sines), kicked at t=0
//! by a sharp broadband contact tick. Higher modes decay faster (frequency-dependent damping),
//! which is what makes an impact read as "hard" rather than "tonal". Driven by usable macros
//! (pitch / spread / ring / brightness / transient / echo) rather than raw mode tables.
//! Output is mono `array<float>` in [-1,1] at `MODAL_SAMPLE_RATE` (44100). Pairs with strudel_sfxr.

require math

let MODAL_SAMPLE_RATE = 44100
let private TWO_PI = 6.28318530717958647692

struct ModalVoice {
    //! Macro parameters for a struck-object voice. Defaults make a mid "knock".
    pitch : float = 1100.0          //!< frequency of the lowest mode, Hz
    spread : float = 1.7            //!< geometric spacing between modes (inharmonicity); 1 = unison
    n_modes : int = 4               //!< number of resonant modes (1..8)
    ring : float = 0.06             //!< ring time of the lowest mode, seconds (bigger = longer)
    brightness : float = 0.6        //!< relative loudness of higher modes (0 = only fundamental, 1 = flat)
    damp_skew : float = 0.7         //!< how much faster high modes decay than low (0 = uniform)
    transient : float = 0.5         //!< contact tick amount (broadband click at onset)
    transient_rate : float = 600.0  //!< tick decay rate (bigger = shorter click)
    echo_amount : float = 0.0       //!< feedback echo / room tail (0 = dry)
    echo_delay : float = 0.04       //!< echo tap spacing, seconds
    duration : float = 0.3          //!< total rendered length, seconds
}

def private noise(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return float((seed >> 9u) & 0x7FFFFFu) / 4194303.0 - 1.0
}

def render_modal_voice(v : ModalVoice) : array<float> {
    //! Render a modal impact to a mono buffer in [-1,1]. Deterministic for given params.
    let n = int(v.duration * float(MODAL_SAMPLE_RATE))
    var buf : array<float>
    if (n <= 0) return <- buf
    buf |> resize(n)

    let nm = clamp(v.n_modes, 1, 8)
    // precompute per-mode frequency / decay rate / gain
    var mf : array<float>
    var mr : array<float>
    var mg : array<float>
    mf |> reserve(nm)
    mr |> reserve(nm)
    mg |> reserve(nm)
    let base_rate = 1.0 / max(v.ring, 0.001)
    for (m in range(nm)) {
        mf |> push(v.pitch * pow(v.spread, float(m)))
        mr |> push(base_rate * (1.0 + float(m) * v.damp_skew))
        mg |> push(pow(v.brightness, float(m)))
    }

    let iSr = 1.0 / float(MODAL_SAMPLE_RATE)
    var seed = 0x51234u
    for (i in range(n)) {
        let t = float(i) * iSr
        var s = 0.0
        for (m in range(nm)) {
            s += mg[m] * exp(-mr[m] * t) * sin(TWO_PI * mf[m] * t)
        }
        s += noise(seed) * v.transient * exp(-v.transient_rate * t)
        buf[i] = s
    }
    delete mf
    delete mr
    delete mg

    // feedback echo / short room tail
    if (v.echo_amount > 0.0) {
        let d = max(int(v.echo_delay * float(MODAL_SAMPLE_RATE)), 1)
        let fb = min(v.echo_amount * 0.7, 0.85)
        for (i in range(d, n)) {
            buf[i] += buf[i - d] * fb
        }
    }

    // normalize to -1 dBFS
    var pk = 0.0
    for (x in buf) {
        pk = max(pk, abs(x))
    }
    if (pk > 0.0) {
        let g = 0.89 / pk
        for (x in buf) {
            x *= g
        }
    }
    return <- buf
}
