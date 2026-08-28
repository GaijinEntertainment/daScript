options gen2
options indenting = 4

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require math

// Source-level probe matrix. Where the AST generator builds trees no source can
// produce, this builds ordinary source text and asks the same question: does the
// compiler survive it? Three matrices:
//
//   mismatch  a local whose declared type and initializer disagree, then a use that
//             inference const-folds - the shape that found the string/bitfield crashes
//   legal     the same uses over legally typed values, so a finding here needs no
//             type error at all (a control: this one is expected to stay empty)
//   depth     deeply nested expressions and types, for stack exhaustion
//
// A compiler ERROR is the expected outcome for the mismatch matrix - only a crash, a
// hang or an abnormal exit counts.

let PROBE_DIR = "utils/internal/ast-fuzz/__probes"

let PRELUDE = "options gen2\n\nstruct S \{\n    a : int\n\}\n\nenum E \{\n    a\n    b\n\}\n\nbitfield BF \{\n    m\n    n\n\}\n\nvariant V \{\n    i : int\n    s : string\n\}\n"

let MISMATCH_TYPES = fixed_array("int", "uint", "int64", "int8", "float", "double",
    "bool", "string", "int2", "float3", "range", "array<int>", "table<string; int>",
    "int?", "tuple<int; string>", "V", "iterator<int>", "block<() : void>",
    "lambda<() : void>", "S", "S?", "E", "BF")

let WRONG_LITS = fixed_array("2", "2.5", "\"s\"", "true", "2l", "2u", "null", "'c'",
    "E.a", "[[S a = 1]]")

let USES = fixed_array("var z = x + x", "var z = x + 2", "var z = x + \"a\"",
    "var z = x == x", "var z = x < x", "var z = -x", "var z = !x", "x ++",
    "var z = x[0]", "var z = x.a", "var z = x?.a", "var z = *x", "var z = x ?? 1",
    "var z = x is i", "var z = x as i", "var z = int(x)", "var z = string(x)",
    "var z = hash(x)", "var z = length(x)", "var z = invoke(x)", "for (q in x) \{ \}",
    "var z := x", "var z <- x", "unsafe \{ delete x \}", "x |> push(1)",
    "var z = \"\{x\}\"", "assert(x)", "var z = true ? x : x",
    "var z = typeinfo sizeof(x)", "var z = typeinfo typename(x)")

let LEGAL_DECLS = fixed_array("let x = 2", "let x = 2u", "let x = 2l", "let x = 2.5",
    "let x = 2.5lf", "let x = true", "let x = \"s\"", "let x = int2(1, 2)",
    "let x = float3(1.0, 2.0, 3.0)", "let x = range(0, 4)", "var x = [\{int 1; 2\}]",
    "var x <- \{ \"k\" => 1 \}", "var x = new int", "var x = new S",
    "let x = [[S a = 1]]", "let x = E.a", "let x = BF.m", "let x = BF.m | BF.n",
    "let x = [[V i = 1]]", "let x = [[tuple<int; string> 1, \"s\"]]")

let DEPTHS = fixed_array(8, 32, 128, 512, 2048, 8192)

// Cycles and self-reference: a declaration that refers to itself, directly or through a
// second declaration. Each one must be diagnosed, not chased until something breaks.
let CYCLES = fixed_array(
    "struct S \{\n    a : S\n\}",
    "struct A \{\n    b : B\n\}\nstruct B \{\n    a : A\n\}",
    "struct S \{\n    a : S?\n    b : array<S>\n\}",
    "variant V \{\n    a : V\n\}",
    "variant V \{\n    a : tuple<V; int>\n\}",
    "class C : C \{\n\}",
    "class A : B \{\n\}\nclass B : A \{\n\}",
    "typedef T = T",
    "typedef A = B\ntypedef B = A",
    "var g = g",
    "var a = b\nvar b = a",
    "let a = b\nlet b = a",
    "enum E \{\n    a = 1\n    b = int(E.a) + 1\n\}",
    "struct S \{\n    a : int = S().a\n\}",
    "def f(a) \{\n    return f(a)\n\}\ndef probe2 \{\n    let z = f(1)\n\}",
    "def f(a) \{\n    return f([\{typeof(a) a\}])\n\}\ndef probe2 \{\n    let z = f(1)\n\}",
    "def f(a) \{\n    return f(f(a))\n\}\ndef probe2 \{\n    let z = f(1)\n\}",
    "def f(a : auto(TT)) : TT \{\n    return f(a)\n\}\ndef probe2 \{\n    let z = f(1)\n\}",
    "struct S \{\n    a : int\n\}\ndef S(x : S) : S \{\n    return S(x)\n\}",
    "template t(a) \{\n    return t(a)\n\}")

// Structural garbage: text that is not a program. The parser has to reject it, and the
// error path has to survive whatever partial tree it built.
// Token-for-token swaps that keep a file parseable, so the mutant reaches inference
// instead of dying in the parser. The var->let row is the class that found the
// const-initializer folding crash.
// Tokens spliced in during mutation: enough to derail a parse without being noise.
let GARBAGE = fixed_array(
    "def", "def f", "def f \{", "def f \{\n    let", "let x =",
    "def f \{\n    return\n", "struct", "struct S \{", "enum E \{",
    "\}\n\}\n\}", "def f() : \{\n\}", "def f(: int) \{\n\}",
    "def f \{\n    x = = 1\n\}", "def f \{\n    ((((\n\}",
    "def f \{\n    let x : = 1\n\}", "def f \{\n    for (in) \{\n    \}\n\}",
    "options", "options =", "options gen2 = ", "require",
    "def f \{\n    var x <- <- 1\n\}", "def f \{\n    x?.?.y\n\}",
    "def f \{\n    [[\n\}", "def f \{\n    \{\{\n\}",
    "def f \{\n    unsafe\n\}", "def f \{\n    return return\n\}",
    "class", "variant V \{", "bitfield B \{", "typedef",
    "def f \{\n    a := := b\n\}", "def f \{\n    \\\\\n\}")

[CommandLineArgs]
struct Config {
    @clarg_short = "b"
    @clarg_doc = "Path to the daslang binary under test (default: bin/daslang)"
    bin : string

    @clarg_doc = "Run the sweep in N parallel shards (default 1)"
    threads : int

    @clarg_doc = "Shard id, 1-based. Set by --threads; pass by hand to run one shard"
    shard : int

    @clarg_name = "shard-count"
    @clarg_doc = "Total shard count, set together with --shard"
    shard_count : int

    @clarg_doc = "Per-probe timeout in seconds (default 20)"
    timeout : float

    @clarg_doc = "Only the mismatch matrix (declared type vs initializer)"
    mismatch : bool

    @clarg_doc = "Only the legal matrix - the control that should report nothing"
    legal : bool

    @clarg_doc = "Only the depth probes"
    depth : bool

    @clarg_doc = "Only the cycle probes - declarations that refer to themselves"
    cycles : bool

    @clarg_doc = "Only the structural-garbage probes - text that is not a program"
    garbage : bool





    @clarg_doc = "Keep every generated probe instead of only the ones that crashed"
    keep : bool

    @clarg_doc = "Run only every Nth probe. The gate samples the matrix; a full sweep does not"
    sample : int

    @clarg_name = "slow-secs"
    @clarg_doc = "Report a probe that COMPILES but takes longer than this (default 8). A compile-time blowup that finishes is invisible to the timeout, so it is tracked separately"
    slow_secs : float

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct private Probe {
    name : string
    text : string
}


// a type spelling is not a filename, so flatten it
def private slug(ty : string) : string {
    var s = ty |> replace(" ", "")
    s = s |> replace("<", "_")
    s = s |> replace(">", "")
    s = s |> replace(";", "")
    s = s |> replace("?", "p")
    s = s |> replace("(", "")
    s = s |> replace(")", "")
    s = s |> replace(":", "")
    s = s |> replace(",", "")
    return s
}

def private mismatch_probes(var out : array<Probe>) : void {
    out |> reserve(length(out) + length(MISMATCH_TYPES) * length(WRONG_LITS) * length(USES))
    for (ty in MISMATCH_TYPES) {
        let tyn = slug(ty)
        for (lit, li in WRONG_LITS, count()) {
            for (use, ui in USES, count()) {
                var p = Probe(name = "m_{tyn}_{li}_{ui}",
                    text = "{PRELUDE}\ndef probe \{\n    let x : {ty} = {lit}\n    {use}\n\}\n")
                out |> push_clone(p)
            }
        }
    }
}

def private legal_probes(var out : array<Probe>) : void {
    out |> reserve(length(out) + length(LEGAL_DECLS) * length(USES))
    for (decl, di in LEGAL_DECLS, count()) {
        for (use, ui in USES, count()) {
            var p = Probe(name = "l_{di}_{ui}",
                text = "{PRELUDE}\ndef probe \{\n    {decl}\n    {use}\n\}\n")
            out |> push_clone(p)
        }
    }
}

def private rep_str(s : string; n : int) : string {
    return build_string() $(var w) {
        for (_i in range(n)) {
            w |> write(s)
        }
    }
}

def private depth_probes(var out : array<Probe>) : void {
    out |> reserve(length(out) + length(DEPTHS) * 4)
    for (d in DEPTHS) {
        var p = Probe(name = "d_paren_{d}",
            text = "options gen2\ndef probe \{\n    var z = {rep_str("(", d)}1{rep_str(")", d)}\n\}\n")
        out |> push_clone(p)
        var q = Probe(name = "d_neg_{d}",
            text = "options gen2\ndef probe \{\n    var z = {rep_str("-", d)}1\n\}\n")
        out |> push_clone(q)
        var r = Probe(name = "d_type_{d}",
            text = "options gen2\ndef probe \{\n    var z : {rep_str("array<", d)}int{rep_str(">", d)}\n\}\n")
        out |> push_clone(r)
        var t = Probe(name = "d_add_{d}",
            text = "options gen2\ndef probe \{\n    var z = 1{rep_str(" + 1", d)}\n\}\n")
        out |> push_clone(t)
    }
}

def private cycle_probes(var out : array<Probe>) : void {
    out |> reserve(length(out) + length(CYCLES))
    for (decl, di in CYCLES, count()) {
        var p = Probe(name = "c_{di}",
            text = "options gen2\n{decl}\n\ndef probe \{\n\}\n")
        out |> push_clone(p)
    }
}

def private garbage_probes(var out : array<Probe>) : void {
    out |> reserve(length(out) + length(GARBAGE))
    for (txt, gi in GARBAGE, count()) {
        var p = Probe(name = "g_{gi}", text = "options gen2\n{txt}\n")
        out |> push_clone(p)
    }
}

def private build_probes(cfg : Config) : array<Probe> {
    var all : array<Probe>
    let any = cfg.mismatch || cfg.legal || cfg.depth || cfg.cycles || cfg.garbage
    if (!any || cfg.mismatch) {
        mismatch_probes(all)
    }
    if (!any || cfg.legal) {
        legal_probes(all)
    }
    if (!any || cfg.depth) {
        depth_probes(all)
    }
    if (!any || cfg.cycles) {
        cycle_probes(all)
    }
    if (!any || cfg.garbage) {
        garbage_probes(all)
    }
    return <- all
}

def private is_crash(exit_code : int; out : string) : bool {
    if (exit_code == popen_timed_out) {
        return false
    }
    if (exit_code >= 128 || exit_code < 0) {
        return true
    }
    return find(out, "CRASH:") >= 0 || find(out, "Segmentation fault") >= 0
}

def private run_shard(cfg : Config; bin : string; probes : array<Probe>;
                      shard, shard_count : int; timeout : float) : int {
    var nCrash = 0
    var nHang = 0
    var nSlow = 0
    var nRun = 0
    let slowLimit = cfg.slow_secs > 0.0 ? cfg.slow_secs : 8.0
    let path = "{PROBE_DIR}/__probe{shard}.das"
    let step = max(cfg.sample, 1)
    for (p, i in probes, count()) {
        if (i % step != 0 || (i / step) % shard_count != shard - 1) {
            continue
        }
        nRun++
        let ptext = p.text
        if (!fwrite(path, ptext)) {
            to_log(LOG_ERROR, "probe: cannot write {path}\n")
            return 2
        }
        var out : string
        let t0 = ref_time_ticks()
        let ec = run_and_capture([bin, "-compile-only", path], out, timeout)
        let secs = float(get_time_usec(t0)) / 1000000.0
        if (secs > slowLimit && ec != popen_timed_out) {
            nSlow++
            to_log(LOG_ERROR, "probe: SLOW {p.name} {secs} s\n")
            fwrite("{PROBE_DIR}/slow_{p.name}.das", ptext)
        }
        if (ec == popen_timed_out) {
            nHang++
            to_log(LOG_ERROR, "probe: HANG {p.name}\n")
            fwrite("{PROBE_DIR}/hang_{p.name}.das", ptext)
        } elif (is_crash(ec, out)) {
            nCrash++
            let banner = find(out, "CRASH:") >= 0 ? slice(out, find(out, "CRASH:")) : ""
            to_log(LOG_ERROR, "probe: CRASH {p.name} exit={ec} {strip(slice(banner, 0, min(length(banner), 70)))}\n")
            fwrite("{PROBE_DIR}/crash_{p.name}.das", ptext)
        } elif (cfg.keep) {
            fwrite("{PROBE_DIR}/kept_{p.name}.das", ptext)
        }
    }
    to_log(LOG_INFO, "probe: shard {shard}/{shard_count} — {nRun} probe(s), {nCrash} CRASH, {nHang} HANG, {nSlow} SLOW\n")
    return nCrash + nHang + nSlow > 0 ? 1 : 0
}

def private run_threaded(cfg : Config; bin : string; threads : int;
                         nprobes : int) : int {
    let cmd = build_string() $(var w) {
        for (i in range(threads)) {
            w |> write("nice -n 10 '{bin}' utils/internal/ast-fuzz/probe.das -- --bin '{bin}'")
            w |> write(" --shard {i + 1} --shard-count {threads}")
            if (cfg.mismatch) {
                w |> write(" --mismatch")
            }
            if (cfg.legal) {
                w |> write(" --legal")
            }
            if (cfg.depth) {
                w |> write(" --depth")
            }
            if (cfg.cycles) {
                w |> write(" --cycles")
            }
            if (cfg.garbage) {
                w |> write(" --garbage")
            }
            if (cfg.slow_secs > 0.0) {
                w |> write(" --slow-secs {cfg.slow_secs}")
            }
            if (cfg.sample > 1) {
                w |> write(" --sample {cfg.sample}")
            }
            if (cfg.timeout > 0.0) {
                w |> write(" --timeout {cfg.timeout}")
            }
            w |> write(" > {PROBE_DIR}/__shard{i + 1}.log 2>&1 & ")
        }
        w |> write("wait")
    }
    to_log(LOG_INFO, "probe: {nprobes} probe(s) across {threads} shard(s)\n")
    var out : string
    let ec = run_and_capture(["/bin/sh", "-c", cmd], out, 60.0 * 60.0)
    if (ec == popen_timed_out) {
        to_log(LOG_ERROR, "probe: the sweep exceeded its hour budget\n")
        return 2
    }
    var crashes = 0
    for (i in range(threads)) {
        for (ln in split(fread("{PROBE_DIR}/__shard{i + 1}.log"), "\n")) {
            if (find(ln, "CRASH ") >= 0 || find(ln, "HANG ") >= 0
                    || find(ln, "SLOW ") >= 0) {
                to_log(LOG_ERROR, "  {strip(ln)}\n")
                crashes++
            } elif (find(ln, "probe(s),") >= 0) {
                to_log(LOG_INFO, "  {strip(ln)}\n")
            }
        }
    }
    to_log(LOG_INFO, "probe: {crashes} finding(s); repros in {PROBE_DIR}/crash_*.das\n")
    return crashes > 0 ? 1 : 0
}

[export]
def main : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "probe")
        return 2
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "probe")
        return 0
    }
    let bin = empty(cfg.bin) ? "bin/daslang" : cfg.bin
    let timeout = cfg.timeout > 0.0 ? cfg.timeout : 20.0
    mkdir(PROBE_DIR)
    let probes <- build_probes(cfg)
    if (cfg.shard > 0 && cfg.shard_count > 0) {
        return run_shard(cfg, bin, probes, cfg.shard, cfg.shard_count, timeout)
    }
    if (cfg.threads > 1) {
        return run_threaded(cfg, bin, cfg.threads, length(probes))
    }
    return run_shard(cfg, bin, probes, 1, 1, timeout)
}
