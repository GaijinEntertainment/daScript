options persistent_heap

require audio/audio_boost
require math
require fio

require audio_resource

[export]
def main
    with_audio_system <|
        print("testing mp3 from pcm, decoded\n")
        var sound_data : array<float>
        var channels, rate : int
        audio_fopen("gong.wav","rb") <| $ ( fr )
            if fr == null
                panic("cannot open file")
            fmap(fr) <| $ ( data )
                var samples <- decode_audio(data, channels, rate)
                sound_data <- samples
        print("{length(sound_data)} samples, {channels} channels, {rate} rate\n")
        print("press enter...")
        getchar()
        for i in urange(10)
            play_sound_from_pcm(MA_SAMPLE_RATE,channels,clone_to_move(sound_data))
            sleep(20u + i*2u)
        set_global_pause(true)
        sleep(1000u)
        set_global_pause(false)
        sleep(1000u)





