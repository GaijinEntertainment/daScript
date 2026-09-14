// dasLLAMA from C++, through the standalone context's own class. The -ctx backend only: a
// jitted -lib artifact has no C++ source to put a real class on.
//
//   dasllama_host_cpp_ctx <model.gguf> ["prompt"] [tokens]
//   dasllama_host_cpp_ctx --asr <asr-model> <audio-file>       speech to text
//   dasllama_host_cpp_ctx --tts <tts-model> "text" <out.wav>   speech synthesis

#include "daScript/daScript.h"
#include "dasllama_lib.das.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char * why ( das::ctx_dasllama_lib::Standalone & ctx ) {
    const char * text = ctx.reason();
    return text ? text : "(no reason given)";
}

static int transcribe_file ( das::ctx_dasllama_lib::Standalone & ctx, char * model, char * audio ) {
    if ( !ctx.asr_open(model) ) {
        printf("asr_open failed: %s\n", why(ctx));
        return 1;
    }
    const char * text = ctx.asr_transcribe(audio);
    if ( !text ) {
        printf("transcribe failed: %s\n", why(ctx));
        ctx.asr_close();
        return 1;
    }
    printf("%s\n\n[%.1fx realtime]\n", text, ctx.asr_speed());
    ctx.asr_close();
    return 0;
}

static int say_to_wav ( das::ctx_dasllama_lib::Standalone & ctx, char * model, char * text, char * out ) {
    if ( !ctx.tts_open(model) ) {
        printf("tts_open failed: %s\n", why(ctx));
        return 1;
    }
    const double seconds = ctx.tts_say(text, out);
    if ( seconds <= 0.0 ) {
        printf("tts_say failed: %s\n", why(ctx));
        ctx.tts_close();
        return 1;
    }
    printf("%s: %.2f s as %s [%.1fx realtime]\n", out, seconds, ctx.tts_voice(), ctx.tts_speed());
    ctx.tts_close();
    return 0;
}

int main ( int argc, char * argv [] ) {
    if ( argc < 2 ) {
        printf("usage: %s <model.gguf> [\"prompt\"] [tokens]\n", argv[0]);
        printf("       %s --asr <asr-model> <audio-file>\n", argv[0]);
        printf("       %s --tts <tts-model> \"text\" <out.wav>\n", argv[0]);
        return 2;
    }
    const bool asr = strcmp(argv[1], "--asr") == 0;
    const bool tts = strcmp(argv[1], "--tts") == 0;
    if ( (asr && argc < 4) || (tts && argc < 5) ) {
        printf("usage: %s --asr <asr-model> <audio-file>\n", argv[0]);
        printf("       %s --tts <tts-model> \"text\" <out.wav>\n", argv[0]);
        return 2;
    }
    char * model = (asr || tts) ? argv[2] : argv[1];
    char * prompt = argc > 2 ? argv[2] : (char *) "Once upon a time";
    const int want = argc > 3 ? atoi(argv[3]) : 48;

    das::ctx_dasllama_lib::Standalone ctx;
    if ( asr ) {
        return transcribe_file(ctx, model, argv[3]);
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
