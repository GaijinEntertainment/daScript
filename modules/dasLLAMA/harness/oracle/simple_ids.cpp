// simple_ids — dasLLAMA parity oracle.
//
// A trimmed llama.cpp `examples/simple` that prints token IDs instead of text:
//   PROMPT_IDS: <prompt token ids, with BOS/specials as llama.cpp tokenizes them>
//   GEN_IDS:    <exactly N greedy (argmax) generated ids, CPU, no EOG early-break>
// Feed PROMPT_IDS to dasLLAMA's harness/parity.das and diff GEN_IDS token-for-token.
//
// CPU-only (n_gpu_layers = 0) so the argmax matches dasLLAMA's CPU forward on ties.
// No EOG break: emit a fixed N so both sides are the same length. This is the oracle
// that mints tests/dasLLAMA fixtures — keep it in sync with the linked llama.cpp.
//
// Build (from the llama.cpp checkout, against its already-built libs):
//   clang++ -std=c++17 -O2 -I include -I ggml/include simple_ids.cpp \
//     -L build/bin -lllama -lggml -Wl,-rpath,"$PWD/build/bin" -o build/bin/simple_ids
// Run:
//   build/bin/simple_ids -m <model.gguf> -n 40 -p "Once upon a time"

#include "llama.h"
#include <clocale>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static void print_usage(const char * argv0) {
    fprintf(stderr, "\nusage: %s -m model.gguf [-n n_predict] [-ngl n_gpu_layers] [-p prompt]\n\n", argv0);
}

int main(int argc, char ** argv) {
    std::setlocale(LC_NUMERIC, "C");

    std::string model_path;
    std::string prompt = "Once upon a time";
    int ngl = 0;          // CPU only — match dasLLAMA's CPU forward so argmax ties agree
    int n_predict = 40;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            model_path = argv[++i];
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n_predict = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "-ngl") == 0 && i + 1 < argc) {
            ngl = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            prompt = argv[++i];
        } else {
            print_usage(argv[0]);
            return 1;
        }
    }
    if (model_path.empty()) {
        print_usage(argv[0]);
        return 1;
    }

    ggml_backend_load_all();

    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = ngl;
    llama_model * model = llama_model_load_from_file(model_path.c_str(), model_params);
    if (model == NULL) {
        fprintf(stderr, "simple_ids: unable to load model\n");
        return 1;
    }

    const llama_vocab * vocab = llama_model_get_vocab(model);

    // tokenize the prompt exactly as llama.cpp would (add BOS + specials)
    const int n_prompt = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), NULL, 0, true, true);
    std::vector<llama_token> prompt_tokens(n_prompt);
    if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), true, true) < 0) {
        fprintf(stderr, "simple_ids: tokenize failed\n");
        return 1;
    }

    printf("PROMPT_IDS:");
    for (auto id : prompt_tokens) {
        printf(" %d", id);
    }
    printf("\n");

    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx   = n_prompt + n_predict;
    ctx_params.n_batch = n_prompt;
    ctx_params.no_perf = true;
    llama_context * ctx = llama_init_from_model(model, ctx_params);
    if (ctx == NULL) {
        fprintf(stderr, "simple_ids: failed to create context\n");
        return 1;
    }

    auto sparams = llama_sampler_chain_default_params();
    sparams.no_perf = true;
    llama_sampler * smpl = llama_sampler_chain_init(sparams);
    llama_sampler_chain_add(smpl, llama_sampler_init_greedy());

    llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
    llama_token new_token_id;
    std::vector<llama_token> gen;
    gen.reserve(n_predict);
    for (int n = 0; n < n_predict; n++) {
        if (llama_decode(ctx, batch)) {
            fprintf(stderr, "simple_ids: decode failed at step %d\n", n);
            return 1;
        }
        new_token_id = llama_sampler_sample(smpl, ctx, -1);   // greedy argmax over the last position
        gen.push_back(new_token_id);
        batch = llama_batch_get_one(&new_token_id, 1);        // &new_token_id stays valid until the next decode
    }

    printf("GEN_IDS:");
    for (auto id : gen) {
        printf(" %d", id);
    }
    printf("\n");

    llama_sampler_free(smpl);
    llama_free(ctx);
    llama_model_free(model);
    return 0;
}
