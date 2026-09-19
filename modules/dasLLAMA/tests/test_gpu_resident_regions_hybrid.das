options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3.5-0.8B Q8_0 (the deltanet hybrid: a recurrent state per session beside the attention layers' K/V): the rig's
// bit-for-bit cells - each recurrent layer's one device state slot goes to whichever session steps, flushed home and
// brought back exactly - and the scheduler's device mode, where a recurrent model prefills each prompt whole and adopts
// no earlier rows. The continuation cells are not this model's: its window chain starts the deltanet state from zero.
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "Qwen3.5-0.8B-Q8_0.gguf"

[test]
def test_regions_interleaved_single_steps(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_device_home_sessions(t : T?) {
    cell_device_home_sessions(t, REGIONS_FILE)
}

[test]
def test_scheduler_device_mode(t : T?) {
    cell_scheduler_device_mode(t, REGIONS_FILE)
}

[test]
def test_gpu_resident_regions_hybrid_scheduler_chunks(t : T?) {
    cell_scheduler_device_prefill_chunks(t, REGIONS_FILE)
}
