options persistent_heap

require audio/audio_boost
require math
require fio

require daslib/jobque_boost

[export]
def main
    with_audio_system <|
        print("loading test data\n")
        var sound_data : array<float>
        var channels, rate : int
        fopen("{get_das_root()}/modules/dasAudio/examples/test.mp3","rb") <| $ ( fr )
            if fr == null
                panic("cannot open file")
            fmap(fr) <| $ ( data )
                var samples <- decode_audio(data, channels, rate)
                sound_data <- samples
        print("{channels} x {rate}\n")
        print("setting 3D position\n")
        set_head_position(float3(0,0,0),float3(0,-1,0))
        print("play looped\n")
        var sin1 <- [{for x in range(MA_SAMPLE_RATE); sin(2.*PI*440.*float(x)/float(48000))}]
        let sid_pcm = play_3d_sound_loop_from_pcm(float3(0),100.0,rate,channels,sound_data)
        sid_pcm |> set_volume(0.25)
        print("left\n")
        sid_pcm |> set_position(float3(-1,0,0))
        sleep(1500u)
        print("right\n")
        sid_pcm |> set_position(float3(1,0,0))
        sleep(1500u)
        print("front\n")
        sid_pcm |> set_position(float3(0,-1,0))
        sleep(1500u)
        print("back\n")
        sid_pcm |> set_position(float3(0, 1,0))
        sleep(1500u)
        print("circling\n")
        for t in range(360)
            let O = 2.*PI*float(t*2)/float(360)
            let pos = float3(sin(O),cos(O),0.)
            sid_pcm |> set_position(pos)
            sleep(10u)
