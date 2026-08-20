options gen2
options _dasllama_internal = true

module tuner_noise shared public

//! The noise probe shared by both dasllama tuner halves — the probe is the measurement, the
//! gate is the one refusal built on it:
//!   races  one fixed dot kernel in ~20 ms windows, at the start, mid-run, and at the end
//!   stamps `noise` (ok / noisy / overridden), `noise_probes`, `noise_floor_cv_pct`
//!   gates  only a BUSY box — cv past the hard ceiling — refuses; the halves' margin rule
//!          is what keeps weather out of the winners

require dasllama/dasllama_lint public
require llvm/daslib/llvm_tune
require dasllama/dasllama_version  // DASLLAMA_VERSION — stamped into sidecar provenance (zero-require leaf)
require tuner_phrases  // provenance phrase leaf — render-charset-pinned by test_exchange_schema
require daslib/strings_boost
require daslib/fio            // popen + get_das_root — the engine-sha probe
require math

// the probe kernel shape; reps scale so one timed window lands near PROBE_WINDOW_US — the
// same window as tune_kernels' TARGET_WINDOW_US, so the probe measures the race's regime
let private PROBE_N = 4096
let private PROBE_REPS_BASE = 2000
let private PROBE_WINDOW_US = 20000.0lf
let private PROBE_MAX_SCALE = 32
let private PROBE_ROUNDS = 12
// the hard ceiling: a probe cv past this is a box doing something else, and a race there
// measures that something; below it the cv is stamped and the margin rule decides. A
// DAS_TUNE_NOISE_CV recalibration above it raises it too — you asked for that gate, you get it
let private HARD_CV_PCT = 10.0lf

var private g_floor_pct = 0.0lf
var private g_overridden = false
var private g_noisy = false
var private g_probe_log : array<string>

// fixed probe kernel — four accumulators like the hand vec bodies, deterministic, cache-resident
def private probe_dot(a : float const?; b : float const?; n : int) : float {
    var s0 = 0.0f
    var s1 = 0.0f
    var s2 = 0.0f
    var s3 = 0.0f
    var j = 0
    while (j < n) {
        s0 += unsafe(a[j]) * unsafe(b[j])
        s1 += unsafe(a[j + 1]) * unsafe(b[j + 1])
        s2 += unsafe(a[j + 2]) * unsafe(b[j + 2])
        s3 += unsafe(a[j + 3]) * unsafe(b[j + 3])
        j += 4
    }
    return (s0 + s1) + (s2 + s3)
}

def private probe_once(settle_ms : int; var med_out : double&) : double {
    var a : array<float>
    var b : array<float>
    a |> resize(PROBE_N)
    b |> resize(PROBE_N)
    for (i in range(PROBE_N)) {
        a[i] = sin(float(i) * 0.1f)
        b[i] = cos(float(i) * 0.07f)
    }
    let pa = unsafe(addr(a[0]))
    let pb = unsafe(addr(b[0]))
    var sink = 0.0f
    // a settle SPINS the probe kernel for the whole window instead of sleeping - a slept thread
    // wakes cold (E-core placement inflates the retry ~2x on Apple silicon)
    let w0 = ref_time_ticks()
    var warmed = false
    var reps = PROBE_REPS_BASE
    while (!warmed || get_time_usec(w0) < settle_ms * 1000) {
        let b0 = ref_time_ticks()
        for (_rep in range(PROBE_REPS_BASE)) {
            sink += probe_dot(pa, pb, PROBE_N)
        }
        if (!warmed) {
            let base_us = double(get_time_usec(b0))
            if (base_us > 0.0lf) {
                reps = PROBE_REPS_BASE * clamp(int(PROBE_WINDOW_US / base_us), 1, PROBE_MAX_SCALE)
            }
        }
        warmed = true
    }
    var samples : array<double>
    samples |> reserve(PROBE_ROUNDS)
    for (_round in range(PROBE_ROUNDS)) {
        let t0 = ref_time_ticks()
        for (_rep in range(reps)) {
            sink += probe_dot(pa, pb, PROBE_N)
        }
        samples |> push(double(get_time_usec(t0)))
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    let cv = tune_cv_pct(samples)
    med_out = tune_median(samples)
    delete samples
    return cv
}

//! The highest probe cv seen so far this run, in percent — the measured noise floor a winner
//! must beat the shipped fallback by before it deserves the seat.
def noise_floor_pct() : double {
    return g_floor_pct
}

//! Probe the box. True = keep measuring; the verdict line prints either way, so drift is
//! visible across a run and across history. Over the note threshold stamps `noise: noisy`; over
//! the hard ceiling refuses (False - the caller exits nonzero, nothing written) unless
//! `DAS_TUNE_NOISE_OVERRIDE` turns it into a pass stamped `noise: overridden`.
def noise_gate(tag : string) : bool {
    let thr = tune_noise_threshold_pct(false)
    let hard = max(HARD_CV_PCT, thr)
    // active overrides announce themselves: a threshold a recalibration shaped says so inline
    let thrTag = tune_noise_threshold_overridden() ? " (DAS_TUNE_NOISE_CV)" : ""
    var med = 0.0lf
    var cv = probe_once(0, med)
    if (cv > thr) {
        // one settle-and-retry: a transient this run itself caused (a GPU race just drained,
        // turbo recovering, a composite walk's writeback draining) fades in seconds; noise
        // from OUTSIDE the process does not. 8s: the in-situ streaming composites leave the
        // memory subsystem hotter than the old 2s window was sized for
        tune_progress_note("noise probe ({tag}): cv {cv:0.2f}% > {thr:0.1f}%{thrTag} - settling 8s hot, re-probing\n")
        let cv0 = cv
        cv = probe_once(8000, med)
        g_probe_log |> push(settle_note(tag, cv0, cv))
    } else {
        g_probe_log |> push("{tag} cv {cv:0.2f}%")
    }
    g_floor_pct = max(g_floor_pct, cv)
    if (cv <= thr) {
        tune_progress_note("noise probe ({tag}): cv {cv:0.2f}% <= {thr:0.1f}%{thrTag} - box is quiet (median {med:0.0f} us)\n")
        return true
    }
    if (cv <= hard) {
        g_noisy = true
        tune_progress_note("noise probe ({tag}): cv {cv:0.2f}% > {thr:0.1f}%{thrTag} - box is not quiet (median {med:0.0f} us); racing on, the mint will carry 'noise: noisy' and only margin-clear winners\n")
        return true
    }
    tune_progress_finish()
    print("NOISE GATE ({tag}): cv {cv:0.2f}% > {hard:0.1f}% (median {med:0.0f} us) - this box is busy, not merely noisy\n")
    if (tune_noise_override()) {
        g_overridden = true
        print("  DAS_TUNE_NOISE_OVERRIDE is set - tuning anyway; the sidecar will carry 'noise: overridden'\n")
        return true
    }
    print("  a winner raced on a busy box measures whatever else is running - stop that, and re-run\n")
    print("  escape hatch: DAS_TUNE_NOISE_OVERRIDE=1 mints anyway, stamped\n")
    return false
}

//! Hard-stop when the supervisor requested it (the `DAS_TUNE_CONTROL` file exists). Both
//! tuner halves poll this between kernel families, so the measurement in flight always
//! completes; minting is end-gated, so an interrupted run writes nothing. Exits 1 — the
//! policy guard then reports the scope incomplete and the supervisor owns what happens next.
def interrupt_gate(at : string) {
    if (tune_interrupt_requested()) {
        tune_progress_finish()
        print("@tune abort reason=interrupted at={at}\n")
        print("tuner: supervisor interrupt at '{at}' - stopping, no sidecar written\n")
        unsafe(exit(1))
    }
}

//! Record this run's noise verdict into the sidecar provenance. Call once, right before the
//! sidecar write, after the end-of-run gate passed.
def noise_provenance_stamp() {
    tune_provenance_note("noise", g_overridden ? "overridden" : (g_noisy ? "noisy" : "ok"))
    tune_provenance_note("noise_probes", join(g_probe_log, "; "))
    tune_provenance_note("noise_floor_cv_pct", "{g_floor_pct:0.2f}")
    tune_provenance_note("mode", "normal")   // one protocol; the key stays for readers of older sidecars
    tune_provenance_note("engine_sha", engine_sha())
    tune_provenance_note("dasllama_version", "{DASLLAMA_VERSION}")
}

// the engine commit this mint measured, "unknown" outside a repo (the shipped SDK)
def private engine_sha() : string {
    var sha = ""
    unsafe {
        // 2>NUL on windows: cmd.exe reads 2>/dev/null as a FILE path and leaks
        // "The system cannot find the path specified." into the tuner output
        popen("git -C \"{get_das_root()}\" rev-parse --short HEAD 2>{get_platform_name() == "windows" ? "NUL" : "/dev/null"}") $(f) {
            if (f != null) {
                sha = strip(fgets(f))
            }
        }
    }
    return empty(sha) ? "unknown" : sha
}
