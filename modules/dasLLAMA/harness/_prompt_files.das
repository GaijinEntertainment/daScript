options gen2
options indenting = 4

module _prompt_files shared public

//! The harness's comma lists of numbers, inline or a file's, and its files of lines.

require daslib/fio
require daslib/strings_convert   // try_to_int64: a part that is not a number is dropped, never a 0
require ../../../utils/common/text_lines.das   // split_trimmed, non_empty_lines

//! the numbers of a comma list, each part stripped; an empty part, one that is not a number, or one outside `lo` .. `hi`
//! is dropped
def id_list(s : string; lo : int64 = -9223372036854775807l - 1l; hi : int64 = 9223372036854775807l) : array<int64> {
    var out : array<int64>
    for (t in split_trimmed(s, ",")) {
        let n = try_to_int64(t)
        if (is_ok(n) && unwrap(n) >= lo && unwrap(n) <= hi) {
            out |> push(unwrap(n))
        }
    }
    return <- out
}

//! the ids of a file of comma-separated token ids; an absent file holds none
def read_ids(path : string) : array<int64> => <- id_list(fread(path))

//! the non-empty lines of a text file, stripped - the first `limit` of them where `limit` is positive; an absent file holds none
def read_lines(path : string; limit : int = 0) : array<string> {
    var lines <- non_empty_lines(fread(path))
    if (limit > 0 && length(lines) > limit) {
        lines |> resize(limit)
    }
    return <- lines
}
