options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // the umbrella alone - what lcpp_bench and the rigs require; never the facade
require dasllama/dasllama_common

// A program that requires the engine umbrella and not the facade still carries the fat exe's
// first-start race: the umbrella's [init] set registers `dasllama_fat_first_start` with the
// box-profile apply. The shipped bench exe is such a program, and without this registration its
// first start on a Metal box would serve the twins uncrowned forever and never write a sidecar.

def private fake_race() : bool {
    return false
}

[test]
def test_the_umbrella_registers_the_first_start_hook(t : T?) {
    t |> run("requiring the umbrella alone installs a first-start race hook") @(t : T?) {
        let old = set_runtime_race_hook(@@fake_race)
        t |> success(old != null, "the umbrella's [init] set left a first-start race hook installed")
        set_runtime_race_hook(old)
    }
}

[export]
def main {
    return
}
