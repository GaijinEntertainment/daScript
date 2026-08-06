options gen2

module tuner_noise shared public

//! The noise gate shared by both dasllama tuner halves. A winner raced on a non-silent box is
//! a random number, and the sidecar is a FILE — one noisy mint persists for days and reads as
//! a code regression. So before (and during, and after) a race, probe the box: race one fixed
//! dot kernel at the real round shape, compute the cv, and REFUSE to tune above the threshold.
//! A noisy end-probe means the sidecar is not written at all.

require llvm/daslib/llvm_tune
require daslib/strings_boost
require daslib/fio            // popen + get_das_root — the engine-sha probe
require math

// the race regime of tune_kernels: one probe round costs the same as one real round there
let private PROBE_N = 4096
let private PROBE_REPS = 2000
let private PROBE_ROUNDS = 12

var private g_floor_pct = 0.0lf
var private g_overridden = false
var private g_paranoid = false
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
    // warm the code and the cache; a settle SPINS the probe kernel for the whole window
    // instead of sleeping — a slept thread wakes cold (E-core placement inflates the retry
    // ~2x on Apple silicon), a spinning one stays hot while an external transient dies out
    let w0 = ref_time_ticks()
    var warmed = false
    while (!warmed || get_time_usec(w0) < settle_ms * 1000) {
        for (_rep in range(PROBE_REPS)) {
            sink += probe_dot(pa, pb, PROBE_N)
        }
        warmed = true
    }
    var samples : array<double>
    samples |> reserve(PROBE_ROUNDS)
    for (_round in range(PROBE_ROUNDS)) {
        let t0 = ref_time_ticks()
        for (_rep in range(PROBE_REPS)) {
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

//! Switch every subsequent gate to the paranoid threshold (1% instead of 2%). Call before the
//! first gate; the flag also rides into the provenance stamp.
def noise_set_paranoid(p : bool) {
    g_paranoid = p
}

//! Probe the box and gate on the cv. True = quiet enough to keep measuring (a one-line verdict
//! prints either way, so drift is visible across a run and across history). False = REFUSED:
//! the caller must stop without writing a sidecar and exit nonzero. `DAS_TUNE_NOISE_OVERRIDE`
//! turns a refusal into a pass, and the sidecar provenance is stamped `noise: overridden`.
def noise_gate(tag : string) : bool {
    let thr = tune_noise_threshold_pct(g_paranoid)
    var med = 0.0lf
    var cv = probe_once(0, med)
    if (cv > thr) {
        // one settle-and-retry: a transient this run itself caused (a GPU race just drained,
        // turbo recovering, a composite walk's writeback draining) fades in seconds; noise
        // from OUTSIDE the process does not. 8s: the in-situ streaming composites leave the
        // memory subsystem hotter than the old 2s window was sized for
        tune_progress_note("noise probe ({tag}): cv {cv:0.2f}% > {thr:0.1f}% — settling 8s hot, re-probing\n")
        let cv0 = cv
        cv = probe_once(8000, med)
        g_probe_log |> push("{tag} cv {cv0:0.2f}% -> settled {cv:0.2f}%")
    } else {
        g_probe_log |> push("{tag} cv {cv:0.2f}%")
    }
    g_floor_pct = max(g_floor_pct, cv)
    if (cv <= thr) {
        tune_progress_note("noise probe ({tag}): cv {cv:0.2f}% <= {thr:0.1f}% — box is quiet (median {med:0.0f} us)\n")
        return true
    }
    tune_progress_finish()
    print("NOISE GATE ({tag}): cv {cv:0.2f}% > {thr:0.1f}% (median {med:0.0f} us) — this box is not quiet enough to tune\n")
    if (tune_noise_override()) {
        g_overridden = true
        print("  DAS_TUNE_NOISE_OVERRIDE is set — tuning anyway; the sidecar will carry 'noise: overridden'\n")
        return true
    }
    print("  a winner raced in this noise is a lottery ticket, not a measurement — a sidecar minted now would poison every run that reads it\n")
    print("  quiet the box (close apps, stop background work) and re-run: the cv above should fall under the threshold\n")
    print("  escape hatches: DAS_TUNE_NOISE_CV=<pct> recalibrates the gate; DAS_TUNE_NOISE_OVERRIDE=1 mints anyway, stamped\n")
    return false
}

//! Record this run's noise verdict into the sidecar provenance. Call once, right before the
//! sidecar write, after the end-of-run gate passed.
def noise_provenance_stamp() {
    tune_provenance_note("noise", g_overridden ? "overridden" : "ok")
    tune_provenance_note("noise_probes", join(g_probe_log, "; "))
    tune_provenance_note("noise_floor_cv_pct", "{g_floor_pct:0.2f}")
    tune_provenance_note("mode", g_paranoid ? "paranoid" : "normal")
    tune_provenance_note("engine_sha", engine_sha())
}

// the engine commit this mint measured, "unknown" outside a repo (the shipped SDK)
def private engine_sha() : string {
    var sha = ""
    unsafe {
        popen("git -C \"{get_das_root()}\" rev-parse --short HEAD 2>/dev/null") $(f) {
            if (f != null) {
                sha = strip(fgets(f))
            }
        }
    }
    return empty(sha) ? "unknown" : sha
}
