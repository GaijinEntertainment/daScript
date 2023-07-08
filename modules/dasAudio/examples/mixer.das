options persistent_heap

require audio/audio_boost
require math
require fio


[export]
def main

    with_audio_system <|
        print("with first file system\n")
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
        sleep(1000u)

    with_audio_system <|
        print("with second file system\n")
        print("play sin-440\n")
        var sin1 <- [{for x in range(MA_SAMPLE_RATE); sin(2.*PI*440.*float(x)/float(48000))}]
        let sid_pcm = play_sound_from_pcm(MA_SAMPLE_RATE,1,sin1)
        sid_pcm |> set_volume(0.25)
        sleep(500u)
        print("gen and append sin-880\n")
        var sin2 <- [{for x in range(MA_SAMPLE_RATE); sin(2.*PI*880.*float(x)/float(48000))}]
        sid_pcm |> append_to_pcm(sin2)
        sleep(1500u)
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
