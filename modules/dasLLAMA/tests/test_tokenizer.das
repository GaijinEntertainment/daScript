options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_tokenizer
require daslib/fio
require daslib/strings_boost
require _model_tier
require _tokenizer_corpus

// Tokenizer regression gates against the upstream test-tokenizer-0 corpora (the .inp/.out files
// shipped next to each ggml-vocab-*.gguf), through the production facade (load_tokenizer_auto ->
// encode/decode) so BOTH backends are covered: byte-level BPE (llama-bpe, qwen2, qwen35, gpt-2),
// SPM-style BPE (gemma-4), and SentencePiece (phi-3, llama-spm). The vocab fixtures are
// machine-local (gitignored model dir), so each case skips cleanly when absent - the file rides
// the runner's model-free suite and reports SKIPPED where the vocab fixtures are not stocked.

// Run the .inp/.out corpus for one vocab: every input must encode to the reference ids, and
// decode(encode(x)) must round-trip losslessly. The first divergence of each kind is carried in
// the assertion message so a red is readable without rerunning.
def private corpus_gate(t : T?; vocab_path : string) {
    var tk <- load_tokenizer_auto(vocab_path)
    var inputs <- split(fread(vocab_path + ".inp"), "\n__ggml_vocab_test__\n")
    var outlines <- split(fread(vocab_path + ".out"), "\n")
    drop_trailing_empty(inputs)
    drop_trailing_empty(outlines)
    // fread returns "" for a missing file — without this check a present .gguf with absent
    // .inp/.out corpora would run 0 vectors and pass trivially
    t |> success(!empty(inputs), "corpus files present and non-empty: {vocab_path}.inp/.out")
    t |> equal(length(inputs), length(outlines))
    var npass = 0
    var rt_ok = 0
    var first_bad = ""
    var first_rt = ""
    for (inp, outl in inputs, outlines) {
        var inscope got <- encode(tk, inp, false)
        if (ids_equal(got, parse_out_ids(outl))) {
            npass++
        } elif (empty(first_bad)) {
            first_bad = "first id mismatch on '{inp}': got {got}, want [{outl}]"
        }
        let dec = decode(tk, got)
        if (dec == inp) {
            rt_ok++
        } elif (empty(first_rt)) {
            first_rt = "first round-trip break on '{inp}': decoded '{dec}'"
        }
        delete got
    }
    t |> equal(npass, length(inputs), first_bad)
    t |> equal(rt_ok, length(inputs), first_rt)
    delete tk
    delete inputs
    delete outlines
}

def private corpus_case(t : T?; title, gguf : string) {
    t |> run(title) @(t : T?) {
        let path = path_join(models_dir(), gguf)
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        corpus_gate(t, path)
    }
}

[test]
def test_tokenizer_gemma4(t : T?) {
    corpus_case(t, "gemma4 SPM-style BPE vs the llama.cpp corpus", "ggml-vocab-gemma-4.gguf")
}

[test]
def test_tokenizer_llama_bpe(t : T?) {
    corpus_case(t, "llama3 byte-level BPE vs the llama.cpp corpus", "ggml-vocab-llama-bpe.gguf")
}

[test]
def test_tokenizer_qwen35(t : T?) {
    corpus_case(t, "qwen35 BPE (mark-absorbing letter runs) vs the llama.cpp corpus", "ggml-vocab-qwen35.gguf")
}

[test]
def test_tokenizer_bos_default(t : T?) {
    t |> run("a BPE vocab without add_bos_token prepends BOS only for upstream's BOS families") @(t : T?) {
        t |> success(bpe_pre_adds_bos("llama-bpe") && bpe_pre_adds_bos("tekken"), "the llama3 family and tekken default to a BOS")
        t |> success(!bpe_pre_adds_bos("qwen35") && !bpe_pre_adds_bos("qwen2") && !bpe_pre_adds_bos("gpt-4o"), "qwen and gpt-4o default to none")
        //! the upstream vocab fixture carries bos_token_id and no add_bos_token key - the unsloth Qwen3.8 shape
        let path = path_join(models_dir(), "ggml-vocab-qwen35.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var tk <- load_tokenizer_auto(path)
        t |> success(!tk.bpe.add_bos, "qwen35 without the key adds no BOS")
        var ids <- encode(tk, "Once upon a time", true)
        t |> equal(4, length(ids), "four BPE ids and no BOS in front: {ids}")
        delete ids
        delete tk
    }
}

[test]
def test_tokenizer_qwen2(t : T?) {
    corpus_case(t, "qwen2 byte-level BPE vs the llama.cpp corpus", "ggml-vocab-qwen2.gguf")
}

[test]
def test_tokenizer_gpt2(t : T?) {
    corpus_case(t, "gpt-2 byte-level BPE vs the llama.cpp corpus", "ggml-vocab-gpt-2.gguf")
}

[test]
def test_tokenizer_phi3(t : T?) {
    corpus_case(t, "phi-3 SentencePiece vs the llama.cpp corpus", "ggml-vocab-phi-3.gguf")
}

[test]
def test_tokenizer_llama_spm(t : T?) {
    corpus_case(t, "llama-2 SentencePiece vs the llama.cpp corpus", "ggml-vocab-llama-spm.gguf")
}

//! a hand-built SentencePiece tokenizer over `pieces`, every score 0,
//! its derived tables built
def private spm_vocab(var pieces : array<string>) : SpmTokenizer {
    var tk = SpmTokenizer()
    tk.vocab <- pieces
    for (id, piece in count(), tk.vocab) {
        tk.scores |> push(0.0)
        tk.lookup[piece] = int64(id)
    }
    build_spm_derived(tk)
    return <- tk
}

//! unk, bos, eos, `n_control` control tokens, the 256 byte pieces,
//! then the text pieces
def private spm_vocab_with_bytes(n_control : int) : SpmTokenizer {
    var pieces <- ["<unk>", "<s>", "</s>"]
    pieces |> reserve(3 + n_control + 256 + 3)
    for (i in range(n_control)) {
        pieces |> push("[control_{i}]")
    }
    for (b in range(256)) {
        pieces |> push("<0x{b:02X}>")
    }
    pieces |> push_from([" ", "a", "b"])
    return <- spm_vocab(pieces)
}

def private pieces_of(tk : SpmTokenizer; ids : array<int64>) : array<string> =>
    <- [for (id in ids); tk.vocab[id]]

[test]
def test_tokenizer_spm_byte_pieces_by_name(t : T?) {
    t |> run("a byte with no piece encodes as its <0xXX> piece in either layout") @(t : T?) {
        // Mistral v0.3's layout: control tokens ahead of the byte
        // pieces, so 3 + the byte is a control token
        var control_first <- spm_vocab_with_bytes(16)
        t |> equal("[control_10]", control_first.vocab[13],
                   "3 + the newline byte is a control token in this layout")
        t |> equal(control_first.lookup["<0x0A>"], control_first.byte_piece_id[10],
                   "the byte table finds the newline's piece by name")
        var ids <- encode(control_first, "a\nb", false, false)
        t |> equal([" ", "a", "<0x0A>", "b"], pieces_of(control_first, ids),
                   "the dummy prefix, a, the newline's byte piece, b")
        t |> success(find_index(ids, 13l) < 0, "no control token among the ids ({ids})")
        // the Llama-2 layout, byte pieces right after eos: the same pieces
        var bytes_first <- spm_vocab_with_bytes(0)
        t |> equal(13l, bytes_first.byte_piece_id[10],
                   "the byte table reads 3 + the byte where the layout puts it there")
        var ids2 <- encode(bytes_first, "a\nb", false, false)
        t |> equal([" ", "a", "<0x0A>", "b"], pieces_of(bytes_first, ids2))
        delete ids2
        delete bytes_first
        delete ids
        delete control_first
    }
    t |> run("a vocabulary without the byte pieces panics on a byte it cannot spell") @(t : T?) {
        var pieces <- ["<unk>", "<s>", "</s>", " ", "a"]
        var tk <- spm_vocab(pieces)
        t |> equal(-1l, tk.byte_piece_id[10], "no <0x0A> piece")
        var ids <- encode(tk, "a", false, false)
        t |> equal([3l, 4l], ids, "a text the pieces spell encodes")
        var panicked = false
        try {
            var bad <- encode(tk, "a\n", false, false)
            delete bad
        } recover {
            panicked = true
        }
        t |> success(panicked,
                     "a byte with no piece and no byte piece is a panic, never a made-up id")
        delete ids
        delete tk
    }
}

[test]
def test_tokenizer_mistral_v03_byte_pieces(t : T?) {
    t |> run("Mistral v0.3: byte pieces past 768 control tokens, llama-tokenize's ids") @(t : T?) {
        let path = path_join(models_dir(), "Mistral-7B-Instruct-v0.3-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var tk <- load_tokenizer_auto(path)
        t |> equal(781l, tk.spm.byte_piece_id[10], "<0x0A> is piece 781 in this vocabulary")
        t |> equal("[control_11]", tk.spm.vocab[13],
                   "... and 13, the Llama-2 layout's newline, is a control token")
        // llama-tokenize --no-bos --ids on the same file
        var ids <- encode(tk, "Newlines\nand\r\nmixed\n\n\nruns\n", false)
        t |> success(ids_equal(ids, [2218l, 8831l, 781l, 1159l, 29569l, 781l, 29487l, 3930l,
                                     781l, 781l, 781l, 3988l, 29481l, 781l]),
                     "the newline sample as llama-tokenize reads it: got {ids}")
        var tabs <- encode(tk, "Tabs\tand", false)
        t |> success(ids_equal(tabs, [1088l, 5505l, 780l, 1159l]),
                     "a tab is <0x09> at 780: got {tabs}")
        t |> equal("Tabs\tand", decode(tk, tabs), "and decodes back")
        delete tabs
        delete ids
        delete tk
    }
}
