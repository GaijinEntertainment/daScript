// dasLLAMA from C++, through the standalone context's own class. The -ctx backend only: a
// jitted -lib artifact has no C++ source to put a real class on.
//
//   dasllama_host_cpp_ctx <model.gguf> ["prompt"] [tokens]
//   dasllama_host_cpp_ctx --asr <asr-model> <vad.bin> <pcm16.wav>   live speech to text
//   dasllama_host_cpp_ctx --tts <tts-model> "text" <out.wav>   speech synthesis

#include "daScript/daScript.h"
#include "dasllama_lib.das.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

static const char * why ( das::ctx_dasllama_lib::Standalone & ctx ) {
    const char * text = ctx.reason();
    return text ? text : "(no reason given)";
}

// a PCM16 WAV's samples mixed to mono f32 - what a capture callback would hand over
static std::vector<float> read_wav_mono ( const char * path, int & rate ) {
    std::vector<float> mono;
    FILE * f = fopen(path, "rb");
    unsigned char h[12], ch[8], fmt[16];
    int channels = 0;
    if ( !f ) return mono;
    if ( fread(h, 1, 12, f) == 12 && !memcmp(h, "RIFF", 4) && !memcmp(h + 8, "WAVE", 4) ) {
        while ( mono.empty() && fread(ch, 1, 8, f) == 8 ) {
            const long size = ch[4] | (ch[5] << 8) | (ch[6] << 16) | ((long)ch[7] << 24);
            if ( !memcmp(ch, "fmt ", 4) && size >= 16 && fread(fmt, 1, 16, f) == 16 ) {
                channels = fmt[2] | (fmt[3] << 8);
                rate = fmt[4] | (fmt[5] << 8) | (fmt[6] << 16) | (fmt[7] << 24);
                if ( (fmt[14] | (fmt[15] << 8)) != 16 ) break;
                fseek(f, size - 16 + (size & 1), SEEK_CUR);
            } else if ( !memcmp(ch, "data", 4) && channels > 0 ) {
                std::vector<short> pcm(size / 2);
                const size_t frames = fread(pcm.data(), 2, pcm.size(), f) / channels;
                mono.resize(frames);
                for ( size_t i = 0; i < frames; i ++ ) {
                    float sum = 0.0f;
                    for ( int c = 0; c < channels; c ++ ) sum += pcm[i * channels + c] / 32768.0f;
                    mono[i] = sum / channels;
                }
            } else {
                fseek(f, size + (size & 1), SEEK_CUR);
            }
        }
    }
    fclose(f);
    return mono;
}

static bool write_wav_pcm16 ( const char * path, const float * pcm, int count, int rate ) {
    FILE * f = fopen(path, "wb");
    if ( !f ) return false;
    auto put = [&]( unsigned v, int bytes ) { for ( int i = 0; i < bytes; i ++ ) fputc((v >> (8 * i)) & 0xff, f); };
    fwrite("RIFF", 1, 4, f); put(36 + 2 * count, 4); fwrite("WAVEfmt ", 1, 8, f);
    put(16, 4); put(1, 2); put(1, 2); put(rate, 4); put(2 * rate, 4); put(2, 2); put(16, 2);
    fwrite("data", 1, 4, f); put(2 * count, 4);
    for ( int i = 0; i < count; i ++ ) {
        const float v = pcm[i] < -1.0f ? -1.0f : (pcm[i] > 1.0f ? 1.0f : pcm[i]);
        put((unsigned)(short)(v * 32767.0f), 2);
    }
    fclose(f);
    return true;
}

// the file plays the microphone: 10 ms chunks at its own rate, each utterance printed when heard
static int transcribe_file ( das::ctx_dasllama_lib::Standalone & ctx, char * model, char * vad, char * wav ) {
    int rate = 0;
    const std::vector<float> mono = read_wav_mono(wav, rate);
    if ( mono.empty() ) {
        printf("%s is not a PCM16 WAV\n", wav);
        return 1;
    }
    if ( !ctx.asr_open(model) || !ctx.asr_listen(vad) ) {
        printf("asr failed: %s\n", why(ctx));
        ctx.asr_close();
        return 1;
    }
    const int frames = (int) mono.size();
    for ( int at = 0; at < frames; at += rate / 100 ) {
        const char * text = ctx.asr_feed(mono.data() + at, frames - at < rate / 100 ? frames - at : rate / 100, rate);
        if ( text && *text ) printf("%s\n", text);
    }
    const char * tail = ctx.asr_listen_end();
    if ( tail && *tail ) printf("%s\n", tail);
    printf("\n[%.1fx realtime]\n", ctx.asr_speed());
    ctx.asr_close();
    return 0;
}

static int say_to_wav ( das::ctx_dasllama_lib::Standalone & ctx, char * model, char * text, char * out ) {
    if ( !ctx.tts_open(model) ) {
        printf("tts_open failed: %s\n", why(ctx));
        return 1;
    }
    const int count = ctx.tts_speak(text);
    const int rate = ctx.tts_pcm_rate();
    if ( count <= 0 || !write_wav_pcm16(out, ctx.tts_pcm(), count, rate) ) {
        printf("tts failed: %s\n", count <= 0 ? why(ctx) : out);
        ctx.tts_close();
        return 1;
    }
    printf("%s: %.2f s as %s [%.1fx realtime]\n", out, (double)count / rate, ctx.tts_voice(), ctx.tts_speed());
    ctx.tts_close();
    return 0;
}

int main ( int argc, char * argv [] ) {
    if ( argc < 2 ) {
        printf("usage: %s <model.gguf> [\"prompt\"] [tokens]\n", argv[0]);
        printf("       %s --asr <asr-model> <vad.bin> <pcm16.wav>\n", argv[0]);
        printf("       %s --tts <tts-model> \"text\" <out.wav>\n", argv[0]);
        return 2;
    }
    const bool asr = strcmp(argv[1], "--asr") == 0;
    const bool tts = strcmp(argv[1], "--tts") == 0;
    if ( (asr || tts) && argc < 5 ) {
        printf("usage: %s --asr <asr-model> <vad.bin> <pcm16.wav>\n", argv[0]);
        printf("       %s --tts <tts-model> \"text\" <out.wav>\n", argv[0]);
        return 2;
    }
    char * model = (asr || tts) ? argv[2] : argv[1];
    char * prompt = argc > 2 ? argv[2] : (char *) "Once upon a time";
    const int want = argc > 3 ? atoi(argv[3]) : 48;

    das::ctx_dasllama_lib::Standalone ctx;
    if ( asr ) {
        return transcribe_file(ctx, model, argv[3], argv[4]);
    }
    if ( tts ) {
        return say_to_wav(ctx, model, argv[3], argv[4]);
    }
    if ( !ctx.open(model) ) {
        printf("open failed: %s\n", why(ctx));
        return 1;
    }
    printf("%s: %d layers, vocab %d, context %d\n", ctx.arch(), ctx.n_layers(), ctx.n_vocab(),
           ctx.context_size());

    const int n_prompt = ctx.prefill(prompt);
    if ( n_prompt < 0 ) {
        printf("prefill failed: %s\n", why(ctx));
        return 1;
    }
    fputs(prompt, stdout);
    int got = 0;
    for ( ; got < want; got ++ ) {
        const char * piece = ctx.next_piece();
        if ( !piece || !*piece ) break;
        fputs(piece, stdout);
        fflush(stdout);
    }
    printf("\n\n[prompt %d tok | gen %d tok | prefill %.1f t/s | gen %.1f t/s]\n",
           n_prompt, got, ctx.prefill_tps(), ctx.gen_tps());

    ctx.close();
    return 0;
}
