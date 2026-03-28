options gen2
options indenting = 4

module strudel_pianoroll shared public

require strudel/strudel_event
require strudel/strudel_time
require strudel/strudel_pattern
require strudel/strudel_player
require daslib/jobque_boost

// --- Data sent from audio thread to GL thread ---

struct PianorollHap {
    whole_start : float
    whole_stop  : float
    part_start  : float
    part_stop   : float
    note        : float
    sound       : string
    gain        : float
    tap_index   : int
}

// --- Per-tap configuration (internal) ---

struct PianorollTap {
    name    : string
    color   : uint = 0xFFCA28u
    scroll  : int = -1
    channel : Channel?
}

// --- Per-track visualization (internal) ---

struct TrackVisData {
    name          : string
    color         : uint = 0xFFCA28u
    hap_channel   : Channel?
    audio_channel : Channel?
    tap_index     : int = -1
}

// --- Globals (internal) ---

var private g_pianoroll_taps : array<PianorollTap>
var private g_track_vis : array<TrackVisData>
var private g_visualization_enabled : bool = false

// --- Public API ---

def public visualization_enable() {
    g_visualization_enabled = true
}

def public visualization_disable() {
    g_visualization_enabled = false
}

// Create visualization channels for a track. Returns an opaque track vis index.
// Call on main thread before strudel_init.
// Automatically registers channels on StrudelState for cross-context access.
def public create_track_vis(name : string; color : uint = 0xFFCA28u) : int {
    // create pianoroll tap
    var hap_ch = unsafe(channel_create())
    hap_ch |> add_ref()
    let tap_idx = length(g_pianoroll_taps)
    g_pianoroll_taps |> emplace(PianorollTap(
        name = name,
        color = color,
        channel = hap_ch
    ))
    // create audio channel
    var audio_ch = unsafe(channel_create())
    audio_ch |> add_ref()
    // store bundle
    let idx = length(g_track_vis)
    g_track_vis |> emplace(TrackVisData(
        name = name,
        color = color,
        hap_channel = hap_ch,
        audio_channel = audio_ch,
        tap_index = tap_idx
    ))
    // register on shared state for cross-context access
    strudel_register_vis(name, idx, hap_ch, audio_ch)
    return idx
}

// Get the number of registered track vis bundles.
def public num_track_vis() : int {
    return length(g_track_vis)
}

// Get the tap index for a track vis (for pianoroll panel registration).
def public track_vis_tap_index(idx : int) : int {
    return g_track_vis[idx].tap_index
}

// Get the color for a track vis.
def public track_vis_color(idx : int) : uint {
    return g_track_vis[idx].color
}

// Get the name for a track vis.
def public track_vis_name(idx : int) : string {
    return g_track_vis[idx].name
}

// Get the hap channel for a track vis (for strudel_add_vis_channel).
def public track_vis_hap_channel(idx : int) : Channel? {
    return g_track_vis[idx].hap_channel
}

// Get the audio channel for a track vis (for strudel_add_track_audio_channel).
def public track_vis_audio_channel(idx : int) : Channel? {
    return g_track_vis[idx].audio_channel
}

// Clean up all track vis and pianoroll taps.
def public destroy_all_track_vis() {
    for (tv in g_track_vis) {
        if (tv.audio_channel != null) {
            tv.audio_channel |> release()
        }
    }
    delete g_track_vis
    for (tap in g_pianoroll_taps) {
        if (tap.channel != null) {
            tap.channel |> release()
        }
    }
    delete g_pianoroll_taps
}

// --- Pianoroll tap access (for rendering) ---

def public pianoroll_get_taps(blk : block<(taps : array<PianorollTap> const) : void>) {
    invoke(blk, g_pianoroll_taps)
}

// --- The pipe method (runs on strudel thread) ---

// Name-based pianoroll tap. Looks up the visualization by name
// in state.vis_registry (populated by create_track_vis on main thread).
def public pianoroll(var pat : Pattern; var state : StrudelState?; name : string) : Pattern {
    var vis_idx = -1
    for (vi in range(length(state.vis_names))) {
        if (state.vis_names[vi] == name) {
            vis_idx = vi
            break
        }
    }
    if (vis_idx < 0 || vis_idx >= length(state.vis_channels)) {
        return <- pat
    }
    var ch = state.vis_channels[vis_idx]
    return <- pianoroll(pat, ch, vis_idx)
}

// Low-level pianoroll tap with explicit channel and index.
def public pianoroll(var pat : Pattern; var vis_channel : Channel?; tap_index : int = 0) : Pattern {
    // Store channel as void? to avoid the capture_jobque_channel/release mechanism.
    // The channel lifetime is managed by destroy_all_track_vis() on the main thread.
    var ch_ptr = unsafe(reinterpret<void?>(vis_channel))
    return @capture(= tap_index, = ch_ptr, <- pat) (span : TimeSpan) : array<Hap> {
        var haps <- pat(span)
        var ch = unsafe(reinterpret<Channel?>(ch_ptr))
        for (h in haps) {
            if (h.has_whole) {
                ch |> push_clone(PianorollHap(
                    whole_start = float(h.whole.start),
                    whole_stop = float(h.whole.stop),
                    part_start = float(h.part.start),
                    part_stop = float(h.part.stop),
                    note = h.value.note,
                    sound = h.value.s,
                    gain = h.value.gain,
                    tap_index = tap_index
                ))
            }
        }
        return <- haps
    }
}
