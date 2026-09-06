# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `CONTROL_PAGE_PLAN.md`.

**A diff to a file that requires - directly or through another file in this folder - a
`dasllama/*` module, or to a `README.md` row stating dasLLAMA engine behavior or a measured
number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff that changes a flag's row in `README.md` also updates that flag's `@clarg_doc` in
`main.das`, in the same change - `--help` is the other copy a user reads.**

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

**A diff that makes `control.html` or a Playwright `.spec.js` under `tests/` (beside this file)
read a response key no fixture under `tests/fixtures/` carries is a defect - re-capture the
fixture for the route that answers with that key first.**

**A diff that adds a key to what a route answers lists it in that route's `README.md` row, in
the same change.** The row is where a consumer learns the key exists.

**A reference to a `dasllama_exchange` or `llvm_tune` symbol in this folder outside a
`static_if (typeinfo module_exists(<that module>))` arm is a defect - a signature cannot carry that
guard, so it never names one of their types.** Both modules are optional (`require ?llvm`) and do
not exist in a build without dasLLVM; the unguarded reference fails that build's compile.
