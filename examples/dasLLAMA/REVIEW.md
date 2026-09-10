# examples/dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

A browser example is a subfolder here with a `web_shell.html` (`ARCHITECTURE.md` sec. 2); the
rules below bind browser examples.

**Never install a GLFW callback - any `glfwSet*Callback` - in a browser example; poll each key
with `glfwGetKey` every frame and edge-detect it.** In the browser build a callback fires outside
any frame of the program and the program traps (`ARCHITECTURE.md` sec. 3.3).

**A string a browser example hands to a thread it starts with `new_thread` crosses as an archived
record on a `Stream?` (`push_archive` / `pop_archive`), never in the lambda's `@capture` list.** A
captured string is a pointer into the starting thread's heap, which that thread reuses before a
slow browser worker reads it (`ARCHITECTURE.md` sec. 3.2).

**A diff that changes or drops a witness line - a line a browser example logs under its own name -
updates every test under `modules/dasLLAMA/tests/` that matches it, in the same change.** The
smoke tests match witness lines as substrings, so the words and their order are an interface
(`ARCHITECTURE.md` sec. 2).

**A diff that adds a model file to a browser example's `models.json` names it by the repository
it is published in and its sha256, never by a local path or a branch name.** The deploy fetches
the file by that name and refuses one whose hash moved; a local path stages nothing on the
runner (`ARCHITECTURE.md` sec. 3.4).
