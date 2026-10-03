options gen2
options indenting = 4

module _corpus shared public

//! The prompt corpus reader, with no engine module behind it: the profiling rig (`profile_common`) and the served
//! instrument (`served_bench.das`, which loads no engine module) read `data/wikitext2_prompts.txt` through one function.

require daslib/fio
require daslib/strings_boost
require strings

//! a prompt corpus file's prompts: segments separated by lines of two percent signs, stripped, the empty and '#' ones skipped
def load_corpus(path : string) : array<string> {
    var texts : array<string>
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("can't open prompt corpus {path}")
        }
        for (p in split(fread(f), "%%")) {
            let tp = strip(p)
            if (!empty(tp) && !starts_with(tp, "#")) {
                texts |> push(clone_string(tp))
            }
        }
    }
    return <- texts
}
