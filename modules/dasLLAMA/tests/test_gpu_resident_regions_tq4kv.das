options gen2
options stack = 524288
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions
require dasllama/dasllama_kv_dtype

// Qwen3-0.6B Q8_0 (the qwen3 regions file's carrier: a Q/K/V-biased attention serves a tq4 ask at q8_0, so Qwen2.5 cannot
// carry this file) with the K/V mirror asked in tq4: every cell of the rig on the rotated 4-bit codec - the staged
// stores and the rotate-and-quantize pass at one row and at N rows, the decode attention's tq4 stamps and the pass that takes
// their rows out of the rotated basis, the prefill's attention over the f16 shadow, the rows a pass brings down as the CPU
// cache's own bytes. The sessions take the asked codec (`mirror_kv_dtype`); the tolerance cells hold the rig's tq4 bars.
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), or where
// DASLLAMA_VK_KV32=1 arms an f32 mirror over the codec asked.
let private REGIONS_FILE = "Qwen3-0.6B-Q8_0.gguf"

[test(description = "Under a tq4 K/V mirror two Qwen3-0.6B sessions in separate regions, stepped one row at a time interleaved, each hold the tq4 bar of that session alone, the one-token-off row the control")]
def test_regions_interleaved_single_steps(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_regions_interleaved_single_steps(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror a prefill continued from a position past zero, and a second chat turn, run the window chain over the rotated region rows within the tq4 bar of the one-call form")]
def test_prefill_continues_on_the_device(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_prefill_continues_on_the_device(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror, with N-row staged stores and the rotate-and-quantize pass, a batched step over two sessions reads what each session reads alone")]
def test_regions_batched_steps(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_regions_batched_steps(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror with both prompts past the unsplit position, the split N-row command batches two sessions as each reads alone")]
def test_regions_batched_steps_split(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_regions_batched_steps_split(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror two device-home sessions step singly and batched as a host-cached session alone reads, and a live device-home region is never handed to an outsider")]
def test_device_home_sessions(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_device_home_sessions(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror a parked claim hands its rows to one later session, which prefills only the rest within bar of the one-call prefill; the claim is spent by adoption and dies when its region is taken")]
def test_device_kv_park_adopt(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_device_kv_park_adopt(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror a device-mode scheduler emits the host-cached scheduler's tokens over the codec's agreeing prefix, and a second turn adopts its first turn's rows")]
def test_scheduler_device_mode(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_scheduler_device_mode(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror a device-home stream prefills in device-window chunks, and a tick hands two streams to one segmented window where the model's window serves")]
def test_gpu_resident_regions_scheduler_chunks(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_scheduler_device_prefill_chunks(t, REGIONS_FILE)
        cell_scheduler_device_handoff(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror a third device-home session over two occupied regions panics with the reason and counts as busy, while the owners step on as before")]
def test_gpu_resident_regions_guards(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_device_home_guards(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror every CPU pass reason reads in distinct words, the plan scales tq4 K/V bytes with the region count and its weight tally matches the upload within 2%")]
def test_gpu_resident_regions_reports(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_driver_reports(t, REGIONS_FILE)
    }
}

[test(description = "Under a tq4 mirror a prefill that skips positions passes as gap and one past the region's context as cap, each bringing the rotated device rows down to the host first")]
def test_gpu_resident_regions_passes(t : T?) {
    with_kv_dtype(KVDtype.tq4) {
        cell_passes_with_their_rows(t, REGIONS_FILE)
    }
}
