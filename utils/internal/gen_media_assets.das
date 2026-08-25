options gen2
options indenting = 4
options persistent_heap

// Regenerates the first-party procedural assets (all public domain):
//   examples/media/audio/gong.wav   - synthesized gong (inharmonic modes)
//   examples/media/audio/music.wav  - synthesized ambient chord loop
//   examples/opengl/orb.obj         - procedural bumpy orb mesh
//   examples/opengl/image.png       - procedural swirl texture
// Deterministic: no wall-clock, no RNG - same bytes every run.
// Run from the repo root: bin/<cfg>/daslang (MSVC) or build/daslang (Make/Ninja)
//   -project_root . utils/internal/gen_media_assets.das

require math
require daslib/fio
require audio/audio_wav
require stbimage/stbimage_boost

let SR = 44100
let STRIKE_SEC = 0.025

def deterministic_noise(i : int) : float {
    let h = uint(i) * 2654435761u
    return float(h & 0xffffu) / 65535.0 - 0.5
}

def synth_gong(var samples : array<float>) {
    // 2.0s total: the audio tutorials play the gong inside fixed 2.0-2.5s
    // windows without stopping the SID, so a longer tail bleeds into the
    // next section
    let dur = 2.0
    let n = int(dur * float(SR))
    samples |> resize(n)
    let base = 110.0
    let ratios <- fixed_array(1.0, 1.008, 1.52, 1.532, 2.02, 2.026, 2.48, 2.92, 3.46, 4.1, 5.04, 6.27)
    let amps <- fixed_array(1.0, 0.9, 0.62, 0.6, 0.5, 0.48, 0.4, 0.32, 0.24, 0.17, 0.11, 0.07)
    let decays <- fixed_array(1.5, 1.55, 1.2, 1.22, 1.0, 1.02, 0.85, 0.7, 0.6, 0.5, 0.4, 0.3)
    for (i in range(n)) {
        let t = float(i) / float(SR)
        var s = 0.0
        for (j in range(12)) {
            s += amps[j] * exp(-t / decays[j]) * sin(2.0 * PI * base * ratios[j] * t)
        }
        if (t < STRIKE_SEC) {
            s += deterministic_noise(i) * 2.5 * (1.0 - t / STRIKE_SEC)
        }
        let bloom = min(t / 0.012, 1.0)
        samples[i] = s * bloom * 0.22
    }
}

def note_hz(semis_from_a4 : int) : float {
    return 440.0 * pow(2.0, float(semis_from_a4) / 12.0)
}

let C3 = -21
let E3 = -17
let F3 = -16
let G3 = -14
let A3 = -12
let B3 = -10
let C4 = -9
let D4 = -7
let E4 = -5
let F4 = -4
let G4 = -2
let A4 = 0
let EIGHTH_SEC = 0.375

def synth_music(var samples : array<float>) {
    let chord_dur = 3.0
    let n = int(4.0 * chord_dur * float(SR))
    samples |> resize(n)
    let chords <- fixed_array(
        fixed_array(A3, C4, E4, A4),
        fixed_array(F3, A3, C4, F4),
        fixed_array(C3, E3, G3, C4),
        fixed_array(G3, B3, D4, G4))
    for (i in range(n)) {
        let t = float(i) / float(SR)
        let ci = min(int(t / chord_dur), 3)
        let ct = t - float(ci) * chord_dur
        let env = min(ct / 0.4, 1.0) * min((chord_dur - ct) / 0.5, 1.0)
        var s = 0.0
        for (k in range(4)) {
            let hz = note_hz(chords[ci][k])
            let ph = 2.0 * PI * hz * t
            s += sin(ph) + 0.35 * sin(2.0 * ph) + 0.12 * sin(3.0 * ph)
        }
        let step = int(ct / EIGHTH_SEC) % 4
        let ahz = 2.0 * note_hz(chords[ci][step])
        let at = ct - float(int(ct / EIGHTH_SEC)) * EIGHTH_SEC
        s += 0.5 * exp(-at * 6.0) * sin(2.0 * PI * ahz * t)
        samples[i] = s * env * 0.11
    }
}

def orb_radius(theta, phi : float) : float {
    return 1.0 + 0.12 * sin(6.0 * theta) * sin(4.0 * phi) + 0.05 * sin(9.0 * phi)
}

def orb_pos(theta, phi : float) : float3 {
    let r = orb_radius(theta, phi)
    return float3(r * sin(phi) * cos(theta), r * cos(phi), r * sin(phi) * sin(theta))
}

let THETA_SEGMENTS = 64
let PHI_RINGS = 32

def write_orb(path : string) {
    fopen(path, "wb") $(f) {
        if (f == null) {
            panic("can't write {path}")
        }
        f |> fwrite("# procedural bumpy orb, generated for the daslang examples (first-party, CC0)\n")
        for (ri in range(PHI_RINGS + 1)) {
            let phi = PI * float(ri) / float(PHI_RINGS)
            for (si in range(THETA_SEGMENTS + 1)) {
                let theta = 2.0 * PI * float(si) / float(THETA_SEGMENTS)
                let p = orb_pos(theta, phi)
                f |> fwrite("v {p.x} {p.y} {p.z}\n")
                f |> fwrite("vt {float(si) / float(THETA_SEGMENTS)} {1.0 - float(ri) / float(PHI_RINGS)}\n")
                // du x dv points outward for this parameterization
                let central_diff = 0.001
                let du = orb_pos(theta + central_diff, phi) - orb_pos(theta - central_diff, phi)
                let dv = orb_pos(theta, phi + central_diff) - orb_pos(theta, phi - central_diff)
                var nrm = normalize(cross(du, dv))
                if (ri == 0 || ri == PHI_RINGS) {
                    nrm = float3(0.0, ri == 0 ? 1.0 : -1.0, 0.0)
                } else {
                    // the surface is star-shaped from the origin, so an outward
                    // normal must have a positive radial component - a flipped
                    // cross order fails here at generation time
                    verify(dot(nrm, p) > 0.0)
                }
                f |> fwrite("vn {nrm.x} {nrm.y} {nrm.z}\n")
            }
        }
        let stride = THETA_SEGMENTS + 1
        for (ri in range(PHI_RINGS)) {
            for (si in range(THETA_SEGMENTS)) {
                let a = ri * stride + si + 1
                let b = a + 1
                let c = a + stride + 1
                let d = a + stride
                f |> fwrite("f {a}/{a}/{a} {b}/{b}/{b} {c}/{c}/{c} {d}/{d}/{d}\n")
            }
        }
    }
}

let TEX_SIZE = 512
let CHANNELS = 3

def write_texture(path : string) {
    var bytes : array<uint8>
    bytes |> resize(TEX_SIZE * TEX_SIZE * CHANNELS)
    for (y in range(TEX_SIZE)) {
        for (x in range(TEX_SIZE)) {
            let u = (float(x) + 0.5) / float(TEX_SIZE) - 0.5
            let v = (float(y) + 0.5) / float(TEX_SIZE) - 0.5
            let r = sqrt(u * u + v * v)
            let a = atan2(v, u)
            let swirl = 0.5 + 0.5 * sin(10.0 * a + 18.0 * r)
            let rings = 0.5 + 0.5 * sin(40.0 * r)
            let grid = (((x / 32) ^ (y / 32)) & 1) != 0 ? 0.12 : 0.0
            let rr = saturate(0.85 * swirl + 0.15 * rings + grid)
            let gg = saturate(0.45 * swirl + 0.35 * rings + grid)
            let bb = saturate(0.25 + 0.35 * rings - 0.15 * swirl + grid)
            let o = (y * TEX_SIZE + x) * CHANNELS
            bytes[o + 0] = uint8(rr * 255.0)
            bytes[o + 1] = uint8(gg * 255.0)
            bytes[o + 2] = uint8(bb * 255.0)
        }
    }
    if (stbi_write_png(path, TEX_SIZE, TEX_SIZE, CHANNELS, unsafe(addr(bytes[0])), TEX_SIZE * CHANNELS) == 0) {
        panic("can't write {path}")
    }
}

[export]
def main() {
    let root = get_das_root()
    var gong : array<float>
    synth_gong(gong)
    if (!write_wav("{root}/examples/media/audio/gong.wav", gong, uint(SR), 1)) {
        panic("can't write gong.wav")
    }
    to_log(LOG_INFO, "gong.wav: {length(gong)} samples\n")
    var music : array<float>
    synth_music(music)
    if (!write_wav("{root}/examples/media/audio/music.wav", music, uint(SR), 1)) {
        panic("can't write music.wav")
    }
    to_log(LOG_INFO, "music.wav: {length(music)} samples\n")
    write_orb("{root}/examples/opengl/orb.obj")
    to_log(LOG_INFO, "orb.obj written\n")
    write_texture("{root}/examples/opengl/image.png")
    to_log(LOG_INFO, "image.png written\n")
}
