options gen2

module class_mint_text shared public

require daslib/json
require daslib/json_boost

//! The text `class_mint` returns, one JSON object either way.

//! a mint that finished: `profile` and `sidecar` are JSON documents, embedded whole
def mint_result_text(profile, sidecar : string) : string =>
    "\{\"profile\": {profile}, \"sidecar\": {sidecar}\}"

//! a mint that stopped at a step: `error` alone
def mint_failure_text(why : string) : string {
    var doc = JV((error = why))
    let text = write_json(doc)
    delete_json(doc)
    return text
}
