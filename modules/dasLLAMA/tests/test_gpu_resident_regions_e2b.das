options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _resident_regions

// gemma-4-E2B Q8_0 (the E-series carrier: a per-layer-embedding side input per token, the upper layers reading the K/V
// of the ones below): the rig's bit-for-bit cells - the N-row command hands each row its own side input and projects
// q alone on a shared-KV layer - and the
// scheduler's device mode. The tolerance cells stay the qwen2 file's: their bar is calibrated there. Skips off the JIT,
// without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir(); the split-forms and the
// fused q/k norm cells skip under DASLLAMA_CUDA=1 - the fuse knob is the Vulkan driver's.
let private REGIONS_FILE = "gemma-4-E2B-it-Q8_0.gguf"

//! nine rows over nine regions: past the N-row command's eight the step runs two commands, each group's per-layer-embedding
//! side rows read from the group's own row base
[test(description = "On gemma-4-E2B nine sessions batched past the N-row command's eight run as two command groups, each group reading its side rows from its own row base, and every row reads bit for bit what its session reads alone")]
def test_regions_batched_groups_e2b(t : T?) {
    cell_regions_batched_groups(t, REGIONS_FILE, 9l, LogitsBar.bit_exact)
}

[test(description = "On gemma-4-E2B every per-layer-embedding side row, alone and batched, gathers on the device off its token id with no host table row, and the batched rows read bit for bit what each session reads alone")]
def test_regions_batched_side_rows_e2b(t : T?) {
    cell_regions_batched_side_rows(t, REGIONS_FILE)
}

[test(description = "On gemma-4-E2B the batched rows gather their token rows on the device off their ids, logits bit for bit those of a load whose host embeds every row, the gather-off counters the control")]
def test_regions_batched_embed_gather_e2b(t : T?) {
    cell_regions_batched_embed_gather(t, REGIONS_FILE)
}

[test(description = "On gemma-4-E2B two sessions in separate mirror regions, stepped one row at a time interleaved, each read what that session reads alone, the one-token-off logits the control")]
def test_regions_interleaved_single_steps(t : T?) {
    cell_regions_interleaved_single_steps(t, REGIONS_FILE)
}

[test(description = "On gemma-4-E2B a batched step over two sessions hands each row its own side input, projects q alone on shared-KV layers, and reads bit for bit what each session reads alone")]
def test_regions_batched_steps(t : T?) {
    cell_regions_batched_steps(t, REGIONS_FILE)
}

[test(description = "On gemma-4-E2B with both prompts past the unsplit position, the N-row command's split attention form batches two sessions bit for bit what each reads alone")]
def test_regions_batched_steps_split(t : T?) {
    cell_regions_batched_steps_split(t, REGIONS_FILE)
}

[test(description = "On gemma-4-E2B with the fuse knob off for the load, batched rows hold bit for bit to sessions alone on the split one-row forms, where the fused act and projection would round apart")]
def test_regions_batched_steps_split_forms(t : T?) {
    cell_regions_batched_steps_split_forms(t, REGIONS_FILE)
}

[test(description = "On gemma-4-E2B two device-home sessions with no host cache step singly and batched as a host-cached session alone reads, and a live device-home region is never handed to an outsider")]
def test_device_home_sessions(t : T?) {
    cell_device_home_sessions(t, REGIONS_FILE)
}

[test(description = "On gemma-4-E2B a device-mode scheduler emits the host-cached scheduler's tokens, a second turn adopts its first turn's rows, and a tick hands two streams to one segmented window where it serves")]
def test_scheduler_device_mode(t : T?) {
    cell_scheduler_device_mode(t, REGIONS_FILE)
    cell_scheduler_device_handoff(t, REGIONS_FILE)
}

//! the window's fused q/k norm, rope and store on a carrier whose shared-KV layers project q alone: the fused form runs no norm
//! pass, a sharer's q heads riding the fused pass with no k heads; the wide bar, since the two norm orders put the E-series' fed
//! steps 7.4-7.6% of the max logit apart where the prompt's row holds the split bar
[test(description = "On gemma-4-E2B, whose shared-KV layers project q alone, the window's fused q/k norm, rope and store reads within the wide bar of the split pair, the fused form dispatching no norm pass")]
def test_window_qkn_fused_e2b(t : T?) {
    cell_window_qkn_fused(t, REGIONS_FILE, LogitsBar.wide_bar)
}
