// dasLLAMA from C, on either backend - the only difference is which generated header to include.
//
//   dasllama_host_c_jit <model.gguf> ["prompt"] [tokens]      the -lib backend
//   dasllama_host_c_ctx <model.gguf> ["prompt"] [tokens]      the -ctx backend
//   dasllama_host_c_ctx --asr <asr-model> <audio-file>        speech to text, either backend

#if defined(DASLLAMA_LIB_STANDALONE_CTX)
#include "dasllama_lib.das.h"
#else
#include "dasllama_lib.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char * why ( dasllama_lib_ctx * ctx ) {
    const char * panicked = dasllama_lib_last_error(ctx);
    const char * text = panicked ? panicked : dasllama_lib_reason(ctx);
    return text ? text : "(no reason given)";
}

static int transcribe_file ( dasllama_lib_ctx * ctx, const char * model, const char * audio ) {
    if ( !dasllama_lib_asr_open(ctx, model) ) {
        printf("asr_open failed: %s\n", why(ctx));
        return 1;
    }
    const char * text = dasllama_lib_asr_transcribe(ctx, audio);
    if ( !text ) {
        printf("transcribe failed: %s\n", why(ctx));
        dasllama_lib_asr_close(ctx);
        return 1;
    }
    printf("%s\n\n[%.1fx realtime]\n", text, dasllama_lib_asr_speed(ctx));
    dasllama_lib_asr_close(ctx);
    return 0;
}

int main ( int argc, char * argv [] ) {
    if ( argc < 2 ) {
        printf("usage: %s <model.gguf> [\"prompt\"] [tokens]\n", argv[0]);
        printf("       %s --asr <asr-model> <audio-file>\n", argv[0]);
        return 2;
    }
    const int asr = strcmp(argv[1], "--asr") == 0;
    if ( asr && argc < 4 ) {
        printf("usage: %s --asr <asr-model> <audio-file>\n", argv[0]);
        return 2;
    }
    const char * model = asr ? argv[2] : argv[1];
    const char * prompt = argc > 2 ? argv[2] : "Once upon a time";
    const int want = argc > 3 ? atoi(argv[3]) : 48;

    dasllama_lib_ctx * ctx = dasllama_lib_create();
    if ( !ctx ) {
        printf("create failed: %s\n", dasllama_lib_last_error(NULL));
        return 1;
    }
    if ( asr ) {
        const int rc = transcribe_file(ctx, model, argv[3]);
        dasllama_lib_destroy(ctx);
        dasllama_lib_shutdown_runtime();
        return rc;
    }
    if ( !dasllama_lib_open(ctx, model) ) {
        printf("open failed: %s\n", why(ctx));
        dasllama_lib_destroy(ctx);
        dasllama_lib_shutdown_runtime();
        return 1;
    }
    printf("%s: %d layers, vocab %d, context %d\n", dasllama_lib_arch(ctx),
           dasllama_lib_n_layers(ctx), dasllama_lib_n_vocab(ctx), dasllama_lib_context_size(ctx));

    const int n_prompt = dasllama_lib_prefill(ctx, prompt);
    if ( n_prompt < 0 ) {
        printf("prefill failed: %s\n", why(ctx));
        dasllama_lib_close(ctx);
        dasllama_lib_destroy(ctx);
        dasllama_lib_shutdown_runtime();
        return 1;
    }
    fputs(prompt, stdout);
    int got = 0;
    for ( ; got < want; got ++ ) {
        const char * piece = dasllama_lib_next_piece(ctx);
        if ( !piece || !*piece ) break;
        fputs(piece, stdout);
        fflush(stdout);
    }
    printf("\n\n[prompt %d tok | gen %d tok | prefill %.1f t/s | gen %.1f t/s]\n",
           n_prompt, got, dasllama_lib_prefill_tps(ctx), dasllama_lib_gen_tps(ctx));

    dasllama_lib_close(ctx);
    dasllama_lib_destroy(ctx);
    dasllama_lib_shutdown_runtime();
    return 0;
}
