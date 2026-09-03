options gen2
options _dasllama_internal = true
options stack = 524288

require dasllama/dasllama_g2p
require dasllama/dasllama_postag
require strings
require daslib/fio
require daslib/clargs
require dasllama/dasllama_env

// The das half of the G2P sweep: phonemize every row of a tts_g2p_sweep.py JSON (fed the
// reference-normalized text) and count the rows identical to the reference; every difference
// is printed for hand classification. Usage:
//   daslang -jit harness/tts_g2p_sweep.das -- --rows sweep.json [--models <dir>]

struct SweepRow {
    id : string
    text : string
    norm : string
    ps_misaki : string
}

[CommandLineArgs]
struct SweepArgs {
    @clarg_doc = "the tts_g2p_sweep.py output"
    @clarg_required
    rows : string = ""
    @clarg_doc = "directory holding tts_g2p.bin and tts_postag.bin (default: the models dir)"
    models : string = ""
}

[export]
def main() : int {
    var args = SweepArgs()
    let rc = parse_args_with_help(args, "harness/tts_g2p_sweep.das")
    return rc if (rc >= 0)
    let dir = empty(args.models) ? (empty(g_env_harness.models_dir) ? "/Users/borisbatkin/Work/llama.cpp/models/" : g_env_harness.models_dir) : args.models
    let g <- load_g2p(path_join(dir, "tts_g2p.bin"))
    let tg <- load_pos_tagger(path_join(dir, "tts_postag.bin"))
    let f = fopen(args.rows, "rb")
    if (f == null) {
        to_log(LOG_ERROR, "cannot open {args.rows}\n")
        return 2
    }
    let text = fread(f)
    fclose(f)
    var rows : array<SweepRow>
    if (!sscan_json(text, rows)) {
        to_log(LOG_ERROR, "{args.rows} does not parse\n")
        return 2
    }
    var same = 0
    for (r in rows) {
        let got = strip(g2p_phonemize(g, tg, r.norm))
        if (got == r.ps_misaki) {
            same++
        } else {
            to_log(LOG_INFO, "{r.id}\n  text {r.text}\n  got  {got}\n  want {r.ps_misaki}\n")
        }
    }
    to_log(LOG_INFO, "sweep: {same}/{length(rows)} rows phoneme-identical with the reference ({float(same) * 100.0 / float(length(rows))} percent)\n")
    return 0
}
