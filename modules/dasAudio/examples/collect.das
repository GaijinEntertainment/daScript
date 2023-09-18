options persistent_heap

require audio/audio_boost
require math
require fio

require daslib/jobque_boost

def sleep_and_log_status ( var status:LockBox?; delay:uint )
    for t in range(delay/100u)
        audio_system_collect()
        if status != null
            let got = status |> get <| $ ( ch:AudioChannelStatus# )
                print("status: {ch}\n")
            if !got
                print("no status\n")
        else
            assert(false, "status is null\n")
        sleep(100u)

[export]
def main
    with_collected_audio_system <|
        print("testing stream\n")
        var sin1 <- [{for x in range(MA_SAMPLE_RATE); sin(2.*PI*440.*float(x)/float(48000))}]
        let sid_pcm = play_sound_from_pcm_stream(MA_SAMPLE_RATE,1)
        with_lock_box <| $ ( box )
            sid_pcm |> set_status_update(box)
            sid_pcm |> set_volume(0.25)
            box |> sleep_and_log_status(500u)
            print("play sin-440\n")
            sid_pcm |> append_to_pcm(sin1)
            box |> sleep_and_log_status(500u)
            print("gen and append sin-880\n")
            var sin2 <- [{for x in range(MA_SAMPLE_RATE); sin(2.*PI*880.*float(x)/float(48000))}]
            sid_pcm |> append_to_pcm(sin2)
            box |> sleep_and_log_status(1400u)
            print("now stopping\n")
            sid_pcm |> stop(0.3)
            box |> sleep_and_log_status(400u)
            print("waiting for box {box.size}\n")
            box |> join
            box |> clear_status
            print("and done\n")
