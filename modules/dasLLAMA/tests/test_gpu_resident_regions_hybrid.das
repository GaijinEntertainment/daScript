options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// Qwen3.5-0.8B Q8_0 (the deltanet hybrid: a recurrent state per session beside the attention layers' K/V): the rig's
// bit-for-bit cells - each recurrent layer keeps a device state slot per region, so the two sessions' batched steps
// advance their own states through the N-row command's recurrent form with the gated q over the rows - the
// continuation cells, whose window chain starts from the region's live state (a tail shorter than the conv taps
// among them), the parked claim that keeps its state beside its rows, and the scheduler's device mode, where a
// stream prefills in the device's windows and a second turn adopts the rows and the state of the turn it continues.
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
// Under DASLLAMA_DEVICE_POOL=0 the pool cell holds the off rail. The speculating pool cell loads the same hybrid's -MTP-
// file (MTP_FILE) and skips under DASLLAMA_DEVICE_POOL=0, without the file, on a file with no draft head, or where the
// driver does not run the whole speculative round on the device.
let private REGIONS_FILE = "Qwen3.5-0.8B-Q8_0.gguf"
let private MTP_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"   //! the same hybrid with its NextN head: the pool cell under a speculating scheduler

[test]
def test_regions_interleaved_single_steps(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test]
def test_region_taken_after_decode_hybrid(t : T?) {
    cell_region_taken_after_decode(t, REGIONS_FILE)
}

[test]
def test_regions_batched_steps(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)   // the N-row command's recurrent form: each row's deltanet step in its region's state slot, the gated q over the rows
}

[test]
def test_regions_batched_first_hybrid(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
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
    cell_scheduler_device_handoff(t, REGIONS_FILE)
}

[test]
def test_prefill_continues_hybrid(t : T?) {
    cell_prefill_continues_on_the_device(t, REGIONS_FILE)
}

[test]
def test_device_kv_park_adopt_hybrid(t : T?) {
    cell_device_kv_park_adopt_recurrent(t, REGIONS_FILE)
}

[test]
def test_scheduler_paged_resident(t : T?) {
    cell_scheduler_paged_resident(t, REGIONS_FILE)
}

[test]
def test_device_checkpoint_adopt_hybrid(t : T?) {
    cell_device_checkpoint_adopt(t, REGIONS_FILE)
}

[test]
def test_device_pool_hybrid(t : T?) {
    cell_device_pool(t, REGIONS_FILE)
}

[test]
def test_device_pool_mtp_hybrid(t : T?) {
    cell_device_pool_mtp(t, MTP_FILE)
}

[test]
def test_device_pool_head_rows_hybrid(t : T?) {
    cell_device_pool_head_rows(t, MTP_FILE)
}
