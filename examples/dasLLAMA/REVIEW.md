# examples/dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

A browser example is a folder with a `web_shell.html`, wherever under `examples/` it sits
(`ARCHITECTURE.md` sec. 2); the rules below bind browser examples; a rule naming `library/` binds
that folder instead.

**A `glfwSet*Callback` written in `.das` in a browser example is a defect - the example polls
inside its own frame instead.** In the browser build a callback lambda fires outside any frame of
the program and the program traps (`ARCHITECTURE.md` sec. 3.3).

**A browser example acts on an input's down edge, never on every frame it stays down -
`IsKeyPressed` / `IsMouseClicked` with `repeat` false on the `imgui_harness`, `glfwGetKey` /
`glfwGetMouseButton` edge-detected per key or button code otherwise - and a repeat comes from
the example's own hold timer.**

**A browser example whose canvas is clicked or tapped gives the canvas element `max-width` /
`max-height` in its `web_shell.html` - never `object-fit` on a canvas stretched to fill the page
area around it.** A click maps through the element's box with one ratio per axis, so a stretched
box mis-maps every click (`ARCHITECTURE.md` sec. 3.3).

**A browser example's `web_shell.html` must reload a page the browser restored from its
back-forward cache - a `pageshow` handler that reloads when `persisted` is set.** Such a page
comes back with its workers and audio output frozen out of step (`ARCHITECTURE.md` sec. 3.1).

**A browser example's `web_shell.html` never refuses a browser on a WebAssembly feature probe -
it gates on the page's cross-origin isolation (`crossOriginIsolated`, `SharedArrayBuffer`)
alone.** The build runs on every engine, so a feature gate only refuses browsers that would have
run it.

**A diff that attaches a status box to a sound (`set_status_update`) attaches a fresh box and
releases it (`unset_status_update`) when that sound ends - never reuse a box across sounds.** The
mixer's last report for the first sound lands after the second attaches, and reads as the
second's stop (`ARCHITECTURE.md` sec. 3.7).

**A diff that changes or drops a witness line - a line a browser example logs under its own name -
updates every test under `modules/dasLLAMA/tests/` that matches it, in the same change.** The
smoke tests match witness lines as substrings, so the words and their order are an interface
(`ARCHITECTURE.md` sec. 2).

**A diff that adds a model file to a browser example's `models.json` names its sha256 and a
location that cannot move - a Hugging Face repository, or a repo-relative path in this repository
under `tree` - never a machine-local path or a branch name.** The deploy fetches by that name and
refuses a file whose hash moved (`ARCHITECTURE.md` sec. 3.4).

**A diff that changes `library/`'s `-ctx` emission command keeps `--disable-module dasLLVM` on
it.** With dasLLVM present the emission stops on the tune framework and writes nothing
(`ARCHITECTURE.md` sec. 3.8).

**A diff that adds an entry point to `library/dasllama_lib.das` gives it `[export_c]` and a
result `daslib/c_api_header.das` can spell in C - a scalar, a string, a pointer, an enum, a
vector, a POD struct, or nothing at all, never an array, a fixed array, a table or a tuple.** An `[export_c]`
whose signature that describer refuses is a hard emit error, not a skipped export
(`ARCHITECTURE.md` sec. 3.8).
