"""A Russian stress-context checkpoint -> dasLLAMA GGUF.

    python convert_ru_stress.py --checkpoint <dir> --out <models dir> --source "<repo> @ <revision>"
                                [--name ru-stress-rubert-base] [--head <stress_head.pt>] [--catalog <forms.jsonl>]
                                [--quant Q4_K --imatrix-corpus <sentences.jsonl>] [--emb-quant Q8_0]
                                [--vocab-keep 0 --vocab-corpus <sentences.jsonl> ...]

The checkpoint directory is what `save_pretrained` writes for a HuggingFace BERT or ELECTRA
encoder and its WordPiece tokenizer, beside `stress_head.pt`, the state dict of the
`Linear(hidden, 8)` that scores the stressed vowel's ordinal (`ru_stress_oracle.py --seed-head`
writes a seeded one beside a pretrained encoder for the parity rail). The sentence files are a text
corpus the caller supplies, one {"text": ...} row a line. Writes `<name>.gguf`:

- weights f32 unless `--quant` stores the layer GEMMs in a ggml format (ggml's own quantizer, under
  an importance matrix collected over `--imatrix-corpus`) and `--emb-quant` the word-embedding
  table; [out, in] like PyTorch, under the names `dasllama_styletts2.das` reads PL-BERT by:
  `bert.embeddings.*`, ELECTRA's embedding projection as `bert.encoder.embedding_hidden_mapping_in`,
  each layer as `bert.layer.<i>.attention.{query,key,value,dense,LayerNorm}`, `bert.layer.<i>.ffn`,
  `bert.layer.<i>.ffn_output`, `bert.layer.<i>.full_layer_layer_norm`; the head as `stress_head`;
- the encoder's shape and activation as `ru_stress.*` metadata;
- the vocabulary as `tokenizer.ggml.tokens` (id order) with its special ids, the piece ids in the
  ascending byte order of their pieces' UTF-8 (`ru_stress.tokenizer.sorted_ids`, which the reader
  searches and refuses a file without), the WordPiece settings and the BERT normalizer folded into two tables (`ru_stress.tokenizer.*`): a class per codepoint
  run - kept, dropped by the text cleaning, whitespace, punctuation, a CJK ideograph - and the
  per-codepoint rewrite the accent strip (NFD, nonspacing marks dropped) and the lowercasing make
  together, so the reader needs no Unicode data and follows the tokenizer's own flags;
- with `--vocab-keep`, the vocabulary pruned to the pieces counted over `--vocab-corpus` (the N most
  frequent, every one that occurs at 0) plus the specials and every single character bare and as
  `##`, renumbered, the word-embedding rows sliced to match;
- the catalog (`--catalog`): the forms whose stress depends on context, with the vowels each may
  stress, as a byte-sorted form list and one bitmask a form (bit i = ordinal i + 1).
"""
import argparse
import json
import os
import sys
import unicodedata

import numpy as np

ARCH = "ru_stress"
GGML_MAX_NAME = 64
N_ORDINALS = 8
MAX_PIECES = 96
RUST_WHITE_SPACE = {9, 10, 11, 12, 13, 32, 0x85, 0xA0, 0x1680, *range(0x2000, 0x200B), 0x2028, 0x2029, 0x202F, 0x205F, 0x3000}
CJK_RANGES = ((0x4E00, 0x9FFF), (0x3400, 0x4DBF), (0x20000, 0x2A6DF), (0x2A700, 0x2B73F), (0x2B740, 0x2B81F),
              (0x2B920, 0x2CEAF), (0xF900, 0xFAFF), (0x2F800, 0x2FA1F))
HANGUL_FIRST, HANGUL_LAST = 0xAC00, 0xD7A3
KEEP, DROP, SPACE, PUNCT, CJK = 0, 1, 2, 3, 4

LAYER_NAMES = {
    "attention.self.query": "attention.query",
    "attention.self.key": "attention.key",
    "attention.self.value": "attention.value",
    "attention.output.dense": "attention.dense",
    "attention.output.LayerNorm": "attention.LayerNorm",
    "intermediate.dense": "ffn",
    "output.dense": "ffn_output",
    "output.LayerNorm": "full_layer_layer_norm",
}


def category(c):
    return "Cs" if 0xD800 <= c <= 0xDFFF else unicodedata.category(chr(c))


def char_class(c, norm):
    """The tokenizers crate's BertNormalizer + BertPreTokenizer view of one codepoint."""
    cat = category(c)
    if norm["clean_text"] and c not in (9, 10, 13) and (c == 0 or c == 0xFFFD or cat[0] == "C"):
        return DROP
    if c in RUST_WHITE_SPACE:
        return SPACE
    if 33 <= c <= 47 or 58 <= c <= 64 or 91 <= c <= 96 or 123 <= c <= 126 or cat[0] == "P":
        return PUNCT
    if norm["handle_chinese_chars"] and any(a <= c <= b for a, b in CJK_RANGES):
        return CJK
    return KEEP


def rewrite(c, strip, lower):
    s = chr(c)
    if strip:
        s = "".join(x for x in unicodedata.normalize("NFD", s) if unicodedata.category(x) != "Mn")
    if lower:
        s = "".join(x.lower() for x in s)
    return s


def normalizer_tables(norm):
    strip = norm["strip_accents"] if norm["strip_accents"] is not None else norm["lowercase"]
    starts, kinds = [], []
    rw_from, rw_off, rw_to = [], [0], []
    for c in range(0x110000):
        k = char_class(c, norm)
        if not kinds or kinds[-1] != k:
            starts.append(c)
            kinds.append(k)
        if k == DROP or 0xD800 <= c <= 0xDFFF or (strip and HANGUL_FIRST <= c <= HANGUL_LAST):
            continue
        s = rewrite(c, strip, norm["lowercase"])
        if s != chr(c):
            rw_from.append(c)
            rw_to += [ord(x) for x in s]
            rw_off.append(len(rw_to))
    return strip, starts, kinds, rw_from, rw_off, rw_to


def encoder_tensors(model, model_type):
    tensors = {}
    for k, v in model.state_dict().items():
        if k.startswith("pooler.") or k.endswith("position_ids"):
            continue
        name = None
        if k.startswith("embeddings."):
            name = "bert." + k
        elif k.startswith("embeddings_project."):
            name = "bert.encoder.embedding_hidden_mapping_in." + k.split(".", 1)[1]
        elif k.startswith("encoder.layer."):
            _, _, idx, rest = k.split(".", 3)
            stem, leaf = rest.rsplit(".", 1)
            name = f"bert.layer.{idx}.{LAYER_NAMES[stem]}.{leaf}"
        assert name is not None, f"{model_type}: unexpected tensor {k}"
        tensors[name] = np.ascontiguousarray(v.detach().cpu().numpy().astype(np.float32))
    return tensors


def read_catalog(path):
    masks = {}
    with open(path, encoding="utf8") as f:
        for line in f:
            row = json.loads(line)
            form = row["form"].lower()
            for v in row["variants"]:
                assert 1 <= v["ordinal"] <= N_ORDINALS, (form, v["ordinal"])
                masks[form] = masks.get(form, 0) | (1 << (v["ordinal"] - 1))
    forms = sorted(masks, key=lambda s: s.encode("utf8"))
    return forms, [masks[f] for f in forms]


def is_layer_gemm(name):
    """Whether `name` is one of a layer's six GEMM weights - what `--quant` stores in a K-quant format."""
    return name.startswith("bert.layer.") and name.endswith(".weight") and "LayerNorm" not in name and "layer_norm" not in name


def ggml_quantizer(llama_cpp):
    """ggml's own quantizer out of a built llama.cpp's ggml-base library."""
    import ctypes
    import glob
    libs = [p for pat in ("ggml-base.dll", "libggml-base.so", "libggml-base.dylib")
            for p in glob.glob(os.path.join(llama_cpp, "build", "bin", pat))]
    assert libs, f"no built ggml-base under {llama_cpp}/build/bin - build llama.cpp first"
    lib = ctypes.CDLL(libs[0])
    lib.ggml_quantize_chunk.restype = ctypes.c_size_t
    lib.ggml_quantize_chunk.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_float), ctypes.c_void_p,
                                        ctypes.c_int64, ctypes.c_int64, ctypes.c_int64, ctypes.POINTER(ctypes.c_float)]
    return lib


def quantize_rows(lib, gguf, w32, fmt, imatrix):
    import ctypes as c
    t = gguf.GGMLQuantizationType[fmt]
    block, type_size = gguf.GGML_QUANT_SIZES[t]
    nrows, n_per_row = w32.shape
    assert n_per_row % block == 0, (fmt, w32.shape)
    out = np.empty((nrows, (n_per_row // block) * type_size), dtype=np.uint8)
    rows = np.ascontiguousarray(w32, dtype=np.float32)
    qw = None if imatrix is None else np.ascontiguousarray(imatrix, dtype=np.float32).ctypes.data_as(c.POINTER(c.c_float))
    n = lib.ggml_quantize_chunk(int(t), rows.ctypes.data_as(c.POINTER(c.c_float)), out.ctypes.data_as(c.c_void_p),
                                0, nrows, n_per_row, qw)
    assert n == out.nbytes, (fmt, n, out.nbytes)
    return out, t


def importance(model, tok, corpus, count):
    """Every layer GEMM's input activations squared, averaged over `count` corpus sentences - the
    per-input-column mean square the quantizer takes as importance weights, keyed by the GGUF tensor name."""
    import torch
    with open(corpus, encoding="utf8") as f:
        texts = [json.loads(line)["text"].lower() for line in f]
    texts = texts[::max(1, len(texts) // count)][:count]
    sums, counts, hooks = {}, {}, []
    for mname, mod in model.named_modules():
        if not isinstance(mod, torch.nn.Linear) or not mname.startswith("encoder.layer."):
            continue
        _, _, idx, rest = mname.split(".", 3)
        key = f"bert.layer.{idx}.{LAYER_NAMES[rest]}.weight"

        def hook(_m, inp, _out, key=key):
            x = inp[0].reshape(-1, inp[0].shape[-1]).double()
            sums[key] = sums.get(key, 0) + (x * x).sum(0)
            counts[key] = counts.get(key, 0) + x.shape[0]
        hooks.append(mod.register_forward_hook(hook))
    with torch.no_grad():
        for text in texts:
            model(**tok(text, truncation=True, max_length=MAX_PIECES, return_tensors="pt"))
    for h in hooks:
        h.remove()
    return {k: (sums[k] / counts[k]).float().numpy() for k in sums}


def piece_counts(tok, corpora, cache):
    """How often every vocabulary id occurs over the corpora's `text`, lowercased and cut to the
    model's piece budget as the reader feeds it; `cache` keeps the count between runs."""
    if cache and os.path.exists(cache):
        with open(cache, encoding="utf8") as f:
            return {int(k): v for k, v in json.load(f).items()}
    counts = {}
    for path in corpora:
        with open(path, encoding="utf8") as f:
            texts = [json.loads(line)["text"].lower() for line in f]
        for start in range(0, len(texts), 2048):
            for ids in tok(texts[start:start + 2048], truncation=True, max_length=MAX_PIECES, add_special_tokens=False)["input_ids"]:
                for i in ids:
                    counts[i] = counts.get(i, 0) + 1
    if cache:
        with open(cache, "w", encoding="utf8") as f:
            json.dump(counts, f)
    return counts


def kept_ids(vocab, counts, keep_n):
    """The ids a pruned vocabulary keeps, ascending: the `keep_n` most frequent (every id that occurs
    where `keep_n` is 0), every bracketed special, and every single character bare and as `##`, so
    every word of known characters still spells without `[UNK]`."""
    ranked = sorted((i for i in counts if counts[i] > 0), key=lambda i: -counts[i])
    keep = set(ranked if keep_n == 0 else ranked[:keep_n])
    for piece, i in vocab:
        if (piece.startswith("[") and piece.endswith("]")) or len(piece) == 1 or (piece.startswith("##") and len(piece) == 3):
            keep.add(i)
    return sorted(keep)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--checkpoint", required=True, help="save_pretrained directory: config.json, weights, vocab.txt")
    ap.add_argument("--out", required=True, help="directory for <name>.gguf")
    ap.add_argument("--name", default="ru-stress-rubert-base")
    ap.add_argument("--head", default="", help="the stress head's state dict (default <checkpoint>/stress_head.pt)")
    ap.add_argument("--catalog", default="", help="jsonl of the forms whose stress depends on context: form -> variants[].ordinal")
    ap.add_argument("--source", required=True, help="the base model the checkpoint came from, '<repo>' or '<repo> @ <revision>'; "
                    "written into the file as ru_stress.source")
    ap.add_argument("--llama-cpp", default=os.path.expanduser("~/Work/llama.cpp"), help="for gguf-py")
    ap.add_argument("--quant", default="", help="store the layers' GEMM weights in this GGUF tensor type (Q4_K, Q5_K, Q6_K, IQ4_XS)")
    ap.add_argument("--keep-f32", default="", help="comma list of tensor-name pieces whose weights --quant leaves f32 (the q8 lane mints those)")
    ap.add_argument("--imatrix-corpus", default="", help="jsonl of {text} rows the --quant importance matrix is collected over")
    ap.add_argument("--imatrix-count", type=int, default=2000)
    ap.add_argument("--emb-quant", default="", help="store the word-embedding table in this GGUF tensor type (Q8_0, Q4_K, Q4_0, ...)")
    ap.add_argument("--vocab-keep", type=int, default=-1, help="prune the vocabulary to the N most frequent ids over --vocab-corpus "
                    "(0 = every id that occurs) plus the specials and every single character")
    ap.add_argument("--vocab-corpus", action="append", default=[], help="jsonl of {text} rows the pieces are counted over")
    ap.add_argument("--vocab-counts", default="", help="a json cache of the piece counts")
    a = ap.parse_args()
    sys.path.insert(0, os.path.join(a.llama_cpp, "gguf-py"))
    import gguf
    import torch
    from transformers import AutoConfig, AutoModel, AutoTokenizer

    cfg = AutoConfig.from_pretrained(a.checkpoint)
    assert cfg.model_type in ("bert", "electra"), f"{cfg.model_type}: only BERT and ELECTRA encoders convert"
    assert getattr(cfg, "position_embedding_type", "absolute") == "absolute", cfg.position_embedding_type
    model = AutoModel.from_pretrained(a.checkpoint, dtype=torch.float32)
    model.eval()
    tok = AutoTokenizer.from_pretrained(a.checkpoint)
    backend = json.loads(tok.backend_tokenizer.to_str())
    norm, pre, wp = backend["normalizer"], backend["pre_tokenizer"], backend["model"]
    assert norm["type"] == "BertNormalizer" and pre["type"] == "BertPreTokenizer" and wp["type"] == "WordPiece", \
        (norm["type"], pre["type"], wp["type"])
    strip, starts, kinds, rw_from, rw_off, rw_to = normalizer_tables(norm)
    vocab = sorted(tok.get_vocab().items(), key=lambda kv: kv[1])
    assert [i for _, i in vocab] == list(range(len(vocab))), "the vocabulary's ids are not dense"

    tensors = encoder_tensors(model, cfg.model_type)
    head_path = a.head or os.path.join(a.checkpoint, "stress_head.pt")
    head = torch.load(head_path, map_location="cpu")
    tensors["stress_head.weight"] = np.ascontiguousarray(head["weight"].numpy().astype(np.float32))
    tensors["stress_head.bias"] = np.ascontiguousarray(head["bias"].numpy().astype(np.float32))
    assert tensors["stress_head.weight"].shape == (N_ORDINALS, cfg.hidden_size), tensors["stress_head.weight"].shape
    long = [k for k in tensors if len(k) >= GGML_MAX_NAME]
    assert not long, long
    specials = {"unknown": tok.unk_token_id, "cls": tok.cls_token_id, "seperator": tok.sep_token_id,
                "padding": tok.pad_token_id, "mask": tok.mask_token_id}
    if a.vocab_keep >= 0:
        keep_ids = kept_ids(vocab, piece_counts(tok, a.vocab_corpus, a.vocab_counts), a.vocab_keep)
        renumber = {old: new for new, old in enumerate(keep_ids)}
        vocab = [(vocab[old][0], new) for new, old in enumerate(keep_ids)]
        specials = {k: renumber[v] for k, v in specials.items()}
        word = "bert.embeddings.word_embeddings.weight"
        tensors[word] = np.ascontiguousarray(tensors[word][keep_ids])
        print(f"vocabulary pruned to {len(vocab)} pieces (--vocab-keep {a.vocab_keep})")
    emb_dim = getattr(cfg, "embedding_size", cfg.hidden_size)
    act = cfg.hidden_act
    assert act in ("gelu", "gelu_new"), f"{act}: the encoder runs gelu (erf) or gelu_new (tanh)"

    path = os.path.join(a.out, a.name + ".gguf")
    w = gguf.GGUFWriter(path, ARCH)
    w.add_name(a.name)
    w.add_string("ru_stress.source", a.source)
    w.add_string("ru_stress.encoder", cfg.model_type)
    w.add_uint32("ru_stress.n_layers", cfg.num_hidden_layers)
    w.add_uint32("ru_stress.n_heads", cfg.num_attention_heads)
    w.add_uint32("ru_stress.hidden", cfg.hidden_size)
    w.add_uint32("ru_stress.emb_dim", emb_dim)
    w.add_uint32("ru_stress.intermediate", cfg.intermediate_size)
    w.add_uint32("ru_stress.max_positions", cfg.max_position_embeddings)
    w.add_float32("ru_stress.layer_norm_eps", float(cfg.layer_norm_eps))
    w.add_string("ru_stress.activation", act)
    w.add_uint32("ru_stress.max_pieces", MAX_PIECES)
    w.add_uint32("ru_stress.n_ordinals", N_ORDINALS)
    w.add_tokenizer_model("bert")
    w.add_token_list([t for t, _ in vocab])
    for key, value in specials.items():
        w.add_uint32(f"tokenizer.ggml.{key}_token_id", int(value))
    w.add_bool("ru_stress.tokenizer.lowercase", bool(norm["lowercase"]))
    w.add_bool("ru_stress.tokenizer.strip_accents", bool(strip))
    w.add_bool("ru_stress.tokenizer.clean_text", bool(norm["clean_text"]))
    w.add_bool("ru_stress.tokenizer.handle_chinese_chars", bool(norm["handle_chinese_chars"]))
    w.add_string("ru_stress.tokenizer.continuing_prefix", wp["continuing_subword_prefix"])
    w.add_uint32("ru_stress.tokenizer.max_word_chars", int(wp["max_input_chars_per_word"]))
    w.add_array("ru_stress.tokenizer.sorted_ids", sorted(range(len(vocab)), key=lambda i: vocab[i][0].encode("utf8")))
    w.add_array("ru_stress.tokenizer.class_starts", starts)
    w.add_array("ru_stress.tokenizer.class_kinds", kinds)
    w.add_array("ru_stress.tokenizer.rewrite_from", rw_from)
    w.add_array("ru_stress.tokenizer.rewrite_off", rw_off)
    w.add_array("ru_stress.tokenizer.rewrite_to", rw_to)
    n_forms = 0
    if a.catalog:
        forms, masks = read_catalog(a.catalog)
        w.add_array("ru_stress.catalog.forms", forms)
        w.add_array("ru_stress.catalog.ordinals", masks)
        n_forms = len(forms)
    keep_f32 = [k for k in a.keep_f32.split(",") if k]
    lib = ggml_quantizer(a.llama_cpp) if a.quant or a.emb_quant else None
    imat = importance(model, tok, a.imatrix_corpus, a.imatrix_count) if a.quant and a.imatrix_corpus else {}
    layer_bytes = 0
    for name in sorted(tensors):
        if a.quant and is_layer_gemm(name) and not any(k in name for k in keep_f32):
            data, t = quantize_rows(lib, gguf, tensors[name], a.quant, imat.get(name))
            w.add_tensor(name, data, raw_dtype=t)
            layer_bytes += data.nbytes
        elif a.emb_quant and name == "bert.embeddings.word_embeddings.weight":
            data, t = quantize_rows(lib, gguf, tensors[name], a.emb_quant, None)
            w.add_tensor(name, data, raw_dtype=t)
            print(f"{a.emb_quant}: the word embeddings hold {data.nbytes} bytes")
        else:
            w.add_tensor(name, tensors[name])
            layer_bytes += tensors[name].nbytes if name.startswith("bert.layer.") else 0
    if a.quant:
        print(f"{a.quant}: the layers hold {layer_bytes} bytes ({'imatrix over ' + str(len(imat)) + ' weights' if imat else 'no imatrix'}; kept f32: {keep_f32})")
    w.write_header_to_file()
    w.write_kv_data_to_file()
    w.write_tensors_to_file()
    w.close()
    total = sum(t.nbytes for t in tensors.values())
    print(f"wrote {path}: {cfg.model_type} {cfg.num_hidden_layers}x{cfg.hidden_size}, {len(tensors)} tensors, "
          f"{total / 1e6:.1f} MB f32, {len(vocab)} pieces, {len(starts)} class runs, {len(rw_from)} rewrites, "
          f"{n_forms} catalog forms; {os.path.getsize(path)} bytes on disk")


if __name__ == "__main__":
    main()
