options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the facade's load and session, inside the model-gated cell
require dasllama/dasllama_common
require dasllama/dasllama_sampling
require dasllama/dasllama_scheduler
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the model-gated cell
require _model_tier

// The sampler under a token constraint: the engine's `TokenConstraint` decides which tokens may
// come next, and the sampler never emits one it refuses - the argmax when it passes, the best admitted
// token in logit order when it does not, the admitted survivors alone under a temperature, -1 and the
// session's dead-end flag when it admits nothing - and tells the constraint which token it emitted; a
// scheduler stream at a dead end finishes `constraint` with no token emitted. Model-gated on tinyllama
// (its session sizes the logits row) and JIT only; the logits themselves are written by the cell.

// a constraint admitting the ids `admit` (every id when `all`), counting its probes and recording every commit
class AdmitOnly : TokenConstraint {
    admit : table<int64>
    all : bool = false
    probes : int = 0
    committed : array<int64>

    def override admits(tok : int64) : bool {
        probes++
        return all || admit |> key_exists(tok)
    }

    def override commit(tok : int64) : void {
        committed |> push(tok)
    }
}

def private admit_only(var admit : table<int64>) : AdmitOnly? {
    var c = new AdmitOnly()
    c.admit <- admit
    return c
}

// both sides of an id compare as text, for the log
def private log_pick(m : Model; what : string; want, got : int64) {
    to_log(LOG_INFO, "{what}: want {want} {want >= 0l ? escape(piece(m, want)) : "<none>"}, got {got} {got >= 0l ? escape(piece(m, got)) : "<none>"}\n")
}

[test]
def test_constrained_sampler(t : T?) {
    t |> run("the sampler emits only admitted tokens, greedy and sampled, and commits what it emits (tinyllama Q8)") <| @(tt : T?) {
        let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        with_dasllama_jobque() {
            constrained_sampler_cell(tt, path)
        }
    }
}

[cold_path]   // a test cell: it drives the hot sampler
def private constrained_sampler_cell(tt : T?; path : string) {
    var m <- load_model_(path, QuantMode.q8)
    var s <- create_session_(m)
    let vocab = long_length(s.logits)
    tt |> success(vocab > 2000l, "a vocab-wide logits row")
    for (i in range64(vocab)) {
        s.logits[i] = -100.0
    }
    s.logits[7l] = 10.0      // the argmax
    s.logits[12l] = 9.0      // the best admitted
    s.logits[9l] = 8.5       // admitted too, lower
    for (i in range64(1000l, 1000l + CONSTRAINED_PROBE_K)) {
        s.logits[i] = 8.4    // CONSTRAINED_PROBE_K refused tokens above the far one: they fill the probe window
    }
    s.logits[300l] = 8.0     // admitted, but past the probe window: only the whole-row walk reaches it
    var admit <- { 12l, 9l, 300l }
    let greedy = SamplingParams()
    let plain = sample_(s, greedy)
    log_pick(m, "unconstrained greedy", 7l, plain)
    tt |> equal(7l, plain, "unconstrained greedy: the argmax")
    var c1 = admit_only(admit)   // the test owns each constraint; the session carries the pointer and never frees it
    s.constrain = c1
    let near = sample_(s, greedy)
    log_pick(m, "constrained greedy", 12l, near)
    tt |> equal(12l, near, "constrained greedy: the best admitted token, the argmax refused")
    tt |> success(s.sampled, "a constrained pick stands the greedy chain down")
    tt |> equal(1, length(c1.committed), "one commit")
    tt |> equal(12l, c1.committed[0], "... of the emitted token")
    // only the far token admitted: the probe window holds CONSTRAINED_PROBE_K refused tokens, so the whole row is walked
    var far <- { 300l }
    var c2 = admit_only(far)
    s.constrain = c2
    let walked = sample_(s, greedy)
    log_pick(m, "whole-row walk", 300l, walked)
    tt |> equal(300l, walked, "the whole row is walked when the probe window's {CONSTRAINED_PROBE_K} largest are all refused")
    tt |> equal(300l, c2.committed[0], "and committed")
    tt |> success(long_length(s.cand) == vocab, "the walk's candidate list is the whole row ({long_length(s.cand)} of {vocab})")
    // nothing admitted: a dead end, -1 and the flag, no commit
    var nothing : table<int64>
    var c0 = admit_only(nothing)
    s.constrain = c0
    s.constrain_dead = false
    let dead = sample_(s, greedy)
    log_pick(m, "dead end", -1l, dead)
    tt |> equal(-1l, dead, "a dead end returns -1")
    tt |> success(s.constrain_dead, "... and raises the session's flag")
    tt |> equal(0, length(c0.committed), "... committing nothing")
    s.constrain_dead = false
    let sampled_dead = sample_(s, SamplingParams(temp = 1.0, top_k = 40l))
    tt |> equal(-1l, sampled_dead, "the sampled path reaches the same dead end")
    tt |> success(s.constrain_dead, "... and raises the flag too")
    s.constrain_dead = false
    sampled_draws_cell(tt, m, s, c2)
    s.constrain = null
    tt |> equal(7l, sample_(s, greedy), "unconstrained again: the argmax")
    unsafe {
        delete c0
        delete c1
        delete c2
    }
    delete s
    scheduler_dead_end_cell(tt, m)
    delete m
}

// the sampled path over the same row: admitted survivors alone, with top-k on and off, and the empty list's fallback
[cold_path]
def private sampled_draws_cell(tt : T?; m : Model; var s : Session; var far : AdmitOnly?) {
    var admit3 <- { 12l, 9l }
    var c3 = admit_only(admit3)
    s.constrain = c3
    let sampled = SamplingParams(temp = 1.0, top_k = 40l)
    var all_admitted = true
    for (_ in range(50)) {
        let tok = sample_(s, sampled)
        all_admitted = all_admitted && (tok == 12l || tok == 9l)
    }
    tt |> success(all_admitted, "fifty sampled draws, every one admitted")
    tt |> equal(50, length(c3.committed), "fifty commits")
    // the draw goes first: a constraint that admits everything is probed once a draw, never once a candidate
    var nothing : table<int64>
    var c4 = admit_only(nothing)
    c4.all = true
    s.constrain = c4
    for (_ in range(50)) {
        sample_(s, sampled)
    }
    tt |> equal(50, c4.probes, "fifty draws under an admit-all constraint: one probe each")
    tt |> equal(50, length(c4.committed), "... and fifty commits")
    s.constrain = c3
    // top-k off: the draw still filters, over the capped candidate list
    let no_topk = sample_(s, SamplingParams(temp = 1.0, top_k = 0l))
    tt |> success(no_topk == 12l || no_topk == 9l, "a draw with top-k off lands on an admitted token")
    // only a token outside the top-k admitted: the filtered list is empty, so the greedy walk takes over
    s.constrain = far
    let outside = sample_(s, sampled)
    log_pick(m, "sampled, admitted token outside top-k", 300l, outside)
    tt |> equal(300l, outside, "an empty filtered list falls back to the best admitted token of the row")
    unsafe {
        delete c3
        delete c4
    }
}

// a stream whose constraint admits nothing finishes `constraint` on its first token, emitting none
[cold_path]
def private scheduler_dead_end_cell(tt : T?; m : Model) {
    var sch <- create_scheduler(m, 1l, 2l, 64l)
    var prompt <- encode(m, "The capital of France is")
    var req <- PendingReq(id = 1l, params = SamplingParams(), max_new = 8l)
    req.prompt <- prompt
    var nothing : table<int64>
    req.constraint.constraint = admit_only(nothing)
    tt |> success(submit(m, sch, req), "the constrained request is queued")
    var events : array<SchedEvent>
    var finish = ""
    var pieces = 0
    var ticks = 0
    while (finish == "" && ticks < 64) {
        scheduler_step(m, sch, events)
        for (e in events) {
            if (e.kind == SchedEventKind.finished) {
                finish = e.finish
            } else {
                pieces++
            }
        }
        events |> clear()
        ticks++
    }
    tt |> equal("constraint", finish, "the stream finishes on the constraint's dead end")
    tt |> equal(0, pieces, "... having emitted no token")
    delete sch
}
