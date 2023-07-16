options persistent_heap

require audio/audio_boost
require math
require fio

require daslib/jobque_boost

def sleep_and_log_status ( var status:LockBox?; delay:uint )
    for t in range(delay/100u)
        sleep(100u)
        if status != null
            let got = status |> get <| $ ( ch:AudioChannelStatus# )
                print("status: {ch}\n")
            if !got
                print("no status\n")
        else
            print("status = null\n")


[export]
def main

    with_audio_system <|
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
            print("and done\n")

    with_audio_system <|
        print("seting panning\n")
        print("play sin-440 looped\n")
        var sin1 <- [{for x in range(MA_SAMPLE_RATE); sin(2.*PI*440.*float(x)/float(48000))}]
        let sid_pcm = play_sound_loop_from_pcm(MA_SAMPLE_RATE,1,sin1)
        sid_pcm |> set_volume(0.25)
        print("left\n")
        sid_pcm |> set_pan(-1.0)
        sleep(1500u)
        print("right\n")
        sid_pcm |> set_pan(1.0)
        sleep(1500u)
        print("panning\n")
        for t in range(300)
            let pan = sin(2.*PI*float(t)/float(150))
            sid_pcm |> set_pan(pan)
            sleep(10u)

    with_audio_system <|
        print("testing mp3 from pcm, decoded\n")
        var sound_data : array<float>
        var channels, rate : int
        fopen("{get_das_root()}/modules/dasAudio/examples/test.mp3","rb") <| $ ( fr )
            if fr == null
                panic("cannot open file")
            fmap(fr) <| $ ( data )
                var samples <- decode_audio(data, channels, rate)
                sound_data <- samples
        print("{length(sound_data)} samples, {channels} channels, {rate} rate\n")
        let sid_pcm = play_sound_from_pcm(MA_SAMPLE_RATE,channels,sound_data)
        sleep(3000u)

    with_audio_system <|
        print("testing play,stop,pause on sound from file\n")
        print("play\n")
        let sid = play_sound_from_file("{get_das_root()}/modules/dasAudio/examples/test.mp3", MA_SAMPLE_RATE, 2)
        sleep(2000u)
        print("set volume 0.5\n")
        sid |> set_volume(0.5)
        sleep(1000u)
        print("set volume 1.0\n")
        sid |> set_volume(1.0, 1.0)
        sleep(2000u)
        print("pause\n")
        sid |> set_pause(true)
        sleep(1000u)
        print("unpause\n")
        sid |> set_pause(false)
        sid |> set_volume(0.25, 1.0)
        sleep(1000u)
        print("stop\n")
        sid |> stop(0.5)
        sleep(1000u)
        print("done\n")
