# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `modules/dasLLAMA/followup_general.md` (repo root).

**A diff to a file that requires - directly or through another file in this folder - a
`dasllama/*` module, or to `README.md` text stating dasLLAMA engine behavior or a measured
number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff that changes a flag's user-visible text updates every copy of that text in the same
change - the `@clarg_doc` in `main.das`, every `README.md` place that documents the flag (its
table row, and the section documenting the key where it has one), its row in
`doc/source/reference/utils/dasllama_server.rst` (repo root) - adding a copy where one is
missing** - a copy left behind sends the user to a flag that no longer does what it says.

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
the route that answers with that key first, or, for a key no machine available for capture can
produce, hand-compose the fixture and name it as hand-composed, with why, in its row of
`tests/fixtures/README.md`.**

**A diff that adds a key to what a route answers lists it in that route's `README.md` row, in
the same change.** The row is where a consumer learns the key exists.

**A `served` or `served_note` value `openai_server.das` writes itself - all of it but the engine
text it quotes - uses no name the engine has for its own parts - `pass`, `region`, `mirror`,
`resident driver`, `tier`, `rails` - and no name of the same kind.**

**A `served_note` that carries the engine's decline text leads with a sentence of
`openai_server.das`'s own that stands without it.**

**A `gpu_cpu_passes` entry the control page prints shows the engine's `words` for it, with
`reason` only in the `title` tooltip; an entry whose `words` are empty prints its `reason`.**

**A diff that adds or changes a `served` value in `openai_server.das` says how the model is
served now, and a value that says the whole model is on the GPU also says where the streams'
caches sit.**

**A diff that adds or changes a `served_note` value in `openai_server.das` names what about the
slot itself holds it back, and is empty when nothing about the slot does.** A slot waiting for
the GPU says that in its `served` value.

**A diff in this folder that opens a stream in device mode - a `create_device_session` call, or
a `set_device_kv` that turns a scheduler's device mode on - keeps that site behind a check that
the slot's live device-home sessions - a scheduler in device mode counts as `max_streams` of
them - stay within the K/V regions its load armed (`ModelSlot.gpu_regions`).** The driver panics
on the session that finds no region.

**A diff in this folder that calls `moe_gpu_drop_model` turns off every device mode this folder
armed before the dropped slot's next step, in the same change.** The regions go with the model,
and a scheduler left in device mode admits a session that has none.

**A reference in this folder to a symbol of a module the folder requires conditionally - a
`require ?<guard>` or a `require [<group>]` line - outside a
`static_if (typeinfo module_exists(...))` arm on that module, or on a module whose own `require`
brings it in, is a defect** - the unguarded reference fails the compile of a build without
dasLLVM.

**A function signature in this folder never names a type from a module this folder requires
conditionally - those types stay inside the guarded arm, and plain types cross the boundary.** A
signature cannot sit inside a `static_if` arm, so no guard fixes it.

**A `[test]` file in this folder that carries a `require dasllama/...` line of its own answers
to `modules/dasLLAMA/tests/REVIEW.md` (repo root) as well.**
