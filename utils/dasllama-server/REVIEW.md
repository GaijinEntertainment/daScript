# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `modules/dasLLAMA/followup_general.md` (repo root).

**A diff to a file that requires a file under `modules/dasLLAMA/` (repo root) - by a
`dasllama/*` module name, a module group, or a relative path, directly or through another
file in this folder - applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff to `README.md` text that states what code under `modules/dasLLAMA/` does, or that
states any measured number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A Playwright `.spec.js` or a captured fixture, wherever the diff puts it, applies the
`tests/` subfolder's `REVIEW.md` (beside this file) too.**

**A `[test]` file in this folder whose own `require` line names a file under
`modules/dasLLAMA/` - by a `dasllama/*` module name, a module group, or a relative path -
answers to `modules/dasLLAMA/tests/REVIEW.md` (repo root) as well.**

**A diff that changes what a `main.das` or `cli_args.das` flag does or says - a flag added, its
default, a file or path it reads or writes, what it makes the program do, the value
`serving_knobs.das` derives from it, or any copy of its text - leaves every copy of that flag
stating the same behavior, default, and files and paths.** A flag's copies are its
`@clarg_doc`, every `README.md` passage that names the flag, its row in
`doc/source/reference/utils/dasllama_server.rst` or
`doc/source/reference/utils/dasllama_cli.rst` (repo root), its synopsis entry, and every passage
saying the flag means the same as the same-named flag of the other file. A copy need not match
another's reasons or measured numbers.

**A diff that adds or edits a flag's entry in a synopsis code block - the `[--flag VALUE]` command
line in `README.md` or either reference page - writes the flag's default as the value, or a
placeholder where the default is not a literal value.**

**Weakening `REVIEW.das` (beside this file) - dropping a check, narrowing what a check scans,
adding a name to the names a check skips, or blunting a finding
text so it no longer names what failed - is a defect.** What the gate enforces is read from
the gate itself.

**Weakening `test_worker_dispatch.das` - removing its `success` check or a condition its `passed`
value combines, dropping a call it makes before that check, or removing its `[test]` annotation -
is a defect; fix the code that fails it instead.** What the test holds is read from the test itself.

**A diff that renames or removes a request field that `openai_server.das` reads updates every
place `control.html` sends that field, in the same change.**

**A diff that makes a route in `openai_server.das` require a request field it did not require
before - a new route requires all of its fields - adds that field to every `control.html`
request to that route, in the same change.**

**A diff that changes the response a fixture's recorded request gets - a key, an item in a list
it answers, or a value the code sets itself (a default it fills in for a key the request and the
configuration leave unset is one), not one the traffic since boot, the configuration, the loaded
model or the machine decides - re-captures every fixture under `tests/fixtures/` that records that
route, in the same change.**
The fixtures are the recorded response shape.

**A diff that adds a read of a response key in `control.html` or in a Playwright `.spec.js`,
wherever the diff puts it, or edits a line that reads one, and no fixture under
`tests/fixtures/` (beside this file) carries that key, is a defect - capture the fixture for
the route that answers with that key first, or, for a key no machine available for capture can
produce, hand-compose the fixture and name it as hand-composed, with why, in its row of
`tests/fixtures/README.md`.**

**A diff that adds a key to what a route answers, or changes when the route answers a key or
what decides that key's value, names the key, when the route answers it and what decides its
value in the route's `README.md` row, and leaves every other `README.md` passage that names the
key, or the request field that decides it, stating what the row states, in the same change.** A
diff changes what decides a key's value only when it adds or removes a request field, flag or
config key the value depends on, or a kind of value the key takes. An error reply's HTTP status
code counts as a key the route answers.

**A `served` or `served_note` value a diff adds or changes in `openai_server.das` - all of it
but the engine text it quotes - uses only words a user understands without knowing how the
engine is built, and names a part of the engine by what that part does for the user.**

**A `served_note` a diff adds or changes that carries the engine's decline text - the reason
`gpu_resident_decline()` gives for not serving the model whole on the GPU - leads with a
sentence of `openai_server.das`'s own that stands without the decline text.**

**A diff that adds or changes how `control.html` prints a `gpu_cpu_passes` entry shows the
engine's `words` for it, with `reason` only in the `title` tooltip, and prints `reason` for an
entry whose `words` are empty.**

**A diff that adds or changes a `served` value in `openai_server.das` says how the model is
served now, and a value that says the whole model is on the GPU also says where the streams'
caches sit.**

**A diff that adds or changes a `served_note` value in `openai_server.das` names what about the
slot itself keeps the GPU from serving more of it, and is empty when nothing about the slot
does.** A slot waiting for the GPU says that in its `served` value.

**A diff in this folder that opens a device-home session - a session whose K/V cache lives only
in a K/V region of the GPU driver, with no host cache - or takes a region by any other call that
does not first check one is free, or turns a scheduler's device mode on (`set_device_kv`), keeps
the live region holders of that context, every slot's and the ones the diff opens included, at or
below the regions that context's driver armed (`gpu_device_sessions()`); a scheduler in device
mode counts as its `max_streams`.** Each context arms its own regions on the shared device, and the
driver panics on the call that finds no region free.

**A diff in this folder that calls `moe_gpu_drop_model` turns off, before the dropped slot's next
step, every scheduler device mode (`set_device_kv`) this folder turned on, in the same change.**
The regions go with the model, and a scheduler left in device mode admits a session that has none.

**A reference in this folder to a symbol of a module the folder requires conditionally - a
`require ?<guard>` or a `require [<group>]` line - outside a `static_if (typeinfo module_exists(M))`
arm, where M is that module or a module whose own `require` brings it in, is a defect - wrap the
reference in that arm.** The unguarded reference fails the compile of every build that lacks that
module.

**A function signature in this folder names only types from modules this folder requires
unconditionally; a type from a conditionally required module stays inside the guarded arm.** A
signature cannot sit inside a `static_if` arm, so no guard fixes it.

**A diff that adds or changes a JSON value `openai_server.das` writes that holds text the model
produced, or text the server parsed out of that text, passes the text through `utf8_sanitize`
before it is written.** A token piece can end mid-character and a byte-fallback piece carries one
raw byte, and a strict client aborts on the invalid byte (`README.md#utf8-replies`).

**A diff that adds or changes how `openai_server.das` streams a delta holds back, with
`utf8_hold_bytes`, the bytes at the delta's end that start a character the delta does not finish,
joins them to the next piece, and when the stream ends sends the bytes still held back as one
last delta through `utf8_sanitize`.**
