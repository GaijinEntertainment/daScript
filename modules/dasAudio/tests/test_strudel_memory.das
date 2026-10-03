options gen2
options gc
options persistent_heap
require strudel/strudel_player
require audio/audio_boost
require daslib/fio
require daslib/jobque_boost
require daslib/runtime_memory
require dastest/testing_boost

def private memory_worker(){}
[test]
def strudel_worker_memory_and_shutdown_balance(t : T?){
    let before = count_jobque_leaks()
    sound_set_null_device(true)
    with_audio_system() $ {
        for (_cycle in range(3)){
            strudel_init(@@memory_worker)
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

require strudel/strudel
struct private WorkerGarbage { values : array<int> }
var private observed_track = -1
var private worker_title = ""
var private before_collection = 0ul

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
[test]
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
