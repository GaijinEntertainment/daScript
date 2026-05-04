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
var @live snd_bonus_fuel : array<float>
var @live snd_bonus_multishot : array<float>
var @live snd_bonus_life : array<float>
var @live snd_bonus_fastshot : array<float>
var @live snd_engine_loop : array<float>

var @live engine_sid : SID = INVALID_SID

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

    // bonus fuel: bright 2-note up chirp
    let bf_notes = fixed_array(440.0, 660.0)
    snd_bonus_fuel <- build_note_sequence(bf_notes, 0.06, 0.16, true)

    // bonus multishot: sharper metallic rise
    snd_bonus_multishot <- gen_sine_sweep(700.0, 1600.0, 0.1, 0.2)

    // bonus life: celebratory triad climb
    let bl_notes = fixed_array(392.0, 523.0, 659.0)
    snd_bonus_life <- build_note_sequence(bl_notes, 0.08, 0.2, true)

    // bonus fast shot: rapid double-blip
    snd_bonus_fastshot <- gen_sine_sweep(900.0, 1800.0, 0.06, 0.18)

    // Engine rumble loop: 0.5 s periodic harmonics so the loop seam is click-free.
    let eng_samples = int(float(AUDIO_RATE) * 0.5)
    var inscope eng : array<float>
    eng |> resize(eng_samples)
    for (i in range(eng_samples)) {
        let t = float(i) / float(AUDIO_RATE)
        eng[i] = (
            sin(2.0 * PI * 80.0 * t) * 0.55 +
            sin(2.0 * PI * 160.0 * t) * 0.28 +
            sin(2.0 * PI * 240.0 * t) * 0.12
        ) * 0.13
    }
    eng[eng_samples - 1] = eng[0]
    snd_engine_loop <- eng
}

def play_sfx(samples : array<float>) {
    if (!audio_initialized) {
        return
    }
    var copy <- clone(samples)
    play_sound_from_pcm(AUDIO_RATE, AUDIO_CHANNELS, copy)
}

def update_engine_sound(active : bool; speed, max_speed : float) {
    if (!audio_initialized || engine_sid == INVALID_SID) {
        return
    }
    set_pause(engine_sid, !active)
    if (active) {
        // Speed range PLAYER_FWD_SPEED_MIN..max_speed maps to pitch 0.75..1.55.
        let hi = max(max_speed, PLAYER_FWD_SPEED_MIN + 0.01)
        let t = clamp((speed - PLAYER_FWD_SPEED_MIN) / (hi - PLAYER_FWD_SPEED_MIN), 0.0, 1.0)
        set_pitch(engine_sid, 0.75 + t * 0.80)
        set_volume(engine_sid, 0.30, 0.05)
    } else {
        set_volume(engine_sid, 0.0, 0.1)
    }
}

// --- Music ---

def replace_named_track(name : string; var pat : Pattern; gain : float) {
    if (key_exists(g_music_tracks, name)) {
        strudel_remove_track(g_music_tracks[name])
    }
    g_music_tracks[name] = strudel_add_track(pat, gain)
}

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
    } elif (action == "rich") {
        // Rich tracks built directly here on the strudel thread so they can
        // use the full Pattern API (chunk/sometimesby/jux/etc.).
        if (name == "0") {
            build_section0_rich()
        } elif (name == "1") {
            build_section1_rich()
        } elif (name == "2") {
            build_section2_rich()
        } elif (name == "3") {
            build_section3_rich()
        } elif (name == "4") {
            build_section4_rich()
        } elif (name == "menu") {
            build_menu_rich()
        } elif (name == "tension") {
            build_tension_rich()
        } elif (name == "gameover") {
            build_game_over_rich()
        } elif (name == "win") {
            build_win_rich()
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
    strudel_command("rich:menu")
}

def start_tension_music() {
    strudel_set_bpm(110.0lf)
    strudel_command("rich:tension")
}

def start_game_over_music() {
    strudel_set_bpm(80.0lf)
    strudel_command("rich:gameover")
}

def start_win_music() {
    strudel_set_bpm(148.0lf)
    strudel_command("rich:win")
}

// ─── Per-voice rich builders ─────────────────────────────────────────
// Same effect chain across every music mode:
//   lead  — chunk(4, fast(2)) rotates a stuttered quarter per cycle so
//           the 16-note loop never repeats identically; sometimesby
//           sprinkles octave jumps; jux(rev) gives stereo motion;
//           phaser adds slow phase shimmer; perlin gain breathes.
//   bass  — sub-octave layer for low end; ply(2) double-taps occasionally.
//   arp   — jux(rev) + occasional double-time bursts.
//   drums — half-bar gain swell.
// Run on the strudel thread (called from music_cmd "rich:..."). BPM is
// set separately by the main-thread caller.

def rich_drums(pattern : string; g : float) {
    replace_named_track("drums",
        s(pattern) |> gain(saw() |> range(0.7, 1.0) |> fast(2.0lf)),
        g)
}

def rich_bass(pattern, osc : string; g : float) {
    replace_named_track("bass",
        note_pattern(pattern, osc)
        |> superimpose(@(var x : Pattern) => x |> add(-12.0) |> gain(0.6))
        |> sometimesby(0.15, @(var x : Pattern) => x |> ply(2))
        |> attack(0.005)
        |> release(0.18),
        g)
}

def rich_lead(pattern, osc : string; g : float) {
    replace_named_track("lead",
        note_pattern(pattern, osc)
        |> chunk(4, @(var x : Pattern) => x |> fast(2.0lf))
        |> sometimesby(0.18, @(var x : Pattern) => x |> add(12.0))
        |> jux(@(var x : Pattern) => rev(x))
        |> phaser(0.25)
        |> attack(0.006)
        |> release(0.16)
        |> gain(perlin() |> range(0.75, 1.0) |> slow(3.0lf)),
        g)
}

def rich_arp(pattern : string; g : float) {
    replace_named_track("arp",
        note_pattern(pattern, "triangle")
        |> jux(@(var x : Pattern) => rev(x))
        |> sometimesby(0.20, @(var x : Pattern) => x |> fast(2.0lf))
        |> attack(0.005)
        |> release(0.12),
        g)
}

def fade_arp(fade : float) {
    if (key_exists(g_music_tracks, "arp")) {
        strudel_fade_track(g_music_tracks["arp"], 0.0, fade)
    }
}

// ─── Section builders (gameplay loop, square+saw+triangle stack) ─────

def build_section0_rich() {
    rich_drums("<bd [~ hh] sd [hh ~]> <bd [~ hh] ~ [hh cp?0.3]> <bd [hh ~] sd [~ hh]> <bd [hh hh] sd [hh ~]>", 0.48)
    rich_bass("<[e2 e2] ~ [b1 b1] ~> <[g2 g2] ~ [d2 d2] ~> <[c2 c2] ~ [g1 g1] ~> <[a1 a1] [b1 b1] [c2 c2] [d2 d2]>", "square", 0.30)
    rich_lead("<e4 g4 b4 e5> <d4 f4 a4 d5> <c4 e4 g4 c5> <b3 d4 f4 b4>", "sawtooth", 0.20)
    rich_arp("<[e5 b4 g4 e4]> <[d5 a4 f4 d4]> <[c5 g4 e4 c4]> <[b4 fs4 d4 b3]>", 0.10)
}

def build_section1_rich() {
    rich_drums("<bd [hh hh] sd hh> <bd [hh cp] sd [hh hh]> <bd [hh ~] [sd sd] hh> <bd hh sd [hh cp]>", 0.52)
    rich_bass("<[a1 a1] ~ [e2 e2] ~> <[c2 c2] ~ [g1 g1] ~> <[d2 d2] ~ [a1 a1] ~> <[f2 f2] [e2 e2] [d2 d2] [c2 c2]>", "square", 0.32)
    rich_lead("<a4 c5 e5 a5> <g4 b4 d5 g5> <f4 a4 c5 f5> <e4 g4 b4 e5>", "sawtooth", 0.22)
    rich_arp("<[a5 e5 c5 a4]> <[g5 d5 b4 g4]> <[f5 c5 a4 f4]> <[e5 b4 g4 e4]>", 0.10)
}

def build_section2_rich() {
    rich_drums("<[bd bd] [hh hh] [sd sd] [hh hh]> <[bd bd] [hh cp] [sd sd] [cp hh]> <bd [hh hh] [sd cp] [hh hh]> <[bd bd] hh sd [hh hh]>", 0.56)
    rich_bass("<[d2 d2] ~ [a1 a1] ~> <[f2 f2] ~ [c2 c2] ~> <[g2 g2] ~ [d2 d2] ~> <[a2 a2] [g2 g2] [f2 f2] [e2 e2]>", "square", 0.34)
    rich_lead("<d4 f4 a4 d5> <c4 e4 g4 c5> <bf3 d4 f4 bf4> <a3 c4 e4 a4>", "sawtooth", 0.24)
    rich_arp("<[d5 a4 f4 d4]> <[c5 g4 e4 c4]> <[bf4 f4 d4 bf3]> <[a4 e4 c4 a3]>", 0.11)
}

def build_section3_rich() {
    rich_drums("<[bd bd] [hh hh] [sd cp] [hh hh]> <[bd bd] [hh hh] [sd sd] [cp cp]> <bd [hh cp] [sd sd] [hh cp]> <[bd bd] [hh hh] [sd cp] [hh hh]>", 0.60)
    rich_bass("<[b1 b1] ~ [fs2 fs2] ~> <[d2 d2] ~ [a1 a1] ~> <[e2 e2] ~ [b1 b1] ~> <[g2 g2] [fs2 fs2] [e2 e2] [d2 d2]>", "square", 0.36)
    rich_lead("<b4 d5 fs5 b5> <a4 c5 e5 a5> <g4 b4 d5 g5> <fs4 a4 c5 fs5>", "sawtooth", 0.26)
    rich_arp("<[b5 fs5 d5 b4]> <[a5 e5 c5 a4]> <[g5 d5 b4 g4]> <[fs5 c5 a4 fs4]>", 0.13)
}

def build_section4_rich() {
    rich_drums("<[bd bd] [hh hh] [sd cp] [hh cp]> <[bd cp] [hh hh] [sd sd] [hh cp]> <[bd bd] [hh cp] [sd cp] [cp hh]> <[bd bd] [hh hh] [sd cp] [hh hh]>", 0.64)
    rich_bass("<[g1 g1] ~ [d2 d2] ~> <[bf1 bf1] ~ [f2 f2] ~> <[c2 c2] ~ [g1 g1] ~> <[d2 d2] [c2 c2] [bf1 bf1] [a1 a1]>", "square", 0.38)
    rich_lead("<g4 bf4 d5 g5> <f4 a4 c5 f5> <ef4 g4 bf4 ef5> <d4 f4 a4 d5>", "sawtooth", 0.28)
    rich_arp("<[g5 d5 bf4 g4]> <[f5 c5 a4 f4]> <[ef5 bf4 g4 ef4]> <[d5 a4 f4 d4]>", 0.14)
}

// ─── Transition builders (menu / tension / game-over / win) ──────────
// Same effect chain, but oscillator/density per their original mood:
//   menu/tension/game-over use sine bass + triangle lead (sparser),
//   win uses square+saw+triangle like the gameplay sections.

def build_menu_rich() {
    rich_drums("<bd ~ ~ ~> <~ ~ hh ~>", 0.18)
    rich_bass("<e2 ~ ~ ~> <d2 ~ ~ ~> <c2 ~ ~ ~> <b1 ~ ~ ~>", "sine", 0.20)
    rich_lead("<e4 ~ g4 ~> <d4 ~ f4 ~> <c4 ~ e4 ~> <b3 ~ d4 ~>", "triangle", 0.10)
    fade_arp(0.4)
}

def build_tension_rich() {
    rich_drums("<bd ~ ~ ~> <bd ~ hh ~> <bd ~ ~ hh> <bd ~ hh ~>", 0.22)
    rich_bass("<e2 ~ d2 ~> <c2 ~ b1 ~> <a1 ~ g1 ~> <e2 ~ ~ ~>", "sine", 0.26)
    rich_lead("<e4 ~ d4 ~> <c4 ~ b3 ~> <a3 ~ g3 ~> <e4 ~ ~ ~>", "triangle", 0.14)
    fade_arp(0.4)
}

def build_game_over_rich() {
    rich_drums("<bd ~ ~ ~> <bd ~ ~ ~> <bd ~ ~ ~> <bd ~ ~ ~>", 0.18)
    rich_bass("<e2 d2 c2 b1> <a1 ~ g1 ~> <e2 d2 c2 b1> <a1 ~ ~ ~>", "sine", 0.26)
    rich_lead("<g4 ~ e4 ~> <d4 ~ c4 ~> <b3 ~ a3 ~> <g3 ~ ~ ~>", "triangle", 0.16)
    fade_arp(0.3)
}

def build_win_rich() {
    rich_drums("<bd [hh hh] sd [hh cp]> <bd [hh hh] [sd sd] [hh hh]> <bd [hh cp] sd [hh hh]> <bd [hh hh] [sd cp] [hh hh]>", 0.54)
    rich_bass("<[e2 e2] [g2 g2] [b2 b2] [e3 e3]> <[d3 d3] [b2 b2] [a2 a2] [g2 g2]> <[c3 c3] [a2 a2] [g2 g2] [b2 b2]> <[e2 e2] [g2 g2] [b2 b2] [e3 e3]>", "square", 0.28)
    rich_lead("<e5 g5 b5 e6> <g5 b5 d6 g6> <a5 c6 e6 a6> <b5 d6 fs6 b6>", "sawtooth", 0.22)
    rich_arp("<[e6 b5 g5 e5]> <[g6 d6 b5 g5]> <[a6 e6 c6 a5]> <[b6 fs6 d6 b5]>", 0.12)
}

// 5 distinct gameplay music variants (cycles through sections 0..4).
// BPM is set on the main thread (matching menu/tension/win functions);
// the rich track build runs on the strudel thread via music_cmd.
def start_section_music(section : int) {
    let s = section % 5
    if (s == 0) {
        strudel_set_bpm(128.0lf)
        strudel_command("rich:0")
    } elif (s == 1) {
        strudel_set_bpm(136.0lf)
        strudel_command("rich:1")
    } elif (s == 2) {
        strudel_set_bpm(140.0lf)
        strudel_command("rich:2")
    } elif (s == 3) {
        strudel_set_bpm(148.0lf)
        strudel_command("rich:3")
    } else {
        strudel_set_bpm(158.0lf)
        strudel_command("rich:4")
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
        var loop_copy <- clone(snd_engine_loop)
        engine_sid = play_sound_loop_from_pcm(AUDIO_RATE, AUDIO_CHANNELS, loop_copy)
        set_volume(engine_sid, 0.0)
        set_pause(engine_sid, true)
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
