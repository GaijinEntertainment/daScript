# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `modules/dasLLAMA/followup_general.md` (repo root).

**A diff to a file that requires - directly or through another file in this folder - a
`dasllama/*` module, or to `README.md` text stating dasLLAMA engine behavior or a measured
number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff that adds a `main.das` flag, changes what one does or defaults to, or edits any copy of
its text leaves the flag's `@clarg_doc`, its `README.md` table row with its `Default` cell, the
`README.md` section on its config-file key where one exists, and its row in
`doc/source/reference/utils/dasllama_server.rst` (repo root) stating the same behavior and the
same default, adding any copy that is missing.** A copy left behind sends the user to a flag that
no longer does what it says.

**A diff that adds a `cli_args.das` flag, changes what one does or defaults to, or edits any copy
of its text leaves the flag's `@clarg_doc`, its entry in the `README.md` dasllama-cli section (the
command's table row, or the shared-flags sentence for a shared flag), and its row in
`doc/source/reference/utils/dasllama_cli.rst` (repo root) stating the same behavior and the same
default, adding any copy that is missing.**

**A diff that changes how `serving_knobs.das` derives a knob updates, for every `main.das` and
`cli_args.das` flag that reads that knob, the flag's `@clarg_doc`, its `README.md` entry and its
row in `doc/source/reference/utils/dasllama_server.rst` or
`doc/source/reference/utils/dasllama_cli.rst` (repo root), in the same change.** Both programs read the one derivation.

**A Playwright `.spec.js` or a captured fixture, wherever the diff puts it, applies the
`tests/` subfolder's `REVIEW.md` (beside this file) too.**

**Weakening `REVIEW.das` (beside this file) - dropping a check, narrowing what a check scans,
adding a name to a check's licensed set (the names it does not flag), or blunting a finding
text so it no longer names what failed - is a defect.** What the gate enforces is read from
the gate itself.

**A diff that renames or removes a request field that `openai_server.das` reads updates every
place `control.html` sends that field, in the same change.**

**A diff that makes a route in `openai_server.das` require a request field it did not require
before - a new route requires all of its fields - adds that field to every `control.html`
request to that route, in the same change.**

**A diff that changes the shape a route answers - a key, an item in a list it answers, or a value
the code sets itself rather than one the traffic since boot, the configuration, the loaded model or
the machine decides, including one a `dasllama/*` module supplies - re-captures every fixture under
`tests/fixtures/` that records that route, in the same change.** The fixtures are the recorded
response shape.

**A diff that adds a read of a response key in `control.html` or in a Playwright `.spec.js`,
wherever the diff puts it, or edits a line that reads one, and no fixture under
`tests/fixtures/` (beside this file) carries that key, is a defect - capture the fixture for
the route that answers with that key first, or, for a key no machine available for capture can
produce, hand-compose the fixture and name it as hand-composed, with why, in its row of
`tests/fixtures/README.md`.**

**A diff that adds a key to what a route answers, or changes the condition under which the route
answers one, updates that route's `README.md` row in the same change.** The row is where a
consumer learns the key exists and when to expect it.

**A `served` or `served_note` value a diff adds or changes in `openai_server.das` - all of it
but the engine text it quotes - uses only words a user understands without knowing how the
engine is built, and names a part of the engine by what that part does for the user.**

**A `served_note` a diff adds or changes that carries the engine's decline text leads with a
sentence of `openai_server.das`'s own that stands without the decline text.**

**A diff that adds or changes how `control.html` prints a `gpu_cpu_passes` entry shows the
engine's `words` for it, with `reason` only in the `title` tooltip, and prints `reason` for an
entry whose `words` are empty.**

**A diff that adds or changes a `served` value in `openai_server.das` says how the model is
served now, and a value that says the whole model is on the GPU also says where the streams'
caches sit.**

**A diff that adds or changes a `served_note` value in `openai_server.das` names what about the
slot itself holds it back, and is empty when nothing about the slot does.** A slot waiting for
the GPU says that in its `served` value.

**A diff in this folder that opens a device-home session - a session whose K/V cache lives only
in a K/V region of the GPU driver, with no host cache - directly (`create_device_session`) or
through a call that homes streams itself (`bench_tg_batched_rep`), or turns a scheduler's device
mode on (`set_device_kv`), keeps the host's live device-home sessions, every slot's and the ones
the diff opens included, at or below the regions the driver armed (`gpu_device_sessions()`); a
scheduler in device mode counts as its `max_streams`.** The driver panics on the session that
finds no region.

**A diff in this folder that calls `moe_gpu_drop_model` turns off every device mode this folder
armed before the dropped slot's next step, in the same change.** The regions go with the model,
and a scheduler left in device mode admits a session that has none.

**A reference in this folder to a symbol of a module the folder requires conditionally - a
`require ?<guard>` or a `require [<group>]` line - outside a `static_if (typeinfo module_exists(M))`
arm, where M is that module or a module whose own `require` brings it in, is a defect - wrap the
reference in that arm.** The unguarded reference fails the compile of a build without dasLLVM.

**A function signature in this folder never names a type from a module this folder requires
conditionally - those types stay inside the guarded arm, and plain types cross the boundary.** A
signature cannot sit inside a `static_if` arm, so no guard fixes it.

**A `[test]` file in this folder that carries a `require dasllama/...` line of its own answers
to `modules/dasLLAMA/tests/REVIEW.md` (repo root) as well.**
