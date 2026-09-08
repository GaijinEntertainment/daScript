# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `CONTROL_PAGE_PLAN.md`.

**A diff to a file that requires - directly or through another file in this folder - a
`dasllama/*` module, or to `README.md` text stating dasLLAMA engine behavior or a measured
number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff that changes a flag's user-visible text in any of its three copies - the
`@clarg_doc` in `main.das`, its row in `README.md`, its row in
`doc/source/reference/utils/dasllama_server.rst` (repo root) - updates the other two in the
same change, adding the copy where one is missing** - a copy left behind sends the user to a
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

**A reference in this folder to a symbol of a module the folder requires only under
`require ?llvm` or `require [tune_framework]` (`dasllama_exchange`, `llvm_tune`, `llvm_code`)
outside a `static_if (typeinfo module_exists(<that module>))` arm is a defect** - the unguarded
reference fails the compile of a build without dasLLVM.

**A function signature in this folder never names a type from one of those modules - those
types stay inside the guarded arm, and plain types cross the boundary.** A signature cannot sit
inside a `static_if` arm, so no guard fixes it.

**A `[test]` file in this folder that carries a `require dasllama/...` line of its own answers
to `modules/dasLLAMA/tests/REVIEW.md` (repo root) as well** - its out-of-folder ledger row lives
there.
