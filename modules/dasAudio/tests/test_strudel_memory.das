options gen2
options gc
options persistent_heap
require strudel/strudel_player
require audio/audio_boost
require daslib/fio
require daslib/jobque_boost
require daslib/runtime_memory
require dastest/testing_boost

def private memory_worker(){strudel_play()}
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
