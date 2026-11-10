options gen2
options _dasllama_internal = true

require daslib/fio
require daslib/clargs
require daslib/strings_boost
require dasllama/dasllama_textnorm_ru

//! The Russian normalizer over a file of sentences, one a line, the normalized line written in the same order -
//! the daslang step of `harness/textnorm_ru_kestrel.py`: `-- <in.txt> <out.txt>`

[export]
def main() : int {
    let args <- get_user_args()
    if (length(args) != 2) {
        print("usage: textnorm_ru_run.das -- <in.txt> <out.txt>\n")
        return 1
    }
    let inPath = args[0]
    let outPath = args[1]
    var whole = ""
    fopen(inPath, "rb") $(fin) {
        whole = fread(fin)
    }
    var count = 0
    fopen(outPath, "wb") $(fout) {
        for (line in split(whole, "\n")) {
            let text = strip(line)
            if (empty(text)) {
                continue
            }
            fwrite(fout, "{normalize_text_ru(text)}\n")
            count++
        }
    }
    print("{count} lines normalized\n")
    return 0
}
