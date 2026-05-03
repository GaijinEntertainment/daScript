options gen2
options persistent_heap

require rr_globals public

// --- Audio State ---

var @live snd_shoot : array<float>
var @live snd_enemy_explode : array<float>
var @live snd_player_hit : array<float>
var @live snd_refuel : array<float>
var @live snd_bridge_destroy : array<float>
var @live snd_section_clear : array<float>
var @live snd_low_fuel_beep : array<float>
var @live snd_game_over : array<float>

var @live audio_initialized = false
var asch : AudioSystemChannels

var @live music_initialized = false
var g_music_tracks : table<string; int>
var prev_music_state = GameState.menu
var low_fuel_music_active = false
var @live prev_section_for_music = -1

// --- PCM Generation ---

def gen_sine_sweep(f1, f2, dur, vol : float) : array<float> {
    let n = int(float(AUDIO_RATE) * dur)
    var inscope s : array<float>
    s |> resize(n)
    for (i in range(n)) {
        let t = float(i) / float(AUDIO_RATE)
        let f = f1 + (f2 - f1) * (t / dur)
        s[i] = sin(2.0 * PI * f * t) * vol
    }
    return <- s
}

def gen_noise_burst(dur, vol : float) : array<float> {
    let n = int(float(AUDIO_RATE) * dur)
    var inscope s : array<float>
    s |> resize(n)
    var seed = 77u
    for (i in range(n)) {
        seed = seed * 1103515245u + 12345u
        let noise = float(int(seed >> 16u) % 1000) / 500.0 - 1.0
        s[i] = noise * vol
    }
    return <- s
}

def mix_into(var dst : array<float>; src : array<float>) {
    let n = min(length(dst), length(src))
    for (i in range(n)) {
        dst[i] += src[i]
    }
}

def build_note_sequence(notes : float[]; note_dur, volume : float; rising : bool) : array<float> {
    let total_dur = note_dur * float(length(notes))
    let total_samples = int(float(AUDIO_RATE) * total_dur)
    var inscope seq : array<float>
    seq |> resize(total_samples)
    for (ni in range(length(notes))) {
        let f1 = notes[ni]
        let f2 = (rising ? notes[ni] * 1.06 : notes[ni] * 0.92)
        let note <- gen_sine_sweep(f1, f2, note_dur, volume)
        let offset = ni * int(float(AUDIO_RATE) * note_dur)
        for (i in range(length(note))) {
            if (offset + i < total_samples) {
                seq[offset + i] += note[i]
            }
        }
    }
    return <- seq
}

def init_audio_samples() {
    // shoot: short upward chirp
    snd_shoot <- gen_sine_sweep(1200.0, 2200.0, 0.055, 0.18)

    // enemy explode: noise + low thump
    snd_enemy_explode <- gen_noise_burst(0.12, 0.15)
    let thump <- gen_sine_sweep(280.0, 60.0, 0.14, 0.22)
    mix_into(snd_enemy_explode, thump)

    // player hit: harsh descending noise
    snd_player_hit <- gen_noise_burst(0.22, 0.25)
    let descend <- gen_sine_sweep(600.0, 80.0, 0.28, 0.2)
    mix_into(snd_player_hit, descend)

    // refuel: ascending chord
    let refuel_notes = fixed_array(330.0, 440.0, 550.0, 660.0)
    snd_refuel <- build_note_sequence(refuel_notes, 0.07, 0.15, true)

    // bridge destroy: deep rumble burst
    snd_bridge_destroy <- gen_noise_burst(0.28, 0.2)
    let rumble <- gen_sine_sweep(180.0, 40.0, 0.3, 0.28)
    mix_into(snd_bridge_destroy, rumble)

    // section clear: triumphant arpeggio
    let section_notes = fixed_array(330.0, 415.0, 523.0, 659.0, 830.0)
    snd_section_clear <- build_note_sequence(section_notes, 0.09, 0.18, true)

    // low fuel beep: short warning tone
    snd_low_fuel_beep <- gen_sine_sweep(880.0, 840.0, 0.07, 0.14)

    // game over: descending sweep
    let go_notes = fixed_array(523.0, 415.0, 330.0, 247.0, 196.0)
    snd_game_over <- build_note_sequence(go_notes, 0.13, 0.22, false)
}

def play_sfx(samples : array<float>) {
    if (!audio_initialized) {
        return
    }
    var copy <- clone(samples)
    play_sound_from_pcm(AUDIO_RATE, AUDIO_CHANNELS, copy)
}

// --- Music ---

def music_cmd(cmd : string) {
    let parts <- split(cmd, ":")
    if (length(parts) < 2) {
        return
    }
    let action = parts[0]
    let name = parts[1]
    if (action == "play" && length(parts) >= 3) {
        let gain = (length(parts) >= 4 ? float(to_double(parts[3])) : 1.0)
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(s(parts[2]), gain)
    } elif (action == "note" && length(parts) >= 4) {
        let gain = (length(parts) >= 5 ? float(to_double(parts[4])) : 0.3)
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(note_pattern(parts[2], parts[3]), gain)
    } elif (action == "stop") {
        if (key_exists(g_music_tracks, name)) {
            let fade_time = (length(parts) >= 3 ? float(to_double(parts[2])) : 0.5)
            strudel_fade_track(g_music_tracks[name], 0.0, fade_time)
        }
    }
}

def stop_all_tracks(fade : float) {
    strudel_command("stop:drums:{fade}")
    strudel_command("stop:bass:{fade}")
    strudel_command("stop:lead:{fade}")
    strudel_command("stop:arp:{fade}")
}

def start_menu_music() {
    strudel_set_bpm(96.0lf)
    strudel_command("play:drums:<bd ~ ~ ~> <~ ~ hh ~>:0.18")
    strudel_command("note:bass:<e2 ~ ~ ~> <d2 ~ ~ ~> <c2 ~ ~ ~> <b1 ~ ~ ~>:sine:0.2")
    strudel_command("note:lead:<e4 ~ g4 ~> <d4 ~ f4 ~> <c4 ~ e4 ~> <b3 ~ d4 ~>:triangle:0.1")
}

def start_play_music() {
    strudel_set_bpm(128.0lf)
    strudel_command("play:drums:<bd [~ hh] sd [hh ~]> <bd [~ hh] ~ [hh cp?0.3]> <bd [hh ~] sd [~ hh]> <bd [hh hh] sd [hh ~]>:0.48")
    strudel_command("note:bass:<[e2 e2] ~ [b1 b1] ~> <[g2 g2] ~ [d2 d2] ~> <[c2 c2] ~ [g1 g1] ~> <[a1 a1] [b1 b1] [c2 c2] [d2 d2]>:square:0.24")
    strudel_command("note:lead:<e4 g4 b4 e5> <d4 f4 a4 d5> <c4 e4 g4 c5> <b3 d4 f4 b4>:sawtooth:0.18")
    strudel_command("note:arp:<[e5 b4 g4 e4]> <[d5 a4 f4 d4]> <[c5 g4 e4 c4]> <[b4 fs4 d4 b3]>:triangle:0.09")
}

def start_tension_music() {
    strudel_set_bpm(110.0lf)
    strudel_command("play:drums:<bd ~ ~ ~> <bd ~ hh ~> <bd ~ ~ hh> <bd ~ hh ~>:0.22")
    strudel_command("note:bass:<e2 ~ d2 ~> <c2 ~ b1 ~> <a1 ~ g1 ~> <e2 ~ ~ ~>:sine:0.26")
    strudel_command("note:lead:<e4 ~ d4 ~> <c4 ~ b3 ~> <a3 ~ g3 ~> <e4 ~ ~ ~>:triangle:0.14")
    strudel_command("stop:arp:0.4")
}

def start_game_over_music() {
    strudel_set_bpm(80.0lf)
    strudel_command("play:drums:<bd ~ ~ ~> <bd ~ ~ ~> <bd ~ ~ ~> <bd ~ ~ ~>:0.18")
    strudel_command("note:bass:<e2 d2 c2 b1> <a1 ~ g1 ~> <e2 d2 c2 b1> <a1 ~ ~ ~>:sine:0.26")
    strudel_command("note:lead:<g4 ~ e4 ~> <d4 ~ c4 ~> <b3 ~ a3 ~> <g3 ~ ~ ~>:triangle:0.16")
    strudel_command("stop:arp:0.3")
}

def start_win_music() {
    strudel_set_bpm(148.0lf)
    strudel_command("play:drums:<bd [hh hh] sd [hh cp]> <bd [hh hh] [sd sd] [hh hh]> <bd [hh cp] sd [hh hh]> <bd [hh hh] [sd cp] [hh hh]>:0.54")
    strudel_command("note:bass:<[e2 e2] [g2 g2] [b2 b2] [e3 e3]> <[d3 d3] [b2 b2] [a2 a2] [g2 g2]> <[c3 c3] [a2 a2] [g2 g2] [b2 b2]> <[e2 e2] [g2 g2] [b2 b2] [e3 e3]>:square:0.28")
    strudel_command("note:lead:<e5 g5 b5 e6> <g5 b5 d6 g6> <a5 c6 e6 a6> <b5 d6 fs6 b6>:sawtooth:0.22")
    strudel_command("note:arp:<[e6 b5 g5 e5]> <[g6 d6 b5 g5]> <[a6 e6 c6 a5]> <[b6 fs6 d6 b5]>:triangle:0.12")
}

// 5 distinct gameplay music variants (cycles through sections 0..4)
def start_section_music(section : int) {
    let s = section % 5
    if (s == 0) {
        strudel_set_bpm(128.0lf)
        strudel_command("play:drums:<bd [~ hh] sd [hh ~]> <bd [~ hh] ~ [hh cp?0.3]> <bd [hh ~] sd [~ hh]> <bd [hh hh] sd [hh ~]>:0.48")
        strudel_command("note:bass:<[e2 e2] ~ [b1 b1] ~> <[g2 g2] ~ [d2 d2] ~> <[c2 c2] ~ [g1 g1] ~> <[a1 a1] [b1 b1] [c2 c2] [d2 d2]>:square:0.24")
        strudel_command("note:lead:<e4 g4 b4 e5> <d4 f4 a4 d5> <c4 e4 g4 c5> <b3 d4 f4 b4>:sawtooth:0.18")
        strudel_command("note:arp:<[e5 b4 g4 e4]> <[d5 a4 f4 d4]> <[c5 g4 e4 c4]> <[b4 fs4 d4 b3]>:triangle:0.09")
    } elif (s == 1) {
        strudel_set_bpm(136.0lf)
        strudel_command("play:drums:<bd [hh hh] sd hh> <bd [hh cp] sd [hh hh]> <bd [hh ~] [sd sd] hh> <bd hh sd [hh cp]>:0.52")
        strudel_command("note:bass:<[a1 a1] ~ [e2 e2] ~> <[c2 c2] ~ [g1 g1] ~> <[d2 d2] ~ [a1 a1] ~> <[f2 f2] [e2 e2] [d2 d2] [c2 c2]>:square:0.26")
        strudel_command("note:lead:<a4 c5 e5 a5> <g4 b4 d5 g5> <f4 a4 c5 f5> <e4 g4 b4 e5>:sawtooth:0.2")
        strudel_command("note:arp:<[a5 e5 c5 a4]> <[g5 d5 b4 g4]> <[f5 c5 a4 f4]> <[e5 b4 g4 e4]>:triangle:0.1")
    } elif (s == 2) {
        strudel_set_bpm(140.0lf)
        strudel_command("play:drums:<[bd bd] [hh hh] [sd sd] [hh hh]> <[bd bd] [hh cp] [sd sd] [cp hh]> <bd [hh hh] [sd cp] [hh hh]> <[bd bd] hh sd [hh hh]>:0.56")
        strudel_command("note:bass:<[d2 d2] ~ [a1 a1] ~> <[f2 f2] ~ [c2 c2] ~> <[g2 g2] ~ [d2 d2] ~> <[a2 a2] [g2 g2] [f2 f2] [e2 e2]>:square:0.28")
        strudel_command("note:lead:<d4 f4 a4 d5> <c4 e4 g4 c5> <bf3 d4 f4 bf4> <a3 c4 e4 a4>:sawtooth:0.22")
        strudel_command("note:arp:<[d5 a4 f4 d4]> <[c5 g4 e4 c4]> <[bf4 f4 d4 bf3]> <[a4 e4 c4 a3]>:triangle:0.11")
    } elif (s == 3) {
        strudel_set_bpm(148.0lf)
        strudel_command("play:drums:<[bd bd] [hh hh] [sd cp] [hh hh]> <[bd bd] [hh hh] [sd sd] [cp cp]> <bd [hh cp] [sd sd] [hh cp]> <[bd bd] [hh hh] [sd cp] [hh hh]>:0.6")
        strudel_command("note:bass:<[b1 b1] ~ [fs2 fs2] ~> <[d2 d2] ~ [a1 a1] ~> <[e2 e2] ~ [b1 b1] ~> <[g2 g2] [fs2 fs2] [e2 e2] [d2 d2]>:square:0.3")
        strudel_command("note:lead:<b4 d5 fs5 b5> <a4 c5 e5 a5> <g4 b4 d5 g5> <fs4 a4 c5 fs5>:sawtooth:0.24")
        strudel_command("note:arp:<[b5 fs5 d5 b4]> <[a5 e5 c5 a4]> <[g5 d5 b4 g4]> <[fs5 c5 a4 fs4]>:triangle:0.13")
    } else {
        strudel_set_bpm(158.0lf)
        strudel_command("play:drums:<[bd bd] [hh hh] [sd cp] [hh cp]> <[bd cp] [hh hh] [sd sd] [hh cp]> <[bd bd] [hh cp] [sd cp] [cp hh]> <[bd bd] [hh hh] [sd cp] [hh hh]>:0.64")
        strudel_command("note:bass:<[g1 g1] ~ [d2 d2] ~> <[bf1 bf1] ~ [f2 f2] ~> <[c2 c2] ~ [g1 g1] ~> <[d2 d2] [c2 c2] [bf1 bf1] [a1 a1]>:square:0.32")
        strudel_command("note:lead:<g4 bf4 d5 g5> <f4 a4 c5 f5> <ef4 g4 bf4 ef5> <d4 f4 a4 d5>:sawtooth:0.26")
        strudel_command("note:arp:<[g5 d5 bf4 g4]> <[f5 c5 a4 f4]> <[ef5 bf4 g4 ef4]> <[d5 a4 f4 d4]>:triangle:0.14")
    }
}

def strudel_music_main() {
    start_menu_music()
    strudel_play(@@music_cmd)
}

def update_music_state() {
    if (!music_initialized) {
        return
    }
    let low_fuel = (game_state == GameState.playing && player_fuel < LOW_FUEL_THRESHOLD)
    if (game_state != prev_music_state) {
        if (game_state == GameState.menu) {
            start_menu_music()
        } elif (game_state == GameState.playing) {
            if (low_fuel) {
                start_tension_music()
                low_fuel_music_active = true
            } else {
                start_section_music(current_section)
                low_fuel_music_active = false
            }
            prev_section_for_music = current_section
        } elif (game_state == GameState.game_over_state) {
            start_game_over_music()
        } elif (game_state == GameState.win_state) {
            start_win_music()
        }
        prev_music_state = game_state
    } elif (game_state == GameState.playing) {
        let section_changed = (current_section != prev_section_for_music)
        if (low_fuel && !low_fuel_music_active) {
            start_tension_music()
            low_fuel_music_active = true
            prev_section_for_music = current_section
        } elif (!low_fuel && (low_fuel_music_active || section_changed)) {
            start_section_music(current_section)
            low_fuel_music_active = false
            prev_section_for_music = current_section
        }
    }
}

def init_audio() {
    if (!audio_initialized) {
        asch = audio_system_create()
        audio_initialized = true
        init_audio_samples()
    }
    if (!music_initialized) {
        strudel_init(@@strudel_music_main)
        strudel_set_volume(0.35, 0.0)
        music_initialized = true
        prev_music_state = game_state
    }
}

def shutdown_audio() {
    if (music_initialized) {
        strudel_shutdown()
        music_initialized = false
    }
    if (audio_initialized) {
        audio_system_finalize(asch.command, asch.next_sid)
        audio_initialized = false
    }
}
