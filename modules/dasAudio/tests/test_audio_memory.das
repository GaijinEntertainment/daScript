options gen2
require audio/audio_boost
require daslib/fio
require daslib/jobque_boost
require daslib/runtime_memory
require dastest/testing_boost

def private await_memory(box : SeqBox?) : ContextMemory {
    var sample = ContextMemory()
    for (_i in range(250)){
        box |> read() $(value : ContextMemory){sample = value}
        if (sample.stack > 0ul){break}
        sleep(10u)
    }
    return sample
}
[test]
def memory_box_publishes_without_stats_and_replaces_ownership(t : T?){
    let before = count_jobque_leaks()
    sound_set_null_device(true)
    with_audio_system() $ {
        var first = seq_box_create(); var second = seq_box_create()
        set_audio_memory_box(first)
        let first_value = await_memory(first)
        t |> success(first_value.stack > 0ul && first_value.heap_reserved > 0ul)
        set_audio_memory_box(second)
        let second_value = await_memory(second)
        t |> success(second_value.stack > 0ul)
        first |> publish(ContextMemory(stack = 777ul))
        sleep(1100u)
        first |> read() $(old : ContextMemory){t |> equal(old.stack, 777ul, "replaced box no longer receives worker snapshots")}
        first |> seq_box_release
        var none : SeqBox?; set_audio_memory_box(none)
        second |> seq_box_release
    }
    sound_set_null_device(false)
    t |> equal(count_jobque_leaks(), before, "audio registrations release every box share")
}
[test]
def native_playback_diagnostics_are_unavailable_zero(t : T?){
    let stats = get_playback_diagnostics()
    t |> equal(stats.callbacks, 0ul); t |> equal(stats.capacity_frames, 0ul)
    t |> equal(stats.underrun_frames, 0ul); t |> equal(stats.recoveries, 0ul)
}
