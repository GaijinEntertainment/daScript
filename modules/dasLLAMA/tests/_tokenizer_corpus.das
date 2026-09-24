options gen2
options _dasllama_internal = true

module _tokenizer_corpus shared public

require strings
require daslib/strings_boost

//! the upstream test-tokenizer-0 corpus helpers the tokenizer suite and the BPE harness read: one .out line
//! (" 1142 220 19 ...") into token ids, tolerant of the leading space
def parse_out_ids(line : string) : array<int64> {
    return <- [for (tok in split(line, " ")); to_int64(tok, false); where !empty(tok)]
}

//! the single trailing empty piece both fixture files' closing delimiter leaves after a split, dropped
def drop_trailing_empty(var xs : array<string>) {
    if (!empty(xs) && empty(xs[length(xs) - 1])) {
        xs |> erase(length(xs) - 1)
    }
}
