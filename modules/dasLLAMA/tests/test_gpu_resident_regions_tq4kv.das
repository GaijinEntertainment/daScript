options gen2
options stack = 524288
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions
require dasllama/dasllama_kv_dtype

// Qwen2.5-0.5B-Instruct Q8_0 with the K/V mirror asked in tq4: every cell of the rig on the rotated 4-bit codec - the staged
// stores and the rotate-and-quantize pass at one row and at N rows, the decode attention's tq4 stamps and the pass that takes
// their rows out of the rotated basis, the prefill's attention over the f16 shadow, the rows a pass brings down as the CPU
// cache's own bytes. The sessions take the asked codec (`mirror_kv_dtype`); the tolerance cells hold the rig's tq4 bars.
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), or where
// DASLLAMA_VK_KV32=1 arms an f32 mirror over the codec asked.
let private REGIONS_FILE = "Qwen2.5-0.5B-Instruct-Q8_0.gguf"

[init]
def private tq4kv_init() {
    pin_file_kv(KVDtype.tq4)
}

[finalize]
def private tq4kv_restore() {
    unpin_file_kv()
}

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
