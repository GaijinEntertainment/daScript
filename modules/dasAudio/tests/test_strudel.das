options gen2
options gc
options persistent_heap
require strudel/strudel
require strudel/strudel_pattern
require strudel/strudel_player
require audio/audio_boost
require daslib/archive
require daslib/fio
require daslib/jobque_boost
require daslib/runtime_memory
require dastest/testing_boost public

//! The strudel player on the null device: its worker's memory and
//! shutdown, collection between steps, the refill telemetry of
//! threaded and main-thread playback, and track controls.

struct private WorkerGarbage { values : array<int> }
var private observed_track = -1
var private worker_title = ""
var private before_collection = 0ul

def private idle_worker(){}

def private setup_collecting_track(){
    worker_title = "track {17}"
    let pat <- note("c4 e4 g4", "sine")
    observed_track = strudel_add_track(pat, .5)
}
def private observe_collecting_track(command : string){
    if (command == "churn"){
        before_collection = gc_collection_stats().x
        var orphan = new WorkerGarbage(values <- [for (i in range(800000)); i])
        orphan = null
        strudel_set_cps(.75lf)
    }else{
        verify(gc_collection_stats().x > before_collection)
        verify(worker_title == "track 17")
        verify(strudel_track_gain(observed_track) == .5)
        strudel_fade_track(observed_track, .25, 0.0)
        verify(strudel_track_gain(observed_track) == .25)
        strudel_set_cps(.9lf)
    }
}
def private wait_worker_tempo(t : T?; tempo : double){
    var ready = false
    for (_i in range(200)){
        let (_, cps) = strudel_get_playback_time()
        if (cps == tempo){ready = true; break}; sleep(10u)
    }
    t |> success(ready, "worker publishes changed tempo after processing a command")
}

[test(description = "Each of three strudel worker starts publishes the worker's own context memory and shutdown clears it, main-thread playback publishes none, and the worker's memory box is released at the end.", timeout = 1200)]
def strudel_worker_memory_and_shutdown_balance(t : T?){
    let before = count_jobque_leaks()
    sound_set_null_device(true)
    with_audio_system() $ {
        for (_cycle in range(3)){
            strudel_init(@@idle_worker)
            var memory = ContextMemory()
            for (_i in range(200)){sleep(10u); memory = strudel_get_worker_memory(); if (memory.stack > 0ul){break}}
            t |> success(memory.stack > 0ul && memory.heap_reserved > 0ul, "worker publishes its own context")
            strudel_shutdown()
            t |> equal(strudel_get_worker_memory().stack, 0ul)
        }
        strudel_create_channel(); strudel_tick()
        t |> equal(strudel_get_worker_memory().stack, 0ul, "main-thread playback has no worker snapshot")
        strudel_shutdown()
    }
    sound_set_null_device(false)
    for (_i in range(100)){if (count_jobque_leaks() == before){break}; sleep(1u)}
    t |> equal(count_jobque_leaks(), before, "worker capture releases its memory box")
}

[test(description = "A garbage collection on the strudel worker between steps keeps its tracks, globals and gains intact, and the worker goes on processing commands and generating audio.", timeout = 1200)]
def strudel_tracks_and_commands_survive_between_step_collection(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        strudel_init(@@setup_collecting_track, @@observe_collecting_track)
        strudel_command("churn"); wait_worker_tempo(t, .75lf)
        strudel_command("inspect"); wait_worker_tempo(t, .9lf)
        let stats = strudel_get_diagnostics()
        t |> success(stats.generated_frames > 0ul && stats.consumed_frames > 0ul)
        strudel_shutdown()
    }
    sound_set_null_device(false)
}

[test(description = "Threaded strudel playback publishes refill counts, generated and consumed frames and refill timings.", timeout = 1200)]
def threaded_playback_publishes_consumer_and_refill_observations(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        strudel_init(@@idle_worker)
        var stats = StrudelDiagnostics()
        for (_i in range(100)){sleep(10u); stats = strudel_get_diagnostics(); if (stats.refills > 2ul && stats.consumed_frames > 0ul){break}}
        t |> success(stats.refills > 0ul && stats.generated_frames > 0ul)
        t |> success(stats.consumed_frames > 0ul)
        t |> success(stats.max_refill_ms >= 0.0 && stats.max_refill_gap_ms >= 0.0)
        strudel_shutdown()
    }
    sound_set_null_device(false)
}

[test(description = "Main-thread strudel playback reports itself unthreaded with refills and consumed frames, and shutdown resets the frame counters to zero.", timeout = 1200)]
def main_thread_playback_reports_consumption_and_resets(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        strudel_create_channel()
        var stats = StrudelDiagnostics()
        for (_i in range(100)){
            strudel_tick(); sleep(10u); stats = strudel_get_diagnostics()
            if (stats.refills > 2ul && stats.consumed_frames > 0ul){break}
        }
        t |> success(!stats.threaded)
        t |> success(stats.refills > 0ul && stats.generated_frames > 0ul)
        t |> success(stats.consumed_frames > 0ul)
        strudel_shutdown()
        let reset = strudel_get_diagnostics()
        t |> equal(reset.generated_frames, 0ul)
        t |> equal(reset.consumed_frames, 0ul)
    }
    sound_set_null_device(false)
}

[test(description = "Under a strudel pause or the global pause the threaded worker stops generating once its bounded queue is full, holding at most six 50ms chunks ahead of the consumer.", timeout = 1200)]
def paused_consumer_bounds_threaded_generation(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        for (global in fixed_array(false, true)){
            strudel_init(@@idle_worker)
            sleep(300u); if (global){set_global_pause(true)}else{strudel_set_pause(true)}; sleep(1000u)
            let before = strudel_get_diagnostics()
            sleep(1000u)
            let after = strudel_get_diagnostics()
            if (global){set_global_pause(false)}else{strudel_set_pause(false)}; strudel_shutdown()
            t |> success(after.generated_frames - before.generated_frames <= 4800ul, "paused stream must stop generation when its bounded queue is full (global={global})")
            t |> success(after.generated_frames - after.consumed_frames <= 14400ul, "at most six 50ms chunks including in-flight output")
        }
    }
    sound_set_null_device(false)
}

[test(description = "A track's gain and fade read back while playback runs and read zero after shutdown, across two playback restarts.")]
def track_controls_survive_playback_restart(t : T?){
    strudel_shutdown()
    for (_cycle in range(2)){
        let pat <- silence(); let track = strudel_add_track_immediate(pat, .75)
        t |> equal(strudel_track_gain(track), .75)
        strudel_fade_track(track, .25, 0.0)
        t |> equal(strudel_track_gain(track), .25)
        strudel_shutdown()
        t |> equal(strudel_track_gain(track), 0.0)
    }
}

[test(description = "Shutting down strudel playback whose channel was restored into a context with no audio command stream, as a context fresh from a reload is, clears the channel instead of failing to stop it.")]
def shutdown_without_command_stream_clears_restored_channel(t : T?){
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    var wall = g_wall_time; var cps = g_cps; var sid = 17ul; var sounds = 0
    arch |> serialize(wall); arch |> serialize(cps)
    arch |> serialize(sid); arch |> serialize(sounds)
    var data <- ser->extractData()
    unsafe { delete ser; }
    t |> success(get_audio_command_stream() == null, "no audio system, as after a reload")
    strudel_restore_state(data)
    strudel_shutdown()
    t |> success(empty(strudel_save_state()), "shutdown clears the restored channel")
}
