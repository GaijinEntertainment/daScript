options gen2
options indenting = 4
options _dasllama_internal = true

module _seam_race shared public

//! The in-process seam race the CUDA races share (`tower_race.das`, `tts_race.das`): a candidate is `key=value` pairs
//! over a table of seams, resolved against the values the race found; the preamble every race main runs; the rounds,
//! every candidate once a round, the order rotated a round; the summary's distinct hashes. A race supplies its seams'
//! getter and setter, its pass and its lines.

require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime, the standing pre-measure stack
require daslib/clargs
require math
require daslib/strings_convert
require strings

let SEAM_KEEP = -1l   //! a seam the candidate does not name: the value the race found

//! one seam a candidate may set: its key, the values it takes (empty = 0 or more) and those values in words
struct RaceSeam {
    key : string
    allowed : array<int64>
    words : string
}

//! one candidate: its spec, and its value for each seam in the table's order, `SEAM_KEEP` where it names none
struct RaceCand {
    name : string
    vals : array<int64>
}

//! the seam `kv` (one `key=value` pair) sets and its value, or the reason it sets none
def race_seam_value(seams : array<RaceSeam>; kv : string) : tuple<at : int; n : int64; why : string> {
    let eq = find(kv, "=")
    return (at = -1, n = 0l, why = "'{kv}' is not key=value") if (eq <= 0)
    let key = slice(kv, 0, eq)
    let v = try_to_int64(slice(kv, eq + 1))
    return (at = -1, n = 0l, why = "'{kv}': the value is not an integer") if (v |> is_err)
    let n = v |> unwrap
    let at = find_index_if(seams) $(s) => s.key == key
    if (at < 0) {
        let keys = join([for (s in seams); s.key], ", ")
        return (at = -1, n = n, why = "unknown seam '{key}' ({keys})")
    }
    let s & = unsafe(seams[at])
    let fits = empty(s.allowed) ? n >= 0l : find_index(s.allowed, n) >= 0
    return (at = -1, n = n, why = "{key}={n}: {s.words}") if (!fits)
    return (at = at, n = n, why = "")
}

//! the values candidate `c` runs its seams at: its own, the found one where it names none
def race_resolve(c : RaceCand; found : array<int64>) : array<int64> =>
    <- [for (v, f in c.vals, found); v != SEAM_KEEP ? v : f]

//! the `RACE seams found` line: each seam's key and the value the race found
def race_seams_line(seams : array<RaceSeam>; found : array<int64>) : string {
    let words = join([for (s, v in seams, found); "{s.key} {v}"], ", ")
    return "RACE seams found: {words}\n"
}

//! the distinct hashes of a candidate's passes, in hex, space-separated
def race_distinct_hashes(hashes : array<uint64>) : string {
    var distinct <- {for (h in hashes); h}
    return join([for (h in keys(distinct)); "{h:x}"], " ")
}

//! `turn(ci, round)` for every candidate `ci` of `count` a round, `rounds` rounds, the order rotated a round
def race_rounds(rounds, count : int; turn : block<(ci, round : int) : void>) {
    for (round in range(rounds)) {
        for (i in range(count)) {
            turn((i + round) % count, round)
        }
    }
}

//! A race main's preamble: the arguments (`argt` the args struct: `help`, `cand`, `rounds`, `warm`), the JIT, the
//! CUDA module (`owner` names whose seams the race sets), two candidates or more parsed over `seams`, the rounds and
//! warm clamped to 1, the box profile and the tune gate; then `run` with them. Answers the exit code.
def race_main(tool, owner : string; argt : auto(TT); seams : array<RaceSeam>;
              run : block<(var cfg : TT -const; cands : array<RaceCand>) : int>) : int {
    var r <- _::parse_args(type<TT -const>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(_::get_command_info(type<TT -const>), tool)
        return 1
    }
    var cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(_::get_command_info(type<TT -const>), tool)
        return 0
    }
    if (!jit_enabled()) {
        print("JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    static_if (!typeinfo builtin_module_exists(cuda)) {
        print("{tool}: this build carries no CUDA module - the seams it races are {owner}\n")
        return 1
    }
    if (length(cfg.cand) < 2) {
        print("{tool}: a race needs two candidates or more (--cand, repeated)\n")
        return 1
    }
    var cands : array<RaceCand>
    cands |> reserve(length(cfg.cand))
    for (spec in cfg.cand) {
        var c = RaceCand(name = spec, vals <- [for (_s in seams); SEAM_KEEP])
        for (kv in split(spec, ",")) {
            let sv = race_seam_value(seams, kv)
            if (!empty(sv.why)) {
                print("{tool}: --cand {spec}: {sv.why}\n")
                return 1
            }
            c.vals[sv.at] = sv.n
        }
        cands |> emplace(c)
    }
    cfg.rounds = max(cfg.rounds, 1)
    cfg.warm = max(cfg.warm, 1)
    apply_box_profile_runtime()
    if (!tune_gate()) {
        print("{tool}: tune_gate refused - point DAS_TUNE_MANIFEST at a complete sidecar\n")
        return 1
    }
    return run(cfg, cands)
}
