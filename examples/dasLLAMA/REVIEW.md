# examples/dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Planned work:
`plans/storywish.md` (repo root). The examples here ship twice from one source: the desktop run
and the wasm64 page `daspkg release wasm` builds for dasllama.io, and every rule below exists
because the browser build broke where the desktop run did not.

**Never install a GLFW callback - any `glfwSet*Callback` - in an example under this folder; poll
the state each frame with `glfwGetKey` and edge-detect it.** In the browser build a callback
fires from a JavaScript event outside any frame of the program, and the example traps there on
the first keystroke.

**A string an example hands to a thread it starts is pushed through a stream that thread reads,
never captured by the thread's lambda.** A captured string is a pointer into the starting
thread's heap, which that thread reuses before a slow browser worker reads it.

**A diff that changes a line an example logs under its own name - the words, their order, or the
values between them - updates the test that reads that line in the same change:
`modules/dasLLAMA/tests/test_storyteller_restart.das` for `storyteller/main.das`,
`modules/dasLLAMA/tests/test_storywish.das` for `storywish/main.das`.** Both tests match those
lines word for word.

**A diff that adds a model file to an example's `models.json` names it by the repository it is
published in and its sha256, never by a local path or a floating branch.** The deploy fetches
the file by that name and refuses one whose hash moved; a local path stages nothing on the
runner.
