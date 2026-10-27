options gen2
options _dasllama_internal = true

module _json_file shared public

require dasllama/dasllama_lint public
require daslib/fio
require daslib/json_boost

//! The JSON file at `path` into `m` (its type picks the schema): false when the file is missing or empty, a panic when it does not parse.
def load_json_file(path : string; var m : auto(TT)) : bool {
    let text = fread(path)
    return false if (empty(text))
    if (!sscan_json(text, m)) {
        panic("{path} does not parse")
    }
    return true
}
