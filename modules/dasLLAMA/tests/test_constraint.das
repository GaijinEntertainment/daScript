options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the facade's load and session, inside the model-gated cell
require dasllama/dasllama_common
require dasllama/dasllama_sampling
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the model-gated cell
require _model_tier

// The sampler under a token constraint: the engine's `TokenConstraint` decides which tokens may
// come next, and the sampler never emits one it refuses - the argmax when it passes, the best admitted
// token in logit order when it does not, the admitted survivors alone under a temperature - and tells the
// constraint which token it emitted. Model-gated on tinyllama (its session sizes the logits row); the
// logits themselves are written by the cell.

// a constraint admitting the ids `admit` and recording every commit
class AdmitOnly : TokenConstraint {
    admit : table<int64>
    committed : array<int64>

    def override admits(tok : int64) : bool => admit |> key_exists(tok)

    def override commit(tok : int64) : void {
        committed |> push(tok)
    }
}

def private admit_only(var admit : table<int64>) : AdmitOnly? {
    var c = new AdmitOnly()
    c.admit <- admit
    return c
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

def private constrained_sampler_cell(tt : T?; path : string) {
        var m <- load_model_(path, QuantMode.q8)
        var s <- create_session_(m)
        let vocab = long_length(s.logits)
        tt |> success(vocab > 1000l, "a vocab-wide logits row")
        for (i in range64(vocab)) {
            s.logits[i] = -100.0
        }
        s.logits[7l] = 10.0      // the argmax
        s.logits[12l] = 9.0      // the best admitted
        s.logits[9l] = 8.5       // admitted too, lower
        s.logits[300l] = 8.0     // past the probe window's reach only if nothing nearer admits
        var admit <- { 12l, 9l, 300l }
        let greedy = SamplingParams()
        tt |> equal(7l, sample_(s, greedy), "unconstrained greedy: the argmax")
        var c1 = admit_only(admit)   // the test owns each constraint; the session carries the pointer and never frees it
        s.constrain = c1
        tt |> equal(12l, sample_(s, greedy), "constrained greedy: the best admitted token, the argmax refused")
        tt |> success(s.sampled, "a constrained pick stands the greedy chain down")
        tt |> equal(1, length(c1.committed), "one commit")
        tt |> equal(12l, c1.committed[0], "... of the emitted token")
        // only a far token admitted: the walk reaches past the probe window
        var far <- { 300l }
        var c2 = admit_only(far)
        s.constrain = c2
        tt |> equal(300l, sample_(s, greedy), "the whole row is walked when the k largest are all refused")
        tt |> equal(300l, c2.committed[0], "and committed")
        // a temperature: every draw lands on an admitted token
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
        s.constrain = null
        tt |> equal(7l, sample_(s, greedy), "unconstrained again: the argmax")
        unsafe {
            delete c1
            delete c2
            delete c3
        }
        delete s
        delete m
}
