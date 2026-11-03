options gen2

require math
require take                 // with_alpha
require imgui/imgui_harness  // the boost widgets and the draw list

// The screen pieces parrot and the interpreter share: the palette, the waveform box and the status head - the promise line
// and the state chip.

let INK = rgba(236, 229, 210, 255)
let DIM = rgba(141, 133, 119, 255)
let ACCENT = rgba(242, 185, 85, 255)
let RED = rgba(226, 84, 74, 255)
let GREEN = rgba(155, 196, 106, 255)
let PANEL = rgba(24, 22, 30, 255)
let WAVE = rgba(120, 160, 220, 255)
let WAVE_DIM = rgba(80, 90, 120, 255)

//! the shaded window is [window_from, window_to); the box spans `span` samples at least, so a growing picture keeps its scale
def draw_wave(peaks : array<float2>; size : float2; per_column : int; span, window_from, window_to, playhead : int64; ticks : array<int64>) {
    let origin = GetCursorScreenPos()
    let cols = length(peaks)
    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(origin, float2(origin.x + size.x, origin.y + size.y), PANEL, 4.0)
        let mid = origin.y + size.y * 0.5
        dl |> add_line(float2(origin.x, mid), float2(origin.x + size.x, mid), WAVE_DIM, 1.0)
        if (cols > 0) {
            let total = max(int64(cols) * int64(per_column), span)
            let spanned_cols = int(total / int64(per_column))
            let per_px = max(1.0, float(spanned_cols) / size.x)
            let x_of = @(sample : int64) => origin.x + size.x * float(double(sample) / double(max(total, 1l)))
            if (window_to > window_from) {
                dl |> add_rect_filled(float2(x_of(window_from), origin.y), float2(x_of(window_to), origin.y + size.y), rgba(242, 185, 85, 40), 0.0)
            }
            var px = 0.0
            let drawn_px = min(size.x, float(cols) / per_px)
            while (px < drawn_px) {
                let c0 = int(px * per_px)
                let c1 = min(cols, int((px + 1.0) * per_px) + 1)
                var lo = 1.0
                var hi = -1.0
                for (c in range(c0, c1)) {
                    lo = min(lo, peaks[c].x)
                    hi = max(hi, peaks[c].y)
                }
                if (hi >= lo) {
                    let y0 = mid - hi * size.y * 0.48
                    let y1 = mid - lo * size.y * 0.48
                    dl |> add_line(float2(origin.x + px, y0), float2(origin.x + px, max(y1, y0 + 1.0)), WAVE, 1.0)
                }
                px++
            }
            for (t in ticks) {
                dl |> add_line(float2(x_of(t), origin.y), float2(x_of(t), origin.y + size.y), WAVE_DIM, 1.0)
            }
            if (playhead > 0l && playhead <= total) {
                dl |> add_line(float2(x_of(playhead), origin.y), float2(x_of(playhead), origin.y + size.y), ACCENT, 2.0)
            }
        }
    }
    dummy(WAVE_BOX, (size = size))
}

//! the promise line, then the chip (pulsing, a spinner beside it while `on`); leaves the cursor where the status line starts
def draw_status_head(promise : string; chip : tuple<text : string; color : uint>; on : bool; elapsed_s : float) {
    with_font(GetFont(), GetStyle().FontSizeBase * 1.15) {   // the base is the unscaled size: the current one over the style's scale
        with_style((ImGuiCol.Text, rgba(255, 252, 244, 255))) {
            text_wrapped(PROMISE_LINE, (text = promise))
        }
    }
    spacing()
    let origin = GetCursorScreenPos()
    let h = GetTextLineHeight() + 12.0
    let pulse = on ? 0.62 + 0.38 * (0.5 + 0.5 * sin(elapsed_s * 4.0)) : 1.0
    let chip_w = 14.0 + CalcTextSize(chip.text).x + 14.0
    let spinner_w = on ? 36.0 : 0.0
    dummy(CHIP_BOX, (size = float2(chip_w + spinner_w, h)))   // the row reserved first, so nothing drawn after lands on the chip
    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(origin, float2(origin.x + chip_w, origin.y + h), with_alpha(chip.color, pulse), 6.0)
        if (on) {
            let c = float2(origin.x + chip_w + 18.0, origin.y + h * 0.5)   // a spinner: an open arc turning once a second
            let a0 = elapsed_s * 6.2832
            dl |> path_clear()
            dl |> path_arc_to(c, 7.0, a0, a0 + 4.4, 16)
            dl |> path_stroke(chip.color, ImDrawFlags.None, 2.5)
        }
        dl |> add_text(float2(origin.x + 14.0, origin.y + 6.0), rgba(20, 18, 24, 255), chip.text)
    }
    same_line((spacing = 14.0))
    SetCursorScreenPos(float2(origin.x + chip_w + spinner_w + 14.0, origin.y + 6.0))
}
