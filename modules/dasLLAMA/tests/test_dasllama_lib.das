options gen2
require dastest/testing_boost public
require ../../../examples/dasLLAMA/library/dasllama_lib.das
require strings

// The library example streams PCM: with no model open its stream and memory calls refuse
// cleanly and say why, instead of panicking across the C ABI.

[test]
def test_stream_calls_refuse_without_a_model(t : T?) {
    var pcm = fixed_array(0.0, 0.0, 0.0, 0.0)
    t |> equal("", asr_feed(unsafe(addr(pcm[0])), 4, 16000), "asr_feed before asr_listen returns no text")
    t |> success(find(reason(), "asr_listen") >= 0, "and says to call asr_listen: {reason()}")
    t |> success(!asr_listen("no_such_vad.bin"), "asr_listen refuses with no ASR model open")
    t |> equal(0, tts_speak("hello"), "tts_speak refuses with no TTS model open")
    t |> success(tts_pcm() == null && tts_pcm_rate() == 0, "nothing was synthesized")
}
