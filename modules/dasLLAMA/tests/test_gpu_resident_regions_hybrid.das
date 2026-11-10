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

[test(description = "On the Qwen3.5-0.8B hybrid two sessions with their own recurrent state slots, stepped one row at a time interleaved, each read what that session reads alone")]
def test_regions_interleaved_single_steps(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test(description = "On the hybrid two host-cached sessions sharing one region take it in turn: the loser's device-only rows and state are saved before the overwrite, and every step reads what each session reads alone")]
def test_region_taken_after_decode_hybrid(t : T?) {
    cell_region_taken_after_decode(t, REGIONS_FILE)
}

[test(description = "On the hybrid the N-row command's recurrent form advances each row's deltanet state in its region's slot with the gated q, a batched step reading bit for bit what each session reads alone")]
def test_regions_batched_steps(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)   // the N-row command's recurrent form: each row's deltanet step in its region's state slot, the gated q over the rows
}

[test(description = "On the hybrid a batched step before any one-row step, one-row steps, then a batched step again all read what each session reads alone")]
def test_regions_batched_first_hybrid(t : T?) {
    cell_regions_batched_first(t, REGIONS_FILE)
}

[test(description = "On the hybrid two device-home sessions with no host cache step singly and batched as a host-cached session alone reads, and a live device-home region is never handed to an outsider")]
def test_device_home_sessions(t : T?) {
    cell_device_home_sessions(t, REGIONS_FILE)
}

[test(description = "On the hybrid a device-mode scheduler emits the host-cached scheduler's tokens and lands picks alone for argmax streams, a recurrent turn prefilling whole where its prefill cannot continue")]
def test_scheduler_device_mode(t : T?) {
    cell_scheduler_device_mode(t, REGIONS_FILE)
}

[test(description = "On the hybrid a device-home stream prefills in device windows or whole, never the scheduler's 64-token chunks, and a tick hands two streams to one segmented window or one stream's call where it declines")]
def test_gpu_resident_regions_hybrid_scheduler_chunks(t : T?) {
    cell_scheduler_device_prefill_chunks(t, REGIONS_FILE)
    cell_scheduler_device_handoff(t, REGIONS_FILE)
}

[test(description = "On the hybrid a prefill continued past zero starts the window chain from the region's live recurrent state - a tail shorter than the conv taps included - within the split bar of one call")]
def test_prefill_continues_hybrid(t : T?) {
    cell_prefill_continues_on_the_device(t, REGIONS_FILE)
}

[test(description = "A recurrent parked claim hands its rows and its last-row state to an adopter of every row, which continues as one call would, and refuses a shorter opening since the state stands at no earlier row")]
def test_device_kv_park_adopt_hybrid(t : T?) {
    cell_device_kv_park_adopt_recurrent(t, REGIONS_FILE)
}

[test(description = "On the hybrid a paged scheduler's host-cached streams step on the device token for token as the flat scheduler's, and a second turn attaches the pages its first turn left")]
def test_scheduler_paged_resident(t : T?) {
    cell_scheduler_paged_resident(t, REGIONS_FILE)
}

[test(description = "A device-home recurrent stream's checkpoint at the system prompt's end serves the next request on that system prompt, which prefills only its own turn, token for token the host prefix cache's run")]
def test_device_checkpoint_adopt_hybrid(t : T?) {
    cell_device_checkpoint_adopt(t, REGIONS_FILE)
}

[test(description = "On the hybrid a device-home scheduler's openings and checkpoints outlive their regions in the host pool, later requests take them up as the host prefix cache does, and a capped pool drops the oldest tails first")]
def test_device_pool_hybrid(t : T?) {
    cell_device_pool(t, REGIONS_FILE)
}

[test(description = "Under a speculating device-home scheduler on the -MTP- hybrid, pooled openings carry the draft head's rows, and a later request drafts off them token for token as the host-cached speculating scheduler")]
def test_device_pool_mtp_hybrid(t : T?) {
    cell_device_pool_mtp(t, MTP_FILE)
}

[test(description = "An opening moved down to the host pool and up again carries the draft head's rows: the fresh session counts them and drafts what the session that went down drafts")]
def test_device_pool_head_rows_hybrid(t : T?) {
    cell_device_pool_head_rows(t, MTP_FILE)
}
