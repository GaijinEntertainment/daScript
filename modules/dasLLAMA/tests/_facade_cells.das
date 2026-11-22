options gen2
options _dasllama_internal = true

module _facade_cells shared public

//! the model facade's cells on a Qwen2.5-0.5B its caller loaded; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama   // the public facade: the model-level encode and piece the cell compares
require dasllama/dasllama_tokenizer   // TokKind, the tokenizer's own encode and piece
require _model_tier   // ids_equal

// The model facade's cells on a loaded Qwen2.5-0.5B (its block's q8 CPU form, `load_model_`): the model owns its
// BPE tokenizer and the model-level calls delegate to it.

//! `load_model_` populated `m`'s BPE tokenizer, and the model-level encode and piece return what the tokenizer's own calls do
def facade_model_cell(t : T?; m : Model) {
    unsafe {
        t |> run("load_model_ owns the tokenizer; model facade delegates") @capture(& m)(t : T ?) {
            let text = "Hello, world"
            let via_model <- encode(m, text, true, false)
            let via_tok <- encode(m.tok, text, true, false)
            let last = via_model[length(via_model) - 1]
            let ok = (m.tok.kind == TokKind.bpe &&
                ids_equal(via_model, via_tok) &&
                piece(m, last) == piece(m.tok, last))
            t |> success(ok, "model.tok populated + encode/piece delegate")
        }
    }
}
