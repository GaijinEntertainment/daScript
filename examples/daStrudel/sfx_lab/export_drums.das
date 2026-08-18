options gen2
options indenting = 4

// Headless: one .das render function per finished drum .sfx, for splicing into strudel_synth.das.
//   Output: <temp dir>/drums_gen.das
//   Tom:    exported parametric (base_freq scales the modal pitch)
//   Run:    bin/daslang examples/daStrudel/sfx_lab/export_drums.das

require strudel/strudel_sfx
require daslib/fio
require strings

let DIR = "{get_das_root()}/examples/daStrudel/sfx_lab/sounds/"

def out_path() : string {
    let tmp = temp_directory_result()
    return path_join(tmp is value ? tmp as value : ".", "drums_gen.das")
}

struct DrumExport {
    sfx : string
    func : string
    pitched : bool
}

[export]
def main {
    let drums <- [
        DrumExport(sfx = "kick", func = "render_bd", pitched = false),
        DrumExport(sfx = "sd", func = "render_sd", pitched = false),
        DrumExport(sfx = "hh", func = "render_hh", pitched = false),
        DrumExport(sfx = "oh", func = "render_oh", pitched = false),
        DrumExport(sfx = "cp", func = "render_cp", pitched = false),
        DrumExport(sfx = "tom_low", func = "render_tom", pitched = true),
        DrumExport(sfx = "crash", func = "render_crash", pitched = false),
        DrumExport(sfx = "ride", func = "render_ride", pitched = false),
        DrumExport(sfx = "rimshot", func = "render_rimshot", pitched = false),
        DrumExport(sfx = "cowbell", func = "render_cowbell", pitched = false),
        DrumExport(sfx = "tambourine", func = "render_tambourine", pitched = false)
    ]
    let all = build_string() $(var writer) {
        for (d in drums) {
            var doc = Sound()
            if (!load_sound("{DIR}{d.sfx}.sfx", doc)) {
                to_log(LOG_ERROR, "{d.sfx}: load failed\n")
                continue
            }
            writer |> write(emit_das(doc, d.func, d.pitched))
            writer |> write("\n")
            to_log(LOG_INFO, "{d.func}: {length(doc.layers)} layers\n")
        }
    }
    let out = out_path()
    if (fwrite(out, all)) {
        to_log(LOG_INFO, "wrote {out}\n")
    }
}
