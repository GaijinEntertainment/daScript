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
// Under DASLLAMA_DEVICE_POOL=0 the pool cell holds the off rail.
let private REGIONS_FILE = "Qwen2.5-0.5B-Instruct-Q8_0.gguf"

[test(description = "On Qwen2.5-0.5B two sessions in separate mirror regions, prefilled and stepped one row at a time interleaved, each read what that session reads alone, the one-token-off logits the control")]
def test_regions_interleaved_single_steps(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test(description = "Two host-cached sessions sharing one region take it in turn: the loser's device-only rows are saved before the overwrite, and every step of both reads what each session reads alone")]
def test_region_taken_after_decode(t : T?) {
    cell_region_taken_after_decode(t, REGIONS_FILE)
}

[test(description = "A prefill continued from a position past zero - short tail, window-boundary cut, unaligned whole window, second chat turn - runs the window chain over the region's rows within the split bar of one call")]
def test_prefill_continues_on_the_device(t : T?) {
    cell_prefill_continues_on_the_device(t, REGIONS_FILE)
}

[test(description = "On Qwen2.5-0.5B a batched step over two sessions in their own regions reads bit for bit what each session reads alone, with no history resync")]
def test_regions_batched_steps(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test(description = "With both prompts past the unsplit position, the N-row command's split attention form batches two sessions bit for bit what each session reads alone")]
def test_regions_batched_steps_split(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test(description = "Two device-home sessions with no host cache step singly and batched as a host-cached session alone reads, every call served; a live device-home region is never handed to an outsider, which passes as busy")]
def test_device_home_sessions(t : T?) {
    cell_device_home_sessions(t, REGIONS_FILE)
}

[test(description = "A parked device claim hands its rows to one later session, which prefills only the rest within bar of the one-call prefill; the claim refuses an overlong ask, is spent by adoption and dies when its region is taken")]
def test_device_kv_park_adopt(t : T?) {
    cell_device_kv_park_adopt(t, REGIONS_FILE)
}

[test(description = "A device-mode scheduler emits the host-cached scheduler's tokens on the shared code path, lands picks alone for argmax streams, parks claims within its slots, and a second turn adopts its first turn's rows")]
def test_scheduler_device_mode(t : T?) {
    cell_scheduler_device_mode(t, REGIONS_FILE)
}

[test(description = "A device-home stream prefills a long prompt in device-window chunks rather than the scheduler's 64-token chunks, and a tick hands two streams to one segmented window where the model's window serves")]
def test_gpu_resident_regions_scheduler_chunks(t : T?) {
    cell_scheduler_device_prefill_chunks(t, REGIONS_FILE)
    cell_scheduler_device_handoff(t, REGIONS_FILE)
}

[test(description = "A third device-home session over two occupied regions panics with the reason and counts as busy, while the owners step on reading what they read alone")]
def test_gpu_resident_regions_guards(t : T?) {
    cell_device_home_guards(t, REGIONS_FILE)
}

[test(description = "Every CPU pass reason reads in distinct words, the plan scales K/V bytes with the region count and its weight tally matches the upload within 2%, and a switched-off route reports never attempted")]
def test_gpu_resident_regions_reports(t : T?) {
    cell_driver_reports(t, REGIONS_FILE)
}

[test(description = "A prefill that skips positions passes to the CPU as gap and one past the region's context as cap, each bringing the session's device rows down to the host first")]
def test_gpu_resident_regions_passes(t : T?) {
    cell_passes_with_their_rows(t, REGIONS_FILE)
}

[test(description = "A paged scheduler's host-cached streams step on the device token for token as the flat scheduler's, and a second turn attaches the pages its first turn left")]
def test_scheduler_paged_resident(t : T?) {
    cell_scheduler_paged_resident(t, REGIONS_FILE)
}

[test(description = "A device-home scheduler's openings outlive their regions in the host pool and later requests take them up token for token as the host prefix cache does; with no pool those requests prefill whole")]
def test_device_pool(t : T?) {
    cell_device_pool(t, REGIONS_FILE)
}
