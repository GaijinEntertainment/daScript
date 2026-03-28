options gen2
options indenting = 4

module vis_renderer shared

require opengl/opengl_boost
require opengl/opengl_ttf
require strudel/strudel_pianoroll
require audio_analysis
require daslib/jobque_boost
require math

// --- Vertex ---

[vertex_buffer]
struct VisVertex {
    xy   : float2
    rgba : float4
}

// --- Pianoroll render state per tap ---

struct PianorollRenderState {
    haps          : array<PianorollHap>
    sound_to_row  : table<string; int>
    sound_names   : array<string>
    is_drum_mode  : bool = true
    min_note      : float = 127.0
    max_note      : float = 0.0
    color         : uint = 0xFFCA28u
    scroll        : int = -1
    name          : string
}

// --- Helpers ---

def uint_to_rgba(c : uint; alpha : float = 1.0) : float4 {
    let r = float((c >> 16u) & 0xFFu) / 255.0
    let g = float((c >> 8u) & 0xFFu) / 255.0
    let b = float(c & 0xFFu) / 255.0
    return float4(r, g, b, alpha)
}

def push_quad(var verts : array<VisVertex>; x0, y0, x1, y1 : float; color : float4) {
    verts |> push(VisVertex(xy = float2(x0, y0), rgba = color))
    verts |> push(VisVertex(xy = float2(x1, y0), rgba = color))
    verts |> push(VisVertex(xy = float2(x1, y1), rgba = color))
    verts |> push(VisVertex(xy = float2(x0, y0), rgba = color))
    verts |> push(VisVertex(xy = float2(x1, y1), rgba = color))
    verts |> push(VisVertex(xy = float2(x0, y1), rgba = color))
}

let NOTE_NAMES = fixed_array("C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B")

def note_name(midi_note : float) : string {
    let n = int(midi_note)
    let octave = n / 12 - 1
    let name_idx = n % 12
    return "{NOTE_NAMES[name_idx]}{octave}"
}

// --- Spectrum ---

def spectrum_render(var verts : array<VisVertex>; aa : AudioAnalysis; x0, y0, x1, y1 : float; color : float4) {
    let num_bars = min(FFT_BINS, 128)
    let bar_w = (x1 - x0) / float(num_bars)
    let h = y1 - y0
    for (i in range(num_bars)) {
        let bar_h = max(aa.spectrum_smooth[i], 0.0) * h
        let bx0 = x0 + float(i) * bar_w + 1.0
        let bx1 = bx0 + bar_w - 2.0
        let t = aa.spectrum_smooth[i]
        let c = float4(color.x * (0.4 + 0.6 * t), color.y * (0.4 + 0.6 * t), color.z * (0.4 + 0.6 * t), 0.8)
        push_quad(verts, bx0, y0, bx1, y0 + bar_h, c)
    }
}

// --- Oscilloscope ---

def scope_render(var verts : array<VisVertex>; aa : AudioAnalysis; x0, y0, x1, y1 : float; color : float4) {
    let w = x1 - x0
    let h = y1 - y0
    let mid_y = y0 + h * 0.5
    let n = min(512, FFT_SIZE)
    let step = w / float(n - 1)
    let thickness = 1.5
    for (i in range(n - 1)) {
        let idx0 = (aa.ring_write + i) % FFT_SIZE
        let idx1 = (aa.ring_write + i + 1) % FFT_SIZE
        let s0 = clamp(aa.sample_ring[idx0], -1.0, 1.0)
        let s1 = clamp(aa.sample_ring[idx1], -1.0, 1.0)
        let px0 = x0 + float(i) * step
        let px1 = x0 + float(i + 1) * step
        let py0 = mid_y + s0 * h * 0.45
        let py1 = mid_y + s1 * h * 0.45
        let min_y = min(py0, py1) - thickness
        let max_y = max(py0, py1) + thickness
        push_quad(verts, px0, min_y, px1, max_y, color)
    }
    push_quad(verts, x0, mid_y - 0.5, x1, mid_y + 0.5, float4(0.3, 0.3, 0.35, 0.3))
}

// --- Vectorscope (M/S rotated) ---
// M (mid = L+R) on vertical axis, S (side = L-R) on horizontal axis.
// Mono content goes up/down, stereo spread goes left/right.

def vectorscope_render(var verts : array<VisVertex>; aa : AudioAnalysis; x0, y0, x1, y1 : float; color : float4) {
    let w = x1 - x0
    let h = y1 - y0
    let cx = x0 + w * 0.5
    let cy = y0 + h * 0.5
    let scale_x = w * 2.0 // let them spill
    let scale_y = h * 2.0
    let n = min(512, FFT_SIZE)
    let dot_size = 1.5
    // crosshair guides: vertical = mono, horizontal = stereo
    let guide_c = float4(0.3, 0.3, 0.35, 0.2)
    push_quad(verts, cx - 0.5, y0, cx + 0.5, y1, guide_c)
    push_quad(verts, x0, cy - 0.5, x1, cy + 0.5, guide_c)
    // plot M/S: mid = (L+R)*0.5 on Y, side = (L-R)*0.5 on X
    for (i in range(n)) {
        let idx = (aa.ring_write_lr + i) % FFT_SIZE
        let l = clamp(aa.sample_ring_l[idx], -1.0, 1.0)
        let r = clamp(aa.sample_ring_r[idx], -1.0, 1.0)
        let mid = (l + r) * 0.5
        let side = (l - r) * 0.5
        let px = cx + side * scale_x
        let py = cy + mid * scale_y
        let age = float(i) / float(n)  // 0=oldest, 1=newest
        let alpha = age * age * 0.9 + 0.1
        let bright = 0.4 + 0.6 * age
        let c = float4(color.x * bright, color.y * bright, color.z * bright, alpha)
        push_quad(verts, px - dot_size, py - dot_size, px + dot_size, py + dot_size, c)
    }
}

// --- Circle helper ---

def push_circle(var verts : array<VisVertex>; cx, cy, radius : float; color : float4; segments : int = 48) {
    let step = 2.0 * PI / float(segments)
    for (i in range(segments)) {
        let a0 = float(i) * step
        let a1 = float(i + 1) * step
        verts |> push(VisVertex(xy = float2(cx, cy), rgba = color))
        verts |> push(VisVertex(xy = float2(cx + cos(a0) * radius, cy + sin(a0) * radius), rgba = color))
        verts |> push(VisVertex(xy = float2(cx + cos(a1) * radius, cy + sin(a1) * radius), rgba = color))
    }
}

// --- Drum panel state ---

struct DrumPad {
    note               : int
    sound              : string
    drum_label         : string
    brightness         : float = 0.1
    last_trigger_cycle : float = -1000.0
}

def drum_pad_key(h : PianorollHap) : string {
    return "{int(h.note)}:{h.sound}"
}

struct DrumPanelState {
    pads    : array<DrumPad>
    pad_map : table<string; int>   // "note:sound" -> pad index
}

let DRUM_FLASH_CYCLES = 0.05  // flash duration in cycles

def drum_panel_update(var state : DrumPanelState; render_states : array<PianorollRenderState>; tap_idx : int; current_cycle : float; dt : float) {
    if (tap_idx < 0 || tap_idx >= length(render_states)) {
        return
    }
    let rs & = unsafe(render_states[tap_idx])
    // register pads and detect triggers
    for (h in rs.haps) {
        let key = drum_pad_key(h)
        var pad_idx = -1
        if (!key_exists(state.pad_map, key)) {
            pad_idx = length(state.pads)
            state.pad_map[key] = pad_idx
            let lbl = empty(h.sound) ? note_name(h.note) : h.sound
            state.pads |> emplace(DrumPad(note = int(h.note), sound = h.sound, drum_label = lbl))
        } else {
            pad_idx = state.pad_map[key]
        }
        // trigger on hap start
        if (h.whole_start <= current_cycle && h.whole_start > unsafe(state.pads[pad_idx]).last_trigger_cycle) {
            unsafe(state.pads[pad_idx]).last_trigger_cycle = h.whole_start
            unsafe(state.pads[pad_idx]).brightness = 1.0
        }
    }
    // decay all pads
    for (si in range(length(state.pads))) {
        var pad & = unsafe(state.pads[si])
        let since = current_cycle - pad.last_trigger_cycle
        if (since > DRUM_FLASH_CYCLES) {
            pad.brightness = max(pad.brightness - dt * 8.0, 0.1)
        }
    }
}

def drum_panel_render(var verts : array<VisVertex>; state : DrumPanelState; x0, y0, x1, y1 : float; color : float4) {
    let n = length(state.pads)
    if (n == 0) {
        return
    }
    let w = x1 - x0
    let h = y1 - y0
    let mid_y = y0 + h * 0.5
    let spacing = w / float(n)
    let max_radius = min(spacing * 0.3, h * 0.3)
    for (i in range(n)) {
        let pad & = unsafe(state.pads[i])
        let cx = x0 + spacing * (float(i) + 0.5)
        let b = pad.brightness
        let radius = max_radius * (0.7 + 0.3 * b)
        let c = float4(color.x * b, color.y * b, color.z * b, 0.3 + 0.7 * b)
        push_circle(verts, cx, mid_y, radius, c)
        // dim ring outline at rest
        if (b < 0.3) {
            let ring_c = float4(color.x * 0.3, color.y * 0.3, color.z * 0.3, 0.4)
            push_circle(verts, cx, mid_y, max_radius, ring_c)
            let inner_c = float4(0.12, 0.12, 0.14, 1.0)
            push_circle(verts, cx, mid_y, max_radius * 0.9, inner_c)
        }
    }
}

// --- Pianoroll update ---

def pianoroll_update(var render_states : array<PianorollRenderState>; current_cycle : float; window_cycles : float) {
    pianoroll_get_taps() $(taps) {
        while (length(render_states) < length(taps)) {
            let idx = length(render_states)
            render_states |> emplace(PianorollRenderState(
                color = taps[idx].color,
                scroll = taps[idx].scroll,
                name = taps[idx].name,
                min_note = 127.0,
                max_note = 0.0,
                is_drum_mode = true
            ))
        }
        for (i in range(length(taps))) {
            let tap & = unsafe(taps[i])
            var rs & = unsafe(render_states[i])
            var new_haps : array<PianorollHap>
            tap.channel |> gather() $(h : PianorollHap#) {
                new_haps |> emplace(PianorollHap(
                    whole_start = h.whole_start,
                    whole_stop = h.whole_stop,
                    part_start = h.part_start,
                    part_stop = h.part_stop,
                    note = h.note,
                    sound := string(h.sound),
                    gain = h.gain,
                    tap_index = h.tap_index
                ))
            }
            for (hap in new_haps) {
                var found = false
                for (existing in rs.haps) {
                    if (abs(existing.whole_start - hap.whole_start) < 0.001 && abs(existing.note - hap.note) < 0.01 && existing.sound == hap.sound) {
                        found = true
                        break
                    }
                }
                if (!found) {
                    if (abs(hap.note - 60.0) > 0.01) {
                        rs.is_drum_mode = false
                    }
                    if (hap.note < rs.min_note) {
                        rs.min_note = hap.note
                    }
                    if (hap.note > rs.max_note) {
                        rs.max_note = hap.note
                    }
                    if (!key_exists(rs.sound_to_row, hap.sound)) {
                        let row = length(rs.sound_names)
                        rs.sound_to_row[hap.sound] = row
                        rs.sound_names |> push(hap.sound)
                    }
                    rs.haps |> push_clone(hap)
                }
            }
            delete new_haps
            let prune_before = current_cycle - window_cycles
            var j = length(rs.haps) - 1
            while (j >= 0) {
                if (rs.haps[j].whole_stop < prune_before) {
                    rs.haps |> erase(j)
                }
                j --
            }
        }
    }
}

// --- Pianoroll panel render ---

def pianoroll_render_panel(var verts : array<VisVertex>; render_states : array<PianorollRenderState>; tap_idx : int; current_cycle : float; window_cycles : float; display_w : int; py0, py1 : float) {
    if (tap_idx < 0 || tap_idx >= length(render_states)) {
        return
    }
    let rs & = unsafe(render_states[tap_idx])
    let ph = py1 - py0
    let fw = float(display_w)
    let half_window = window_cycles * 0.5
    let time_start = current_cycle - half_window
    let time_end = current_cycle + half_window
    var y_min = 0.0
    var y_max = 1.0
    if (rs.is_drum_mode) {
        y_min = -0.5
        y_max = float(max(length(rs.sound_names), 1)) - 0.5
    } else {
        y_min = rs.min_note - 2.0
        y_max = rs.max_note + 2.0
        if (y_max - y_min < 12.0) {
            let mid = (y_min + y_max) * 0.5
            y_min = mid - 6.0
            y_max = mid + 6.0
        }
    }
    // grid — cycle boundaries
    let grid_color = float4(0.3, 0.3, 0.35, 0.5)
    var cyc = ceil(time_start)
    while (cyc <= time_end) {
        let x = (float(cyc) - time_start) / (time_end - time_start) * fw
        push_quad(verts, x - 0.5, py0, x + 0.5, py1, grid_color)
        cyc += 1.0
    }
    // grid — horizontal
    if (rs.is_drum_mode) {
        for (row in range(length(rs.sound_names))) {
            let y_frac = (float(row) - y_min) / (y_max - y_min)
            let y = py0 + y_frac * ph
            push_quad(verts, 0.0, y - 0.5, fw, y + 0.5, float4(0.25, 0.25, 0.28, 0.3))
        }
    } else {
        var note_val = ceil(y_min)
        while (note_val <= y_max) {
            if (int(note_val) % 12 == 0) {
                let y_frac = (note_val - y_min) / (y_max - y_min)
                let y = py0 + y_frac * ph
                push_quad(verts, 0.0, y - 0.5, fw, y + 0.5, float4(0.3, 0.3, 0.35, 0.4))
            }
            note_val += 1.0
        }
    }
    // hap rectangles
    let base_color = uint_to_rgba(rs.color)
    for (h in rs.haps) {
        let x0 = (h.whole_start - time_start) / (time_end - time_start) * fw
        let x1 = (h.whole_stop - time_start) / (time_end - time_start) * fw
        var y_val : float
        if (rs.is_drum_mode) {
            y_val = float(rs.sound_to_row?[h.sound] ?? 0)
        } else {
            y_val = h.note
        }
        let row_height = ph / (y_max - y_min)
        let y_frac = (y_val - y_min) / (y_max - y_min)
        let y_center = py0 + y_frac * ph
        let half_h = row_height * 0.4
        let is_active = h.whole_start <= current_cycle && h.whole_stop > current_cycle
        if (is_active) {
            // bright, slightly larger
            let grow = half_h * 0.15
            let c = float4(min(base_color.x * 1.3, 1.0), min(base_color.y * 1.3, 1.0), min(base_color.z * 1.3, 1.0), 1.0)
            push_quad(verts, x0, y_center - half_h - grow, x1, y_center + half_h + grow, c)
        } else {
            let alpha = h.gain * 0.3
            push_quad(verts, x0, y_center - half_h, x1, y_center + half_h, float4(base_color.x, base_color.y, base_color.z, alpha))
        }
    }
    // playhead (center)
    let playhead_x = fw * 0.5
    push_quad(verts, playhead_x - 1.0, py0, playhead_x + 1.0, py1, float4(1.0, 1.0, 1.0, 0.8))
}

def render_panel_name(var fnt : Font; name : string; display_w, display_h : int; py0, py1 : float) {
    if (empty(name)) {
        return
    }
    var lbl <- fnt |> create_quads(name)
    let dim = quads_dim(lbl)
    let text_w = dim.vmax.x - dim.vmin.x
    let fh = float(display_h)
    let mid_y = (py0 + py1) * 0.5
    let screen_y = int(fh - mid_y - 6.0)
    let screen_x = int((float(display_w) - text_w) * 0.5)
    fnt |> draw_quads_2d(lbl, display_w, display_h, screen_x, screen_y, float3(0.6, 0.6, 0.65))
    delete lbl
}
