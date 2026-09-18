# examples/dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

A browser example is a subfolder here with a `web_shell.html` (`ARCHITECTURE.md` sec. 2); the
rules below bind browser examples; a rule naming `library/` binds that folder instead.

**A browser example reads its input inside its own frame - a `glfwSet*Callback` written in
`.das` is a defect; an example that draws its own text polls each input every frame and acts
when it goes down, never on every frame it stays down - a repeat comes from the example's own
hold timer (`glfwGetKey`, `glfwGetMouseButton`) - and one on the imgui harness reads ImGui's
state.** In the browser build a callback lambda fires outside any frame
of the program and the program traps (`ARCHITECTURE.md` sec. 3.3).

**A browser example whose `main.das` reads the mouse - `glfwGetMouseButton`, or ImGui's mouse
state through the imgui harness - gives the canvas element `max-width` / `max-height` in its
`web_shell.html` - never `object-fit` on a canvas stretched to fill the page area around it.** A
click maps through the element's box with one ratio per axis, so a stretched box mis-maps every
click (`ARCHITECTURE.md` sec. 3.3).

**A browser example's `web_shell.html` must reload a page the browser restored from its
back-forward cache - a `pageshow` handler that reloads when `persisted` is set.** Such a page
comes back with its workers and audio output frozen out of step (`ARCHITECTURE.md` sec. 3.1).

**A browser example's `web_shell.html` never refuses a browser on a WebAssembly feature probe -
it gates on the page's cross-origin isolation (`crossOriginIsolated`, `SharedArrayBuffer`)
alone.** The build runs on every engine, so a feature gate only refuses browsers that would have
run it.

**A diff that plays a second sound creates a new status box (`set_status_update`) for it and
releases the first when its sound ends - never reuse a box across sounds.** The mixer's last
report for the first sound lands after the second attaches, and reads as the second's stop
(`ARCHITECTURE.md` sec. 3.7).

**A diff that changes or drops a witness line - a line a browser example logs under its own name -
updates every test under `modules/dasLLAMA/tests/` that matches it, in the same change.** The
smoke tests match witness lines as substrings, so the words and their order are an interface
(`ARCHITECTURE.md` sec. 2).

**A diff that adds or changes an entry in a browser example's `models.json` gives that entry a
sha256 and a location that cannot move - a Hugging Face repository, or, in the file's `tree`
list, a repo-relative path to a file this repository itself carries - never a machine-local path
or a branch name.** The deploy fetches by that location and refuses a file whose hash moved
(`ARCHITECTURE.md` sec. 3.4).

**A diff that adds or changes an entry in a browser example's `models.json` states in the PR
body that the entry's sha256 is the hash of the file its location now resolves to - the
published file for a Hugging Face entry, the committed file for a `tree` entry.**

**A diff that changes how `library/` emits its standalone C++ context keeps `--disable-module
dasLLVM` on that command.** The tune framework is a macro module with no AOT form and a
standalone context links no interpreter, so with dasLLVM present the emission stops on it and
writes nothing (`ARCHITECTURE.md` sec. 3.8).

**A diff that adds an entry point to `library/dasllama_lib.das` gives it `[export_c]` and a
result `daslib/c_api_header.das` can spell in C - a scalar, a string, a pointer, an enum, a
vector, a POD struct, or nothing at all, never an array, a fixed array, a table or a tuple.**
An `[export_c]` whose signature that describer refuses is a hard emit error, not a skipped
export (`ARCHITECTURE.md` sec. 3.8).
