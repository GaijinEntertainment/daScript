// dasLLAMA from C, on either backend - the only difference is which generated header to include.
//
//   dasllama_host_c_jit <model.gguf> ["prompt"] [tokens]      the -lib backend
//   dasllama_host_c_ctx <model.gguf> ["prompt"] [tokens]      the -ctx backend
//   dasllama_host_c_ctx --asr <asr-model> <vad.bin> <pcm16.wav>   live speech to text, either backend
//   dasllama_host_c_ctx --tts <tts-model> "text" <out.wav>    speech synthesis, either backend
//   dasllama_host_c_jit --interpret <asr-model> <vad.bin> <translation-model> <tts-model> <in.wav> <out.wav> [--from-memory]
//       speech in one language to speech in another, either backend: the transcript and its
//       translation go to stdout, the spoken translation to <out.wav>. --from-memory reads the VAD
//       and every model given as a prepared image (.dlim) into memory first, as an engine with its
//       own file system would, and opens them from the bytes.

#if defined(DASLLAMA_LIB_STANDALONE_CTX)
#include "dasllama_lib.das.h"
#else
#include "dasllama_lib.h"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char * why ( dasllama_lib_ctx * ctx ) {
    const char * panicked = dasllama_lib_last_error(ctx);
    const char * text = panicked ? panicked : dasllama_lib_reason(ctx);
    return text ? text : "(no reason given)";
}

// a PCM16 WAV's samples mixed to mono f32 - what a capture callback would hand over
static float * read_wav_mono ( const char * path, int * rate, int * frames ) {
    FILE * f = fopen(path, "rb");
    unsigned char h[12], ch[8], fmt[16];
    int channels = 0;
    float * mono = NULL;
    if ( !f ) return NULL;
    if ( fread(h, 1, 12, f) == 12 && !memcmp(h, "RIFF", 4) && !memcmp(h + 8, "WAVE", 4) ) {
        while ( !mono && fread(ch, 1, 8, f) == 8 ) {
            const long size = ch[4] | (ch[5] << 8) | (ch[6] << 16) | ((long)ch[7] << 24);
            if ( !memcmp(ch, "fmt ", 4) && size >= 16 && fread(fmt, 1, 16, f) == 16 ) {
                channels = fmt[2] | (fmt[3] << 8);
                *rate = fmt[4] | (fmt[5] << 8) | (fmt[6] << 16) | (fmt[7] << 24);
                if ( (fmt[14] | (fmt[15] << 8)) != 16 ) break;
                fseek(f, size - 16 + (size & 1), SEEK_CUR);
            } else if ( !memcmp(ch, "data", 4) && channels > 0 ) {
                short * pcm = (short *) malloc(size);
                *frames = (int)(fread(pcm, 1, size, f) / (2 * channels));
                mono = (float *) malloc(sizeof(float) * (*frames + 1));
                for ( int i = 0; i < *frames; i ++ ) {
                    float sum = 0.0f;
                    for ( int c = 0; c < channels; c ++ ) sum += pcm[i * channels + c] / 32768.0f;
                    mono[i] = sum / channels;
                }
                free(pcm);
            } else {
                fseek(f, size + (size & 1), SEEK_CUR);
            }
        }
    }
    fclose(f);
    return mono;
}

static void put_le ( FILE * f, unsigned v, int bytes ) {
    for ( int i = 0; i < bytes; i ++ ) fputc((v >> (8 * i)) & 0xff, f);
}

static int write_wav_pcm16 ( const char * path, const float * pcm, int count, int rate ) {
    FILE * f = fopen(path, "wb");
    if ( !f ) return 0;
    fwrite("RIFF", 1, 4, f); put_le(f, 36 + 2 * count, 4); fwrite("WAVEfmt ", 1, 8, f);
    put_le(f, 16, 4); put_le(f, 1, 2); put_le(f, 1, 2); put_le(f, rate, 4); put_le(f, 2 * rate, 4);
    put_le(f, 2, 2); put_le(f, 16, 2); fwrite("data", 1, 4, f); put_le(f, 2 * count, 4);
    for ( int i = 0; i < count; i ++ ) {
        const float v = pcm[i] < -1.0f ? -1.0f : (pcm[i] > 1.0f ? 1.0f : pcm[i]);
        put_le(f, (unsigned)(short)(v * 32767.0f), 2);
    }
    fclose(f);
    return 1;
}

static void print_heard ( const char * text ) {
    if ( text && *text ) printf("%s\n", text);
    fflush(stdout);
}

// the file plays the microphone: 10 ms chunks at its own rate, each utterance printed when heard
static int transcribe_file ( dasllama_lib_ctx * ctx, const char * model, const char * vad, const char * wav ) {
    int rate = 0, frames = 0;
    float * mono = read_wav_mono(wav, &rate, &frames);
    if ( !mono ) {
        printf("%s is not a PCM16 WAV\n", wav);
        return 1;
    }
    if ( !dasllama_lib_asr_open(ctx, model) || !dasllama_lib_asr_listen(ctx, vad) ) {
        printf("asr failed: %s\n", why(ctx));
        free(mono);
        dasllama_lib_asr_close(ctx);
        return 1;
    }
    for ( int at = 0; at < frames; at += rate / 100 ) {
        const int n = frames - at < rate / 100 ? frames - at : rate / 100;
        print_heard(dasllama_lib_asr_feed(ctx, mono + at, n, rate));
    }
    print_heard(dasllama_lib_asr_listen_end(ctx));
    printf("\n[%.1fx realtime]\n", dasllama_lib_asr_speed(ctx));
    free(mono);
    dasllama_lib_asr_close(ctx);
    return 0;
}

static int say_to_wav ( dasllama_lib_ctx * ctx, const char * model, const char * text, const char * out ) {
    if ( !dasllama_lib_tts_open(ctx, model) ) {
        printf("tts_open failed: %s\n", why(ctx));
        return 1;
    }
    const int count = dasllama_lib_tts_speak(ctx, text);
    const int rate = dasllama_lib_tts_pcm_rate(ctx);
    if ( count <= 0 || !write_wav_pcm16(out, dasllama_lib_tts_pcm(ctx), count, rate) ) {
        printf("tts failed: %s\n", count <= 0 ? why(ctx) : out);
        dasllama_lib_tts_close(ctx);
        return 1;
    }
    printf("%s: %.2f s as %s [%.1fx realtime]\n", out, (double)count / rate, dasllama_lib_tts_voice(ctx),
           dasllama_lib_tts_speed(ctx));
    dasllama_lib_tts_close(ctx);
    return 0;
}

// a file's bytes in memory, starting on an `align` boundary
static void * read_file_aligned ( const char * path, size_t align, long long * bytes, void ** to_free ) {
    FILE * f = fopen(path, "rb");
    long long size = 0;
    char * at = NULL;
    *to_free = NULL;
    if ( !f ) return NULL;
#if defined(_WIN32)
    _fseeki64(f, 0, SEEK_END); size = _ftelli64(f); _fseeki64(f, 0, SEEK_SET);
#else
    fseek(f, 0, SEEK_END); size = ftell(f); fseek(f, 0, SEEK_SET);
#endif
    if ( size > 0 ) *to_free = malloc((size_t)size + align);
    if ( *to_free ) {
        at = (char *)(((uintptr_t)*to_free + align - 1) / align * align);
        if ( fread(at, 1, (size_t)size, f) != (size_t)size ) {
            free(*to_free);
            *to_free = NULL;
            at = NULL;
        }
    }
    fclose(f);
    *bytes = size;
    return at;
}

static int is_image ( const char * path ) {
    const size_t n = strlen(path);
    return n > 5 && strcmp(path + n - 5, ".dlim") == 0;
}

// `more` appended to the heap string `*all`, a space between
static void append_text ( char ** all, const char * more ) {
    const size_t have = *all ? strlen(*all) : 0, add = strlen(more);
    *all = (char *) realloc(*all, have + add + 2);
    if ( have ) (*all)[have] = ' ';
    memcpy(*all + have + (have ? 1 : 0), more, add + 1);
}

static void interpret_utterance ( dasllama_lib_ctx * ctx, const char * heard, char ** spoken ) {
    if ( !heard || !*heard ) return;
    char * said = NULL;
    append_text(&said, heard);   // a copy: the library's string lives until its next call
    printf("heard: %s\n", said);
    const char * translated = dasllama_lib_translate_text(ctx, said);
    if ( translated && *translated ) {
        printf("means: %s\n", translated);
        append_text(spoken, translated);
    }
    fflush(stdout);
    free(said);
}

typedef bool (* open_path_fn) ( dasllama_lib_ctx *, const char * );
typedef bool (* open_memory_fn) ( dasllama_lib_ctx *, void *, int64_t );

// opens `path` through `from_path`, or - a prepared image with `in_memory` set - reads it into
// `*to_free` and opens the bytes through `from_memory`: the host keeps them until the model closes
static int open_model ( dasllama_lib_ctx * ctx, const char * what, const char * path, int in_memory,
                        open_path_fn from_path, open_memory_fn from_memory, void ** to_free ) {
    long long bytes = 0;
    if ( !in_memory || !is_image(path) ) return from_path(ctx, path);
    void * image = read_file_aligned(path, (size_t) dasllama_lib_image_alignment(ctx), &bytes, to_free);
    if ( !image || !from_memory(ctx, image, bytes) ) return 0;
    printf("%s: %lld MB borrowed from memory\n", what, bytes >> 20);
    return 1;
}

static int interpret ( dasllama_lib_ctx * ctx, char * argv [], int from_memory ) {
    const char * asr = argv[2], * vad = argv[3], * nmt = argv[4], * tts = argv[5];
    const char * in_wav = argv[6], * out_wav = argv[7];
    void * asr_bytes = NULL, * vad_bytes = NULL, * nmt_bytes = NULL;
    long long vad_size = 0;
    int rate = 0, frames = 0, ok = 1, rc = 1;
    char * spoken = NULL;
    float * mono = read_wav_mono(in_wav, &rate, &frames);
    if ( !mono ) {
        printf("%s is not a PCM16 WAV\n", in_wav);
        return 1;
    }
    printf("kernels: %s\n", dasllama_lib_kernels(ctx));
    ok = open_model(ctx, "speech to text", asr, from_memory,
                    dasllama_lib_asr_open, dasllama_lib_asr_open_memory, &asr_bytes);
    if ( ok && from_memory ) {
        void * bin = read_file_aligned(vad, 1, &vad_size, &vad_bytes);
        ok = bin && dasllama_lib_asr_listen_memory(ctx, bin, vad_size);
        free(vad_bytes);   // the voice detector copied what it needs
        vad_bytes = NULL;
    } else if ( ok ) {
        ok = dasllama_lib_asr_listen(ctx, vad);
    }
    ok = ok && open_model(ctx, "translation", nmt, from_memory,
                          dasllama_lib_translate_open, dasllama_lib_translate_open_memory, &nmt_bytes);
    ok = ok && dasllama_lib_tts_open(ctx, tts);
    if ( !ok ) {
        printf("interpret failed: %s\n", why(ctx));
    } else {
        printf("translating %s\n", dasllama_lib_translate_direction(ctx));
        for ( int at = 0; at < frames; at += rate / 100 ) {
            const int n = frames - at < rate / 100 ? frames - at : rate / 100;
            interpret_utterance(ctx, dasllama_lib_asr_feed(ctx, mono + at, n, rate), &spoken);
        }
        interpret_utterance(ctx, dasllama_lib_asr_listen_end(ctx), &spoken);
        const int count = spoken ? dasllama_lib_tts_speak(ctx, spoken) : 0;
        const int out_rate = dasllama_lib_tts_pcm_rate(ctx);
        if ( count > 0 && write_wav_pcm16(out_wav, dasllama_lib_tts_pcm(ctx), count, out_rate) ) {
            printf("%s: %.2f s as %s\n", out_wav, (double)count / out_rate, dasllama_lib_tts_voice(ctx));
            rc = 0;
        } else {
            printf("nothing spoken: %s\n", spoken ? why(ctx) : "no speech was heard");
        }
    }
    dasllama_lib_tts_close(ctx);
    dasllama_lib_translate_close(ctx);
    dasllama_lib_asr_close(ctx);
    // the borrowed images outlive their models: freed only now
    free(nmt_bytes);
    free(asr_bytes);
    free(spoken);
    free(mono);
    return rc;
}

int main ( int argc, char * argv [] ) {
    if ( argc < 2 ) {
        printf("usage: %s <model.gguf> [\"prompt\"] [tokens]\n", argv[0]);
        printf("       %s --asr <asr-model> <vad.bin> <pcm16.wav>\n", argv[0]);
        printf("       %s --tts <tts-model> \"text\" <out.wav>\n", argv[0]);
        printf("       %s --interpret <asr-model> <vad.bin> <translation-model> <tts-model> <in.wav> <out.wav> [--from-memory]\n", argv[0]);
        return 2;
    }
    if ( strcmp(argv[1], "--interpret") == 0 ) {
        if ( argc < 8 ) {
            printf("usage: %s --interpret <asr-model> <vad.bin> <translation-model> <tts-model> <in.wav> <out.wav> [--from-memory]\n", argv[0]);
            return 2;
        }
        dasllama_lib_ctx * ctx = dasllama_lib_create();
        if ( !ctx ) {
            printf("create failed: %s\n", dasllama_lib_last_error(NULL));
            return 1;
        }
        const int rc = interpret(ctx, argv, argc > 8 && strcmp(argv[8], "--from-memory") == 0);
        dasllama_lib_destroy(ctx);
        dasllama_lib_shutdown_runtime();
        return rc;
    }
    const int asr = strcmp(argv[1], "--asr") == 0;
    const int tts = strcmp(argv[1], "--tts") == 0;
    if ( (asr || tts) && argc < 5 ) {
        printf("usage: %s --asr <asr-model> <vad.bin> <pcm16.wav>\n", argv[0]);
        printf("       %s --tts <tts-model> \"text\" <out.wav>\n", argv[0]);
        return 2;
    }
    const char * model = (asr || tts) ? argv[2] : argv[1];
    const char * prompt = argc > 2 ? argv[2] : "Once upon a time";
    const int want = argc > 3 ? atoi(argv[3]) : 48;

    dasllama_lib_ctx * ctx = dasllama_lib_create();
    if ( !ctx ) {
        printf("create failed: %s\n", dasllama_lib_last_error(NULL));
        return 1;
    }
    if ( asr || tts ) {
        const int rc = asr ? transcribe_file(ctx, model, argv[3], argv[4])
                           : say_to_wav(ctx, model, argv[3], argv[4]);
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
