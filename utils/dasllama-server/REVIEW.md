# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `modules/dasLLAMA/followup_general.md` (repo root).

**A diff to a file that requires - directly or through another file in this folder - a
`dasllama/*` module, or to `README.md` text stating dasLLAMA engine behavior or a measured
number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff that changes a flag's user-visible text in any of its three copies - the
`@clarg_doc` in `main.das`, its entry in `README.md` (a table row, or the section that documents
the key), its row in `doc/source/reference/utils/dasllama_server.rst` (repo root) - updates the
other two in the same change, adding the copy where one is missing** - a copy left behind sends the user to a
flag that no longer does what it says.

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

**A diff that changes what a route answers - an item, a field, or a value, including one a
`dasllama/*` module supplies - re-captures every fixture under `tests/fixtures/` that
records that route, in the same change.** The fixtures are the recorded response shape.

**A diff that adds a read of a response key in `control.html` or in a Playwright `.spec.js`,
wherever the diff puts it, or edits a line that reads one, and no fixture under
`tests/fixtures/` (beside this file) carries that key, is a defect - capture the fixture for
the route that answers with that key first.**

**A diff that adds a key to what a route answers lists it in that route's `README.md` row, in
the same change.** The row is where a consumer learns the key exists.

**A string the control page puts in front of a reader to say how a slot is served - a `served`
or `served_note` value, the words for a `gpu_cpu_passes` reason - never uses the engine's own
nouns for its parts (a pass, a region, a mirror, the resident driver, the tier, the rails):
plain words instead.** A reason's machine name rides only as a tooltip, and the engine's own
decline text only after a plain sentence that stands without it.

**A `served` value says where the weights sit and, on a GPU slot, where the streams' caches
sit; a `served_note` value says what holds the slot back.**

**A diff in this folder that calls `create_device_session`, or turns a scheduler's device mode
on (`set_device_kv`), shows at that call site that the slot's live device-home sessions stay
within the K/V regions its load armed (`ModelSlot.gpu_regions`).** The driver panics on the
session that finds no region.

**A reference in this folder to a symbol of a module the folder requires conditionally - a
`require ?<guard>` or a `require [<group>]` line (today `dasllama_exchange` and `llvm_tune`) -
outside a `static_if (typeinfo module_exists(...))` arm on that module, or on a module whose own
`require` brings it in, is a defect** - the unguarded reference fails the compile of a build
without dasLLVM.

**A function signature in this folder never names a type from one of those modules - those
types stay inside the guarded arm, and plain types cross the boundary.** A signature cannot sit
inside a `static_if` arm, so no guard fixes it.

**A `[test]` file in this folder that carries a `require dasllama/...` line of its own answers
to `modules/dasLLAMA/tests/REVIEW.md` (repo root) as well** - its out-of-folder ledger row lives
there.
