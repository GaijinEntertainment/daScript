options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen2.5-0.5B-Instruct Q8_0 (the smallest carrier the whole-model driver takes): every cell of the rig - two sessions
// in two mirror regions, the prefill continued from a position past zero, device-home sessions, the region pin, the
// parked claim and the scheduler's device mode. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver
// serves, or without the file under models_dir().
let private REGIONS_FILE = "Qwen2.5-0.5B-Instruct-Q8_0.gguf"

[test]
def test_regions_interleaved_single_steps(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test]
def test_prefill_continues_on_the_device(t : T?) {
    cell_prefill_continues_on_the_device(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps_split(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test]
def test_device_home_sessions(t : T?) {
    cell_device_home_sessions(t, REGIONS_FILE)
}

[test]
def test_device_kv_park_adopt(t : T?) {
    cell_device_kv_park_adopt(t, REGIONS_FILE)
}

[test]
def test_scheduler_device_mode(t : T?) {
    cell_scheduler_device_mode(t, REGIONS_FILE)
}

[test]
def test_gpu_resident_regions_scheduler_chunks(t : T?) {
    cell_scheduler_device_prefill_chunks(t, REGIONS_FILE)
}

[test]
def test_gpu_resident_regions_guards(t : T?) {
    cell_device_home_guards(t, REGIONS_FILE)
}

[test]
def test_gpu_resident_regions_reports(t : T?) {
    cell_driver_reports(t, REGIONS_FILE)
}

[test]
def test_gpu_resident_regions_passes(t : T?) {
    cell_passes_with_their_rows(t, REGIONS_FILE)
}
