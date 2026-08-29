options gen2
options indenting = 4
options _dasllama_internal = true

// dastest fixture, spawned by test_tune_interrupt.das: one interrupt_gate() call between a
// BEFORE and an AFTER print. While DAS_TUNE_CONTROL names an existing file the gate must
// abort (exit 1, nothing past it runs); otherwise it passes through.

require ../harness/tuner_noise.das

[export]
def main {
    print("PROBE: before gate\n")
    interrupt_gate("probe_family")
    print("PROBE: gate passed through\n")
}
